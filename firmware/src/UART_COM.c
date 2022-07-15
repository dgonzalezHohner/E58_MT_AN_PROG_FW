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

bool UARTRxCmdBufferAdd (char* RxCmdPtr)
{
    if(UART3RxCmdBuffer.CmdCnt < RX3_CMD_BUFF_LEN)
    {
        UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.WrIndex++] = RxCmdPtr;
        UART3RxCmdBuffer.WrIndex %= RX3_CMD_BUFF_LEN;
        UART3RxCmdBuffer.CmdCnt++;
        return (bool)1;
    }
    return (bool)0;
}

void UARTRxDataBufferAdd(uintptr_t testparam)
{
    char* CmdPtr = NULL;

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
                    if((*((uint8_t*)testparam) == 0x03) /*&& (UART3RxBuffer.RxCnt == 1)*/) //ETX = 0x03
                    {
                        UART3RxBuffer.Data[UART3RxBuffer.WrIndex++] = (*((uint8_t*)testparam));
                        CmdPtr = calloc(UART3RxBuffer.WrIndex-1, sizeof(uint8_t));
                        if(CmdPtr != NULL)
                        {
                            if(UARTRxCmdBufferAdd(CmdPtr)) memcpy(CmdPtr,(char*)&UART3RxBuffer.Data[1],UART3RxBuffer.WrIndex-2);
                            else free(CmdPtr);
                        }
                        UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                    }
                    else
                    {
                        UART3RxBuffer.Data[UART3RxBuffer.WrIndex++] = (*((uint8_t*)testparam));
                    }
                }
                else UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                break;
        }
    }
    //Setup new byte reception
    SERCOM3_USART_Read((void*)&Sercom3RxData, SERCOM3_RXDATA_LENGTH);
}

bool UART3CmdValParse(char *pRxFrame,uint8_t CmdLen, int* RegVal)
{
    uint8_t DigitLen = strlen(pRxFrame)-CmdLen-UART_RX3_CRC_BYTES;
    char ConvStr[DigitLen+1];
    uint8_t i = 0;

    memset(ConvStr,0,DigitLen+1);
    pRxFrame += CmdLen;
    for (i=0;i<DigitLen;i++)
    {
        if(isdigit(*(pRxFrame+i))) ConvStr[i]= (*(pRxFrame+i));
        else break;
    }
    if(i < DigitLen)return false;
    else
    {
        *RegVal = atoi(ConvStr);
        return true;
    }
}

void UART3Task(void)
{
    char *CmdPtr;
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
            CmdPtr = UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex];
            if(!strncmp(CmdPtr, "UserProg ON", strlen("UserProg ON")))              USR_SCL_EN_WR(1);
            else if(!strncmp(CmdPtr, "UserProg OFF", strlen("UserProg OFF")))       USR_SCL_EN_WR(0);
            else if(!strncmp(CmdPtr, "FractRange ON", strlen("FractRange ON")))     USR_SCL_FRACT_RNG_WR(1);
            else if(!strncmp(CmdPtr, "FractRange OFF", strlen("FractRange OFF")))   USR_SCL_FRACT_RNG_WR(0);
            else if(!strncmp(CmdPtr, "RollOver ON", strlen("RollOver ON")))         USR_SCL_ROLL_OVER_WR(1);
            else if(!strncmp(CmdPtr, "RollOver OFF", strlen("RollOver OFF")))       USR_SCL_ROLL_OVER_WR(0);
            else if(!strncmp(CmdPtr, "LimitSW ON", strlen("LimitSW ON")))           USR_SCL_LIMIT_SW_WR(1);
            else if(!strncmp(CmdPtr, "LimitSW OFF", strlen("LimitSW OFF")))         USR_SCL_LIMIT_SW_WR(0);
            else if(!strncmp(CmdPtr, "FractRng = ", strlen("FractRng = ")))
            {
                if(UART3CmdValParse(CmdPtr,strlen("FractRng = "),&TempVal))
                {
                    if(((uint16_t)TempVal >= FRACT_RANGE_MIN)&&((uint16_t)TempVal <= FRACT_RANGE_MAX))
                        CommonVars.FractRange = (uint16_t)TempVal;
                    else{}//Value out of range
                }
                else{}//invalid value
                
            }
            else if(!strncmp(CmdPtr, "Factory MT Res = ", strlen("Factory MT Res = ")))
            {
                if(UART3CmdValParse(CmdPtr,strlen("Factory MT Res = "),&TempVal))
                {
                    if((uint16_t)TempVal <= FACTORY_RESOMT_MAX) FACTORY_RESOMT_WR(TempVal);
                    else{}//Value too high
                }
                else{}//invalid value
            }
            else if(!strncmp(CmdPtr, "Factory OFFSET = ", strlen("Factory OFFSET = ")))
            {
                if(UART3CmdValParse(CmdPtr,strlen("Factory OFFSET = "),&TempVal))
                {
                    if((uint16_t)TempVal <= FACT_OFFSET_MAX) CommonVars.FactOffset = (uint16_t)TempVal;
                    else{}//Value too high
                }
                else{}//invalid value
            }
            else if(!strncmp(CmdPtr, "IntDACLowLS = ", strlen("IntDACLowLS = ")))
            {
                if(UART3CmdValParse(CmdPtr,strlen("IntDACLowLS = "),&TempVal))
                {
                    if((uint16_t)TempVal <= INT_DAC_MAX)
                    {
                        //Activar flag per traslladar el valor a la sortida
                        CommonVars.IntDACLowLS = (uint16_t)TempVal;
                    }
                    else{}//Value too high
                }
                else{}//invalid value
            }
            else if(!strncmp(CmdPtr, "IntDACLow = ", strlen("IntDACLow = ")))
            {
                if(UART3CmdValParse(CmdPtr,strlen("IntDACLow = "),&TempVal))
                {
                    if((uint16_t)TempVal <= INT_DAC_MAX)
                    {
                        //Activar flag per traslladar el valor a la sortida
                        CommonVars.IntDACLow = (uint16_t)TempVal;
                    }
                    else{}//Value too high
                }
                else{}//invalid value
            }
            else if(!strncmp(CmdPtr, "ExtDACMAx = ", strlen("ExtDACMAx = ")))
            {
                if(UART3CmdValParse(CmdPtr,strlen("ExtDACMAx = "),&TempVal))
                {
                    //Activar flag per traslladar el valor a la sortida
                    CommonVars.ExtDACMax = (uint16_t)TempVal;
                }
                else{}//invalid value
            }
            else if(!strncmp(CmdPtr, "IntDACHighLS = ", strlen("IntDACHighLS = ")))
            {
               if(UART3CmdValParse(CmdPtr,strlen("IntDACHighLS = "),&TempVal))
                {
                    if((uint16_t)TempVal <= INT_DAC_MAX)
                    {
                        //Activar flag per traslladar el valor a la sortida
                        CommonVars.IntDACHighLS = (uint16_t)TempVal;
                    }
                    else{}//Value too high
                }
                else{}//invalid value
            }
            else if(!strncmp(CmdPtr, "SaveCfg", strlen("SaveCfg")))
            {

            }
            else if(!strncmp(CmdPtr, "CancelCfg", strlen("CancelCfg")))
            {

            }
            else if(!strncmp(CmdPtr, "ReadCfgRAM", strlen("ReadCfgRAM")))
            {

            }
            else if(!strncmp(CmdPtr, "ReadCfgEE", strlen("ReadCfgEE")))
            {

            }
            else {}            
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