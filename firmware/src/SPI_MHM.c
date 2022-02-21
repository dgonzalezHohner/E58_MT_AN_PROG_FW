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
static uint8_t PositionRead[7];
static uint8_t StatusReg[4];
static uint8_t IC_MHMCmdfsm = 0;
static uint8_t IC_MHMProcFsm = 0;
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

uint8_t IC_MHM_ReadPos(uint8_t* Data)
{
    uint8_t Result = 0;
    
    if(pMHMRegAccData == NULL) MHMRegAccBufferInit(&pMHMRegAccData, POS_READ_OPC, 1, 8);
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

void IC_MHM_Task()
{
    static IC_MHMfsmType IC_MHMfsm = MHM_STARTUP_1;
    static uint8_t PowerUp = 1;
    static uint8_t StartUpCnt = 0;
    uint8_t TempResult = 0;
    
    if(BISS_MASTER_Get())
    {
        IC_MHMfsm = MHM_STARTUP_1;
        IC_MHMAccessFree = 0;
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
                TempResult = IC_MHM_PresetPV();
                if(TempResult & IC_MHM_STAT_VALID_Msk) IC_MHMfsm = MHM_STARTUP_5;
                break;

            case MHM_STARTUP_5:
                TempResult = IC_MHM_RegWr(IC_MHM_PRES_RES_REG, IC_MHM_0x74_RESET_Msk);
                if(TempResult & IC_MHM_STAT_VALID_Msk) IC_MHMfsm = MHM_STARTUP_1;
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
                TempResult = IC_MHM_ReadPos(PositionRead);
                if(TempResult & IC_MHM_STAT_VALID_Msk)
                {
                    if(!(PositionRead[6] & IC_MHM_SPI_nERR_Msk)) 
                    {
                        //nERR bit detectec
                        IC_MHMfsm = READ_STATUS_1;
                    }
                    else
                    {
                        //Check for nWARN bit
                        if(!(PositionRead[6] & IC_MHM_SPI_nWARN_Msk))
                        {
                            //nWARN bit detectec, excessive rotor speed
                        }
                        //Read MT Position from PositionRead[1] to PositionRead[4]
                        //Read ST Position from PositionRead[5] to PositionRead[6]
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
void InitExtEEpromData(ExtEEpromDataType* pExtEEpromData)
{
	pExtEEpromData->TxData = (uint8_t*)malloc(pExtEEpromData->TxLength * sizeof(*(pExtEEpromData->TxData)));
	pExtEEpromData->RxData = (uint8_t*)malloc(pExtEEpromData->RxLength * sizeof(*(pExtEEpromData->RxData)));
}

void DeInitExtEEpromData(ExtEEpromDataType* pExtEEpromData)
{
	free(pExtEEpromData->TxData);
	free(pExtEEpromData->RxData);
}

void ExtEEpromDataBufferInit(ExtEEpromDataType** pExtEEpromData, uint8_t SlaveAddr, uint8_t TxLength, uint8_t RxLength)
{
	ExtEEpromDataType* ptr = malloc(sizeof(ExtEEpromDataType));

	ptr->SlaveAddr = SlaveAddr;
    ptr->TxLength = TxLength;
    ptr->RxLength = RxLength;
	ptr->Result = 0;
    ptr->MemoryAddr = 0;
    ptr->NextPgAddr = 0;
    ptr->NextPgGap = 0;
    ptr->TxCnt = 0;
    ptr->pInitExtEEpromData = InitExtEEpromData;
	ptr->pDeInitExtEEpromData = DeInitExtEEpromData;
	ptr->pInitExtEEpromData (ptr);
	*pExtEEpromData = ptr;
}

void ExtEEpromDataBufferFree(ExtEEpromDataType** pExtEEpromData)
{
	(*pExtEEpromData)->pDeInitExtEEpromData (*pExtEEpromData);
	free(*pExtEEpromData);
	(*pExtEEpromData) = NULL;
}

void ExtEEpromTask()
{
    static uint8_t ExtEEpromfsm = 0;
    static uint8_t* pData = NULL;
    
    if(!SERCOM2_I2C_IsBusy() && pExtEEpromData != NULL)
    {
        switch (ExtEEpromfsm)
        {
            case 0:
                ExtEEpromfsm = ((pExtEEpromData->SlaveAddr & 0x01) == I2C_TRANSFER_READ) ? 1 : 3;
                break;
            
            case 1:
                //Read
                //Check memory size limit.
                if((pExtEEpromData->MemoryAddr >= EEPROM_SIZE_BYTES) || ((pExtEEpromData->MemoryAddr + pExtEEpromData->RxLength) > EEPROM_SIZE_BYTES))
                {
                    //Try to read more bytes than EEPROM_SIZE_BYTES, stop process
                    ExtEEpromfsm = 0;
                    pExtEEpromData->Result = 0x01;
                }
                else
                {
                    if (SERCOM2_I2C_Write((((pExtEEpromData->MemoryAddr >> 7) & 0x0E) | pExtEEpromData->SlaveAddr) >> 1, (uint8_t*)(&pExtEEpromData->MemoryAddr), 1))
                        ExtEEpromfsm = 2;
                }
                break;
                
            case 2:
                if (SERCOM2_I2C_Read((((pExtEEpromData->MemoryAddr >> 7) & 0x0E) | pExtEEpromData->SlaveAddr) >> 1, pExtEEpromData->RxData, pExtEEpromData->RxLength))
                {
                    ExtEEpromfsm = 0;
                    pExtEEpromData->Result = 0x02;
                }
                break;
                
            case 3:
                //Write
                //Check memory size limit.
                if((pExtEEpromData->MemoryAddr >= EEPROM_SIZE_BYTES) || ((pExtEEpromData->MemoryAddr + pExtEEpromData->TxLength) > EEPROM_SIZE_BYTES))
                {
                    //Try to write more bytes than EEPROM_SIZE_BYTES, stop process
                    ExtEEpromfsm = 0;
                    pExtEEpromData->Result = 0x01;
                }
                else if (pExtEEpromData->TxLength > pExtEEpromData->TxCnt)
                {
                    //Check current page overflow
                    pExtEEpromData->NextPgAddr = ((((pExtEEpromData->MemoryAddr + pExtEEpromData->TxCnt)/WR_PAGE_SIZE_BYTES)+1)*WR_PAGE_SIZE_BYTES);
                    if((pExtEEpromData->MemoryAddr + pExtEEpromData->TxLength) >= pExtEEpromData->NextPgAddr)
                    {
                        pExtEEpromData->NextPgGap = pExtEEpromData->NextPgAddr - (pExtEEpromData->MemoryAddr + pExtEEpromData->TxCnt);
                        pData = (uint8_t*)malloc(sizeof(*pData) * (pExtEEpromData->NextPgGap + 1));
                        pData[0] = (uint8_t)(pExtEEpromData->MemoryAddr + pExtEEpromData->TxCnt);
                        memcpy(&pData[1], &pExtEEpromData->TxData[pExtEEpromData->TxCnt], pExtEEpromData->NextPgGap);
                        if (SERCOM2_I2C_Write(((((pExtEEpromData->MemoryAddr+pExtEEpromData->TxCnt)>>7)&0x0E)|pExtEEpromData->SlaveAddr)>>1, pData, (pExtEEpromData->NextPgGap + 1)))
                        {
                            pExtEEpromData->TxCnt += pExtEEpromData->NextPgGap;
                            ExtEEpromTimer = EXT_EEPROM_TWR_SET;
                            ExtEEpromfsm = 4;
                        }
                        else
                        {
                            free(pData);
                            pData= NULL;
                        }
                    }
                    //no page overflow
                    else
                    {
                        pExtEEpromData->NextPgGap = 0;
                        pData = (uint8_t*)malloc(sizeof(*pData) * (pExtEEpromData->TxLength - pExtEEpromData->TxCnt + 1));
                        pData[0] = (uint8_t)(pExtEEpromData->MemoryAddr + pExtEEpromData->TxCnt);
                        memcpy(&pData[1], &pExtEEpromData->TxData[pExtEEpromData->TxCnt], pExtEEpromData->TxLength - pExtEEpromData->TxCnt);
                        if (SERCOM2_I2C_Write(((((pExtEEpromData->MemoryAddr+pExtEEpromData->TxCnt)>>7)&0x0E)|pExtEEpromData->SlaveAddr)>>1, pData, pExtEEpromData->TxLength-pExtEEpromData->RxLength+1))
                        {
                            pExtEEpromData->TxCnt += (pExtEEpromData->TxLength-pExtEEpromData->TxCnt);
                            ExtEEpromTimer = EXT_EEPROM_TWR_SET;
                            ExtEEpromfsm = 4;
                        }
                        else
                        {
                            free(pData);
                            pData= NULL;
                        }
                    }
                }
                else
                {
                    ExtEEpromfsm = 0;
                    pExtEEpromData->Result = 0x02;
                }
                break;

            case 4:
                if(ExtEEpromTimer == (uint8_t)1)
                {
                    free(pData);
                    pData= NULL;
                    ExtEEpromfsm = 3;
                }
                break;
        }
    }
    
}

uint8_t IntEEpromWrite(uint8_t* Data, uint32_t Address, uint16_t length)
{
    static uint32_t* ptr;
    static uint8_t IntEEpromWrfsm = 0;
    static uint8_t PageCnt = 0;
    static uint8_t ByteCnt = 0;
    static uint32_t RowStartAddr = 0;
    static uint32_t NextRowAddr = 0;
    static uint32_t BytesToWr = 0;
    uint8_t result = 0;
    
    if(!NVMCTRL_IsBusy())
    {
        switch (IntEEpromWrfsm)
        {
            case 0:
                //Check memory size limit
                if ((Address>=(NVMCTRL_RWWEEPROM_START_ADDRESS+NVMCTRL_RWWEEPROM_SIZE))||((Address+length)>(NVMCTRL_RWWEEPROM_START_ADDRESS + NVMCTRL_RWWEEPROM_SIZE)))
                {

                    ByteCnt = 0;
                    result = 1;

                }
                else if(length>ByteCnt)
                {
                    //Check page size limit
                    RowStartAddr = ((Address+ByteCnt)/NVMCTRL_RWWEEPROM_ROWSIZE)*NVMCTRL_RWWEEPROM_ROWSIZE;
                    NextRowAddr = RowStartAddr + NVMCTRL_RWWEEPROM_ROWSIZE;
                    //Check row overflow
                    if(((Address+ByteCnt)+(length-ByteCnt))>=NextRowAddr)
                        BytesToWr = NextRowAddr-(Address+ByteCnt);
                    //no row overflow
                    else
                        BytesToWr = (length-ByteCnt);
                    //Allocate 256 bytes corresponding to RWWEE_ROW_SIZE, 4 pages, pointed by 4 bytes
                    ptr = (uint32_t*)malloc(NVMCTRL_RWWEEPROM_ROWSIZE/sizeof(*ptr));
                    //Read whole page information and copy it to ptr
                    NVMCTRL_RWWEEPROM_Read(ptr, NVMCTRL_RWWEEPROM_ROWSIZE/sizeof(*ptr), RowStartAddr);
                    //copy new information to ptr
                    memcpy((((uint8_t*)ptr)+Address+ByteCnt-RowStartAddr), Data+ByteCnt, BytesToWr);
                    //Erase row before writing pages
                    NVMCTRL_RWWEEPROM_RowErase(RowStartAddr);
                    PageCnt = 0;
                    IntEEpromWrfsm++;
                }
                else
                {
                    ByteCnt = 0;
                    result = 2;
                }
                break;

            case 1:
                //Check page counter when writing pages
                if(PageCnt < 4)
                {
                    NVMCTRL_RWWEEPROM_PageWrite(ptr, RowStartAddr+(PageCnt*NVMCTRL_RWWEEPROM_PAGESIZE));
                    PageCnt++;
                }
                else
                {
                    free(ptr);
                    ByteCnt += BytesToWr;
                    IntEEpromWrfsm = 0;
                }
                break;
        }
    }
    return result;
}
/* *****************************************************************************
 End of File
 */
