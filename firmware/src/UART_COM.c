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
#include <stdio.h>
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
const char RxMsg00[]={"UserProg = "};
const char RxMsg01[]={"FractRange = "};
const char RxMsg02[]={"RollOver = "};
const char RxMsg03[]={"LimitSW = "};
const char RxMsg04[]={"FractRng = "};
const char RxMsg05[]={"Factory MT Res = "};
const char RxMsg06[]={"Factory OFFSET = "};
const char RxMsg07[]={"IntDACLowLS = "};
const char RxMsg08[]={"IntDACLow = "};
const char RxMsg09[]={"ExtDACMAx = "};
const char RxMsg10[]={"IntDACHighLS = "};
const char RxMsg11[]={"SaveCfg"};
const char RxMsg12[]={"CancelCfg"};
const char RxMsg13[]={"ReadCfgRAM"};
const char RxMsg14[]={"ReadCfgEE"};

struct _RxMsg
{
    const char* MsgData[RX3_CMD_NUMBER];
    uint8_t MsgLen[RX3_CMD_NUMBER];
};

const struct _RxMsg RxMsg = {
                                {RxMsg00,RxMsg01,RxMsg02,RxMsg03,RxMsg04,RxMsg05,RxMsg06,RxMsg07,RxMsg08,RxMsg09,RxMsg10,RxMsg11,RxMsg12,RxMsg13,RxMsg14},
                                {strlen(RxMsg00),strlen(RxMsg01),strlen(RxMsg02),strlen(RxMsg03),strlen(RxMsg04),strlen(RxMsg05),strlen(RxMsg06),strlen(RxMsg07),strlen(RxMsg08),strlen(RxMsg09),strlen(RxMsg10),strlen(RxMsg11),strlen(RxMsg12),strlen(RxMsg13),strlen(RxMsg14)}
                            };
volatile UARTRxCmdBuffType UART3RxCmdBuffer;
static UARTTxCmdBuffType UART3TxCmdBuffer;
static uint8_t RxCmdRun = NO_CMD;
static uint8_t TxCmdCnt = 0;
volatile uint8_t TxCmdSent = 0;
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
void UART3RxInit(void)
{
    UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
    UARTRxCmdBufferInit((UARTRxCmdBuffType*) &UART3RxCmdBuffer);
    SERCOM3_USART_ReadCallbackRegister(UARTRxDataBufferAdd,(uintptr_t)((void*)&Sercom3RxData));
    SERCOM3_USART_Read((void*)&Sercom3RxData, SERCOM3_RXDATA_LENGTH);
}

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
bool UARTTxCmdBufferAdd (char* TxCmdPtr)
{
    if(UART3TxCmdBuffer.CmdCnt < TX3_CMD_BUFF_LEN)
    {
        UART3TxCmdBuffer.CmdPtr[UART3TxCmdBuffer.WrIndex++] = TxCmdPtr;
        UART3TxCmdBuffer.WrIndex %= TX3_CMD_BUFF_LEN;
        UART3TxCmdBuffer.CmdCnt++;
        return (bool)1;
    }
    return (bool)0;
}

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
    if(i < DigitLen) return false;
    else
    {
        *RegVal = atoi(ConvStr);
        return true;
    }
}

void UART3Task(void)
{
    char *CmdPtr = NULL;
    char *pTxCmd = NULL;
    uint8_t TxCmdLen = 0;
    int TempVal;
    uint8_t i=0;
    
    if (UART3RxBuffer.RxTimeout == 1) 
    {
        UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
    }
    switch (RxCmdRun)
    {
        case NO_CMD:
            if((UART3RxCmdBuffer.CmdCnt))
            {
                if(UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex] != NULL)
                {
                    CmdPtr = UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex];

                    for(i=0;i<=RX3_CMD_NUMBER;i++)
                    {
                        if(!strncmp(CmdPtr, RxMsg.MsgData[i],RxMsg.MsgLen[i])) break;
                    }
                    switch(i)
                    {
                        case 0:
                            CmdPtr += RxMsg.MsgLen[i];
                            if(!strncmp(CmdPtr,"ON",2))
                                USR_SCL_EN_WR(SCALABLE);
                            else if(!strncmp(CmdPtr,"OFF",3))
                                USR_SCL_EN_WR(NO_SCALABLE);
                            break;
                        case 1:
                            CmdPtr += RxMsg.MsgLen[i];
                            if(!strncmp(CmdPtr,"ON",2))
                                USR_SCL_FRACT_RNG_WR(USR_SCL_FRACT_RNG_USED);
                            else if(!strncmp(CmdPtr,"OFF",3))
                                USR_SCL_FRACT_RNG_WR(USR_SCL_FRACT_RNG_UNUSED);
                            break;
                        case 2:
                            CmdPtr += RxMsg.MsgLen[i];
                            if(!strncmp(CmdPtr,"ON",2))
                                USR_SCL_ROLL_OVER_WR(USR_SCL_ROLL_OVER_USED);
                            else if(!strncmp(CmdPtr,"OFF",3))
                                USR_SCL_ROLL_OVER_WR(USR_SCL_ROLL_OVER_UNUSED);
                            break;
                        case 3:
                            CmdPtr += RxMsg.MsgLen[i];
                            if(!strncmp(CmdPtr,"ON",2))
                                USR_SCL_LIMIT_SW_WR(USR_SCL_LIMIT_SW_USED);
                            else if(!strncmp(CmdPtr,"OFF",3))
                                USR_SCL_LIMIT_SW_WR(USR_SCL_LIMIT_SW_UNUSED);
                            break;
                        case 4:
                            if(UART3CmdValParse(CmdPtr,RxMsg.MsgLen[i],&TempVal))
                            {
                                if((TempVal >= FRACT_RANGE_MIN)&&(TempVal <= FRACT_RANGE_MAX))
                                {
                                    EncCfg.FractRange = (uint16_t)TempVal;
                                }
                            }
                            break;
                        case 5:
                            if(UART3CmdValParse(CmdPtr,RxMsg.MsgLen[i],&TempVal))
                            {
                                if((TempVal >= 0) && (TempVal <= FACTORY_RESOMT_MAX))
                                {
                                    EncCfg.UserSclCfg[1] = (uint8_t)TempVal;
                                }
                            }
                            break;
                        case 6:
                            if(UART3CmdValParse(CmdPtr,RxMsg.MsgLen[i],&TempVal))
                            {
                                if((TempVal >= 0) && (TempVal <= FACT_OFFSET_MAX))
                                {
                                    EncCfg.FactOffset = (uint8_t)TempVal;
                                }
                            }
                            break;
                        case 7:
                            if(UART3CmdValParse(CmdPtr,RxMsg.MsgLen[i],&TempVal))
                            {
                                if((TempVal >= 0) && (TempVal <= INT_DAC_MAX))
                                {
                                    DACAdjust = 1;
                                    EncCfg.IntDACLowLS = (uint16_t)TempVal;
                                }
                            }
                            break;
                        case 8:
                            if(UART3CmdValParse(CmdPtr,RxMsg.MsgLen[i],&TempVal))
                            {
                                if( (TempVal <= INT_DAC_MAX) && (TempVal >= EncCfg.IntDACLowLS))
                                {
                                    DACAdjust = 2;
                                    EncCfg.IntDACLow = (uint16_t)TempVal;
                                }
                            }
                            break;
                        case 9:
                            if(UART3CmdValParse(CmdPtr,RxMsg.MsgLen[i],&TempVal))
                            {
                                if(TempVal >= 0)
                                {
                                    EncCfg.ExtDACMax = (uint16_t)TempVal;
                                    DACAdjust = 3;
                                }
                            }
                            break;
                        case 10:
                            if(UART3CmdValParse(CmdPtr,RxMsg.MsgLen[i],&TempVal))
                            {
                                if((TempVal <= INT_DAC_MAX)&&(TempVal >= EncCfg.IntDACLow))
                                {
                                    EncCfg.IntDACHighLS = (uint16_t)TempVal;
                                    DACAdjust = 4;
                                }
                            }
                            break;
                        case 11:
                            DACAdjust = 0;
                            RxCmdRun = SAVE_CMD;
                            break;
                        case 12:
                            DACAdjust = 0;
                            RxCmdRun = CANCEL_CMD;
                            break;
                        case 13:
                            DACAdjust = 0;
                            RxCmdRun = READ_CFG_RAM;
                            break;
                        case 14:
                            DACAdjust = 0;
                            RxCmdRun = READ_CFG_EE;
                            break;
                        default:

                            break;
                    }
                    free(UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex]);
                    UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex] = NULL;
                }
                UART3RxCmdBuffer.RdIndex++;
                UART3RxCmdBuffer.RdIndex %= RX3_CMD_BUFF_LEN;
                UART3RxCmdBuffer.CmdCnt--;
            }
            break;
        case SAVE_CMD:
            
            break;
        case CANCEL_CMD:
            
            break;
        case READ_CFG_RAM:
            if(UART3TxCmdBuffer.CmdCnt < TX3_CMD_BUFF_LEN)
            {
                switch (TxCmdCnt)
                {
                    case 0:
                        if(USR_SCL_EN_RD == SCALABLE)
                        {
                            TxCmdLen = RxMsg.MsgLen[TxCmdCnt]+5;
                            pTxCmd = calloc(TxCmdLen,sizeof(char));
                            if(pTxCmd != NULL)
                            {
                                sprintf(pTxCmd,"\2%s%s\3",RxMsg.MsgData[TxCmdCnt],"ON");
                                UARTTxCmdBufferAdd (pTxCmd);
                                TxCmdCnt++;
                            }
                        }
                        else
                        {
                            TxCmdLen = RxMsg.MsgLen[TxCmdCnt]+6;
                            pTxCmd = calloc(TxCmdLen,sizeof(char));
                            if(pTxCmd != NULL)
                            {
                                sprintf(pTxCmd,"\2%s%s\3",RxMsg.MsgData[TxCmdCnt],"OFF");
                                UARTTxCmdBufferAdd (pTxCmd);
                                TxCmdCnt++;
                            }
                        }
                        break;
                    case 1:
                        if(USR_SCL_FRACT_RNG_RD == USR_SCL_FRACT_RNG_USED)
                        {
                            TxCmdLen = RxMsg.MsgLen[TxCmdCnt]+5;
                            pTxCmd = calloc(TxCmdLen,sizeof(char));
                            if(pTxCmd != NULL)
                            {
                                sprintf(pTxCmd,"\2%s%s\3",RxMsg.MsgData[TxCmdCnt],"ON");
                                UARTTxCmdBufferAdd (pTxCmd);
                                TxCmdCnt++;
                                TxCmdCnt = 11;
                            }
                        }
                        else
                        {
                            TxCmdLen = RxMsg.MsgLen[TxCmdCnt]+6;
                            pTxCmd = calloc(TxCmdLen,sizeof(char));
                            if(pTxCmd != NULL)
                            {
                                sprintf(pTxCmd,"\2%s%s\3",RxMsg.MsgData[TxCmdCnt],"OFF");
                                UARTTxCmdBufferAdd (pTxCmd);
                                TxCmdCnt++;
                                TxCmdCnt = 11;
                            }
                        }
                        break;

                    case 10:
                        
                        break;
                    default:
                        TxCmdCnt = 0;
                        UARTRxBufferInit((UARTRxBuffType*)&UART3RxBuffer);
                        UARTRxCmdBufferInit((UARTRxCmdBuffType*)&UART3RxCmdBuffer);
                        RxCmdRun = NO_CMD;
                        break;
                }
            }
            break;
        case READ_CFG_EE:
            
            break;
    }
    
    if(UART3TxCmdBuffer.CmdCnt)
    {
        if(UART3TxCmdBuffer.CmdPtr[UART3TxCmdBuffer.RdIndex] != NULL)
        {
            if(!SERCOM3_USART_WriteIsBusy())
            {
                if(!TxCmdSent)
                {
                    SERCOM3_USART_WriteCallbackRegister(UARTTxCmdSent,(uintptr_t)((void*)&TxCmdSent));
                    SERCOM3_USART_Write(UART3TxCmdBuffer.CmdPtr[UART3TxCmdBuffer.RdIndex],strlen(UART3TxCmdBuffer.CmdPtr[UART3TxCmdBuffer.RdIndex]));
                }
                else
                {
                    free(UART3TxCmdBuffer.CmdPtr[UART3TxCmdBuffer.RdIndex]);
                    UART3TxCmdBuffer.CmdPtr[UART3TxCmdBuffer.RdIndex++] = NULL;
                    UART3TxCmdBuffer.RdIndex %= TX3_CMD_BUFF_LEN;
                    UART3TxCmdBuffer.CmdCnt--;
                    TxCmdSent=0;
                }
            }
        }
        else
        {
            UART3TxCmdBuffer.RdIndex++;
            UART3TxCmdBuffer.RdIndex %= TX3_CMD_BUFF_LEN;
            UART3TxCmdBuffer.CmdCnt--;
            TxCmdSent=0;
        }
    }
}

void UARTTxCmdSent(uintptr_t Flag)
{
    *((uint8_t*)Flag) = 1;
}
/* *****************************************************************************
 End of File
 */