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
    {"WR "},
    {"RD "},
    {"SVC"}
};
const char UARTRegName[RX3_REG_NUMBER][RX3_REG_LENGTH]=
{
    {"USRSCEN"},
    {"FRCRGEN"},
    {"ROVEREN"},
    {"LIMSWEN"},
    {"FYRESMT"},
    {"EDACMAX"},
    {"IDACLOW"},
    {"IDACLLS"},
    {"IDACHLS"},
    ("FRACRNG"),
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
    char ConvStr[RX3_MAX_REG_DIGIT_POS];
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
                                memcpy(ConvStr,(char*)&UART3RxBuffer.Data[1],UART3RxBuffer.WrIndex-1);
                                UART3RxBuffer.RxCnt = (uint8_t)atoi(ConvStr);
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
                            if(UARTRxCmdBufferAdd(CmdPtr)) memcpy(CmdPtr,(char*)UART3RxBuffer.Data,UART3RxBuffer.WrIndex);
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

bool UART3CmdWRParse(char *pFirst,char *pLast,int* RegVal)
{
    char ConvStr[RX3_MAX_VAL_DIGIT_POS];
    uint8_t i=0;
    
    while ((pFirst+i)<pLast)
    {
        if(*(pFirst+i) == ' ') i++;
        else break;
    }
    if(*(pFirst+i) == '=')
    {
        i++;
        while ((pFirst+i)<pLast)
        {
            if(*(pFirst+i) == ' ') i++;
            else break;
        }
        pFirst+=i;
        i=0;
        while ((pFirst+i)<pLast)
        {
            if(isdigit(*(pFirst+i)))
            {
                ConvStr[i]= (*(pFirst+i));
                i++;
            }
            else break;
        }
        if((pFirst+i) >= pLast)
        {
            *RegVal = atoi(ConvStr);
            return true;
        }
        else return false;
    }
    else return false;
}

void UART3Task(void)
{
    char *CmdPtr,*pLastData;
    uint8_t CmdNum,RegNum;
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
                CmdPtr = strstr(UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex],UART3Cmd[CmdNum]);
                if(CmdPtr != NULL)
                {
                    CmdPtr += strlen(UART3Cmd[CmdNum]);
                    break;
                }
            }
            pLastData = (UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex]+strlen((char*)UART3RxCmdBuffer.CmdPtr[UART3RxCmdBuffer.RdIndex])-UART_RX3_END_SIZE);
            switch(CmdNum)
            {
                case 0:
                    for (RegNum=0;RegNum<RX3_REG_NUMBER;RegNum++)
                    {
                        if(strstr(CmdPtr,UARTRegName[RegNum]) != NULL)
                        {
                            CmdPtr = strstr(CmdPtr,UARTRegName[RegNum])+strlen(UARTRegName[RegNum]);
                            break;
                        }
                    }
                    if(RegNum < RX3_REG_NUMBER)
                    {
                        if(UART3CmdWRParse(CmdPtr,pLastData,&TempVal))
                        {
                            switch(RegNum)
                            {
                                case 0:
                                    if((uint16_t)TempVal <= 1) {USR_SCL_EN_WR(TempVal);}
                                    else{}//invalid setting value
                                    break;
                                case 1:
                                    if((uint16_t)TempVal <= 1) {USR_SCL_FRACT_RNG_WR(TempVal);}
                                    else{}//invalid setting value
                                    break;
                                case 3:
                                    if((uint16_t)TempVal <= 1) {USR_SCL_ROLL_OVER_WR(TempVal);}
                                    else{}//invalid setting value
                                    break;
                                case 4:
                                    if((uint16_t)TempVal <= 1) {USR_SCL_LIMIT_SW_WR(TempVal);}
                                    else{}//invalid setting value
                                    break;
                                case 5:
                                    if((uint16_t)TempVal <= FACTORY_RESOMT_MAX) {FACTORY_RESOMT_WR(TempVal);}
                                    else{}//invalid setting value
                                    break;
                                case 6:
                                    CommonVars.ExtDACMax = (uint16_t)TempVal;
                                    break;
                                case 7:
                                    if((uint16_t)TempVal <= INT_DAC_MAX) CommonVars.IntDACLow = (uint16_t)TempVal;
                                    else{}//invalid setting value
                                    break;
                                case 8:
                                    if((uint16_t)TempVal <= INT_DAC_MAX) CommonVars.IntDACLowLS = (uint16_t)TempVal;
                                    else{}//invalid setting value
                                    break;
                                case 9:
                                    if((uint16_t)TempVal <= INT_DAC_MAX) CommonVars.IntDACHighLS = (uint16_t)TempVal;
                                    else{}//invalid setting value
                                    break;
                                case 10:
                                    if(((uint16_t)TempVal >= FRACT_RANGE_MIN)&&((uint16_t)TempVal <= FRACT_RANGE_MAX))
                                        CommonVars.FractRange = (uint16_t)TempVal;
                                    else{}//invalid setting value
                                    break;
                                case 11:
                                    if((uint16_t)TempVal <= FACT_OFFSET_MAX) CommonVars.FactOffset = (uint16_t)TempVal;
                                    break;
                            }
                        }
                        else{}//Cmd parse failed
                    }
                    else{}//Register not found
                    break;
                case 1:
                    if(strstr(CmdPtr,"ALL") != NULL)
                    {
                        if(strstr(CmdPtr,"ALL")+strlen("ALL") == pLastData){}//Read All configurations
                        else{}//Wrong read all configurations Cmd format
                    }
                    else
                    {
                        for (RegNum=0;RegNum<RX3_REG_NUMBER;RegNum++)
                        {
                            if(strstr(CmdPtr,UARTRegName[RegNum]) != NULL)
                            {
                                CmdPtr = strstr(CmdPtr,UARTRegName[RegNum])+strlen(UARTRegName[RegNum]);
                                break;
                            }
                        }
                        if(RegNum < RX3_REG_NUMBER)
                        {
                            if(CmdPtr == pLastData)
                            {
                                switch(RegNum)
                                {
                                    case 0:
                                        //send RegNum 0
                                        break;
                                    case 1:
                                        //send RegNum 0
                                        break;
                                    case 2:
                                        //send RegNum 0
                                        break;
                                    case 3:
                                        //send RegNum 0
                                        break;
                                    case 4:
                                        //send RegNum 0
                                        break;
                                    case 5:
                                        //send RegNum 0
                                        break;
                                    case 6:
                                        //send RegNum 0
                                        break;
                                    case 7:
                                        //send RegNum 0
                                        break;
                                    case 8:
                                        //send RegNum 0
                                        break;
                                    case 9:
                                        //send RegNum 0
                                        break;
                                    case 10:
                                        //send RegNum 0
                                        break;
                                    case 11:
                                        //send RegNum 0
                                        break;
                                }
                            }
                            else{};//Cmd parse failed
                        }
                        else{};//Register not found
                    }
                    break;
                case 2:
                    
                    break;
                default:
                    //Command not found
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