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
#include "SPI_MHM.h"
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

//IC-MHM variables and pointers.
//bool IC_MHMAccessFree = 0;
static IC_MHM_REG_ACCType* pMHMRegAccData = NULL;

static SPI_IC_MHMType* pSPIMHM = NULL;

//Externla DAC variables and pointers.
static uint8_t ExtDACData[3] = {0,0,0};
static uint8_t* pExtDACData = NULL;

static uint16_t ExtDACVal = 0;
//static uint16_t IntDACVal = 0;

//External EEPROM variables and pointers
static ExtEEpromDataType* pExtEEpromData = NULL;
volatile uint8_t ExtEEpromTimer = 0;

static IC_MHMfsmType IC_MHMfsm = MHM_STARTUP_1;
static uint8_t RegAccessfsm = 0;
static uint8_t StatusReg[4];
static uint8_t IC_MHMCmdfsm = 0;
static uint8_t IC_MHMProcFsm = 0;

//Internal EEPROM, RWWEEprom variables and pointers
static IntRWWEEWrType* pIntRWWEEWr = NULL;
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
//bool IsMHMAccessFree()
//{
//    return IC_MHMAccessFree;
//}
//
//void SetMHMAccessFree()
//{
//    IC_MHMAccessFree = (bool)1;
//}
//
//void SetMHMAccessDenied()
//{
//    IC_MHMAccessFree = (bool)0;
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

void TimerTask()
{
     if( MHMTimer > 1)  MHMTimer--;
     if( MHMProcTimer > 1)  MHMProcTimer--;
     if( ExtEEpromTimer > 1) ExtEEpromTimer--;
}

uint8_t IC_MHM_RegAccesTask()
{
    //static SPI_IC_MHMType* pSPIMHM = NULL;
    uint8_t RetVal = IC_MHM_STAT_BUSY_Msk;
    
//    if(BISS_MASTER_Get())
//    {
//        RegAccessfsm = 6;
//        NCS_MHM_Set();
//    }
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
                        RetVal = pSPIMHM->RxData[1];
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
                
//            case 6:
//                if(pSPIMHM != NULL) IC_MHM_SPIBufferFree(&pSPIMHM);
//                RegAccessfsm = 0;
//                break;
        }
    }
    return RetVal;
}

uint8_t IC_MHM_ReadPos(uint8_t* Data, uint8_t RxLength)
{
    uint8_t RetVal;
    
    if(pMHMRegAccData == NULL) MHMRegAccBufferInit(&pMHMRegAccData, POS_READ_OPC, 1, RxLength+1);

    RetVal = IC_MHM_RegAccesTask();
    if(!(RetVal & IC_MHM_STAT_BUSY_Msk))
    {
        memcpy(Data, &pMHMRegAccData->RxData[1], 7);
        MHMRegAccBufferFree(&pMHMRegAccData);
    }
    return RetVal;
}

uint8_t IC_MHM_RdStatus(uint8_t* Data)
{
    uint8_t RetVal;
    
    if(pMHMRegAccData == NULL) MHMRegAccBufferInit(&pMHMRegAccData, READ_STATUS_OPC, 1, 5);

    RetVal = IC_MHM_RegAccesTask();
    if(!(RetVal & IC_MHM_STAT_BUSY_Msk))
    {
        memcpy(Data, &pMHMRegAccData->RxData[1], 4);
        MHMRegAccBufferFree(&pMHMRegAccData);
    }
    return RetVal;
}

uint8_t IC_MHM_RegRdCtd(uint8_t Address, uint8_t* Data, uint8_t Length)
{
    uint8_t RetVal;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, REG_RD_CTD_OPC, 2, Length+2);
        pMHMRegAccData->TxData[1] = Address;
    }
    
    RetVal = IC_MHM_RegAccesTask();
    if(!(RetVal & IC_MHM_STAT_BUSY_Msk))
    {
        memcpy(Data, &pMHMRegAccData->RxData[2], pMHMRegAccData->RxLength-2);
        MHMRegAccBufferFree(&pMHMRegAccData);
    }
    return RetVal;
}

uint8_t IC_MHM_RegWrCt(uint8_t Address, uint8_t* Data, uint8_t Length)
{
    uint8_t RetVal;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, REG_WR_CTD_OPC, Length+2, 0);
        pMHMRegAccData->TxData[1] = Address;
        memcpy(&pMHMRegAccData->TxData[2], Data, Length);
    }

    RetVal = IC_MHM_RegAccesTask();
    if(!(RetVal & IC_MHM_STAT_BUSY_Msk))
        MHMRegAccBufferFree(&pMHMRegAccData);

    return RetVal;
}

uint8_t IC_MHM_WrInstr(uint8_t* Data, uint8_t Length)
{
    uint8_t RetVal;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, WR_INST_OPC, Length+1, 0);
        memcpy(&pMHMRegAccData->TxData[1], Data, Length);
    }

    RetVal = IC_MHM_RegAccesTask();
    if(!(RetVal & IC_MHM_STAT_BUSY_Msk))
        MHMRegAccBufferFree(&pMHMRegAccData);

    return RetVal;
}

uint8_t IC_MHM_RegWr(uint8_t Address, uint8_t Data)
{
    uint8_t RetVal;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, REG_WR_OPC, 3, 0);
        pMHMRegAccData->TxData[1] = Address;
        pMHMRegAccData->TxData[2] = Data;
    }

    RetVal = IC_MHM_RegAccesTask();
    if(!(RetVal & IC_MHM_STAT_BUSY_Msk))
        MHMRegAccBufferFree(&pMHMRegAccData);

    return RetVal;
}

uint8_t IC_MHM_RegRd(uint8_t Address, uint8_t* Data)
{
    uint8_t RetVal;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, REG_RD_OPC, 2, 3);
        pMHMRegAccData->TxData[1] = Address;
    }

    RetVal = IC_MHM_RegAccesTask();
    if(!(RetVal & IC_MHM_STAT_BUSY_Msk))
    {
        *Data = pMHMRegAccData->RxData[2];
        MHMRegAccBufferFree(&pMHMRegAccData);
    }

    return RetVal;
}

uint8_t IC_MHM_Activate(uint8_t Data)
{
    uint8_t RetVal;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, ACTIVATE_OPC, 2, 0);
        pMHMRegAccData->TxData[1] = Data | 0x80;
    }

    RetVal = IC_MHM_RegAccesTask();
    if(!(RetVal & IC_MHM_STAT_BUSY_Msk))
        MHMRegAccBufferFree(&pMHMRegAccData);

    return RetVal;
}

uint8_t IC_MHM_SetFIO(uint8_t Data)
{
    uint8_t TempResult = IC_MHM_STAT_BUSY_Msk;
    static uint8_t TempData = 0;
    
    switch (IC_MHMCmdfsm)
    {
        case 0:
            TempResult = IC_MHM_RegRd(IC_MHM_FIO_REG_ADDR, &TempData);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                TempData |= Data;
                TempResult = IC_MHM_STAT_BUSY_Msk;
                IC_MHMCmdfsm++;
            }
            break;
            
        case 1:
            TempResult = IC_MHM_RegWr(IC_MHM_FIO_REG_ADDR, TempData);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                TempData = 0;
                IC_MHMCmdfsm = 0;
            }
            break;
    }
    return TempResult;
}

uint8_t IC_MHM_ClrFIO(uint8_t Data)
{
    uint8_t TempResult = IC_MHM_STAT_BUSY_Msk;
    static uint8_t TempData = 0;
    
    switch (IC_MHMCmdfsm)
    {
        case 0:
            TempResult = IC_MHM_RegRd(IC_MHM_FIO_REG_ADDR, &TempData);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                TempData &= ~(Data);
                IC_MHMCmdfsm++;
            }
            break;
            
        case 1:
            TempResult = IC_MHM_RegWr(IC_MHM_FIO_REG_ADDR, TempData);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                TempData = 0;
                IC_MHMCmdfsm = 0;
            }
            break;
    }
    return TempResult;
}

uint8_t IC_MHM_PresetPV()
{
    uint8_t TempResult = IC_MHM_STAT_BUSY_Msk;
    
    switch (IC_MHMProcFsm)
    {
        case 0:
            TempResult = IC_MHM_SetFIO(IC_MHM_0x75_FIO_3_Msk);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                MHMProcTimer = PV_PRESET_TIMER_SET;
                TempResult = IC_MHM_STAT_BUSY_Msk;
                IC_MHMProcFsm++;
            }
            break;
            
        case 1:
            if(MHMProcTimer == 1)
            {
                TempResult = IC_MHM_ClrFIO(IC_MHM_0x75_FIO_3_Msk);
                if(TempResult & IC_MHM_STAT_VALID_Msk)
                {
                    MHMProcTimer = PV_PRESET_TIMER_SET;
                    TempResult = IC_MHM_STAT_BUSY_Msk;
                    IC_MHMProcFsm++;
                }
            }
            break;
        
        case 2:
            if(MHMProcTimer == 1)
            {
                MHMProcTimer = 0;
                TempResult = IC_MHM_STAT_VALID_Msk;
                IC_MHMProcFsm = 0;
            }
            break;
    }
    return TempResult;
}

void CalcDACsVal(void)
{
    uint8_t ResoMT;
    
    ResoMT = CalcMTResCode (RESDIR_RESO_MT);
    switch (SCALE_ACTIVE_RD)
    {
        case FACTORY_SCALE:
            ResoMT = (FACTORY_RESOMT <= ResoMT)?FACTORY_RESOMT:ResoMT;
            if(!ResoMT)
            {
                if(USR_SCL_FRACT_RNG_RD == USR_SCL_FRACT_RNG_USED)
                {
                    (*((uint16_t*)(CommonVars.pNormPos))) = ((*((uint16_t*)(CommonVars.pPosition)))&(*((uint16_t*)(CommonVars.pROverRange))));
                    if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pPosRange))))
                    {
                        ExtDACVal = (uint16_t)((((uint32_t)(*((uint16_t*)(CommonVars.pNormPos))))*((uint32_t)(CommonVars.ExtDACMax)))/(*((uint16_t*)(CommonVars.pPosRange))));
                    }
                    else if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pTransition))))
                    {
                        ExtDACVal = CommonVars.ExtDACMax;
                    }
                    else
                    {
                        ExtDACVal = 0x0000;
                    }
                }
                else
                {
                    (*((uint16_t*)(CommonVars.pNormPos))) = ((*((uint16_t*)(CommonVars.pPosition)))&(*((uint16_t*)(CommonVars.pROverRange))));
                    ExtDACVal = (uint16_t)((((uint32_t)(*((uint16_t*)(CommonVars.pNormPos))))*((uint32_t)(CommonVars.ExtDACMax)))/(*((uint16_t*)(CommonVars.pPosRange))));
                }
            }
            else if(ResoMT<=16)
            {
                (*((uint32_t*)(CommonVars.pNormPos))) = ((*((uint32_t*)(CommonVars.pPosition)))&(*((uint32_t*)(CommonVars.pROverRange))));
                ExtDACVal = (uint16_t)((((uint64_t)(*((uint32_t*)(CommonVars.pNormPos))))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint32_t*)(CommonVars.pPosRange))));
            }
            else
            {
                (*((uint64_t*)(CommonVars.pNormPos))) = ((*((uint64_t*)(CommonVars.pPosition)))&(*((uint64_t*)(CommonVars.pROverRange))));
                ExtDACVal = (uint16_t)(((*((uint64_t*)(CommonVars.pNormPos)))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint64_t*)(CommonVars.pPosRange))));
            }
            break;
            
        case DEFAULT_SCALE:
            ResoMT = (DEFAULT_RESOMT <= ResoMT)?DEFAULT_RESOMT:ResoMT;
            if(!ResoMT)
            {
                (*((uint16_t*)(CommonVars.pNormPos))) = ((*((uint16_t*)(CommonVars.pPosition)))&(*((uint16_t*)(CommonVars.pROverRange))));
                ExtDACVal = (uint16_t)((((uint32_t)(*((uint16_t*)(CommonVars.pNormPos))))*((uint32_t)(CommonVars.ExtDACMax)))/(*((uint16_t*)(CommonVars.pPosRange))));                
            }
            else if (ResoMT<=16)
            {
                (*((uint32_t*)(CommonVars.pNormPos))) = ((*((uint32_t*)(CommonVars.pPosition)))&(*((uint32_t*)(CommonVars.pROverRange))));
                ExtDACVal = (uint16_t)((((uint64_t)(*((uint32_t*)(CommonVars.pNormPos))))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint32_t*)(CommonVars.pPosRange))));
            }
            else 
            {
                (*((uint64_t*)(CommonVars.pNormPos))) = ((*((uint64_t*)(CommonVars.pPosition)))&(*((uint64_t*)(CommonVars.pROverRange))));
                ExtDACVal = (uint16_t)(((*((uint64_t*)(CommonVars.pNormPos)))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint64_t*)(CommonVars.pPosRange))));
            }
            break;
        default:
            if(!ResoMT)
            {
                if(USR_SCL_UF_OF_RD == USR_SCL_NOT_OVFLW)
                {
                    if((*((uint16_t*)(CommonVars.pPosLowOut))) <= (*((uint16_t*)(CommonVars.pPosHighOut))))
                    {
                        (*((uint16_t*)(CommonVars.pNormPos))) = (((*((uint16_t*)(CommonVars.pPosition)))-(*((uint16_t*)(CommonVars.pPosLowOut))))&(*((uint16_t*)(CommonVars.pROverRange))));
                        if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = (uint16_t)((((uint32_t)(*((uint16_t*)(CommonVars.pNormPos))))*((uint32_t)(CommonVars.ExtDACMax)))/(*((uint16_t*)(CommonVars.pPosRange))));
                        }
                        else if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                        else
                        {
                            ExtDACVal = 0x0000;
                        }
                    }
                    else
                    {
                        (*((uint16_t*)(CommonVars.pNormPos))) = (((*((uint16_t*)(CommonVars.pPosition)))-(*((uint16_t*)(CommonVars.pPosHighOut))))&(*((uint16_t*)(CommonVars.pROverRange))));
                        if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = ~((uint16_t)((((uint32_t)(*((uint16_t*)(CommonVars.pNormPos))))*((uint32_t)(CommonVars.ExtDACMax)))/(*((uint16_t*)(CommonVars.pPosRange)))));
                        }
                        else if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = 0x0000;
                        }
                        else
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                    }
                }
                else
                {
                    if((*((uint16_t*)(CommonVars.pPosHighOut))) <= (*((uint16_t*)(CommonVars.pPosLowOut))))
                    {
                        (*((uint16_t*)(CommonVars.pNormPos))) = (((*((uint16_t*)(CommonVars.pPosition)))-(*((uint16_t*)(CommonVars.pPosLowOut))))&(*((uint16_t*)(CommonVars.pROverRange))));
                        if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = (uint16_t)((((uint32_t)(*((uint16_t*)(CommonVars.pNormPos))))*((uint32_t)(CommonVars.ExtDACMax)))/(*((uint16_t*)(CommonVars.pPosRange))));
                        }
                        else if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                        else
                        {
                            ExtDACVal = 0x0000;
                        }
                    }
                    else
                    {
                        (*((uint16_t*)(CommonVars.pNormPos))) = (((*((uint16_t*)(CommonVars.pPosition)))-(*((uint16_t*)(CommonVars.pPosHighOut))))&(*((uint16_t*)(CommonVars.pROverRange))));
                        if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = ~((uint16_t)((((uint32_t)(*((uint16_t*)(CommonVars.pNormPos))))*((uint32_t)(CommonVars.ExtDACMax)))/(*((uint16_t*)(CommonVars.pPosRange)))));
                        }
                        else if((*((uint16_t*)(CommonVars.pNormPos))) <= (*((uint16_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = 0x0000;
                        }
                        else
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                    }
                }
            }
            else if(ResoMT<=16)
            {
                if(USR_SCL_UF_OF_RD == USR_SCL_NOT_OVFLW)
                {
                    if((*((uint32_t*)(CommonVars.pPosLowOut))) <= (*((uint32_t*)(CommonVars.pPosHighOut))))
                    {
                        (*((uint32_t*)(CommonVars.pNormPos))) = (((*((uint32_t*)(CommonVars.pPosition)))-(*((uint32_t*)(CommonVars.pPosLowOut))))&(*((uint32_t*)(CommonVars.pROverRange))));
                        if((*((uint32_t*)(CommonVars.pNormPos))) <= (*((uint32_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = (uint16_t)((((uint64_t)(*((uint32_t*)(CommonVars.pNormPos))))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint32_t*)(CommonVars.pPosRange))));
                        }
                        else if((*((uint32_t*)(CommonVars.pNormPos))) <= (*((uint32_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                        else
                        {
                            ExtDACVal = 0x0000;
                        }
                    }
                    else
                    {
                        (*((uint32_t*)(CommonVars.pNormPos))) = (((*((uint32_t*)(CommonVars.pPosition)))-(*((uint32_t*)(CommonVars.pPosHighOut))))&(*((uint32_t*)(CommonVars.pROverRange))));
                        if((*((uint32_t*)(CommonVars.pNormPos))) <= (*((uint32_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = ~((uint16_t)((((uint64_t)(*((uint32_t*)(CommonVars.pNormPos))))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint32_t*)(CommonVars.pPosRange)))));
                        }
                        else if((*((uint32_t*)(CommonVars.pNormPos))) <= (*((uint32_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = 0x0000;
                        }
                        else
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                    }
                }
                else
                {
                    if((*((uint32_t*)(CommonVars.pPosHighOut))) <= (*((uint32_t*)(CommonVars.pPosLowOut))))
                    {
                        (*((uint32_t*)(CommonVars.pNormPos))) = (((*((uint32_t*)(CommonVars.pPosition)))-(*((uint32_t*)(CommonVars.pPosLowOut))))&(*((uint32_t*)(CommonVars.pROverRange))));
                        if((*((uint32_t*)(CommonVars.pNormPos))) <= (*((uint32_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = (uint16_t)((((uint64_t)(*((uint32_t*)(CommonVars.pNormPos))))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint32_t*)(CommonVars.pPosRange))));
                        }
                        else if((*((uint32_t*)(CommonVars.pNormPos))) <= (*((uint32_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                        else
                        {
                            ExtDACVal = 0x0000;
                        }
                    }
                    else
                    {
                        (*((uint32_t*)(CommonVars.pNormPos))) = (((*((uint32_t*)(CommonVars.pPosition)))-(*((uint32_t*)(CommonVars.pPosHighOut))))&(*((uint32_t*)(CommonVars.pROverRange))));
                        if((*((uint32_t*)(CommonVars.pNormPos))) <= (*((uint32_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = ~((uint16_t)((((uint64_t)(*((uint32_t*)(CommonVars.pNormPos))))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint32_t*)(CommonVars.pPosRange)))));
                        }
                        else if((*((uint32_t*)(CommonVars.pNormPos))) <= (*((uint32_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = 0x0000;
                        }
                        else
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                    }
                }
            }
            else
            {
                if(USR_SCL_UF_OF_RD == USR_SCL_NOT_OVFLW)
                {
                    if((*((uint64_t*)(CommonVars.pPosLowOut))) <= (*((uint64_t*)(CommonVars.pPosHighOut))))
                    {
                        (*((uint64_t*)(CommonVars.pNormPos))) = (((*((uint64_t*)(CommonVars.pPosition)))-(*((uint64_t*)(CommonVars.pPosLowOut))))&(*((uint64_t*)(CommonVars.pROverRange))));
                        if((*((uint64_t*)(CommonVars.pNormPos))) <= (*((uint64_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = (uint16_t)(((*((uint64_t*)(CommonVars.pNormPos)))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint64_t*)(CommonVars.pPosRange))));
                        }
                        else if((*((uint64_t*)(CommonVars.pNormPos))) <= (*((uint64_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                        else
                        {
                            ExtDACVal = 0x0000;
                        }
                    }
                    else
                    {
                        (*((uint64_t*)(CommonVars.pNormPos))) = (((*((uint64_t*)(CommonVars.pPosition)))-(*((uint64_t*)(CommonVars.pPosHighOut))))&(*((uint64_t*)(CommonVars.pROverRange))));
                        if((*((uint64_t*)(CommonVars.pNormPos))) <= (*((uint64_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = ~((uint16_t)(((*((uint64_t*)(CommonVars.pNormPos)))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint64_t*)(CommonVars.pPosRange)))));
                        }
                        else if((*((uint64_t*)(CommonVars.pNormPos))) <= (*((uint64_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = 0x0000;
                        }
                        else
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                    }
                }
                else
                {
                    if((*((uint64_t*)(CommonVars.pPosHighOut))) <= (*((uint64_t*)(CommonVars.pPosLowOut))))
                    {
                        (*((uint64_t*)(CommonVars.pNormPos))) = (((*((uint64_t*)(CommonVars.pPosition)))-(*((uint64_t*)(CommonVars.pPosLowOut))))&(*((uint64_t*)(CommonVars.pROverRange))));
                        if((*((uint64_t*)(CommonVars.pNormPos))) <= (*((uint64_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = (uint16_t)(((*((uint64_t*)(CommonVars.pNormPos)))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint64_t*)(CommonVars.pPosRange))));
                        }
                        else if((*((uint64_t*)(CommonVars.pNormPos))) <= (*((uint64_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                        else
                        {
                            ExtDACVal = 0x0000;
                        }
                    }
                    else
                    {
                        (*((uint64_t*)(CommonVars.pNormPos))) = (((*((uint64_t*)(CommonVars.pPosition)))-(*((uint64_t*)(CommonVars.pPosHighOut))))&(*((uint64_t*)(CommonVars.pROverRange))));
                        if((*((uint64_t*)(CommonVars.pNormPos))) <= (*((uint64_t*)(CommonVars.pPosRange))))
                        {
                            ExtDACVal = ~((uint16_t)(((*((uint64_t*)(CommonVars.pNormPos)))*((uint64_t)(CommonVars.ExtDACMax)))/(*((uint64_t*)(CommonVars.pPosRange)))));
                        }
                        else if((*((uint64_t*)(CommonVars.pNormPos))) <= (*((uint64_t*)(CommonVars.pTransition))))
                        {
                            ExtDACVal = 0x0000;
                        }
                        else
                        {
                            ExtDACVal = CommonVars.ExtDACMax;
                        }
                    }
                }
            }
            break;
    }
}

void CalcPosTransition(uint8_t ResoMT)
{
    if(!ResoMT)
    {
        *((uint16_t*)(CommonVars.pTransition)) = (*((uint16_t*)(CommonVars.pPosRange)))+(((*((uint16_t*)(CommonVars.pROverRange)))-(*((uint16_t*)(CommonVars.pPosRange))))>>1);
    }
    else if(ResoMT<=16)
    {
        *((uint32_t*)(CommonVars.pTransition)) = (*((uint32_t*)(CommonVars.pPosRange)))+(((*((uint32_t*)(CommonVars.pROverRange)))-(*((uint32_t*)(CommonVars.pPosRange))))>>1);
    }
    else
    {
        *((uint64_t*)(CommonVars.pTransition)) = (*((uint64_t*)(CommonVars.pPosRange)))+(((*((uint64_t*)(CommonVars.pROverRange)))-(*((uint64_t*)(CommonVars.pPosRange))))>>1);
    }
}

void CalcROverRange(uint8_t ResoMT)
{
    uint8_t i;
    
    if(!ResoMT)
    {
        *((uint16_t*)(CommonVars.pROverRange)) = 0xFFFF >> RESDIR_RESO_ST;
    }
    else if(ResoMT<=16)
    {
        for(i=(16-ResoMT);i<=16;i++)
        {
            if((0xFFFFFFFF >> (i+RESDIR_RESO_ST)) < (*((uint32_t*)CommonVars.pPosRange))) break;
        }
        if(i<=16) (*((uint32_t*)CommonVars.pROverRange)) = (0xFFFFFFFF >> (i-1+RESDIR_RESO_ST));
        else (*((uint32_t*)CommonVars.pROverRange)) = 0x0000FFFF >> RESDIR_RESO_ST;
    }
    else
    {
        for(i=(32-ResoMT);i<=32;i++)
        {
            if((0x0000FFFFFFFFFFFF >> (i+RESDIR_RESO_ST)) < (*((uint64_t*)CommonVars.pPosRange))) break;
        }
        if(i<=32) (*((uint64_t*)CommonVars.pROverRange)) = (0x0000FFFFFFFFFFFF >> (i-1+RESDIR_RESO_ST));
        else (*((uint64_t*)CommonVars.pROverRange)) = 0x000000000000FFFF >> RESDIR_RESO_ST;
    }
}

void CalcPosRange(uint8_t ResoMT, int8_t UF_OF)
{
    if(!ResoMT)
    {
        (*((uint16_t*)CommonVars.pPosRange)) = ((*((uint16_t*)CommonVars.pPosHighOut))<=(*((uint16_t*)CommonVars.pPosLowOut))) ?
                        (*((uint16_t*)CommonVars.pPosLowOut))-(*((uint16_t*)CommonVars.pPosHighOut)):
                        (*((uint16_t*)CommonVars.pPosHighOut))-(*((uint16_t*)CommonVars.pPosLowOut));
        if(UF_OF != USR_SCL_NOT_OVFLW) (*((uint16_t*)CommonVars.pPosRange)) = (~(*((uint16_t*)CommonVars.pPosRange))+1);
    }
    else if(ResoMT<=16)
    {
        (*((uint32_t*)CommonVars.pPosRange)) = ((*((uint32_t*)CommonVars.pPosHighOut))<=(*((uint32_t*)CommonVars.pPosLowOut))) ?
                        (*((uint32_t*)CommonVars.pPosLowOut))-(*((uint32_t*)CommonVars.pPosHighOut)):
                        (*((uint32_t*)CommonVars.pPosHighOut))-(*((uint32_t*)CommonVars.pPosLowOut));
        if(UF_OF != USR_SCL_NOT_OVFLW) (*((uint32_t*)CommonVars.pPosRange)) = (~(*((uint32_t*)CommonVars.pPosRange))+1);
    }
    else
    {
        (*((uint64_t*)CommonVars.pPosRange)) = ((*((uint64_t*)CommonVars.pPosHighOut))<=(*((uint64_t*)CommonVars.pPosLowOut))) ?
                        0x0000FFFFFFFFFFFF-((*((uint64_t*)CommonVars.pPosLowOut))-(*((uint64_t*)CommonVars.pPosHighOut))-1):
                        0x0000FFFFFFFFFFFF-((*((uint64_t*)CommonVars.pPosHighOut))-(*((uint64_t*)CommonVars.pPosLowOut))-1);
        if(UF_OF != USR_SCL_NOT_OVFLW) (*((uint64_t*)CommonVars.pPosRange)) = ((~(*((uint64_t*)CommonVars.pPosRange))+1) & 0x0000FFFFFFFFFFFF);
    }
}

uint8_t CheckUserScaling()
{
    uint8_t RetVal=0;
    uint8_t ResoMT;
    
    ResoMT = CalcMTResCode (RESDIR_RESO_MT);
    
    //Calc user scaling range
    switch (CommonVars.UF_OF_Cnt)
    {
        case (-1):
            if(!ResoMT)
            {
                if((*((uint16_t*)CommonVars.pPosition)) > (*((uint16_t*)(CommonVars.pPosOffset)))) CalcPosRange(ResoMT,CommonVars.UF_OF_Cnt);
            }
            else if(ResoMT<=16)
            {
                if((*((uint32_t*)CommonVars.pPosition)) > (*((uint32_t*)(CommonVars.pPosOffset)))) CalcPosRange(ResoMT,CommonVars.UF_OF_Cnt);
            }
            else
            {
                if((*((uint64_t*)CommonVars.pPosition)) > (*((uint64_t*)(CommonVars.pPosOffset)))) CalcPosRange(ResoMT,CommonVars.UF_OF_Cnt);
            }
            break;

        case 0:
            CalcPosRange(ResoMT,CommonVars.UF_OF_Cnt);
            break;
            
        case 1:
            if(!ResoMT)
            {
                if((*((uint16_t*)CommonVars.pPosition)) < (*((uint16_t*)(CommonVars.pPosOffset)))) CalcPosRange(ResoMT,CommonVars.UF_OF_Cnt);
            }
            else if(ResoMT<=16)
            {
                if((*((uint32_t*)CommonVars.pPosition)) < (*((uint32_t*)(CommonVars.pPosOffset)))) CalcPosRange(ResoMT,CommonVars.UF_OF_Cnt);
            }
            else
            {
                if((*((uint64_t*)CommonVars.pPosition)) < (*((uint64_t*)(CommonVars.pPosOffset)))) CalcPosRange(ResoMT,CommonVars.UF_OF_Cnt);
            }
            break;
    }
    //Check minimum scaling range
    if(!ResoMT)
    {
        if(((*((uint16_t*)CommonVars.pPosRange))) > (0xFFFF>>(RESDIR_RESO_ST+4)))
        {
            if(!CommonVars.UF_OF_Cnt) USR_SCL_UF_OF_WR(USR_SCL_NOT_OVFLW);
            else USR_SCL_UF_OF_WR(USR_SCL_OVFLW);
            CalcROverRange(ResoMT);
            CalcPosTransition(ResoMT);
            RetVal=1;
        }
    }
    else if (ResoMT<=16)
    {
        if(((*((uint32_t*)CommonVars.pPosRange))) > (0x0000FFFF>>(RESDIR_RESO_ST+4)))
        {
            if(!CommonVars.UF_OF_Cnt) USR_SCL_UF_OF_WR(USR_SCL_NOT_OVFLW);
            else USR_SCL_UF_OF_WR(USR_SCL_OVFLW);
            CalcROverRange(ResoMT);
            CalcPosTransition(ResoMT);
            RetVal=1;
        }
    }
    else
    {
        if(((*((uint64_t*)CommonVars.pPosRange))) > (0x000000000000FFFF>>(RESDIR_RESO_ST+4)))
        {
            if(!CommonVars.UF_OF_Cnt) USR_SCL_UF_OF_WR(USR_SCL_NOT_OVFLW);
            else USR_SCL_UF_OF_WR(USR_SCL_OVFLW);
            CalcROverRange(ResoMT);
            CalcPosTransition(ResoMT);
            RetVal=1;
        }
    }
    return RetVal;
}

void ComparePosition(uint8_t* pNewPos, uint8_t* pOldPos)
{
    uint8_t ResoMT;
    
    ResoMT = CalcMTResCode (RESDIR_RESO_MT);
    if(!ResoMT)
    {
        if((*((uint16_t*)pNewPos)) >= (*((uint16_t*)pOldPos)))
        {
            //if((*((uint16_t*)pNewPos))-(*((uint16_t*)pOldPos)) >= (*((uint16_t*)(CommonVars.pPosOffset))))
            if((*((uint16_t*)pNewPos))-(*((uint16_t*)pOldPos)) >= (0x8000 >> RESDIR_RESO_ST))
            {
                if(CommonVars.UF_OF_Cnt > UF_OV_MIN) CommonVars.UF_OF_Cnt--;
            }
        }
        else
        {
            //if((*((uint16_t*)pOldPos))-(*((uint16_t*)pNewPos)) >= (*((uint16_t*)(CommonVars.pPosOffset))))
            if((*((uint16_t*)pOldPos))-(*((uint16_t*)pNewPos)) >= (0x8000 >> RESDIR_RESO_ST))
            {
                if(CommonVars.UF_OF_Cnt < UF_OV_MAX) CommonVars.UF_OF_Cnt++;
            }
        }
    }
    else if (ResoMT<=16)
    {
        if((*((uint32_t*)pNewPos)) >= (*((uint32_t*)pOldPos)))
        {
            //if((*((uint32_t*)pNewPos))-(*((uint32_t*)pOldPos)) >= (*((uint32_t*)(CommonVars.pPosOffset))))
            if((*((uint32_t*)pNewPos))-(*((uint32_t*)pOldPos)) >= (((uint32_t)0x80000000)>>((16-ResoMT)+RESDIR_RESO_ST)))
            {
                if(CommonVars.UF_OF_Cnt > UF_OV_MIN) CommonVars.UF_OF_Cnt--;
            }
        }
        else
        {
            //if((*((uint32_t*)pOldPos))-(*((uint32_t*)pNewPos)) >= (*((uint32_t*)(CommonVars.pPosOffset))))
            if((*((uint32_t*)pOldPos))-(*((uint32_t*)pNewPos)) >= (((uint32_t)0x80000000)>>((16-ResoMT)+RESDIR_RESO_ST)))
            {
                if(CommonVars.UF_OF_Cnt < UF_OV_MAX) CommonVars.UF_OF_Cnt++;
            }
        }
    }
    else
    {
        if((*((uint64_t*)pNewPos)) >= (*((uint64_t*)pOldPos)))
        {
            //if((*((uint64_t*)pNewPos))-(*((uint64_t*)pOldPos)) >= (*((uint64_t*)(CommonVars.pPosOffset))))
            if((*((uint64_t*)pNewPos))-(*((uint64_t*)pOldPos)) >= (((uint64_t)0x0000800000000000)>>((32-ResoMT)+RESDIR_RESO_ST)))
                
            {
                if(CommonVars.UF_OF_Cnt > UF_OV_MIN) CommonVars.UF_OF_Cnt--;
            }
        }
        else
        {
            //if((*((uint64_t*)pOldPos))-(*((uint64_t*)pNewPos)) >= (*((uint64_t*)(CommonVars.pPosOffset))))
            if((*((uint64_t*)pOldPos))-(*((uint64_t*)pNewPos)) >= (((uint64_t)0x0000800000000000)>>((32-ResoMT)+RESDIR_RESO_ST)))
            {
                if(CommonVars.UF_OF_Cnt < UF_OV_MAX) CommonVars.UF_OF_Cnt++;
            }
        }
    }
}

void CopyPosition (uint8_t* Source , uint8_t* Dest)
{
    uint8_t ResoMT;
    
    ResoMT = CalcMTResCode (RESDIR_RESO_MT);
    if(!ResoMT)
        *((uint16_t*)Dest) = *((uint16_t*)Source);
    else if(ResoMT<=16)
        *((uint32_t*)Dest) = *((uint32_t*)Source);
    else
        *((uint64_t*)Dest) = *((uint64_t*)Source);
}

void BuildPosition (UsedScaleType Scaling)
{
    uint8_t ResoMT;
    
    for (uint8_t i=0;i<((CommonVars.SPIPosByteLen-1)>=CommonVars.PosByteLen)?CommonVars.PosByteLen:(CommonVars.SPIPosByteLen-1);i++)
    {
        //pSPIPosition contains data MSB first, BIG endian
        //MCU works in Little endian, swap bytes required
        CommonVars.pPosition[i] = CommonVars.pSPIPosition[CommonVars.SPIPosByteLen-2-i];
    }
    ResoMT = CalcMTResCode (RESDIR_RESO_MT);
    switch (Scaling)
    {
        case FACTORY_SCALE:
            ResoMT = (FACTORY_RESOMT <= ResoMT)?FACTORY_RESOMT:ResoMT;
            break;
        case DEFAULT_SCALE:
            ResoMT = (DEFAULT_RESOMT <= ResoMT)?DEFAULT_RESOMT:ResoMT;
            break;
        default:
            ResoMT = ResoMT;
            break;
    }
    if(!ResoMT)
    {
        *((uint16_t*)(CommonVars.pPosition)) += (*((uint16_t*)(CommonVars.pPosOffset)));
//        *((uint16_t*)(CommonVars.pPosition)) &= (*((uint16_t*)(CommonVars.pROverRange)));
    }
    else if (ResoMT<=16)
    {
        *((uint16_t*)(CommonVars.pPosition)) <<= RESDIR_RESO_ST;
        *((uint32_t*)(CommonVars.pPosition)) >>= RESDIR_RESO_ST;
        *((uint32_t*)(CommonVars.pPosition)) += (*((uint32_t*)(CommonVars.pPosOffset)));
//        *((uint32_t*)(CommonVars.pPosition)) &= (*((uint32_t*)(CommonVars.pROverRange)));
    }
    else
    {
        *((uint16_t*)(CommonVars.pPosition)) <<= RESDIR_RESO_ST;
        *((uint64_t*)(CommonVars.pPosition)) >>= RESDIR_RESO_ST;
        *((uint64_t*)(CommonVars.pPosition)) += (*((uint64_t*)(CommonVars.pPosOffset)));
//        *((uint64_t*)(CommonVars.pPosition)) &= (*((uint64_t*)(CommonVars.pROverRange)));
    }
}

void pPosSetUp (uint8_t ResoMT)
{
    if(CommonVars.pPosOffset != NULL)
    {
        free(CommonVars.pPosOffset);
        CommonVars.pPosOffset = NULL;
    }
    if(CommonVars.pPosHighOut != NULL)
    {
        free(CommonVars.pPosHighOut);
        CommonVars.pPosHighOut = NULL;
    }
    if(CommonVars.pPosLowOut != NULL)
    {
        free(CommonVars.pPosLowOut);
        CommonVars.pPosLowOut = NULL;
    }
    if(CommonVars.pPosition != NULL)
    {
        free(CommonVars.pPosition);
        CommonVars.pPosition = NULL;
    }
    if(CommonVars.pLastPos != NULL)
    {
        free(CommonVars.pLastPos);
        CommonVars.pLastPos = NULL;
    }
    if(CommonVars.pPosRange != NULL)
    {
        free(CommonVars.pPosRange);
        CommonVars.pPosRange = NULL;
    }
    if(CommonVars.pROverRange != NULL)
    {
        free(CommonVars.pROverRange);
        CommonVars.pROverRange = NULL;
    }
    if(CommonVars.pNormPos != NULL)
    {
        free(CommonVars.pNormPos);
        CommonVars.pNormPos = NULL;
    }
    if(CommonVars.pTransition != NULL)
    {
        free(CommonVars.pTransition);
        CommonVars.pTransition = NULL;
    }
    
    if(!ResoMT)
        CommonVars.PosByteLen = 2;
    else if(ResoMT<=16)
        CommonVars.PosByteLen = 4;
    else
        CommonVars.PosByteLen = 8;
    
    CommonVars.pPosition = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pPosLowOut = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pPosHighOut = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pPosOffset = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pLastPos = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pPosRange = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pROverRange = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pNormPos = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pTransition = (uint8_t*)malloc(CommonVars.PosByteLen);

    memset(CommonVars.pPosition, 0x00, CommonVars.PosByteLen);
    memset(CommonVars.pPosLowOut, 0x00, CommonVars.PosByteLen);
    memset(CommonVars.pPosHighOut, 0x00, CommonVars.PosByteLen);
    memset(CommonVars.pPosOffset, 0x00, CommonVars.PosByteLen);
    memset(CommonVars.pLastPos, 0x00, CommonVars.PosByteLen);
    memset(CommonVars.pPosRange, 0x00, CommonVars.PosByteLen);
    memset(CommonVars.pROverRange, 0x00, CommonVars.PosByteLen);
    memset(CommonVars.pNormPos, 0x00, CommonVars.PosByteLen);
    memset(CommonVars.pTransition, 0x00, CommonVars.PosByteLen);
}

void SetScale(UsedScaleType Scaling)
{
    uint8_t ResoMT;
    
    ResoMT = CalcMTResCode (RESDIR_RESO_MT);
    switch (Scaling)
    {
        case FACTORY_SCALE:
            ResoMT = (FACTORY_RESOMT <= ResoMT)?FACTORY_RESOMT:ResoMT;
            pPosSetUp(ResoMT);
            if(!ResoMT)
            {
                //Check ST fractional setting
                if(USR_SCL_FRACT_RNG_RD == USR_SCL_FRACT_RNG_USED)
                {
                    *((uint16_t*)(CommonVars.pPosHighOut)) = (uint16_t)(((*((uint16_t*)RWWEE_FRACT_RANGE_ADDR))*(((uint32_t)0xFFFF)>>RESDIR_RESO_ST))/3600);
                }
                //ST no fractional
                else
                {
                    *((uint16_t*)(CommonVars.pPosHighOut)) = 0xFFFF >> RESDIR_RESO_ST;
                }
                *((uint16_t*)(CommonVars.pPosOffset)) = (uint16_t)((((*((uint16_t*)(CommonVars.pPosHighOut)))*((uint32_t)(*((uint8_t*)RWWEE_FACT_OFFSET_ADDR))))/200)+1);
                *((uint16_t*)(CommonVars.pPosRange)) = (*((uint16_t*)(CommonVars.pPosHighOut)));
                *((uint16_t*)(CommonVars.pROverRange)) = 0xFFFF >> RESDIR_RESO_ST;
                *((uint16_t*)(CommonVars.pTransition)) = (*((uint16_t*)(CommonVars.pPosRange)))+(((*((uint16_t*)(CommonVars.pROverRange)))-(*((uint16_t*)(CommonVars.pPosRange))))>>1);
            }
            else
            {
                if(ResoMT<=16)
                {
                    *((uint32_t*)(CommonVars.pPosHighOut)) = ((uint32_t)0xFFFFFFFF)>>((16-ResoMT)+RESDIR_RESO_ST);
                    *((uint16_t*)(&CommonVars.pPosOffset[2])) = (uint16_t)(((((uint32_t)0xFFFF)*((uint32_t)(*((uint8_t*)RWWEE_FACT_OFFSET_ADDR))))/200)+1);
                    *((uint32_t*)(CommonVars.pPosOffset)) >>= ((16-ResoMT)+RESDIR_RESO_ST);
                    *((uint32_t*)(CommonVars.pPosRange)) = (*((uint32_t*)(CommonVars.pPosHighOut)));
                    *((uint32_t*)(CommonVars.pROverRange)) = (*((uint32_t*)(CommonVars.pPosHighOut)));
                    *((uint32_t*)(CommonVars.pTransition)) = (*((uint32_t*)(CommonVars.pPosHighOut)));
                }
                else
                {
                    *((uint64_t*)(CommonVars.pPosHighOut)) = ((uint64_t)0x0000FFFFFFFFFFFF)>>((32-ResoMT)+RESDIR_RESO_ST);
                    *((uint32_t*)(&CommonVars.pPosOffset[2])) = (uint32_t)(((((uint64_t)0xFFFFFFFF)*((uint32_t)(*((uint8_t*)RWWEE_FACT_OFFSET_ADDR))))/200)+1);
                    *((uint64_t*)(CommonVars.pPosOffset)) >>= ((32-ResoMT)+RESDIR_RESO_ST);
                    *((uint64_t*)(CommonVars.pPosRange)) = (*((uint64_t*)(CommonVars.pPosHighOut)));
                    *((uint64_t*)(CommonVars.pROverRange)) = (*((uint64_t*)(CommonVars.pPosHighOut)));
                    *((uint64_t*)(CommonVars.pTransition)) = (*((uint64_t*)(CommonVars.pPosHighOut)));
                }
            }
            break;
            
        case DEFAULT_SCALE:
            CommonVars.UserSclCfg[0] = (uint8_t)0;
            CommonVars.UserSclCfg[1] = CommonVars.ResoAndDir;
            ResoMT = (DEFAULT_RESOMT <= ResoMT)?DEFAULT_RESOMT:ResoMT;
            pPosSetUp(ResoMT);
            if (!ResoMT)
            {
                *((uint16_t*)(CommonVars.pPosHighOut)) = 0xFFFF >> RESDIR_RESO_ST;
                *((uint16_t*)(CommonVars.pPosOffset)) = 0x8000 >> RESDIR_RESO_ST;
                *((uint16_t*)(CommonVars.pPosRange)) = (*((uint16_t*)(CommonVars.pPosHighOut)));
                *((uint16_t*)(CommonVars.pROverRange)) = (*((uint16_t*)(CommonVars.pPosHighOut)));
                *((uint16_t*)(CommonVars.pTransition)) = (*((uint16_t*)(CommonVars.pPosHighOut)));
            }
            else if(ResoMT<=16)
            {
                *((uint32_t*)(CommonVars.pPosHighOut)) = ((uint32_t)0xFFFFFFFF)>>((16-ResoMT)+RESDIR_RESO_ST);
                *((uint32_t*)(CommonVars.pPosOffset)) = ((uint16_t)0x80000000)>>((16-ResoMT)+RESDIR_RESO_ST);
                *((uint32_t*)(CommonVars.pPosRange)) = (*((uint32_t*)(CommonVars.pPosHighOut)));
                *((uint32_t*)(CommonVars.pROverRange)) = (*((uint32_t*)(CommonVars.pPosHighOut)));
                *((uint32_t*)(CommonVars.pTransition)) = (*((uint32_t*)(CommonVars.pPosHighOut)));
            }
            else
            {
                *((uint64_t*)(CommonVars.pPosHighOut)) = ((uint64_t)0x0000FFFFFFFFFFFF)>>((32-ResoMT)+RESDIR_RESO_ST);
                *((uint64_t*)(CommonVars.pPosOffset)) = ((uint64_t)0x0000800000000000)>>((32-ResoMT)+RESDIR_RESO_ST);
                *((uint64_t*)(CommonVars.pPosRange)) = (*((uint64_t*)(CommonVars.pPosHighOut)));
                *((uint64_t*)(CommonVars.pROverRange)) = (*((uint64_t*)(CommonVars.pPosHighOut)));
                *((uint64_t*)(CommonVars.pTransition)) = (*((uint64_t*)(CommonVars.pPosHighOut)));
            }
            break;
            
        default:
            CommonVars.UF_OF_Cnt = 0;
            pPosSetUp(ResoMT);
            if(!ResoMT)
            {
                *((uint16_t*)(CommonVars.pPosOffset)) = 0x8000 >> RESDIR_RESO_ST;
            }
            else if(ResoMT<=16)
            {
                *((uint32_t*)(CommonVars.pPosOffset)) = ((uint32_t)0x80000000)>>((16-ResoMT)+RESDIR_RESO_ST);
            }
            else
            {
                *((uint64_t*)(CommonVars.pPosOffset)) = ((uint64_t)0x0000800000000000)>>((32-ResoMT)+RESDIR_RESO_ST);
            }
            break;
    }
    SCALE_ACTIVE_WR(Scaling);
}

uint8_t CalcMTResCode (uint8_t MHM_MT_Res)
{
    return (MHM_MT_Res > 6)? 32:(4*MHM_MT_Res);
}

void IC_MHM_BISS_Detection()
{
    IC_MHMfsm = MHM_STARTUP_1;
    IC_MHMAccessFree = 0;
    IC_MHMCmdfsm = 0;
    IC_MHMProcFsm = 0;
    if(pMHMRegAccData != NULL) MHMRegAccBufferFree(&pMHMRegAccData);

    RegAccessfsm = 0;
    NCS_MHM_Set();
    if(pSPIMHM != NULL) IC_MHM_SPIBufferFree(&pSPIMHM);

    CommonVars.ExchgFlags &= ~EXCHG_FLAG_PRESETON;
}

void IC_MHM_Task()
{
    uint8_t TempResult = 0;
    uint8_t* pTemp = NULL;

    switch (IC_MHMfsm)
    {
        case MHM_STARTUP_1:
            IC_MHMAccessFree = 0;
            MHMTimer = START_UP_TIMER_SET;
            IC_MHMfsm = MHM_STARTUP_2;
            break;
        //Wait start up time then check NERR pin
        case MHM_STARTUP_2:
            if(MHMTimer == (uint8_t)1)
            {
                if(NERR_Get())
                {
                    if(EXCHG_FLAG_PRESETON) IC_MHMfsm = PV_PRESET;
                    else IC_MHMfsm = (!EXCHG_FLAG_PRESET)?READ_RESO_DIR:READ_POS_1;
                }
                else IC_MHMfsm = MHM_STARTUP_1;
            }
            break;

        case READ_RESO_DIR:
            //Read IC-MHM registers 0 and 1 and get RESO_MT i RESO_ST
            pTemp = (uint8_t*)malloc(IC_MHM_INTRPLTR_LEN);
            TempResult = IC_MHM_RegRdCtd(IC_MHM_REG0_ADDR, pTemp, IC_MHM_INTRPLTR_LEN);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                CommonVars.ResoAndDir = 0x00 | (((*pTemp >> IC_MHM_REG0_DIR_POS)&IC_MHM_REG0_DIR_MSK)<<USR_SCL_MHM_DIR_POS);
                pTemp++;
                CommonVars.ResoAndDir |= (*pTemp & 0x77);
                if(CommonVars.pSPIPosition != NULL)
                {
                    free(CommonVars.pSPIPosition);
                    CommonVars.pSPIPosition = NULL;
                }
                switch (RESDIR_RESO_MT)
                {
                    case 0:
                        CommonVars.SPIPosByteLen = 3;
                        break;
                    case 1:
                    case 2:
                        CommonVars.SPIPosByteLen = 4;
                        break;
                    case 3:
                    case 4:
                        CommonVars.SPIPosByteLen = 5;
                        break;
                    case 5:
                    case 6:
                        CommonVars.SPIPosByteLen = 6;
                        break;
                    default:
                        CommonVars.SPIPosByteLen = 7;
                        break;
                }
                CommonVars.pSPIPosition = (uint8_t*)malloc(CommonVars.SPIPosByteLen);
                IC_MHMfsm = READ_CFG;
            }
            else if(TempResult & (IC_MHM_STAT_FAIL_Msk | IC_MHM_STAT_DISMISS_Msk |IC_MHM_STAT_ERROR_Msk))
            {
                IC_MHMfsm = READ_RESO_DIR;                
            }
            free(pTemp);
            break;

        case READ_CFG:
            pTemp = (uint8_t*)RWWEE_ENC_CFG_ADDR;
            if(pTemp[RWWEE_ENC_CFG_TOTAL_LEN-1] == CalcCRC (IC_MHM_CRC_POLY, IC_MHM_CRC_START_VALUE, pTemp, RWWEE_ENC_CFG_TOTAL_LEN-1))
            {    
                memcpy((uint8_t*)(CommonVars.UserSclCfg),pTemp,sizeof(CommonVars.UserSclCfg));
                if((USR_SCL_EN_RD == SCALABLE)&&(USR_SCL_AVAIL_RD == ENC_CFG_AVAIL)&&(CommonVars.UserSclCfg[1] == CommonVars.ResoAndDir))
                {
                    SetScale(USR_SCALE);
                    memcpy(CommonVars.pPosLowOut, (uint8_t*)RWWEE_SCL_POS_L_H_ADDR, CommonVars.PosByteLen);
                    memcpy(CommonVars.pPosHighOut, (uint8_t*)(RWWEE_SCL_POS_L_H_ADDR+CommonVars.PosByteLen), CommonVars.PosByteLen);
                    CalcPosRange(CalcMTResCode(RESDIR_RESO_MT), USR_SCL_UF_OF_RD);
                    CalcROverRange(CalcMTResCode(RESDIR_RESO_MT));
                    CalcPosTransition(RESDIR_RESO_MT);
                }
                else
                {
                    SetScale(FACTORY_SCALE);
                }
            }
            else
            {
                SetScale(DEFAULT_SCALE);
            }
            IC_MHMfsm = READ_POS_1;
            break;

        case READ_POS_1:
            CommonVars.ExchgFlags = 0;
            IC_MHMAccessFree = 1;
            MHMTimer = READ_POS_TIMER_SET;
            IC_MHMfsm = READ_POS_2;
            break;

        case READ_POS_2:
            if(MHMTimer == (uint8_t)1)
            {
                MHMTimer = READ_POS_TIMER_SET;
                if(IC_MHMAccessFree)
                {
                    IC_MHMAccessFree = 0;
                    IC_MHMfsm = READ_POS_3;
                }
            }
            else if(IC_MHMAccessFree)
            {
                if(EXCHG_FLAG_PRESET)
                {
                    IC_MHMAccessFree = 0;
                    IC_MHMfsm = PV_PRESET;
                }
            }
            break;

        case READ_POS_3:
            TempResult = IC_MHM_ReadPos(CommonVars.pSPIPosition, CommonVars.SPIPosByteLen);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                //Check last byte received during SPI Read Position
                if(!(CommonVars.pSPIPosition[CommonVars.SPIPosByteLen-1] & IC_MHM_SPI_nERR_Msk)) 
                {
                    //nERR bit detected
                    IC_MHMfsm = READ_STATUS_1;
                }
                else
                {
                    //Check for nWARN bit
                    if(!(CommonVars.pSPIPosition[CommonVars.SPIPosByteLen-1] & IC_MHM_SPI_nWARN_Msk))
                    {
                        //nWARN bit detectec, excessive rotor speed
                    }

                    if(SCALE_ACTIVE_RD == USR_SCALE)
                    {
                        BuildPosition(SCALE_ACTIVE_RD);
                        if(EXCHG_FLAG_1STREAD)
                        {

                            ComparePosition(CommonVars.pPosition, CommonVars.pLastPos);
                            EXCHG_FLAG_NEWPOS_SET;
                        }
                        else
                        {
                            EXCHG_FLAG_1STREAD_SET;
                            //CommonVars.UF_OF_Cnt = 0;
                        }
                        CopyPosition (CommonVars.pPosition, CommonVars.pLastPos);
                    }
                    else
                    {   
                        BuildPosition(SCALE_ACTIVE_RD);
                        EXCHG_FLAG_NEWPOS_SET;
                    }
                    IC_MHMAccessFree = 1;
                    IC_MHMfsm = READ_POS_2;
                }
            }
            else if(TempResult & (IC_MHM_STAT_FAIL_Msk | IC_MHM_STAT_DISMISS_Msk |IC_MHM_STAT_ERROR_Msk))
            {
                IC_MHMfsm = READ_POS_2;                
            }
            break;

        case READ_STATUS_1:
            TempResult = IC_MHM_RdStatus(StatusReg);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                //Check Status Register 1, address 0x70
                if(StatusReg[0])
                {
                    //error detected
                }
                IC_MHMAccessFree = 1;
                IC_MHMfsm = READ_POS_2;
            }
            else if (TempResult & (IC_MHM_STAT_FAIL_Msk | IC_MHM_STAT_DISMISS_Msk |IC_MHM_STAT_ERROR_Msk))
            {
                IC_MHMfsm = READ_STATUS_1;
            }
            break;

        case PV_PRESET:
            EXCHG_FLAG_PRESETON_SET;
            TempResult = IC_MHM_PresetPV();
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                IC_MHMfsm = MHM_PRESET;
            }
            else if(TempResult & (IC_MHM_STAT_FAIL_Msk | IC_MHM_STAT_DISMISS_Msk |IC_MHM_STAT_ERROR_Msk))
            {
                IC_MHMAccessFree = 1;
                IC_MHMfsm = READ_POS_2;
            }
            break;

        case MHM_PRESET:
            TempResult = IC_MHM_RegWr(IC_MHM_PRES_RES_REG, IC_MHM_0x74_PRESET_Msk);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                IC_MHMfsm = MHM_STARTUP_1;
                EXCHG_FLAG_PRESETON_CLR;
            }
            else if(TempResult & (IC_MHM_STAT_FAIL_Msk | IC_MHM_STAT_DISMISS_Msk |IC_MHM_STAT_ERROR_Msk))
            {
                IC_MHMAccessFree = 1;
                IC_MHMfsm = READ_POS_2;
            }
            break;
    }
}

uint8_t* ExtDACWrite (uint8_t Command, uint8_t Data)
{
    ExtDACData[0] = Command;
    //Value is little endian, take MSB
    ExtDACData[1] = (*(((uint8_t*)(&Data))+1));
    //Value is little endian, take LSB
    ExtDACData[2] = (*((uint8_t*)(&Data)));
    //initializes pointer to external DAC Data
    pExtDACData = ExtDACData;
    return pExtDACData;
}

void ExtDACTask ()
{
    static uint8_t ExtDACTaskfsm = 0;
    
    //Chekc whether SPI1 is not busy and pointer to External DAC is initialized
    if(!SERCOM1_SPI_IsBusy() && pExtDACData != NULL)
    {
        switch (ExtDACTaskfsm)
        {
            case 0:
                NCS_DAC_Clear();
                //If SPI1 Write is possible then go to next step
                if(SERCOM1_SPI_Write(pExtDACData, sizeof(ExtDACData))) ExtDACTaskfsm++;
                else NCS_DAC_Set();
                break;
                
            case 1:
                NCS_DAC_Set();
                //Uninitializes pointer to External DAC Data
                pExtDACData = NULL;
                ExtDACTaskfsm = 0;
                break;
        }
    }
}

uint8_t CalcCRC (uint16_t CRCPoly, uint8_t StartVal, uint8_t* pData, uint8_t Length)
{
    uint8_t i=0,ucDataStream=0,ucCRC=StartVal;

    for (i=0;i<Length;i++)
    {
        ucDataStream = pData[i];
        for (i=0;i<8;i++)
        {
            if ((ucCRC & 0x80) != (ucDataStream & 0x80))
                ucCRC = (ucCRC<<1) ^ CRCPoly ;
            else
            ucCRC = (ucCRC<<1);
            ucDataStream = ucDataStream<<1 ;
        }
    }
    return ucCRC;
}

//External EEprom management functions
bool InitExtEEpromData(ExtEEpromDataType* pExtEEpromData)
{
	pExtEEpromData->TxData = (pExtEEpromData->TxLength)? (uint8_t*)malloc(pExtEEpromData->TxLength * sizeof(*(pExtEEpromData->TxData))) : NULL;
	pExtEEpromData->RxData = (pExtEEpromData->RxLength)? (uint8_t*)malloc(pExtEEpromData->RxLength * sizeof(*(pExtEEpromData->RxData))) : NULL;
    
    if(((pExtEEpromData->TxLength) && (pExtEEpromData->TxData == NULL))||((pExtEEpromData->RxLength) && (pExtEEpromData->RxData == NULL)))
        return false;
	else 
        return true;
}

void DeInitExtEEpromData(ExtEEpromDataType* pExtEEpromData)
{
	if(pExtEEpromData->TxData != NULL) free(pExtEEpromData->TxData);
    if(pExtEEpromData->RxData != NULL) free(pExtEEpromData->RxData);
}

bool ExtEEpromDataBufferInit(ExtEEpromDataType** pExtEEpromData, uint8_t SlaveAddr, uint8_t TxLength, uint8_t RxLength)
{
	ExtEEpromDataType* ptr = malloc(sizeof(ExtEEpromDataType));

    if(ptr != NULL)
    {
        ptr->ExtEEpromfsm = 0;
        ptr->SlaveAddr = SlaveAddr;
        ptr->TxLength = TxLength;
        ptr->RxLength = RxLength;
        ptr->MemoryAddr = 0;
        ptr->PgStartAddr = 0;
        ptr->NextPgAddr = 0;
        ptr->BytesToWr = 0;
        ptr->TxCnt = 0;
        ptr->pInitExtEEpromData = InitExtEEpromData;
        ptr->pDeInitExtEEpromData = DeInitExtEEpromData;
        if(ptr->pInitExtEEpromData(ptr))
        {
            (*pExtEEpromData) = ptr;
            return true;
        }
        else
        {
            if(ptr->TxData != NULL) free(ptr->TxData);
            if(ptr->RxData != NULL) free(ptr->RxData);
            return false;
        }
	}
    else
    {
        (*pExtEEpromData) = NULL;
        return false;
    }
}

void ExtEEpromDataBufferFree(ExtEEpromDataType** pExtEEpromData)
{
	(*pExtEEpromData)->pDeInitExtEEpromData (*pExtEEpromData);
	free(*pExtEEpromData);
	(*pExtEEpromData) = NULL;
}

uint8_t ExtEEpromRdWr(void)
{
    uint8_t Result = 0;
    
    if(!SERCOM2_I2C_IsBusy() && (pExtEEpromData != NULL))
    {
        switch (pExtEEpromData->ExtEEpromfsm)
        {
            case 0:
                pExtEEpromData->ExtEEpromfsm = ((pExtEEpromData->SlaveAddr & 0x01) == I2C_TRANSFER_READ) ? 1 : 3;
                break;
            
            case 1:
                //Read
                //Check memory size limit. Try to read more bytes than EEPROM_SIZE_BYTES, stop process.
                if((pExtEEpromData->MemoryAddr >= EEPROM_SIZE_BYTES) || ((pExtEEpromData->MemoryAddr + pExtEEpromData->RxLength) > EEPROM_SIZE_BYTES))
                    Result = 1;
                else
                {
                    if (SERCOM2_I2C_Write((((pExtEEpromData->MemoryAddr >> 7) & 0x0E) | pExtEEpromData->SlaveAddr) >> 1, (uint8_t*)(&pExtEEpromData->MemoryAddr), 1))
                        pExtEEpromData->ExtEEpromfsm = 2;
                    else
                        Result = 1;
                }
                break;
                
            case 2:
                if(SERCOM2_I2C_ErrorGet() != SERCOM_I2C_ERROR_NONE)
                {
                    if (SERCOM2_I2C_Read((((pExtEEpromData->MemoryAddr >> 7) & 0x0E) | pExtEEpromData->SlaveAddr) >> 1, pExtEEpromData->RxData, pExtEEpromData->RxLength))
                        Result = 2;
                    else
                        Result = 1;
                }
                //Any error while addressing I2C slave
                else
                    Result = 1;
                break;
                
            case 3:
                //Write
                //Check memory size limit. //Try to write more bytes than EEPROM_SIZE_BYTES, stop process.
                if((pExtEEpromData->MemoryAddr >= EEPROM_SIZE_BYTES) || ((pExtEEpromData->MemoryAddr + pExtEEpromData->TxLength) > EEPROM_SIZE_BYTES))
                    Result = 1;
                else if (pExtEEpromData->TxLength > pExtEEpromData->TxCnt)
                {
                    //Check current page overflow
                    pExtEEpromData->PgStartAddr = ((pExtEEpromData->MemoryAddr + pExtEEpromData->TxCnt)/WR_PAGE_SIZE_BYTES)*WR_PAGE_SIZE_BYTES;
                    pExtEEpromData->NextPgAddr = pExtEEpromData->PgStartAddr + WR_PAGE_SIZE_BYTES;
                    //page overflow
                    if((pExtEEpromData->MemoryAddr + pExtEEpromData->TxLength) >= pExtEEpromData->NextPgAddr)
                        pExtEEpromData->BytesToWr = pExtEEpromData->NextPgAddr - (pExtEEpromData->MemoryAddr + pExtEEpromData->TxCnt);
                    //no page overflow
                    else
                        pExtEEpromData->BytesToWr = pExtEEpromData->TxLength - pExtEEpromData->TxCnt;
                    pExtEEpromData->pData = (uint8_t*)malloc(sizeof(*pExtEEpromData->pData) * (pExtEEpromData->BytesToWr + 1));
                    if(pExtEEpromData->pData != NULL)
                    {                    
                        pExtEEpromData->pData[0] = (uint8_t)(pExtEEpromData->MemoryAddr + pExtEEpromData->TxCnt);
                        memcpy(&pExtEEpromData->pData[1], &pExtEEpromData->TxData[pExtEEpromData->TxCnt], pExtEEpromData->BytesToWr);
                        if (SERCOM2_I2C_Write(((((pExtEEpromData->MemoryAddr+pExtEEpromData->TxCnt)>>7)&0x0E)|pExtEEpromData->SlaveAddr)>>1, pExtEEpromData->pData, pExtEEpromData->BytesToWr + 1))
                        {
                            pExtEEpromData->TxCnt += pExtEEpromData->BytesToWr;
                            ExtEEpromTimer = EXT_EEPROM_TWR_SET;
                            pExtEEpromData->ExtEEpromfsm = 4;
                        }
                        else
                        {
                            free(pExtEEpromData->pData);
                            pExtEEpromData->pData= NULL;
                            Result = 1;
                        }
                    }
                    else
                        Result = 1;
                }
                else
                    Result = 0x02;
                break;

            case 4:
                if(SERCOM2_I2C_ErrorGet() != SERCOM_I2C_ERROR_NONE)
                {
                    if(ExtEEpromTimer == (uint8_t)1)
                    {
                        free(pExtEEpromData->pData);
                        pExtEEpromData->pData= NULL;
                        pExtEEpromData->ExtEEpromfsm = 3;
                    }
                }
                else
                    Result = 1;
                break;
        }
    }
    return Result;
}

//Internal MCU EEPROM (RWWEEprom) Writing functions
bool InitRWWEEWrData (IntRWWEEWrType* ptr)
{
    ptr->pData = (uint8_t*)malloc((ptr->length) * sizeof(*ptr->pData));
    ptr->pRowData = (uint32_t*)malloc(NVMCTRL_RWWEEPROM_ROWSIZE/sizeof(*ptr->pRowData));
    if((ptr->pData != NULL) && (ptr->pRowData != NULL)) return true;
    else return false;
}
    
void DeInitRWWEEWrData(IntRWWEEWrType* ptr)
{
    if(ptr->pData != NULL) free(ptr->pData);
    if(ptr->pRowData != NULL) free(ptr->pRowData);
}

bool RWWEEWrInit (IntRWWEEWrType** pIntRWWEEWr, uint32_t Address, uint16_t length)
{
    IntRWWEEWrType* ptr = (IntRWWEEWrType*)malloc(sizeof(IntRWWEEWrType));
    
    if(ptr != NULL)
    {
        ptr->IntEEpromWrfsm = 0;
        ptr->PageCnt = 0;
        ptr->ByteCnt = 0;
        ptr->RowStartAddr = 0;
        ptr->NextRowAddr = 0;
        ptr->BytesToWr = 0;
        ptr->Address = Address;
        ptr->length = length;
        ptr->pInitRWWEEWrData = InitRWWEEWrData;
        ptr->pDeInitRWWEEWrData = DeInitRWWEEWrData;
        if(ptr->pInitRWWEEWrData(ptr))
        {
            (*pIntRWWEEWr) = ptr;
            return true;
        }
        else
        {
            if(ptr->pData != NULL) free(ptr->pData);
            if(ptr->pRowData != NULL) free(ptr->pRowData);
            return false;
        }
    }
    else
    {
        (*pIntRWWEEWr) = NULL;
        return false;
    }
}

void RWWEEWRfree (IntRWWEEWrType** pIntRWWEEWr)
{
    (*pIntRWWEEWr)->pDeInitRWWEEWrData(*pIntRWWEEWr);
    free(*pIntRWWEEWr);
    (*pIntRWWEEWr) = NULL;
}

uint8_t IntEEpromWrite(void)
{
    uint8_t result = 0;
    
    if(!NVMCTRL_IsBusy() || (pIntRWWEEWr != NULL))
    {
        switch (pIntRWWEEWr->IntEEpromWrfsm)
        {
            case 0:
                //Check memory size limit
                if ((pIntRWWEEWr->Address >= (NVMCTRL_RWWEEPROM_START_ADDRESS+NVMCTRL_RWWEEPROM_SIZE))||((pIntRWWEEWr->Address+pIntRWWEEWr->length) > (NVMCTRL_RWWEEPROM_START_ADDRESS + NVMCTRL_RWWEEPROM_SIZE)))
                    result = 1;
                //Check if all bytes stored
                else if(pIntRWWEEWr->length > pIntRWWEEWr->ByteCnt)
                {
                    //Check page size limit
                    pIntRWWEEWr->RowStartAddr = ((pIntRWWEEWr->Address + pIntRWWEEWr->ByteCnt)/NVMCTRL_RWWEEPROM_ROWSIZE)*NVMCTRL_RWWEEPROM_ROWSIZE;
                    pIntRWWEEWr->NextRowAddr = pIntRWWEEWr->RowStartAddr + NVMCTRL_RWWEEPROM_ROWSIZE;
                    //Check row overflow
                    if((pIntRWWEEWr->Address + pIntRWWEEWr->length) >= pIntRWWEEWr->NextRowAddr)
                        pIntRWWEEWr->BytesToWr = pIntRWWEEWr->NextRowAddr-(pIntRWWEEWr->Address + pIntRWWEEWr->ByteCnt);
                    //no row overflow
                    else
                        pIntRWWEEWr->BytesToWr = (pIntRWWEEWr->length - pIntRWWEEWr->ByteCnt);
                    //Read whole page information and copy it to ptr
                    NVMCTRL_RWWEEPROM_Read(pIntRWWEEWr->pRowData, NVMCTRL_RWWEEPROM_ROWSIZE/sizeof(*pIntRWWEEWr->pRowData), pIntRWWEEWr->RowStartAddr);
                    //copy new information to ptr
                    memcpy(( ((uint8_t*)pIntRWWEEWr->pRowData) + pIntRWWEEWr->Address + pIntRWWEEWr->ByteCnt - pIntRWWEEWr->RowStartAddr), pIntRWWEEWr->pData + pIntRWWEEWr->ByteCnt, pIntRWWEEWr->BytesToWr);
                    //Erase row before writing pages
                    NVMCTRL_RWWEEPROM_RowErase(pIntRWWEEWr->RowStartAddr);
                    pIntRWWEEWr->PageCnt = 0;
                    pIntRWWEEWr->IntEEpromWrfsm++;
                }
                else
                    pIntRWWEEWr->ByteCnt = 0;
                break;

            case 1:
                //Check page counter when writing pages
                if(pIntRWWEEWr->PageCnt < 4)
                {
                    NVMCTRL_RWWEEPROM_PageWrite(pIntRWWEEWr->pRowData, pIntRWWEEWr->RowStartAddr + (pIntRWWEEWr->PageCnt * NVMCTRL_RWWEEPROM_PAGESIZE));
                    pIntRWWEEWr->PageCnt++;
                }
                else
                {
                    pIntRWWEEWr->ByteCnt += pIntRWWEEWr->BytesToWr;
                    pIntRWWEEWr->IntEEpromWrfsm = 0;
                }
                break;
        }
    }
    return result;
}
/* *****************************************************************************
 End of File
 */
