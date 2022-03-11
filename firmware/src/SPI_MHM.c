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
bool IC_MHMAccessFree = 0;
static IC_MHM_REG_ACCType* pMHMRegAccData = NULL;

//Externla DAC variables and pointers.
static uint8_t ExtDACData[3] = {0,0,0};
static uint8_t* pExtDACData = NULL;

//External EEPROM variables and pointers
static ExtEEpromDataType* pExtEEpromData = NULL;
volatile uint8_t ExtEEpromTimer = 0;

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
bool IsMHMAccessFree()
{
    return IC_MHMAccessFree;
}

void SetMHMAccessFree()
{
    IC_MHMAccessFree = (bool)1;
}

void SetMHMAccessDenied()
{
    IC_MHMAccessFree = (bool)0;
}

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

void TimerTask()
{
     if( MHMTimer > 1)  MHMTimer--;
     if( MHMProcTimer > 1)  MHMProcTimer--;
     if( ExtEEpromTimer > 1) ExtEEpromTimer--;
}

void IC_MHM_RegAccesTask()
{
    static SPI_IC_MHMType* pSPIMHM = NULL;
    
    if(BISS_MASTER_Get())
    {
        RegAccessfsm = 6;
        NCS_MHM_Set();
    }
    else if(!SERCOM0_SPI_IsBusy())
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
                
            case 6:
                if(pSPIMHM != NULL) IC_MHM_SPIBufferFree(&pSPIMHM);
                RegAccessfsm = 0;
                break;
        }
    }
}

uint8_t IC_MHM_ReadPos(uint8_t* Data, uint8_t RxLength)
{
    uint8_t Result = 0;
    
    if(pMHMRegAccData == NULL) MHMRegAccBufferInit(&pMHMRegAccData, POS_READ_OPC, 1, RxLength+1);
    else
    {
        IC_MHM_RegAccesTask();
        if(pMHMRegAccData->Result)
        {
            memcpy(Data, &pMHMRegAccData->RxData[1], 7);
            Result = pMHMRegAccData->Result;
            MHMRegAccBufferFree(&pMHMRegAccData);
        }
    }
    return Result;
}

uint8_t IC_MHM_RdStatus(uint8_t* Data)
{
    uint8_t Result = 0;
    
    if(pMHMRegAccData == NULL) MHMRegAccBufferInit(&pMHMRegAccData, READ_STATUS_OPC, 1, 5);
    else
    {
        IC_MHM_RegAccesTask();
        if(pMHMRegAccData->Result)
        {
            memcpy(Data, &pMHMRegAccData->RxData[1], 4);
            Result = pMHMRegAccData->Result;
            MHMRegAccBufferFree(&pMHMRegAccData);
        }
    }
    return Result;
}

uint8_t IC_MHM_RegRdCtd(uint8_t Address, uint8_t* Data, uint8_t Length)
{
    uint8_t Result = 0;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, REG_RD_CTD_OPC, 2, Length+2);
        pMHMRegAccData->TxData[1] = Address;
    }
    else
    {
        IC_MHM_RegAccesTask();
        if(pMHMRegAccData->Result)
        {
            memcpy(Data, &pMHMRegAccData->RxData[2], pMHMRegAccData->RxLength-2);
            Result = pMHMRegAccData->Result;
            MHMRegAccBufferFree(&pMHMRegAccData);
        }
    }
    return Result;
}

uint8_t IC_MHM_RegWrCt(uint8_t Address, uint8_t* Data, uint8_t Length)
{
    uint8_t Result = 0;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, REG_WR_CTD_OPC, Length+2, 0);
        pMHMRegAccData->TxData[1] = Address;
        memcpy(&pMHMRegAccData->TxData[2], Data, Length);
    }
    else
    {
        IC_MHM_RegAccesTask();
        if(pMHMRegAccData->Result)
        {
            Result = pMHMRegAccData->Result;
            MHMRegAccBufferFree(&pMHMRegAccData);
        }
    }
    return Result;
}

uint8_t IC_MHM_WrInstr(uint8_t* Data, uint8_t Length)
{
    uint8_t Result = 0;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, WR_INST_OPC, Length+1, 0);
        memcpy(&pMHMRegAccData->TxData[1], Data, Length);
    }
    else
    {
        IC_MHM_RegAccesTask();
        if(pMHMRegAccData->Result)
        {
            Result = pMHMRegAccData->Result;
            MHMRegAccBufferFree(&pMHMRegAccData);
        }
    }
    return Result;
}

uint8_t IC_MHM_RegWr(uint8_t Address, uint8_t Data)
{
    uint8_t Result = 0;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, REG_WR_OPC, 3, 0);
        pMHMRegAccData->TxData[1] = Address;
        pMHMRegAccData->TxData[2] = Data;
    }
    else
    {
        IC_MHM_RegAccesTask();
        if(pMHMRegAccData->Result)
        {
            Result = pMHMRegAccData->Result;
            MHMRegAccBufferFree(&pMHMRegAccData);
        }
    }
    return Result;
}

uint8_t IC_MHM_RegRd(uint8_t Address, uint8_t* Data)
{
    uint8_t Result = 0;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, REG_RD_OPC, 2, 3);
        pMHMRegAccData->TxData[1] = Address;
    }
    else
    {
        IC_MHM_RegAccesTask();
        if(pMHMRegAccData->Result)
        {
            Result = pMHMRegAccData->Result;
            *Data = pMHMRegAccData->RxData[2];
            MHMRegAccBufferFree(&pMHMRegAccData);
        }
    }
    return Result;
}

uint8_t IC_MHM_Activate(uint8_t Data)
{
    uint8_t Result = 0;
    
    if(pMHMRegAccData == NULL)
    {
        MHMRegAccBufferInit(&pMHMRegAccData, ACTIVATE_OPC, 2, 0);
        pMHMRegAccData->TxData[1] = Data | 0x80;
    }
    else
    {
        IC_MHM_RegAccesTask();
        if(pMHMRegAccData->Result)
        {
            Result = pMHMRegAccData->Result;
            MHMRegAccBufferFree(&pMHMRegAccData);
        }
    }
    return Result;
}

uint8_t IC_MHM_SetFIO(uint8_t Data)
{
    uint8_t TempResult = 0;
    static uint8_t TempData = 0;
    
    switch (IC_MHMCmdfsm)
    {
        case 0:
            TempResult = IC_MHM_RegRd(IC_MHM_FIO_REG_ADDR, &TempData);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                TempData |= Data;
                IC_MHMCmdfsm++;
            }
            TempResult = 0;
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
    uint8_t TempResult = 0;
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
            TempResult = 0;
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
    uint8_t TempResult = 0;
    
    switch (IC_MHMProcFsm)
    {
        case 0:
            TempResult = IC_MHM_SetFIO(IC_MHM_0x75_FIO_3_Msk);
            if(TempResult & IC_MHM_STAT_VALID_Msk)
            {
                MHMProcTimer = PV_PRESET_TIMER_SET;
                IC_MHMProcFsm++;
            }
            TempResult = 0;
            break;
            
        case 1:
            if(MHMProcTimer == 1)
            {
                TempResult = IC_MHM_ClrFIO(IC_MHM_0x75_FIO_3_Msk);
                if(TempResult & IC_MHM_STAT_VALID_Msk)
                {
                    MHMProcTimer = PV_PRESET_TIMER_SET;
                    IC_MHMProcFsm++;
                }
            }
            break;
        
        case 2:
            if(MHMProcTimer == 1)
            {
                MHMProcTimer = 0;
                IC_MHMProcFsm++;
            }
            break;
    }
    return TempResult;
}
void BuildPosition (uint8_t Scaling)
{
    uint8_t ResoMT;
    
    for (uint8_t i=0;i<((CommonVars.SPIPosByteLen-1)>=CommonVars.PosByteLen)?CommonVars.PosByteLen:(CommonVars.SPIPosByteLen-1);i++)
    {
        //pSPIPosition contains data MSB first, BIG endian
        //MCU works in Little endian, swap bytes required
        CommonVars.pPosition[i] = CommonVars.pSPIPosition[CommonVars.SPIPosByteLen-2-i];
    }
    
    switch (Scaling)
    {
        case FACTORY_SCALE:
            ResoMT = (USR_SCL_RESOMT <= RESDIR_RESO_MT)?USR_SCL_RESOMT:RESDIR_RESO_MT;
            break;
        case DEFAULT_SCALE:
            ResoMT = 0;
            break;
        default:
            ResoMT = RESDIR_RESO_MT;
            break;
    }
    
    switch (ResoMT)
    {
        case 0:
            *((uint16_t*)(CommonVars.pPosition)) = (0xFFFF >> RESDIR_RESO_ST);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            *((uint16_t*)(&CommonVars.pPosition[0])) &= (0xFFFF >> RESDIR_RESO_ST);
            *((uint16_t*)(&CommonVars.pPosition[0])) <<= RESDIR_RESO_ST;
            *((uint16_t*)(&CommonVars.pPosition[2])) &= ((uint16_t)0xFFFF)>>(16-(4*ResoMT));
            *((uint32_t*)(&CommonVars.pPosition[0])) >>= RESDIR_RESO_ST;
            break;
        default:
            *((uint16_t*)(&CommonVars.pPosition[0])) &= (0xFFFF >> RESDIR_RESO_ST);
            *((uint16_t*)(&CommonVars.pPosition[0])) <<= RESDIR_RESO_ST;
            *((uint32_t*)(&CommonVars.pPosition[2])) &= ((uint32_t)0xFFFFFFFF)>>(32-(4*ResoMT));
            *((uint64_t*)(&CommonVars.pPosition[0])) >>= RESDIR_RESO_ST;
            *((uint16_t*)(&CommonVars.pPosition[6])) = 0x0000;
            break;
    }
}

void pPosSetUp (uint8_t ResoMT)
{
    if(CommonVars.pPosition != NULL)
    {
        free(CommonVars.pPosition);
        CommonVars.pPosition = NULL;
    }
    if(CommonVars.pPosLowOut != NULL)
    {
        free(CommonVars.pPosLowOut);
        CommonVars.pPosLowOut = NULL;
    }
    if(CommonVars.pPosHighOut != NULL)
    {
        free(CommonVars.pPosHighOut);
        CommonVars.pPosHighOut = NULL;
    }

    switch (ResoMT)
    {
        case 0:
            CommonVars.PosByteLen = 2;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            CommonVars.PosByteLen = 4;
            break;
        default:
            CommonVars.PosByteLen = 8;
            break;
    }
    CommonVars.pPosition = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pPosLowOut = (uint8_t*)malloc(CommonVars.PosByteLen);
    CommonVars.pPosHighOut = (uint8_t*)malloc(CommonVars.PosByteLen);
    
}

void SetScale(UsedScaleType Scaling)
{
    uint8_t ResoMT;

    //Set position low to 0, position high to 1 turn.
    memset(CommonVars.pPosLowOut, 0x00, CommonVars.PosByteLen);
    memset(CommonVars.pPosHighOut, 0x00, CommonVars.PosByteLen);
    
    switch (Scaling)
    {
        case FACTORY_SCALE:
            ResoMT = (USR_SCL_RESOMT <= RESDIR_RESO_MT)?USR_SCL_RESOMT:RESDIR_RESO_MT;
            switch(ResoMT)
            {
                case 0:
                    //Check ST fractional setting
                    if(USR_SCL_FRACT_RNG_USE)
                    {
                        *((uint16_t*)(CommonVars.pPosHighOut)) = (uint16_t)(((*((uint16_t*)RWWEE_FRACT_RANGE_ADDR))*(((uint32_t)0x10000)>>RESDIR_RESO_ST))/3600);
                    }
                    //ST no fractional
                    else
                    {
                        *((uint16_t*)(CommonVars.pPosHighOut)) = 0xFFFF >> RESDIR_RESO_ST;
                    }
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                    *((uint16_t*)(&CommonVars.pPosHighOut[0])) = 0xFFFF;
                    *((uint16_t*)(&CommonVars.pPosHighOut[2])) = ((uint16_t)0xFFFF)>>(16-(4*ResoMT));
                    *((uint32_t*)(&CommonVars.pPosHighOut[0])) >>= RESDIR_RESO_ST;
                    break;
                case 5:
                case 6:
                    *((uint16_t*)(&CommonVars.pPosHighOut[0])) = 0xFFFF;
                    *((uint32_t*)(&CommonVars.pPosHighOut[2])) = ((uint32_t)0xFFFFFFFF)>>(32-(4*ResoMT));
                    *((uint64_t*)(&CommonVars.pPosHighOut[0])) >>= RESDIR_RESO_ST;
                    break;
                default:
                    *((uint16_t*)(&CommonVars.pPosHighOut[0])) = 0xFFFF;
                    *((uint32_t*)(&CommonVars.pPosHighOut[2])) = (uint32_t)0xFFFFFFFF;
                    *((uint64_t*)(&CommonVars.pPosHighOut[0])) >>= RESDIR_RESO_ST;
                    break;
            }
            break;
            
        case DEFAULT_SCALE:
            CommonVars.UserSclCfg[0] = (uint8_t)0;
            CommonVars.UserSclCfg[1] = CommonVars.ResoAndDir;
            *((uint16_t*)(CommonVars.pPosHighOut)) = 0xFFFF >> RESDIR_RESO_ST;
            break;
            
        default:
            memcpy(CommonVars.pPosLowOut, (uint8_t*)RWWEE_SCL_POS_L_H_ADDR, CommonVars.PosByteLen);
            memcpy(CommonVars.pPosHighOut, (uint8_t*)(RWWEE_SCL_POS_L_H_ADDR+CommonVars.PosByteLen), CommonVars.PosByteLen);
            break;
    }
    SCALE_ACTIVE_WR(Scaling);
}
void IC_MHM_Task()
{
    static IC_MHMfsmType IC_MHMfsm = MHM_STARTUP_1;
//    static uint8_t PowerUp = 1;
//    static uint8_t StartUpCnt = 0;
    uint8_t TempResult = 0;
    uint8_t* pTemp = NULL;
    
    if(BISS_MASTER_Get())
    {
        IC_MHMfsm = MHM_STARTUP_1;
        IC_MHMAccessFree = 0;
        IC_MHMCmdfsm = 0;
        IC_MHMProcFsm = 0;
        if(pMHMRegAccData != NULL) MHMRegAccBufferFree(&pMHMRegAccData);
    }
    else
    {
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
                    if(NERR_Get()) IC_MHMfsm = READ_RESO_DIR;
                    else IC_MHMfsm = MHM_STARTUP_1;
                }
                break;
                
            case READ_RESO_DIR:
                //Read IC-MHM registers 0 and 1 and get RESO_MT i RESO_ST
                pTemp = (uint8_t*)malloc(USER_SCL_CFG_LEN);
                TempResult = IC_MHM_RegRdCtd(IC_MHM_REG0_ADDR, pTemp, USER_SCL_CFG_LEN);
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
                    memcpy((uint8_t*)(&CommonVars.UserSclCfg),pTemp,sizeof(CommonVars.UserSclCfg));
                    if((USR_SCL_EN == SCALABLE)&&(USR_SCL_AVAIL == RWWEE_ENC_CFG_AVAIL)&&(CommonVars.UserSclCfg[1] == CommonVars.ResoAndDir))
                    {
                        pPosSetUp(RESDIR_RESO_MT);
                        SetScale(USR_SCALE);
                    }
                    else
                    {
                        pPosSetUp((USR_SCL_RESOMT <= RESDIR_RESO_MT)?USR_SCL_RESOMT:RESDIR_RESO_MT);
                        SetScale(FACTORY_SCALE);
                    }
                }
                else
                {
                    pPosSetUp(RESDIR_RESO_MT);
                    SetScale(DEFAULT_SCALE);
                }
                IC_MHMfsm = READ_POS_1;
                break;

            case READ_POS_1:
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
                break;
                
            case READ_POS_3:
                TempResult = IC_MHM_ReadPos(CommonVars.pSPIPosition, CommonVars.SPIPosByteLen);
                if(TempResult & IC_MHM_STAT_VALID_Msk)
                {
                    //Check last byte received during SPI Read Position
                    if(!(CommonVars.pSPIPosition[CommonVars.SPIPosByteLen-1] & IC_MHM_SPI_nERR_Msk)) 
                    {
                        //nERR bit detectec
                        IC_MHMfsm = READ_STATUS_1;
                    }
                    else
                    {
                        //Check for nWARN bit
                        if(!(CommonVars.pSPIPosition[CommonVars.SPIPosByteLen-1] & IC_MHM_SPI_nWARN_Msk))
                        {
                            //nWARN bit detectec, excessive rotor speed
                        }
                        //Read ST Position from PositionRead[5] to PositionRead[6]
                        BuildPosition(SCALE_ACTIVE_RD);
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
        }
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
                //If SPI1 Write is possible then go to next step
                if(SERCOM1_SPI_Write(pExtDACData, sizeof(ExtDACData))) ExtDACTaskfsm++;
                break;
                
            case 1:
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
