/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <math.h>
#include <stdio.h>
#include "squircular.h"
#include "drive_command.h"
    
//FLAGS
#define FLAG_STATE_CHANGED  0b00000001
#define FLAG_STATE_QUIT     0b00000010
// insert other flags here
#ifdef DEBUG
#define FLAG_STATE_TEST     0b10000000
#endif // DEBUG
    
//-UART
#define UART_COMMAND_DRIVE  'd'
#define UART_COMMAND_LIGHT  'l'
#define UART_COMMAND_QUIT   'q'
#ifdef DEBUG
#define UART_COMMAND_TEST   't'
#endif // DEBUG

#define UART_RESPONSE_MSG_UNKNOWN   'u'

//STRUCT DEFINITIONS AND STRUCT SPECIFIC FUNCTIONS

typedef struct state {
    driveCommand_t driveCommand;
    uint8 flags;
} state_t;

typedef struct speeds {
    int16 left;
    int16 right;
} speeds_t;

//FUNCTION PROTOTYPES
void initialize();
void terminate();
driveCommand_t readDriveCommand();
speeds_t convertDriveCommand(driveCommand_t command);

//-INTERRUPT PROTOTYPES
CY_ISR_PROTO(commandReceived);

//-FOR TESTING PURPOSES
#ifdef DEBUG
void runTestRoutine(int delayMs);
int testCounter = 0;
#endif //DEBUG

//GLOBAL VARIABLES
volatile state_t state = {
    {DC_CIRCLE_DOMAIN, {0.0, 0.0}},
    FLAG_STATE_CHANGED // (Set because "The creation of state is a change of state" - <INSERT RANDOM PHILOSOPHER>)
};

//MAIN
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    initialize();

    while(1)
    {
        #ifdef DEBUG
        if(state.flags & FLAG_STATE_TEST) {
            runTestRoutine(1000);
        }
        #endif //DEBUG
        if(state.flags & FLAG_STATE_CHANGED) {
            speeds_t speeds = convertDriveCommand(state.driveCommand);
            MotorController_SetSpeeds(speeds.left, speeds.right);
            state.flags = state.flags & ~FLAG_STATE_CHANGED;
        }
        
        if(state.flags & FLAG_STATE_QUIT){
            terminate();
        }
    }
}

//HELPER FUNCTION PREDECLERATIONS
//-INIT FUNCTIONS
void initUart();
void terminUart();

//INTERRUPT DEFINITIONS
CY_ISR(commandReceived){
    char commandIdentifier = UART_GetChar();

    switch(commandIdentifier){
        case UART_COMMAND_DRIVE:
            state.driveCommand = readDriveCommand();
            state.flags = state.flags | FLAG_STATE_CHANGED;
            break;
        case UART_COMMAND_LIGHT: //Switch Light (for testing connection)
            Out_LED_Write(~Out_LED_Read());
            break;
        case UART_COMMAND_QUIT:
            state.flags = state.flags | FLAG_STATE_QUIT;
            break;
        #ifdef DEBUG
        case UART_COMMAND_TEST:
            state.flags = state.flags | FLAG_STATE_TEST;
            break;
        #endif //DEBUG
        default: //Signal that an unknown message was received.
            UART_PutChar(UART_RESPONSE_MSG_UNKNOWN);
            
    }
}

//FUNCTION DEFINITIONS
//-TEST ROUTINE
#ifdef DEBUG
void runTestRoutine(int delay){
    Out_LED_Write(1);
    switch(testCounter){
        case 0:
            state.driveCommand.domain = DC_SQUARE_DOMAIN;
            state.driveCommand.moveVector.x = 1.0;
            state.driveCommand.moveVector.y = 0.0;
            state.flags = state.flags | FLAG_STATE_CHANGED;
            testCounter++;
            break;
        case 1:
            CyDelay(delay);
            state.driveCommand.domain = DC_SQUARE_DOMAIN;
            state.driveCommand.moveVector.x = -1.0;
            state.driveCommand.moveVector.y = 0.0;
            state.flags = state.flags | FLAG_STATE_CHANGED;
            testCounter++;
            break;
        case 2:
            CyDelay(delay);
            state.driveCommand.domain = DC_SQUARE_DOMAIN;
            state.driveCommand.moveVector.x = 0.0;
            state.driveCommand.moveVector.y = 1.0;
            state.flags = state.flags | FLAG_STATE_CHANGED;
            testCounter++;
            break;
        case 3:
            CyDelay(delay);
            state.driveCommand.domain = DC_SQUARE_DOMAIN;
            state.driveCommand.moveVector.x = 0.0;
            state.driveCommand.moveVector.y = -1.0;
            state.flags = state.flags | FLAG_STATE_CHANGED;
            testCounter++;
            break;
        case 4:
            CyDelay(delay);
            state.driveCommand.domain = DC_SQUARE_DOMAIN;
            state.driveCommand.moveVector.x = 1.0;
            state.driveCommand.moveVector.y = 1.0;
            state.flags = state.flags | FLAG_STATE_CHANGED;
            testCounter++;
            break;
        case 5:
            CyDelay(delay);
            state.driveCommand.domain = DC_SQUARE_DOMAIN;
            state.driveCommand.moveVector.x = -1.0;
            state.driveCommand.moveVector.y = -1.0;
            state.flags = state.flags | FLAG_STATE_CHANGED;
            testCounter++;
            break;
        case 6:
            CyDelay(delay);
            state.driveCommand.domain = DC_SQUARE_DOMAIN;
            state.driveCommand.moveVector.x = 0.5;
            state.driveCommand.moveVector.y = 0.0;
            state.flags = state.flags | FLAG_STATE_CHANGED;
            testCounter++;
            break;
        default:
            CyDelay(delay);
            state.driveCommand.domain = DC_SQUARE_DOMAIN;
            state.driveCommand.moveVector.x = 0.0;
            state.driveCommand.moveVector.y = 0.0;
            testCounter = 0;
            state.flags = (state.flags | FLAG_STATE_CHANGED) & ~FLAG_STATE_TEST ;
            Out_LED_Write(0);
    }
    
}
#endif //DEBUG


//-INIT FUNCTIONS
void initialize(){
    MotorController_Start();
    initUart();
}

void terminate(){
    MotorController_Stop();
    terminUart();
    CyPmHibernate();
}

void initUart(){
    InterruptCommandReceived_StartEx(commandReceived);
    UART_Start();
}

void terminUart(){
    InterruptCommandReceived_Stop();
    UART_Stop();
}

//-UART FUNCTIONS
driveCommand_t readDriveCommand(){
    driveCommand_t command;
    
    char* pointer = (char*) &command;
    for(unsigned int i = 0; i<sizeof(driveCommand_t); i++){
        pointer[i] = UART_GetChar();
    }
    
    return command;
}


//-MOTOR FUNCTIONS

/**
 * This function transforms a vector (direction, forward), which is found in driveCommand, to the vector (left, right) which represents 
 * the speed at which the left and the right motors run.
 * This can be achieved by rotating the vector by 45 degrees and projecting it on a square. (if (d,f) == (1.0, 0.0) then (l,r) == (1.0, 1.0))
 * (direction, forward) is expected to be on or inside the unit circle when command.space == 'c'
 * (direction, forward) is expected to be on or inside the square with range ([-1.0, 1.0], [-1.0, 1.0]) command.space == 's'
 * This is not tested during runtime (except if ran in debug mode)
 * If command.space is anything else it will return (0.0, 0.0)
 *
 * @param command The drive command containing the vector (d,f) and the space on which they lie
 * @return The vector (r,l) In the range [-255,255]
**/
speeds_t convertDriveCommand(driveCommand_t command){
    fvector2_t circledVector;
    
    // project on circle if needed
    if (command.domain == DC_SQUARE_DOMAIN){
        square_to_circle(&command.moveVector);
    }
    else if(command.domain == DC_CIRCLE_DOMAIN){
        circledVector = command.moveVector;
    } else {
        //invalid
        speeds_t speeds = {0,0};
        return speeds;
    }
    
    //rotate by 45 degrees
    fvector2_t rotatedVector = fvector_rotate(&circledVector, M_PI_4);
    
    //project back on square
    fvector2_t squaredVector = circle_to_square(&rotatedVector);
    speeds_t speeds;
    speeds.left = (int16) (squaredVector.x * 255.0);
    speeds.right = (int16) (squaredVector.y * 255.0);
    return speeds;
}

/* [] END OF FILE */
