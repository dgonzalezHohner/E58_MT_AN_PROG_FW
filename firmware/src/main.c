/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "push_buttons.h"
#include "SPI_MHM.h"
#include "UART_COM.h"

extern SERCOM_USART_OBJECT sercomtest;
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    PBInit (&pPushButtons , NUMBER_PUSH_BUTTONS);
    
    MHMTimer = 0;
    SYSTICK_TimerPeriodSet (SYSTYCK_PERIOD);
    SYSTICK_TimerRestart();
    ExtDACInit();
    
    UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
    UARTRxCmdBufferInit((UARTRxCmdBuffType*) &UART3RxCmdBuffer);
    SERCOM3_USART_ReadCallbackRegister(UARTRxDataBufferAdd,(uintptr_t)((void*)&Sercom3RxData));
    SERCOM3_USART_Read((void*)&Sercom3RxData, SERCOM3_RXDATA_LENGTH);
    //WDT_Enable();
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        //WDT_Clear();
        if(BISS_MASTER_Get())
        {
            //IC_MHM_BISS_Detection();
            //PB_BISS_Detection();
        }
        else
        {
            //PushButtonsTask();
            IC_MHM_Task();
            ExtDACTask();
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

