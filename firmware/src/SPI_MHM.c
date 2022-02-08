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
#include "SPI_MHM.h"
#include <string.h>
#include <stdlib.h>

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
SPI_IC_MHMType* pSPI0Data;
uint8_t MHMTimer = 0;
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
void Init_IC_MHM_SPIData(SPI_IC_MHMType* IC_MHM_SPIData)
{
	IC_MHM_SPIData->TxData = (uint8_t*)malloc(IC_MHM_SPIData->TxLength * sizeof(*(IC_MHM_SPIData->TxData)));
	IC_MHM_SPIData->RxData = (uint8_t*)malloc(IC_MHM_SPIData->RxLength * sizeof(*(IC_MHM_SPIData->RxData)));
}

void DeInit_IC_MHM_SPIData(SPI_IC_MHMType* IC_MHM_SPIData)
{
	free(IC_MHM_SPIData->TxData);
	free(IC_MHM_SPIData->RxData);
}

void IC_MHM_SPIBufferInit(SPI_IC_MHMType** pIC_MHM_SPIData, uint8_t TxLength, uint8_t RxLength)
{
	SPI_IC_MHMType* ptr = malloc(sizeof(SPI_IC_MHMType));

	ptr->TxLength = TxLength;
    ptr->RxLength = RxLength;
	ptr->pInitSPIData = Init_IC_MHM_SPIData;
	ptr->pDeInitSPIData = DeInit_IC_MHM_SPIData;
	ptr->pInitSPIData (ptr);
	*pIC_MHM_SPIData = ptr;
}
void IC_MHM_SPIBufferFree(SPI_IC_MHMType** pIC_MHM_SPIData)
{
	(*pIC_MHM_SPIData)->pDeInitSPIData (*pIC_MHM_SPIData);
	free(*pIC_MHM_SPIData);
	(*pIC_MHM_SPIData) = NULL;
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */

void IC_MHMTimerTask()
{
     if( MHMTimer > 1)  MHMTimer--;
}

//TxLength does not include opcode
//RxLength includes opcode
uint8_t IC_MHM_RegAcces(uint8_t Opcode, uint8_t* pTxData, uint8_t TxLength, uint8_t* pRxData, uint8_t RxLength)
{
    static SPI_IC_MHMType* pSPIMHM = NULL;
    static uint8_t RegAccessfsm = 0;
    uint8_t result = 0;

    if(!SERCOM0_SPI_IsBusy())
    {
        switch (RegAccessfsm)
        {
            case 0:
            case 4:
                //Read Register Status
                IC_MHM_SPIBufferInit(&pSPIMHM, 1, 3);
                pSPIMHM->TxData[0] = READ_REG_STAT_OPC;
                NCS_MHM_Clear();
                SERCOM0_SPI_WriteRead (pSPIMHM->TxData, pSPIMHM->TxLength, pSPIMHM->RxData, pSPIMHM->RxLength);
                RegAccessfsm = (RegAccessfsm == 0) ? 1 : 5;
                break;

            case 1:
            case 5:
                NCS_MHM_Set();
                if(RegAccessfsm == 1)
                {
                    //Check if Register Status bit Busy is set
                    if(pSPIMHM->RxData[1] & IC_MHM_STAT_BUSY_Msk) RegAccessfsm = 0;
                    else RegAccessfsm = 2;
                }
                else
                {
                    //Check if Register Status bit Busy is set
                    if(pSPIMHM->RxData[1] & IC_MHM_STAT_BUSY_Msk) RegAccessfsm = 4;
                    else
                    {
                        RegAccessfsm = 0;
                        result = pSPIMHM->RxData[1];
                    }
                }
                IC_MHM_SPIBufferFree(&pSPIMHM);
                break;

            case 2:
                IC_MHM_SPIBufferInit(&pSPIMHM, TxLength+1, RxLength);
                pSPIMHM->TxData[0] = Opcode;
                if((TxLength) & (pTxData != NULL)) memcpy(&pSPIMHM->TxData[1], pTxData, TxLength);
                NCS_MHM_Clear();
                SERCOM0_SPI_WriteRead (pSPIMHM->TxData, pSPIMHM->TxLength, pSPIMHM->RxData, pSPIMHM->RxLength);
                RegAccessfsm = 3;
                break;

            case 3:
                NCS_MHM_Set();
                if(pSPIMHM->RxLength) memcpy(pRxData, pSPIMHM->RxData,RxLength);
                RegAccessfsm = 4;
                IC_MHM_SPIBufferFree(&pSPI0Data);
                break;
        }
    }
    return result;
}

void IC_MHM_Task()
{
    static IC_MHMfsmType IC_MHMfsm = MHM_STARTUP_1;
    static uint8_t PowerUp = 1;
    static uint8_t StartUpCnt = 0;
    static uint8_t *pTxData, *pRxData;
    uint8_t RegStat = 0;
    
    if(BISS_MASTER_Get())
    {
        NCS_MHM_Set();
        IC_MHMfsm = MHM_STARTUP_1;
    }
    else
    {
        switch (IC_MHMfsm)
        {
            case MHM_STARTUP_1:
                MHMTimer = START_UP_TIMER_SET;
                pRxData = (uint8_t*)malloc(8 * sizeof(*pRxData));
                pTxData = NULL;
                IC_MHMfsm = MHM_STARTUP_2;
                break;
            //Try to send SPI POSITION READ command to IC-MHM
            case MHM_STARTUP_2:
                if(MHMTimer == (uint8_t)1)
                {
                    if(IC_MHM_RegAcces(POS_READ_OPC, pTxData, 0, pRxData, 8*sizeof(*pRxData)) & IC_MHM_STAT_VALID_Msk) IC_MHMfsm = MHM_STARTUP_3;
                }
                break;
            //Checks if received opcode is POSTION READ and nERR and nWARN bits
            //During Start up, MISO is stuck high so No data will be received
            case MHM_STARTUP_3:
                //pRxData contains SPI Received data from IC-MHM
                if(pRxData[0] !=  POS_READ_OPC)
                {
                    if(PowerUp)
                    {
                        //To Do EEPROM LOAD FAIL
                        PowerUp = 0;
                    }
                    IC_MHMfsm = READ_POS_1;
                }
                else
                {
                    if(PowerUp)
                    {
                        //To Do EERPOM LOAD SUCCEED
                        PowerUp = 0;
                    }
                    //During Start Up, nWARN and nERR are reset. 
                    if(!pRxData[7])
                    {
                        if(StartUpCnt < 2)
                        {
                            StartUpCnt++;
                            IC_MHMfsm = MHM_STARTUP_4;
                        }
                        else
                        {
                            //To Do START UP ERROR 
                            IC_MHMfsm = READ_POS_1;
                        }
                    }
                    else
                    {
                        //To Do START UP SUCCEED
                        IC_MHMfsm = READ_POS_1;
                    }
                }
                free(pRxData);
                pRxData = NULL;
                break;
                
            case MHM_STARTUP_4:
                pRxData = malloc(sizeof(*pRxData)*4);
                pTxData = malloc(sizeof(*pRxData)*1);
                *pTxData = IC_MHM_PRES_RES_REG;
                IC_MHMfsm = MHM_STARTUP_5;
                break;
            case MHM_STARTUP_5:
                if(IC_MHM_RegAcces(REG_RD_CTD_OPC, pTxData, sizeof(*pTxData)*1, pRxData, sizeof(*pRxData)*4) & IC_MHM_STAT_VALID_Msk)
                {
                    free(pTxData);
                    pTxData = malloc(sizeof(*pTxData)*2);
                    memcpy(pTxData, &pRxData[2], 2);
                    free(pRxData);
                    pRxData = NULL;
                    pTxData[1] |= IC_MHM_0x75_FIO_3_Msk;
                    IC_MHMfsm = MHM_STARTUP_6;
                }
                break;

            case MHM_STARTUP_6:
                if(IC_MHM_RegAcces(WR_INST_OPC, pTxData, sizeof(*pTxData)*2, pRxData, 0) & IC_MHM_STAT_VALID_Msk)
                {
                    free(pTxData);
                    pTxData = NULL;
                    MHMTimer = PV_PRESET_TIMER_SET;
                    IC_MHMfsm = MHM_STARTUP_7;
                }
                break;
                
            case MHM_STARTUP_7:
                if(MHMTimer ==1)
                {
                    pRxData = malloc(sizeof(*pRxData)*4);
                    pTxData = malloc(sizeof(*pRxData)*1);
                    *pTxData = IC_MHM_PRES_RES_REG;
                    IC_MHMfsm = MHM_STARTUP_8;
                }
                break;    
                
            case MHM_STARTUP_8:
                if(IC_MHM_RegAcces(REG_RD_CTD_OPC, pTxData, sizeof(*pTxData)*1, pRxData, sizeof(*pRxData)*4) & IC_MHM_STAT_VALID_Msk)
                {
                    free(pTxData);
                    pTxData = malloc(sizeof(*pTxData)*2);
                    memcpy(pTxData, &pRxData[2], 2);
                    free(pRxData);
                    pRxData = NULL;
                    pTxData[0] |= IC_MHM_0x74_RESET_Msk;
                    pTxData[1] &= ~(IC_MHM_0x75_FIO_3_Msk);
                    IC_MHMfsm = MHM_STARTUP_9;
                }
                break;
                
            case MHM_STARTUP_9:
                if(IC_MHM_RegAcces(WR_INST_OPC, pTxData, sizeof(*pTxData)*2, pRxData, 0) & IC_MHM_STAT_VALID_Msk)
                {
                    free(pTxData);
                    pTxData = NULL;
                    IC_MHMfsm = MHM_STARTUP_1;
                }
                break;

            case READ_POS_1:
                MHMTimer = READ_POS_TIMER_SET;
                IC_MHMfsm = READ_POS_2;
                break;
                
            case READ_POS_2:
                pRxData = (uint8_t*)malloc(8 * sizeof(*pRxData));
                pTxData = NULL;
                IC_MHMfsm = READ_POS_3;
                break;
            case READ_POS_3:
                if(MHMTimer == (uint8_t)1)
                {
                    MHMTimer = READ_POS_TIMER_SET;
                    IC_MHMfsm = READ_POS_4;
                }
                break;
            case READ_POS_4:
                RegStat = IC_MHM_RegAcces(POS_READ_OPC, pTxData, 0, pRxData, 8*sizeof(*pRxData));
                if(RegStat & IC_MHM_STAT_VALID_Msk) IC_MHMfsm = READ_POS_5;
                else if (RegStat & (IC_MHM_STAT_FAIL_Msk | IC_MHM_STAT_DISMISS_Msk |IC_MHM_STAT_ERROR_Msk)) IC_MHMfsm = READ_POS_3;                
                break;
            case READ_POS_5:
                if(!(pRxData[7] & IC_MHM_SPI_nERR_Msk)) 
                {
                    //nERR bit detectec
                    free(pRxData);
                    pRxData = NULL;
                    IC_MHMfsm = READ_STATUS_1;
                }
                else
                {
                    //Check for nWARN bit
                    if(!(pRxData[7] & IC_MHM_SPI_nWARN_Msk))
                    {
                        //nWARN bit detectec, excessive rotor speed
                    }
                    //Read MT Position from pRxData[1] to pRxData[4]
                    //Read ST Position from pRxData[5] to pRxData[6]
                    IC_MHMfsm = READ_POS_3;
                }
                break;
                
            case READ_STATUS_1:
                pRxData = (uint8_t*)malloc(3 * sizeof(*pRxData));
                pTxData = NULL;
                IC_MHMfsm = READ_STATUS_2;
                break;
                
            case READ_STATUS_2:
                RegStat = IC_MHM_RegAcces(READ_STATUS_OPC, pTxData, 0, pRxData, 3*sizeof(*pRxData));
                if(RegStat & IC_MHM_STAT_VALID_Msk) IC_MHMfsm = READ_STATUS_3;
                else if (RegStat & (IC_MHM_STAT_FAIL_Msk | IC_MHM_STAT_DISMISS_Msk |IC_MHM_STAT_ERROR_Msk)) IC_MHMfsm = READ_STATUS_2;                
                break;
                
            case READ_STATUS_3:
                //Check Status Register 1, address 0x70
                if(pSPI0Data->RxData[1])
                {
                    //error detected
                }
                else IC_MHMfsm = READ_POS_2;
                free(pRxData);
                pRxData = NULL;
                break;
        }
    }
}



/* *****************************************************************************
 End of File
 */
