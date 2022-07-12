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
#include "SPI_MHM.h"
#include "UART_COM.h"
/* TODO:  Include other files here if needed. */
const char UART3Cmd[RX3_CMD_NUMBER][RX3_CMD_LENGTH] = 
{
    {"WR"},
    {"RD"},
    {"ST"}
};
const char UARTRegName[RX3_REG_NUMBER][RX3_REG_LENGTH]=
{
    {"USR_SCL"},
    {"FRACTRG"},
    {"ROLOVER"},
    {"LIMITSW"},
    {"F_MTRES"},
    {"EDACMAX"},
    {"IDACLOW"},
    {"IDACLLS"},
    {"IDACHLS"},
    {"FOFFSET"}
};
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

uint8_t UARTRxCmdBufferAdd (uint8_t* RxCmdPtr)
{
    if(UART3RxCmdBuffer.CmdCnt < RX3_CMD_BUFF_LEN)
    {
        UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.WrIndex++] = RxCmdPtr;
        UART3RxCmdBuffer.WrIndex %= RX3_CMD_BUFF_LEN;
        UART3RxCmdBuffer.CmdCnt++;
        return 1;
    }
    return 0;
}

void UARTRxDataBufferAdd(uintptr_t testparam)
{
    uint8_t ConvStr[RX3_MAX_REG_DIGIT_POS];
    uint8_t* CmdPtr = NULL;

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
            default:
                if(UART3RxBuffer.WrIndex < RX3_BUFF_LEN)
                {
                    if(!UART3RxBuffer.RxCnt)
                    {
                        if(isdigit(*((uint8_t*)testparam)))
                        {
                            UART3RxBuffer.Data[UART3RxBuffer.WrIndex++] = (*((uint8_t*)testparam));
                        }
                        else
                        {
                            if((UART3RxBuffer.WrIndex > 1)&&(UART3RxBuffer.WrIndex <= RX3_MAX_REG_DIGIT_POS))
                            {
                                memcpy(ConvStr,(uint8_t*)&UART3RxBuffer.Data[1],UART3RxBuffer.WrIndex-1);
                                UART3RxBuffer.RxCnt = (uint8_t)atoi((char*)ConvStr);
                                if(UART3RxBuffer.RxCnt < 1) UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                                else UART3RxBuffer.Data[UART3RxBuffer.WrIndex++] = (*((uint8_t*)testparam));
                            }
                            else UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                        }
                    }
                    else if((*((uint8_t*)testparam) == 0x03) && (UART3RxBuffer.WrIndex == UART3RxBuffer.RxCnt+1)) //ETX = 0x03
                    {
                        UART3RxBuffer.Data[UART3RxBuffer.WrIndex++] = (*((uint8_t*)testparam));
                        CmdPtr = calloc(UART3RxBuffer.WrIndex+1, sizeof(uint8_t));
                        if(CmdPtr != NULL)
                        {
                            if(UARTRxCmdBufferAdd(CmdPtr)) memcpy(CmdPtr,(uint8_t*)UART3RxBuffer.Data,UART3RxBuffer.WrIndex);
                            else free(CmdPtr);
                        }
                        UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                    }
                    else UART3RxBuffer.Data[UART3RxBuffer.WrIndex++] = (*((uint8_t*)testparam));
                }
                else UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                break;
        }
    }
    //Setup new byte reception
    SERCOM3_USART_Read((void*)&Sercom3RxData, SERCOM3_RXDATA_LENGTH);
}

void UART3Task(void)
{
    uint8_t* CmdPtr;
    uint8_t CmdNum,RegNum,i;
    uint8_t ConvStr[RX3_MAX_VAL_DIGIT_POS];
    int TempVal;
    
    if (UART3RxBuffer.RxTimeout == 1) 
    {
        //Rx timeout expired
        UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
    }
    
    if(UART3RxCmdBuffer.CmdCnt)
    {
        if(UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex] != NULL)
        {
            for (CmdNum=0;CmdNum<RX3_CMD_NUMBER;CmdNum++)
            {
                CmdPtr = (uint8_t*)strstr((char*)UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex],UART3Cmd[CmdNum]);
                if(CmdPtr != NULL)break;
            }
            switch(CmdNum)
            {
                case 0:
                    for (RegNum=0;RegNum<RX3_REG_NUMBER;RegNum++)
                    {
                        CmdPtr = (uint8_t*)strstr((char*)UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex],UART3Cmd[RegNum]);
                        if(CmdPtr != NULL)break;
                    }
                    CmdPtr += strlen(UART3Cmd[RegNum]);
                    if(*CmdPtr == '=')
                    {
                        CmdPtr++;
                        i=0;
                        while ((CmdPtr+i) < (UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex]+strlen((char*)(UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex]))-1))
                        {
                            if(isdigit(*(CmdPtr+i)))
                            {
                                ConvStr[i]= (*(CmdPtr+i));
                                i++;
                            }
                            else break;
                        }
                        if((CmdPtr+i) >= (UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex]+strlen((char*)(UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex]))-1))
                        {
                            if(i)
                            {
                                TempVal = atoi((char*)ConvStr);
                                switch (RegNum)
                                {
                                    case 0:
                                        if(TempVal < 2) USR_SCL_EN_WR(TempVal);
//                                        else
//                                        {
//                                            //invalid setting value
//                                        }
                                        break;
                                    default:
                                    
                                        break;
                                }
                            }
//                            else
//                            {
//                                //cmd size error, wrong cmd format
//                            }
                        }
//                        else
//                        {
//                            //non digit found in register value, wrong cmd format
//                        }
                    }
//                    else
//                    {
//                        //= character not found, wrong cmd format
//                    }
                    break;
                case 1:
                    
                    break;
                case 2:
                    
                    break;
                default:
                    //command not found
                    break;
            }
            free(UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex]);
        }
        UART3RxCmdBuffer.RdIndex++;
        UART3RxCmdBuffer.RdIndex %= RX3_CMD_BUFF_LEN;
        UART3RxCmdBuffer.CmdCnt--;
    }
}
/* *****************************************************************************
 End of File
 */