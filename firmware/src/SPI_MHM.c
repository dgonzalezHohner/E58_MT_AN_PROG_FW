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
IC_MHMfsmType IC_MHMfsm = READ_REG_STAT_1;
uint8_t MHMTimer = 0, AuxTimer = 0;
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
void IC_MCB_SPIBufferFree(SPI_IC_MHMType** pIC_MHM_SPIData)
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
     if( AuxTimer > 1)  AuxTimer--;
}

uint8_t SPI0SendCMD(SPI_IC_MHMType* ptr)
{
    static uint8_t SendCMDfsm = 0;
    uint8_t SendResult = SPI0_BUSY;
    
    switch (SendCMDfsm)
    {
        case 0:
            NCS_MHM_Clear();
            if (SERCOM0_SPI_WriteRead (ptr->TxData, ptr->TxLength, ptr->RxData, ptr->RxLength) == true)
            {
                SendCMDfsm++;
                SendResult = SPI0_CMD_SENDING;
            }
            else
            {
                NCS_MHM_Set();
                SendResult = SPI0_BUSY;
            }
            break;
        case 1:
            if(SERCOM0_SPI_IsBusy()) SendResult = SPI0_BUSY;
            else
            {
                SendResult = SPI0_CMD_SENT;
                SendCMDfsm = 0;
                NCS_MHM_Set();
            }
            break;
    }
    return SendResult;
}

uint8_t IC_MHMCmd(uint8_t Opcode, uint8_t* pData, uint8_t TxLength, uint8_t RxLength)
{
    if(!SERCOM0_SPI_IsBusy())
    {
        IC_MHM_SPIBufferInit(&pSPI0Data, TxLength+1, RxLength+1);
        pSPI0Data->TxData[0] = Opcode;
        if(TxLength & (pData != NULL)) memcpy(&pSPI0Data->TxData[1], pData, TxLength);
        if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENDING)
            return SPI0_CMD_SENDING;
        else
        {
            IC_MCB_SPIBufferFree(&pSPI0Data);
            return SPI0_BUSY;
        }
    }
    else
        return SPI0_BUSY;
}

uint8_t IC_MHM_Preset_IC_PV()
{
    static uint8_t PresetPVfsm = 0;
    uint8_t result = 0;
    static uint8_t* pTxData = NULL;
    static uint8_t* pRxData = NULL;
    
    switch (PresetPVfsm)
    {
        case 0:
            //Read Register Status
            if(IC_MHM_READ_REG_STAT == SPI0_CMD_SENDING) PresetPVfsm = 1;
            break;
            
        case 1:
            //Wait for last SPI command is sent.
            if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENT)
            {
                //Check if Register Status bit Busy is set
                if(pSPI0Data->RxData[1] & IC_MHM_STAT_BUSY_Msk) PresetPVfsm = 0;
                else PresetPVfsm = 2;
                IC_MCB_SPIBufferFree(&pSPI0Data);
            }
            break;
            
        case 2:
            pTxData = (uint8_t*)malloc(1 * sizeof(*pTxData));
            pRxData = (uint8_t*)malloc(2 * sizeof(*pTxData));
            *pTxData = 0x74;    //First instruction register address in IC-MHM
            PresetPVfsm = 3;
            break;
            
        case 3:
            //Read instruction registers 1 and 2 to get output settings
            if(IC_MHMCmd(REG_RD_CTD, pTxData, 1, 2) == SPI0_CMD_SENDING) PresetPVfsm = 4;
            break;
            
        case 4:
            //Wait for last SPI command is sent.
            if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENT)
            {
                //Copy Instruction Registers read to pointer pRxData
                memcpy(pRxData, &pSPI0Data->RxData[1],2);
                free(pTxData);
                IC_MCB_SPIBufferFree(&pSPI0Data);
                PresetPVfsm = 5;
            }
            break;
        case 5:
            //Read Register Status
            if(IC_MHM_READ_REG_STAT == SPI0_CMD_SENDING) PresetPVfsm = 6;
            break;
        case 6:
            //Wait for last SPI command is sent.
            if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENT)
            {
                //Check if data requested failed, wrong read of instruction registers
                if(pSPI0Data->RxData[1] & IC_MHM_STAT_FAIL_Msk)
                {
                    free(pRxData);
                    PresetPVfsm = 0;
                }
                //Check if IC-MHM is busy at the moment
                else if (pSPI0Data->RxData[1] & IC_MHM_STAT_BUSY_Msk) IC_MHMfsm = PresetPVfsm = 5;
                //Check if last requested data is valid
                else if (pSPI0Data->RxData[1] & IC_MHM_STAT_VALID_Msk) IC_MHMfsm = PresetPVfsm = 7;
                IC_MCB_SPIBufferFree(&pSPI0Data);
            }
            break;
        case 7:
            pTxData = (uint8_t*)malloc(2 * sizeof(*pTxData));
            memcpy(pTxData,pRxData,2);
            if(!AuxTimer) pTxData[1] |= IC_MHM_0x75_FIO_3_Msk; //Set FIO3 -> PV PRESET pin
            else pTxData[1] &= ~(IC_MHM_0x75_FIO_3_Msk); //Clear FIO3 -> PV PRESET pin
            free(pRxData);
            PresetPVfsm = 8;
            break;
            
        case 8:
            if(IC_MHMCmd(WR_INST, pTxData, 2, 0) == SPI0_CMD_SENDING) PresetPVfsm = 9;
            break;
        
        case 9:
            //Wait for last SPI command is sent.
            if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENT)
            {
                PresetPVfsm = 10;
                IC_MCB_SPIBufferFree(&pSPI0Data);
            }
            break;
        case 10:
            //Read Register Status
            if(IC_MHM_READ_REG_STAT == SPI0_CMD_SENDING) PresetPVfsm = 11;
            break;
        case 11:
            //Wait for last SPI command is sent.
            if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENT)
            {
                //Check if data requested failed, wrong read of instruction registers
                if(pSPI0Data->RxData[1] & IC_MHM_STAT_FAIL_Msk) PresetPVfsm = 8;
                //Check if IC-MHM is busy at the moment
                else if (pSPI0Data->RxData[1] & IC_MHM_STAT_BUSY_Msk) IC_MHMfsm = PresetPVfsm = 10;
                //Check if last requested data is valid
                else if (pSPI0Data->RxData[1] & IC_MHM_STAT_VALID_Msk)
                {
                    if(!AuxTimer)
                    {
                        IC_MHMfsm = PresetPVfsm = 12;
                        AuxTimer = PV_PRESET_TIMER_SET;
                    }
                    else if (AuxTimer == 1)
                    {
                        free(pTxData);
                        IC_MHMfsm = PresetPVfsm = 0;
                        result = 1;
                    }
                }
                //Free SPI0 Buffer
                IC_MCB_SPIBufferFree(&pSPI0Data);
            }
            break;
        case 12:
            if(AuxTimer == (uint8_t)1)
            {
                free(pTxData);
                IC_MHMfsm = PresetPVfsm = 0;
            }
            break;
    }
    return result;
}
void IC_MHM_Task()
{
    static uint8_t PowerUp = 1;
    static uint8_t StartUpCnt = 0;
    
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
                    if(IC_MHM_POS_READ == SPI0_CMD_SENDING) IC_MHMfsm = MHM_STARTUP_3;
                }
                break;
            //Checks if received opcode is POSTION READ and nERR and nWARN bits
            //During Start up, MISO is stuck high so No data will be received
            case MHM_STARTUP_3:
                //Wait for last SPI command is sent.
                if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENT)
                {
                    //Check recived opcode, if not POSITION READ opcode then repeat process.
                    if(pSPI0Data->RxData[0] !=  POS_READ)
                    {
                        if(PowerUp)
                        {
                            //To Do EEPROM LOAD FAIL
                            PowerUp = 0;
                        }
                        MHMTimer = READ_POS_TIMER_SET;
                        IC_MHMfsm = READ_POS_1;
                    }
                    //POSITION READ Opcode Received
                    else
                    {
                        if(PowerUp)
                        {
                            //To Do EERPOM LOAD SUCCEED
                            PowerUp = 0;
                        }
                        //Check for any error bit or warning bit
                        //During Start Up, nWARN and nERR are reset. 
                        if(!pSPI0Data->RxData[7])
                        {
                            if(StartUpCnt < 2)
                            {
                                StartUpCnt++;
                                IC_MHMfsm = MHM_STARTUP_4;
                            }
                            else
                            {
                                //To Do START UP ERROR 
                                MHMTimer = READ_POS_TIMER_SET;
                                IC_MHMfsm = READ_POS_1;
                            }
                        }
                        else
                        {
                            //To Do START UP SUCCEED
                            MHMTimer = READ_POS_TIMER_SET;
                            IC_MHMfsm = READ_POS_1;
                        }
                    }
                    IC_MCB_SPIBufferFree(&pSPI0Data);
                }
                break;
                
            case MHM_STARTUP_4:
                if(IC_MHM_Preset_IC_PV()) IC_MHMfsm = MHM_STARTUP_5;
                break;
            case MHM_STARTUP_5:
                
                IC_MHMfsm = MHM_STARTUP_6;
                break;

            case MHM_STARTUP_6:
                
                IC_MHMfsm = MHM_STARTUP_7;
                break;
                
            case MHM_STARTUP_7:
                IC_MHMfsm = MHM_STARTUP_1;
                break;                

            case READ_POS_1:
                if(IC_MHM_POS_READ == SPI0_CMD_SENDING) IC_MHMfsm = READ_POS_2;
                break;
                
            case READ_POS_2:
                //Wait for last SPI command is sent.
                if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENT)
                {
                    //Check for nERR bit 
                    if(!(pSPI0Data->RxData[7] & IC_MHM_SPI_nERR_Msk)) 
                    {
                        //nERR bit detectec
                        IC_MHMfsm = READ_STATUS_1;
                    }
                    else
                    {
                        //Check for nWARN bit
                        if(!(pSPI0Data->RxData[7] & IC_MHM_SPI_nWARN_Msk))
                        {
                            //nWARN bit detectec, excessive rotor speed
                        }
                        //Read MT Position from pSPI0Data->RxData[1] to pSPI0Data->RxData[4]
                        //Read ST Position from pSPI0Data->RxData[5] to pSPI0Data->RxData[6]
                        IC_MHMfsm = READ_REG_STAT_1;
                    }
                    IC_MCB_SPIBufferFree(&pSPI0Data);
                }
                break;
                
            case READ_STATUS_1:
                if(IC_MHM_READ_STATUS == SPI0_CMD_SENDING) IC_MHMfsm = READ_STATUS_2;
                break;
                
            case READ_STATUS_2:
                //Wait for last SPI command is sent.
                if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENT)
                {
                    //Check Status Register 1, address 0x70
                    if(pSPI0Data->RxData[1])
                    {
                        //error detected
                    }
                    else IC_MHMfsm = READ_POS_1;
                    IC_MCB_SPIBufferFree(&pSPI0Data);
                }
                break;
                
            case READ_REG_STAT_1:
                if(IC_MHM_READ_REG_STAT == SPI0_CMD_SENDING) IC_MHMfsm = READ_REG_STAT_2;
                break;
                
            case READ_REG_STAT_2:
                                //Wait for last SPI command is sent.
                if(SPI0SendCMD(pSPI0Data) == SPI0_CMD_SENT)
                {
                    if(pSPI0Data->RxData[1])
                    {
                        //Check every flag in REGISTER STATUS/DATA
                    }
                    IC_MHMfsm = READ_POS_1;
                    IC_MCB_SPIBufferFree(&pSPI0Data);
                }
                break;
        }
    }
}
//void SPI1Task()
//{
//	switch (SPI1fsm)
//	{
//		case 0:
//			if(pSPI1Data != NULL)
//			{
//                //Enable SERCOM0
//                SERCOM0_REGS->SPIM.SERCOM_CTRLA |= SERCOM_SPIM_CTRLA_ENABLE_Msk ;
//                /* Wait for synchronization */
//                while((SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY) != 0U)
//                {
//                    /* Do nothing */
//                }
//				//Enable SPI Reception Interupt
//                SERCOM0_REGS->I2CM.SERCOM_INTENSET |= SERCOM_SPIM_INTENSET_RXC_Msk;
//				//Enable SPI Data register Empty
//                SERCOM0_REGS->SPIM.SERCOM_INTENSET |= SERCOM_SPIM_INTENSET_DRE_Msk;
//				SPI1fsm++;
//			}
//		break;
//
//		case 1:
//			if(pSPI1Data->RxCnt == pSPI1Data->length)
//			{
//               //Disable SERCOM0
//                SERCOM0_REGS->SPIM.SERCOM_CTRLA &= ~(SERCOM_SPIM_CTRLA_ENABLE_Msk);
//                /* Wait for synchronization */
//                while((SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY) != 0U)
//                {
//                    /* Do nothing */
//                }
//                //Disable SPI Reception Interupt
//                SERCOM0_REGS->I2CM.SERCOM_INTENCLR |= SERCOM_SPIM_INTENSET_RXC_Msk;
//				//Disable SPI Data register Empty
//                SERCOM0_REGS->SPIM.SERCOM_INTENCLR |= SERCOM_SPIM_INTENSET_DRE_Msk;
//				SPI1fsm++;
//			}
//		break;
//
//		case 2:
//			if(pSPI1Data == NULL) SPI1fsm = 0;
//		break;
//	}
//}


/* *****************************************************************************
 End of File
 */
