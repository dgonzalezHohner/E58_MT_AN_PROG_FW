/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "definitions.h"                // SYS function prototypes
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "UART_COM.h"
/* TODO:  Include other files here if needed. */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
//int global_data;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */
//static int ExampleLocalFunction(int param1, int param2) {
//    return 0;
//}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */
void UARTRxBufferInit(UARTRxBuffType* Buff)
{
    memset((uint8_t*)Buff,0,sizeof(UARTRxBuffType));
}

void UARTRxCmdBufferInit(UARTRxCmdBuffType* Buff)
{
    memset((uint8_t*)Buff,0,sizeof(UARTRxCmdBuffType));
}
// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
//int ExampleInterfaceFunction(int param1, int param2) {
//    return 0;
//}

void UARTRxCmdBufferAdd (uint8_t* RxCmdPtr)
{
    if(UART3RxCmdBuffer.CmdCnt < RX3_CMD_BUFF_LEN)
    {
        UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.WrIndex++] = RxCmdPtr;
        UART3RxCmdBuffer.WrIndex %= RX3_CMD_BUFF_LEN;
    }
}

void UARTRxDataBufferAdd(uintptr_t testparam)
{
    uint8_t ConvStr[6];
    uint8_t* CmdPtr = NULL;
// TO DO in UART3 TASK    
//    if (UART3RxBuffer.RxTimeout == 1) 
//    {
//        //Rx timeout expired
//        UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
//    }
    if (UART3RxBuffer.RxTimeout != 1)
    {
        switch (UART3RxBuffer.WrIndex)
        {
            case 0:
                if(*((uint8_t*)testparam) == 0x02) //STX = 0x02
                {
                    UART3RxBuffer.Data[UART3RxBuffer.WrIndex++] = (*((uint8_t*)testparam));
                    UART3RxBuffer.RxTimeout = RX3_FRAME_TOUT_SET;
                }
                else UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                break;
                
            case 1:
            case 2:
                if(isdigit(*((uint8_t*)testparam)))
                {
                    UART3RxBuffer.Data[UART3RxBuffer.WrIndex] = (*((uint8_t*)testparam));
                    if(UART3RxBuffer.WrIndex == 2)
                    {
                        memcpy(ConvStr,(uint8_t*)&UART3RxBuffer.Data[1],2);
                        UART3RxBuffer.RxCnt = atoi((char*)ConvStr);
                        if(UART3RxBuffer.RxCnt < 2) UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                    }
                    UART3RxBuffer.WrIndex++;
                }
                else UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                break;
            default:
                if(UART3RxBuffer.WrIndex < RX3_BUFF_LEN)
                {
                    if((*((uint8_t*)testparam) == 0x03) && (UART3RxBuffer.WrIndex == UART3RxBuffer.RxCnt+2)) //ETX = 0x03
                    {
                        UART3RxBuffer.Data[UART3RxBuffer.WrIndex] = (*((uint8_t*)testparam));
                        //generate command
                        CmdPtr = calloc(UART3RxBuffer.Data[UART3RxBuffer.WrIndex]+2, sizeof(uint8_t));
                        if(CmdPtr != NULL) UARTRxCmdBufferAdd (CmdPtr);
                        UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                    }
                    else UART3RxBuffer.Data[UART3RxBuffer.WrIndex++] = (*((uint8_t*)testparam));
                }
                else
                {
                    //command exceeds maximum length
                    UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                }
                break;
        }
    }
    //Setup new bute reception
    SERCOM3_USART_Read((void*)&Sercom3RxData, SERCOM3_RXDATA_LENGTH);
}

void UART3Task(void)
{
    
}
/* *****************************************************************************
 End of File
 */
