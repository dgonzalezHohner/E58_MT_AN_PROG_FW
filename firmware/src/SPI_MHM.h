/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _SPI_MHM    /* Guard against multiple inclusion */
#define _SPI_MHM


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
//#define EXAMPLE_CONSTANT 0

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
//    typedef struct _example_struct_t {
//        /* Describe structure member. */
//        int some_number;
//
//        /* Describe structure member. */
//        bool some_flag;
//
//    } example_struct_t;
typedef enum
{
    NO_SCALE = (uint8_t)0,
    PB_SCALE,
    CT_SCALE,
    UART_SCALE
}ScaleModeType;

typedef struct
{
    ScaleModeType Scaling;
}CommonVarsType;

CommonVarsType CommonVars;

typedef enum
{
    NO_SCALABLE = (uint8_t)0,
    SCALABLE
}ScalabilityType;

typedef enum
{
    CCW = (uint8_t)0,
    CW
}CSenseType;

typedef struct
{
    ScalabilityType Scalability;
    CSenseType CodeSense;
}EncoderCfgType;

EncoderCfgType EncoderCfg;

volatile uint8_t MHMTimer;
volatile uint8_t MHMProcTimer;

typedef struct __SPI_IC_MHMType
{
	uint8_t 	TxLength;
	uint8_t*	TxData;
	uint8_t		RxLength;
	uint8_t*	RxData;
	void (*pInitSPIData) (struct __SPI_IC_MHMType* IC_MHM_SPIData);
	void (*pDeInitSPIData) (struct __SPI_IC_MHMType* IC_MHM_SPIData);
}SPI_IC_MHMType;

typedef struct __IC_MHM_REG_ACCType
{
    uint8_t     Opcode;
    uint8_t 	TxLength;
	uint8_t*	TxData;
	uint8_t		RxLength;
	uint8_t*	RxData;
    uint8_t     Result;
	void (*pInitMHMRegAccData) (struct __IC_MHM_REG_ACCType* MHMRegAccData);
	void (*pDeInitMHMRegAccData) (struct __IC_MHM_REG_ACCType* MHMRegAccData);
}IC_MHM_REG_ACCType;

typedef enum
{
    MHM_STARTUP_1 = (uint8_t)0,
    MHM_STARTUP_2,
    MHM_STARTUP_3,
    MHM_STARTUP_4,
    MHM_STARTUP_5,
    READ_POS_1,
    READ_POS_2,
    READ_POS_3,
    READ_STATUS_1
}IC_MHMfsmType;

enum MHM_OPCODE
{
    ACTIVATE_OPC = (uint8_t)0xB0,
    POS_READ_OPC = (uint8_t)0xA6,
    REG_RD_CTD_OPC = (uint8_t)0x8A,
    REG_WR_CTD_OPC = (uint8_t)0xCF,
    READ_STATUS_OPC = (uint8_t)0x9C,
    WR_INST_OPC = (uint8_t)0xD9,
    REG_RD_OPC = (uint8_t)0x97,
    REG_WR_OPC = (uint8_t)0xD2,
    READ_REG_STAT_OPC = (uint8_t)0xAD
};

enum SPI0_STATUS
{
    SPI0_BUSY = (uint8_t)0,
    SPI0_CMD_SENDING,
    SPI0_CMD_SENT
};

#define IC_MHM_STAT_VALID_Msk       (uint8_t)0x01   //Data Valid
#define IC_MHM_STAT_BUSY_Msk        (uint8_t)0x02   //Slave Busy
#define IC_MHM_STAT_FAIL_Msk        (uint8_t)0x04   //Data Request Failed
#define IC_MHM_STAT_DISMISS_Msk     (uint8_t)0x08   //Illegal Address
#define IC_MHM_STAT_ERROR_Msk       (uint8_t)0x80   //Invalid opcode
                
#define IC_MHM_ERROR_REG            (uint8_t)0x70
#define IC_MHM_0x70_ERR_CFG_Msk     (uint8_t)0x01
#define IC_MHM_0x70_ERR_OFFS_Msk    (uint8_t)0x02
#define IC_MHM_0x70_ERR_POS_Msk     (uint8_t)0x04
#define IC_MHM_0x70_ERR_EXT_Msk     (uint8_t)0x08
#define IC_MHM_0x70_ERR_AMIN_Msk    (uint8_t)0x10
#define IC_MHM_0x70_ERR_AMAX_Msk    (uint8_t)0x20
#define IC_MHM_0x70_ERR_MTI_Msk     (uint8_t)0x40
#define IC_MHM_0x70_ERR_MT_Msk      (uint8_t)0x80

#define IC_MHM_SPI_nERR_Msk         (uint8_t)0x80
#define IC_MHM_SPI_nWARN_Msk        (uint8_t)0x40

#define IC_MHM_PRES_RES_REG         (uint8_t)0x74
#define IC_MHM_0x74_RESET_Msk       (uint8_t)0x01
#define IC_MHM_0x74_PRESET_Msk      (uint8_t)0x02

#define IC_MHM_FIO_REG_ADDR         (uint8_t)0x75
#define IC_MHM_0x75_FIO_0_Msk       (uint8_t)0x01
#define IC_MHM_0x75_FIO_1_Msk       (uint8_t)0x02
#define IC_MHM_0x75_FIO_2_Msk       (uint8_t)0x04
#define IC_MHM_0x75_FIO_3_Msk       (uint8_t)0x08

#define START_UP_T_ms               500
#define START_UP_TIMER_SET          (((START_UP_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

#define READ_POS_T_ms               20
#define READ_POS_TIMER_SET          (((READ_POS_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

#define PV_PRESET_PULSE_T_ms        20
#define PV_PRESET_TIMER_SET         (((PV_PRESET_PULSE_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

//CRC defines for IC-PV and IC-MHM
#define IC_PV_CRC_POLY              ((uint16_t)0x012F)
#define IC_PV_CRC_START_VALUE       ((uint8_t)0)
#define IC_MHM_CRC_POLY             ((uint16_t)0x011D)
#define IC_MHM_CRC_START_VALUE      ((uint8_t)2)

//I2C defines for IC-PV
#define EEPROM_I2C_ADDR             ((uint8_t)0xA0)
#define IC_PV_CFG_ADDR              ((uint8_t)0x00)
#define IC_PV_CFG_LENGTH            ((uint8_t)5)
#define IC_PV_CNT_ADDR              ((uint8_t)0x05)
#define IC_PV_CNT_LENGTH            ((uint8_t)6)

//I2C defines for IC-MHM
#define IC_MHM_CFG_ADDR             ((uint8_t)0x10)
#define IC_MHM_CFG_LENGTH           ((uint8_t)13)
#define IC_MHM_OFFS_ADDR            ((uint8_t)0x1D)
#define IC_MHM_OFFS_LENGTH          ((uint8_t)7)

//External EEPROM defines
#define WR_PAGE_SIZE_BYTES          ((uint8_t)16)
#define EEPROM_SIZE_16KBITS         ((uint8_t)16)
//#define EEPROM_SIZE_8KBITS          ((uint8_t)8)
//#define EEPROM_SIZE_4KBITS          ((uint8_t)4)
//#define EEPROM_SIZE_2KBITS          ((uint8_t)2)
#ifdef EEPROM_SIZE_16KBITS
    #define EEPROM_SIZE_BYTES           ((uint16_t)(1024/8)* EEPROM_SIZE_16KBITS)
#endif
#ifdef EEPROM_SIZE_8KBITS
    #define EEPROM_SIZE_BYTES           ((uint16_t)(1024/8)* EEPROM_SIZE_8KBITS)
#endif
#ifdef EEPROM_SIZE_4KBITS
    #define EEPROM_SIZE_BYTES           ((uint16_t)(1024/8)* EEPROM_SIZE_4KBITS)
#endif
#ifdef EEPROM_SIZE_2KBITS
    #define EEPROM_SIZE_BYTES           ((uint16_t)(1024/8)* EEPROM_SIZE_2KBITS)
#endif

typedef struct __ExtEEpromDataType
{
    uint8_t ExtEEpromfsm;
    uint8_t SlaveAddr;  //LSB contains R/nW operation
    uint16_t MemoryAddr;
    uint16_t TxLength;
    uint8_t* TxData;
    uint16_t TxCnt;
    uint8_t* pData;
    uint8_t PgStartAddr;
    uint8_t NextPgAddr;
    uint8_t BytesToWr;
    uint16_t RxLength;
    uint8_t* RxData;
    bool (*pInitExtEEpromData) (struct __ExtEEpromDataType* pExtEEpromData);
	void (*pDeInitExtEEpromData) (struct __ExtEEpromDataType* pExtEEpromData);
}ExtEEpromDataType;

#define EXT_EEPROM_TWR_ms           20
#define EXT_EEPROM_TWR_SET         (((EXT_EEPROM_TWR_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

//Internal EEPROM defines, RWWEE memory
typedef struct __IntRWWEEWrType
{
    uint8_t IntEEpromWrfsm;
    uint8_t PageCnt;
    uint8_t ByteCnt;
    uint32_t RowStartAddr;
    uint32_t NextRowAddr;
    uint32_t BytesToWr;
    uint8_t* pData;
    uint32_t Address;
    uint16_t length;
    uint32_t* pRowData;
    bool (*pInitRWWEEWrData) (struct __IntRWWEEWrType* pIntRWWEEWr);
    void (*pDeInitRWWEEWrData) (struct __IntRWWEEWrType* pIntRWWEEWr);
}IntRWWEEWrType;

#define RWWEE_PAGE_SIZE             ((uint8_t)64)
#define RWWEE_ROW_SIZE              ((uint16_t)64 * 4)
#define RWEEE_PV_CFG_ADDR           ((uint32_t)0x400000)
#define RWWEE_PV_CFG_LEN            ((uint8_t)5)
#define RWEEE_PV_CNT_ADDR           ((uint32_t)(RWEEE_PV_CFG_ADDR+RWWEE_PV_CFG_LEN))
#define RWWEE_PV_CNT_LEN            ((uint8_t)6)
#define RWWEE_MHM_CFG_ADDR          ((uint32_t)0x400010)
#define RWWEE_MHM_CFG_LEN           ((uint8_t)13)
#define RWWEE_MHM_OFFS_ADDR         ((uint32_t)(RWWEE_MHM_CFG_ADDR+RWWEE_MHM_CFG_LEN))
#define RWWEE_MHM_OFFS_LEN          ((uint8_t)7)
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

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
//    int ExampleFunction(int param1, int param2);
bool IsMHMAccessFree();
void Init_IC_MHM_SPIData(SPI_IC_MHMType* IC_MHM_SPIData);
void DeInit_IC_MHM_SPIData(SPI_IC_MHMType* IC_MHM_SPIData);
void IC_MHM_SPIBufferInit(SPI_IC_MHMType** pIC_MHM_SPIData, uint8_t TxLength, uint8_t RxLength);
void IC_MHM_SPIBufferFree(SPI_IC_MHMType** pIC_MHM_SPIData);
void InitMHMRegAccData(IC_MHM_REG_ACCType* MHMRegAccData);
void DeInitMHMRegAccData(IC_MHM_REG_ACCType* MHMRegAccData);
void MHMRegAccBufferInit(IC_MHM_REG_ACCType** pMHMRegAccData, uint8_t Opcode, uint8_t TxLength, uint8_t RxLength);
void MHMRegAccBufferFree(IC_MHM_REG_ACCType** pMHMRegAccData);
void TimerTask();

void IC_MHM_RegAccesTask();

uint8_t IC_MHM_Activate(uint8_t Data);
uint8_t IC_MHM_RdPos(uint8_t* Data);
uint8_t IC_MHM_RdStatus(uint8_t* Data);
uint8_t IC_MHM_RegRdCtd(uint8_t Address, uint8_t* Data, uint8_t Length);
uint8_t IC_MHM_RegWrCtd(uint8_t Address, uint8_t* Data, uint8_t Length);
uint8_t IC_MHM_WrInstr(uint8_t* Data, uint8_t Length);
uint8_t IC_MHM_RegWr(uint8_t Address, uint8_t Data);
uint8_t IC_MHM_RegRd(uint8_t Address, uint8_t* Data);
uint8_t IC_MHM_SetFIO(uint8_t Data);
uint8_t IC_MHM_ClrFIO(uint8_t Data);
uint8_t IC_MHM_PresetPV();
        
void IC_MHM_Task();

bool InitExtEEpromData(ExtEEpromDataType* pExtEEpromData);
void DeInitExtEEpromData(ExtEEpromDataType* pExtEEpromData);
bool ExtEEpromDataBufferInit(ExtEEpromDataType** pExtEEpromData, uint8_t SlaveAddr, uint8_t TxLength, uint8_t RxLength);
void ExtEEpromDataBufferFree(ExtEEpromDataType** pExtEEpromData);
uint8_t ExtEEpromRdWr(void);

bool InitRWWEEWrData (IntRWWEEWrType* ptr);
void DeInitRWWEEWrData(IntRWWEEWrType* ptr);
bool RWWEEWrInit (IntRWWEEWrType** pIntRWWEEWr, uint32_t Address, uint16_t length);
void RWWEEWRfree (IntRWWEEWrType** pIntRWWEEWr);
uint8_t IntEEpromWrite(void);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _SPI_MHM */

/* *****************************************************************************
 End of File
 */
