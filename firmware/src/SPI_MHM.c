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
static IC_MHM_REG_ACCType* pMHMRegAccData = NULL;
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

void InitMHMRegAccData(IC_MHM_REG_ACCType* MHMRegAccData)
{
	MHMRegAccData->TxData = (uint8_t*)malloc(MHMRegAccData->TxLength * sizeof(*(MHMRegAccData->TxData)));
	MHMRegAccData->RxData = (uint8_t*)malloc(MHMRegAccData->RxLength * sizeof(*(MHMRegAccData->RxData)));
}

void DeInitMHMRegAccData(IC_MHM_REG_ACCType* MHMRegAccData)
{
	free(MHMRegAccData->TxData);
	free(MHMRegAccData->RxData);
}

void MHMRegAccBufferInit(IC_MHM_REG_ACCType** pMHMRegAccData, uint8_t Opcode, uint8_t TxLength, uint8_t RxLength)
{
	IC_MHM_REG_ACCType* ptr = malloc(sizeof(IC_MHM_REG_ACCType));

    ptr->Result = (uint8_t)0;
	ptr->TxLength = TxLength;
    ptr->RxLength = RxLength;
	ptr->pInitMHMRegAccData = InitMHMRegAccData;
	ptr->pDeInitMHMRegAccData = DeInitMHMRegAccData;
	ptr->pInitMHMRegAccData (ptr);
    *(ptr->TxData) = Opcode;
	*pMHMRegAccData = ptr;
}
void MHMRegAccBufferFree(IC_MHM_REG_ACCType** pMHMRegAccData)
{
	(*pMHMRegAccData)->pDeInitMHMRegAccData (*pMHMRegAccData);
	free(*pMHMRegAccData);
	(*pMHMRegAccData) = NULL;
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
//uint8_t IC_MHM_RegAcces(uint8_t Opcode, uint8_t* pTxData, uint8_t TxLength, uint8_t* pRxData, uint8_t RxLength)
void IC_MHM_RegAccesTask()
{
    static SPI_IC_MHMType* pSPIMHM = NULL;
    static uint8_t RegAccessfsm = 0;

    if(!SERCOM0_SPI_IsBusy() && pMHMRegAccData != NULL)
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
                        pMHMRegAccData->Result = pSPIMHM->RxData[1];
                    }
                }
                IC_MHM_SPIBufferFree(&pSPIMHM);
                break;

            case 2:
                IC_MHM_SPIBufferInit(&pSPIMHM, pMHMRegAccData->TxLength, pMHMRegAccData->RxLength);
                memcpy(pSPIMHM->TxData, pMHMRegAccData->TxData, pSPIMHM->TxLength);
                NCS_MHM_Clear();
                SERCOM0_SPI_WriteRead (pSPIMHM->TxData, pSPIMHM->TxLength, pSPIMHM->RxData, pSPIMHM->RxLength);
                RegAccessfsm = 3;
                break;

            case 3:
                NCS_MHM_Set();
                if(pSPIMHM->RxLength) memcpy(pMHMRegAccData->RxData, pSPIMHM->RxData, pSPIMHM->RxLength);
                RegAccessfsm = 4;
                IC_MHM_SPIBufferFree(&pSPIMHM);
                break;
        }
    }
}

void IC_MHM_Task()
{
    static IC_MHMfsmType IC_MHMfsm = MHM_STARTUP_1;
    static uint8_t PowerUp = 1;
    static uint8_t StartUpCnt = 0;
//    static uint8_t *pTxData, *pRxData;

    
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
                IC_MHMfsm = MHM_STARTUP_2;
                break;
            //Try to send SPI POSITION READ command to IC-MHM
            case MHM_STARTUP_2:
                if(MHMTimer == (uint8_t)1)
                {
                    MHMRegAccBufferInit(&pMHMRegAccData, POS_READ_OPC, 1, 8);
                    IC_MHMfsm = MHM_STARTUP_3;
                }
                break;
            //Checks if received opcode is POSTION READ and nERR and nWARN bits
            //During Start up, MISO is stuck high so No data will be received
            case MHM_STARTUP_3:
                if(pMHMRegAccData->Result & IC_MHM_STAT_VALID_Msk)
                {
                    //RxData contains SPI Received data from IC-MHM
                    if(pMHMRegAccData->RxData[0] !=  POS_READ_OPC)
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
                        if(!(pMHMRegAccData->RxData[7]))
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
                }
                MHMRegAccBufferFree(&pMHMRegAccData);
                break;
                
            case MHM_STARTUP_4:
            case MHM_STARTUP_7:
                if(( (MHMTimer == 1) && (IC_MHMfsm == MHM_STARTUP_7) ) || (IC_MHMfsm == MHM_STARTUP_4))
                {
                    MHMRegAccBufferInit(&pMHMRegAccData, REG_RD_CTD_OPC, 2, 4);
                    pMHMRegAccData->TxData[1] = IC_MHM_PRES_RES_REG;
                    if(IC_MHMfsm == MHM_STARTUP_4) IC_MHMfsm = MHM_STARTUP_5;
                    else IC_MHMfsm = MHM_STARTUP_8;
                }
                break;
            case MHM_STARTUP_5:
            case MHM_STARTUP_8:
                if(pMHMRegAccData->Result & IC_MHM_STAT_VALID_Msk)
                {
                    free(pMHMRegAccData->TxData);
                    pMHMRegAccData->TxLength = 3;
                    pMHMRegAccData->TxData = (uint8_t*)malloc(sizeof(*(pMHMRegAccData->TxData)) * pMHMRegAccData->TxLength);
                    pMHMRegAccData->TxData[0] = WR_INST_OPC;
                    memcpy(&pMHMRegAccData->TxData[1], &pMHMRegAccData->RxData[2], 2);
                    if(IC_MHMfsm == MHM_STARTUP_5)
                    {
                        pMHMRegAccData->TxData[2] |= IC_MHM_0x75_FIO_3_Msk;
                        IC_MHMfsm = MHM_STARTUP_6;
                    }
                    else
                    {
                        pMHMRegAccData->TxData[1] |= IC_MHM_0x74_RESET_Msk;
                        pMHMRegAccData->TxData[2] &= ~(IC_MHM_0x75_FIO_3_Msk);
                        IC_MHMfsm = MHM_STARTUP_9;
                    }
                    free(pMHMRegAccData->RxData);
                    pMHMRegAccData->RxLength = 0;
                    pMHMRegAccData->RxData = NULL;
                }
                break;

            case MHM_STARTUP_6:
                if(pMHMRegAccData->Result & IC_MHM_STAT_VALID_Msk)
                {
                    MHMRegAccBufferFree(&pMHMRegAccData);
                    MHMTimer = PV_PRESET_TIMER_SET;
                    IC_MHMfsm = MHM_STARTUP_7;
                }
                break;

            case MHM_STARTUP_9:
                if(pMHMRegAccData->Result & IC_MHM_STAT_VALID_Msk)
                {
                    MHMRegAccBufferFree(&pMHMRegAccData);
                    IC_MHMfsm = MHM_STARTUP_1;
                }
                break;

            case READ_POS_1:
                MHMTimer = READ_POS_TIMER_SET;
                IC_MHMfsm = READ_POS_2;
                break;
                
            case READ_POS_2:
                if(MHMTimer == (uint8_t)1)
                {
                    MHMTimer = READ_POS_TIMER_SET;
                    MHMRegAccBufferInit(&pMHMRegAccData, POS_READ_OPC, 1, 8);
                    IC_MHMfsm = READ_POS_3;
                }
                break;
            case READ_POS_3:
                if(pMHMRegAccData->Result & IC_MHM_STAT_VALID_Msk) IC_MHMfsm = READ_POS_4;
                else if(pMHMRegAccData->Result & (IC_MHM_STAT_FAIL_Msk | IC_MHM_STAT_DISMISS_Msk |IC_MHM_STAT_ERROR_Msk))
                {
                    MHMRegAccBufferFree(&pMHMRegAccData);
                    IC_MHMfsm = READ_POS_2;                
                }
                break;
            case READ_POS_4:
                if(!(pMHMRegAccData->RxData[7] & IC_MHM_SPI_nERR_Msk)) 
                {
                    //nERR bit detectec
                    IC_MHMfsm = READ_STATUS_1;
                }
                else
                {
                    //Check for nWARN bit
                    if(!(pMHMRegAccData->RxData[7] & IC_MHM_SPI_nWARN_Msk))
                    {
                        //nWARN bit detectec, excessive rotor speed
                    }
                    //Read MT Position from pRxData[1] to pRxData[4]
                    //Read ST Position from pRxData[5] to pRxData[6]
                    IC_MHMfsm = READ_POS_2;
                }
                MHMRegAccBufferFree(&pMHMRegAccData);
                break;
                
            case READ_STATUS_1:
                MHMRegAccBufferInit(&pMHMRegAccData, READ_STATUS_OPC, 1, 3);
                IC_MHMfsm = READ_STATUS_2;
                break;
                
            case READ_STATUS_2:
                if(pMHMRegAccData->Result & IC_MHM_STAT_VALID_Msk) IC_MHMfsm = READ_STATUS_3;
                else if (pMHMRegAccData->Result & (IC_MHM_STAT_FAIL_Msk | IC_MHM_STAT_DISMISS_Msk |IC_MHM_STAT_ERROR_Msk)) IC_MHMfsm = READ_STATUS_2;                
                break;
                
            case READ_STATUS_3:
                //Check Status Register 1, address 0x70
                if(pMHMRegAccData->RxData[1])
                {
                    //error detected
                }
                else IC_MHMfsm = READ_POS_2;
                MHMRegAccBufferFree(&pMHMRegAccData);
                break;
        }
    }
}



/* *****************************************************************************
 End of File
 */
