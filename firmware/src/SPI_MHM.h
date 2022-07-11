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
    PB_SCALE_SOLID,
    PB_SCALE_BLINK,
    CT_SCALE,
    UART_SCALE,
    SCALE_SET1,
    SCALE_SET2,
}ScaleModeType;

typedef enum
{
    FACTORY_SCALE = (uint8_t)0,
    USR_SCALE,
    DEFAULT_SCALE
}UsedScaleType;

#define DEFAULT_RESOMT	2 // from 0 to 32

#if DEFAULT_RESOMT > 32
	#error "Default MT resolution 'DEFAULT_RESOMT' must be between 0 and 32"
#endif

typedef enum
{
    NO_SCALABLE = (uint8_t)1,
    SCALABLE = (uint8_t)0
}ScalabilityType;

typedef enum
{
    CCW = (uint8_t)0,
    CW = (uint8_t)1
}CSenseType;

#define USER_SCL_CFG_LEN ((uint8_t)3)

typedef struct
{
	uint8_t UserSclCfg[USER_SCL_CFG_LEN];
	uint16_t ExtDACMax;
	uint16_t IntDACLow;
	uint16_t IntDACLowLS;
	uint16_t IntDACHighLS;
	uint16_t FractRange;
	uint8_t FactOffset;
    uint8_t Scaling;
    uint8_t ResoAndDir;
    uint8_t* pSPIPosition;
    uint8_t SPIPosByteLen;  // includes MT bytes, ST bytes and Report byte (NWARN and nERR bits) in Big Endian, MSB first as received from IC-MHM
    uint8_t* pPosition;
    uint8_t* pPosLowOut;
    uint8_t* pPosHighOut;
	uint8_t* pPosOffset;
	uint8_t* pLastPos;
	uint8_t* pPosRange;
	uint8_t* pROverRange;
	uint8_t* pNormPos;
	uint8_t* pTransition;
    uint8_t PosByteLen;     // includes ST and MT both in Little Endian, LSB first
	uint8_t ExchgFlags;
	int8_t	UF_OF_Cnt;

}CommonVarsType;

CommonVarsType CommonVars;
bool IC_MHMAccessFree;
//User Scaling configuration definitions
//UserSclCfg[0]
#define USR_SCL_EN_POS              ((uint8_t)0)
#define USR_SCL_EN_MSK              ((uint8_t)1)
#define USR_SCL_EN_RD               ((ScalabilityType)((CommonVars.UserSclCfg[0]&(USR_SCL_EN_MSK<<USR_SCL_EN_POS))>>USR_SCL_EN_POS))
#define USR_SCL_EN_WR(val)			(CommonVars.UserSclCfg[0] = (CommonVars.UserSclCfg[0]&(~(USR_SCL_EN_MSK<<USR_SCL_EN_POS)))|((((uint8_t)val)&USR_SCL_EN_MSK)<<USR_SCL_EN_POS))

#define USR_SCL_FRACT_RNG_POS		((uint8_t)1)
#define USR_SCL_FRACT_RNG_MSK		((uint8_t)1)
#define USR_SCL_FRACT_RNG_RD		((uint8_t)((CommonVars.UserSclCfg[0]&(USR_SCL_FRACT_RNG_MSK<<USR_SCL_FRACT_RNG_POS))>>USR_SCL_FRACT_RNG_POS))
#define USR_SCL_FRACT_RNG_USED		((uint8_t)1)
#define USR_SCL_FRACT_RNG_UNUSED	((uint8_t)0)
#define USR_SCL_FRACT_RNG_WR(val)	(CommonVars.UserSclCfg[0] = (CommonVars.UserSclCfg[0]&(~(USR_SCL_FRACT_RNG_MSK<<USR_SCL_FRACT_RNG_POS)))|((val&USR_SCL_FRACT_RNG_MSK)<<USR_SCL_FRACT_RNG_POS))

#define USR_SCL_ROLL_OVER_POS		((uint8_t)2)
#define USR_SCL_ROLL_OVER_MSK		((uint8_t)1)
#define USR_SCL_ROLL_OVER_RD		((uint8_t)((CommonVars.UserSclCfg[0]&(USR_SCL_ROLL_OVER_MSK<<USR_SCL_ROLL_OVER_POS))>>USR_SCL_ROLL_OVER_POS))
#define USR_SCL_ROLL_OVER_USED		((uint8_t)1)
#define USR_SCL_ROLL_OVER_UNUSED	((uint8_t)0)
#define USR_SCL_ROLL_OVER_WR(val)	(CommonVars.UserSclCfg[0] = (CommonVars.UserSclCfg[0]&(~(USR_SCL_ROLL_OVER_MSK<<USR_SCL_ROLL_OVER_POS)))|((val&USR_SCL_ROLL_OVER_MSK)<<USR_SCL_ROLL_OVER_POS))

#define USR_SCL_LIMIT_SW_POS		((uint8_t)3)
#define USR_SCL_LIMIT_SW_MSK		((uint8_t)1)
#define USR_SCL_LIMIT_SW_RD			((uint8_t)((CommonVars.UserSclCfg[0]&(USR_SCL_ROLL_OVER_MSK<<USR_SCL_ROLL_OVER_POS))>>USR_SCL_ROLL_OVER_POS))
#define USR_SCL_LIMIT_SW_USED		((uint8_t)1)
#define USR_SCL_LIMIT_SW_UNUSED		((uint8_t)0)
#define USR_SCL_LIMIT_SW_WR(val)	(CommonVars.UserSclCfg[0] = (CommonVars.UserSclCfg[0]&(~(USR_SCL_ROLL_OVER_MSK<<USR_SCL_ROLL_OVER_POS)))|((val&USR_SCL_ROLL_OVER_MSK)<<USR_SCL_ROLL_OVER_POS))

#define USR_SCL_AVAIL_POS           ((uint8_t)4)
#define USR_SCL_AVAIL_MSK           ((uint8_t)1)
#define USR_SCL_AVAIL_RD            ((uint8_t)((CommonVars.UserSclCfg[0]&(USR_SCL_AVAIL_MSK<<USR_SCL_AVAIL_POS))>>USR_SCL_AVAIL_POS))
#define ENC_CFG_AVAIL				((uint8_t)0)
#define ENC_CFG_NOT_AVAIL			((uint8_t)1)
#define USR_SCL_AVAIL_WR(val)		(CommonVars.UserSclCfg[0] = (CommonVars.UserSclCfg[0]&(~(USR_SCL_AVAIL_MSK<<USR_SCL_AVAIL_POS)))|((val&USR_SCL_AVAIL_MSK)<<USR_SCL_AVAIL_POS))

#define USR_SCL_UF_OF_POS			((uint8_t)5)
#define USR_SCL_UF_OF_MSK			((uint8_t)1)
#define USR_SCL_UF_OF_RD			((uint8_t)((CommonVars.UserSclCfg[0]&(USR_SCL_UF_OF_MSK<<USR_SCL_UF_OF_POS))>>USR_SCL_UF_OF_POS))
#define USR_SCL_OVFLW				((uint8_t)1)
#define USR_SCL_NOT_OVFLW			((uint8_t)0)
#define USR_SCL_UF_OF_WR(val)		(CommonVars.UserSclCfg[0] = (CommonVars.UserSclCfg[0]&(~(USR_SCL_UF_OF_MSK<<USR_SCL_UF_OF_POS)))|((val&USR_SCL_UF_OF_MSK)<<USR_SCL_UF_OF_POS))

//UserSclCfg[1]
#define USR_SCL_MHM_RESOMT_POS      ((uint8_t)0)
#define USR_SCL_MHM_RESOMT_MSK      ((uint8_t)7)
#define USR_SCL_MHM_RESOMT          ((uint8_t)((CommonVars.UserSclCfg[1]&(USR_SCL_MHM_RESOMT_MSK<<USR_SCL_MHM_RESOMT_POS))>>USR_SCL_MHM_RESOMT_POS))

#define USR_SCL_MHM_DIR_POS			((uint8_t)3)
#define USR_SCL_MHM_DIR_MSK         ((uint8_t)1)
#define USR_SCL_MHM_DIR             ((CSenseType)((CommonVars.UserSclCfg[1]&(USR_SCL_MHM_DIR_POS<<USR_SCL_MHM_DIR_MSK))>>USR_SCL_MHM_DIR_MSK))

#define USR_SCL_MHM_RESOST_POS	    ((uint8_t)4)
#define USR_SCL_MHM_RESOST_MSK	    ((uint8_t)7)
#define USR_SCL_MHM_RESOST			((uint8_t)((CommonVars.UserSclCfg[1]&(USR_SCL_MHM_RESOST_MSK<<USR_SCL_MHM_RESOST_POS))>>USR_SCL_MHM_RESOST_POS))

//UserSclCfg[2]
#define FACTORY_RESOMT_POS          ((uint8_t)0)
#define FACTORY_RESOMT_MSK          ((uint8_t)0x3F)
#define FACTORY_RESOMT	            ((uint8_t)((CommonVars.UserSclCfg[2]&(FACTORY_RESOMT_MSK<<FACTORY_RESOMT_POS))>>FACTORY_RESOMT_POS))
#define FACTORY_RESOMT_WR(val)		(CommonVars.UserSclCfg[2] = (CommonVars.UserSclCfg[2]&(~(FACTORY_RESOMT_MSK<<FACTORY_RESOMT_POS)))|((val&FACTORY_RESOMT_MSK)<<FACTORY_RESOMT_POS))

//ResoAndDir
#define RESDIR_RESO_MT_POS			((uint8_t)0)
#define RESDIR_RESO_MT_MSK			((uint8_t)7)
#define RESDIR_RESO_MT				((uint8_t)((CommonVars.ResoAndDir&(RESDIR_RESO_MT_MSK<<RESDIR_RESO_MT_POS))>>RESDIR_RESO_MT_POS))

#define RESDIR_RESO_DIR_POS			((uint8_t)3)
#define RESDIR_RESO_DIR_MSK			((uint8_t)1)
#define RESDIR_RESO_DIR				((uint8_t)((CommonVars.ResoAndDir&(RESDIR_RESO_DIR_MSK<<RESDIR_RESO_DIR_POS))>>RESDIR_RESO_DIR_POS))

#define RESDIR_RESO_ST_POS			((uint8_t)4)
#define RESDIR_RESO_ST_MSK			((uint8_t)7)
#define RESDIR_RESO_ST				((uint8_t)((CommonVars.ResoAndDir&(RESDIR_RESO_ST_MSK<<RESDIR_RESO_ST_POS))>>RESDIR_RESO_ST_POS))

//Scaling
#define SCALE_MODE_POS				((uint8_t)0)
#define SCALE_MODE_MSK				((uint8_t)15)
#define SCALE_MODE_RD				((ScaleModeType)((CommonVars.Scaling&(SCALE_MODE_MSK<<SCALE_MODE_POS))>>SCALE_MODE_POS))
#define SCALE_MODE_WR(val)			CommonVars.Scaling = (CommonVars.Scaling&(~(SCALE_MODE_MSK<<SCALE_MODE_POS)))|((val&SCALE_MODE_MSK)<<SCALE_MODE_POS)

#define SCALE_ACTIVE_POS			((uint8_t)4)
#define SCALE_ACTIVE_MSK			((uint8_t)3)
#define SCALE_ACTIVE_RD				((UsedScaleType)((CommonVars.Scaling&(SCALE_ACTIVE_MSK<<SCALE_ACTIVE_POS))>>SCALE_ACTIVE_POS))
#define SCALE_ACTIVE_WR(val)	    CommonVars.Scaling = (CommonVars.Scaling&(~(SCALE_ACTIVE_MSK<<SCALE_ACTIVE_POS)))|((val&SCALE_ACTIVE_MSK)<<SCALE_ACTIVE_POS)

//Exhange Flags
#define EXCHG_FLAG_NEWPOS_POS		((uint8_t)0)
#define EXCHG_FLAG_NEWPOS_MSK		((uint8_t)1)
#define EXCHG_FLAG_NEWPOS			((uint8_t)((CommonVars.ExchgFlags&(EXCHG_FLAG_NEWPOS_MSK<<EXCHG_FLAG_NEWPOS_POS))>>EXCHG_FLAG_NEWPOS_POS))
#define EXCHG_FLAG_NEWPOS_SET		CommonVars.ExchgFlags |= (EXCHG_FLAG_NEWPOS_MSK<<EXCHG_FLAG_NEWPOS_POS)
#define EXCHG_FLAG_NEWPOS_CLR		CommonVars.ExchgFlags &= (~(EXCHG_FLAG_NEWPOS_MSK<<EXCHG_FLAG_NEWPOS_POS))

#define EXCHG_FLAG_PRESET_POS		((uint8_t)1)
#define EXCHG_FLAG_PRESET_MSK		((uint8_t)1)
#define EXCHG_FLAG_PRESET			((uint8_t)((CommonVars.ExchgFlags&(EXCHG_FLAG_PRESET_MSK<<EXCHG_FLAG_PRESET_POS))>>EXCHG_FLAG_PRESET_POS))
#define EXCHG_FLAG_PRESET_SET		CommonVars.ExchgFlags |= (EXCHG_FLAG_PRESET_MSK<<EXCHG_FLAG_PRESET_POS)
#define EXCHG_FLAG_PRESET_CLR		CommonVars.ExchgFlags &= (~(EXCHG_FLAG_PRESET_MSK<<EXCHG_FLAG_PRESET_POS))

#define EXCHG_FLAG_1STREAD_POS		((uint8_t)2)
#define EXCHG_FLAG_1STREAD_MSK		((uint8_t)1)
#define EXCHG_FLAG_1STREAD			((uint8_t)((CommonVars.ExchgFlags&(EXCHG_FLAG_1STREAD_MSK<<EXCHG_FLAG_1STREAD_POS))>>EXCHG_FLAG_1STREAD_POS))
#define EXCHG_FLAG_1STREAD_SET		CommonVars.ExchgFlags |= (EXCHG_FLAG_1STREAD_MSK<<EXCHG_FLAG_1STREAD_POS)
#define EXCHG_FLAG_1STREAD_CLR		CommonVars.ExchgFlags &= (~(EXCHG_FLAG_1STREAD_MSK<<EXCHG_FLAG_1STREAD_POS))

#define EXCHG_FLAG_PRESETON_POS		((uint8_t)3)
#define EXCHG_FLAG_PRESETON_MSK		((uint8_t)1)
#define EXCHG_FLAG_PRESETON			((uint8_t)((CommonVars.ExchgFlags&(EXCHG_FLAG_PRESETON_MSK<<EXCHG_FLAG_PRESETON_POS))>>EXCHG_FLAG_PRESETON_POS))
#define EXCHG_FLAG_PRESETON_SET		CommonVars.ExchgFlags |= (EXCHG_FLAG_PRESETON_MSK<<EXCHG_FLAG_PRESETON_POS)
#define EXCHG_FLAG_PRESETON_CLR		CommonVars.ExchgFlags &= (~(EXCHG_FLAG_PRESETON_MSK<<EXCHG_FLAG_PRESETON_POS))

#define EXCHG_FLAG_EXTDACINIT_POS	((uint8_t)4)
#define EXCHG_FLAG_EXTDACINIT_MSK	((uint8_t)1)
#define EXCHG_FLAG_EXTDACINIT		((uint8_t)((CommonVars.ExchgFlags&(EXCHG_FLAG_EXTDACINIT_MSK<<EXCHG_FLAG_EXTDACINIT_POS))>>EXCHG_FLAG_EXTDACINIT_POS))
#define EXCHG_FLAG_EXTDACINIT_SET	CommonVars.ExchgFlags |= (EXCHG_FLAG_EXTDACINIT_MSK<<EXCHG_FLAG_EXTDACINIT_POS)
#define EXCHG_FLAG_EXTDACINIT_CLR	CommonVars.ExchgFlags &= (~(EXCHG_FLAG_EXTDACINIT_MSK<<EXCHG_FLAG_EXTDACINIT_POS))
//UF_OF_Cnt
#define UF_OV_MAX					(100)
#define UF_OV_MIN					((-1)*UF_OV_MAX)

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
	void (*pInitMHMRegAccData) (struct __IC_MHM_REG_ACCType* MHMRegAccData);
	void (*pDeInitMHMRegAccData) (struct __IC_MHM_REG_ACCType* MHMRegAccData);
}IC_MHM_REG_ACCType;

typedef enum
{
    MHM_STARTUP_1 = (uint8_t)0,
    MHM_STARTUP_2,
    READ_RESO_DIR,
    READ_CFG,
    READ_POS_1,
    READ_POS_2,
    READ_POS_3,
    READ_STATUS_1,
	PV_PRESET,
	MHM_PRESET
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

#define IC_MHM_STAT_VALID_Msk       ((uint8_t)0x01)   //Data Valid
#define IC_MHM_STAT_BUSY_Msk        ((uint8_t)0x02)   //Slave Busy
#define IC_MHM_STAT_FAIL_Msk        ((uint8_t)0x04)   //Data Request Failed
#define IC_MHM_STAT_DISMISS_Msk     ((uint8_t)0x08)   //Illegal Address
#define IC_MHM_STAT_ERROR_Msk       ((uint8_t)0x80)   //Invalid opcode
                
#define IC_MHM_INTRPLTR_LEN			((uint8_t)2)

#define IC_MHM_REG0_ADDR            ((uint8_t)0)
#define IC_MHM_REG0_DIR_POS         ((uint8_t)5)
#define IC_MHM_REG0_DIR_MSK         ((uint8_t)1)

#define IC_MHM_RESO_REG             ((uint8_t)1)
#define IC_MHM_RESO_MT_POS          ((uint8_t)0)
#define IC_MHM_RESO_MT_MSK          ((uint8_t)7)
#define IC_MHM_RESO_ST_POS          ((uint8_t)4)
#define IC_MHM_RESO_ST_MSK          ((uint8_t)7)

#define IC_MHM_ERROR_REG            ((uint8_t)0x70)
#define IC_MHM_0x70_ERR_CFG_Msk     ((uint8_t)0x01)
#define IC_MHM_0x70_ERR_OFFS_Msk    ((uint8_t)0x02)
#define IC_MHM_0x70_ERR_POS_Msk     ((uint8_t)0x04)
#define IC_MHM_0x70_ERR_EXT_Msk     ((uint8_t)0x08)
#define IC_MHM_0x70_ERR_AMIN_Msk    ((uint8_t)0x10)
#define IC_MHM_0x70_ERR_AMAX_Msk    ((uint8_t)0x20)
#define IC_MHM_0x70_ERR_MTI_Msk     ((uint8_t)0x40)
#define IC_MHM_0x70_ERR_MT_Msk      ((uint8_t)0x80)

#define IC_MHM_SPI_nERR_Msk         ((uint8_t)0x80)
#define IC_MHM_SPI_nWARN_Msk        ((uint8_t)0x40)

#define IC_MHM_PRES_RES_REG         ((uint8_t)0x74)
#define IC_MHM_0x74_RESET_Msk       ((uint8_t)0x01)
#define IC_MHM_0x74_PRESET_Msk      ((uint8_t)0x02)

#define IC_MHM_FIO_REG_ADDR         ((uint8_t)0x75)
#define IC_MHM_0x75_FIO_0_Msk       ((uint8_t)0x01)
#define IC_MHM_0x75_FIO_1_Msk       ((uint8_t)0x02)
#define IC_MHM_0x75_FIO_2_Msk       ((uint8_t)0x04)
#define IC_MHM_0x75_FIO_3_Msk       ((uint8_t)0x08)

#define START_UP_T_ms               200
#define START_UP_TIMER_SET          (((START_UP_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

#define READ_POS_T_ms               10
#define READ_POS_TIMER_SET          (((READ_POS_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

#define PV_PRESET_PULSE_T_ms        30
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

//Internal EEPROM MAP
#define RWWEE_PAGE_SIZE             ((uint8_t)64)
#define RWWEE_ROW_SIZE              ((uint16_t)64 * 4)

#define RWEEE_PV_CFG_ADDR           ((uint32_t)0x400000)
#define RWWEE_PV_CFG_LEN            ((uint8_t)5)

#define RWEEE_PV_CNT_ADDR           ((uint32_t)(RWEEE_PV_CFG_ADDR+RWWEE_PV_CFG_LEN))
#define RWWEE_PV_CNT_LEN            ((uint8_t)6)

#define RWWEE_MHM_CFG_ADDR          ((uint32_t)RWEEE_PV_CNT_ADDR+RWWEE_PV_CNT_LEN)
#define RWWEE_MHM_CFG_LEN           ((uint8_t)13)

#define RWWEE_MHM_OFFS_ADDR         ((uint32_t)(RWWEE_MHM_CFG_ADDR+RWWEE_MHM_CFG_LEN))
#define RWWEE_MHM_OFFS_LEN          ((uint8_t)7)

#define RWWEE_PV_MHM_CFG_OK_ADDR    ((uint32_t)(RWWEE_MHM_OFFS_ADDR+RWWEE_MHM_OFFS_LEN))
#define RWWEE_PV_MHM_CFG_OK_LEN     ((uint8_t)1)
#define RWWEE_CFG_PV_MHM_OK_VAL     ((uint8_t)0x55)

#define RWWEE_ENC_CFG_ADDR          ((uint32_t)(RWWEE_PV_MHM_CFG_OK_ADDR+RWWEE_PV_MHM_CFG_OK_LEN))
#define RWWEE_ENC_CFG_LEN           ((uint8_t)(sizeof(CommonVars.UserSclCfg)))

#define RWWEE_EXTDAC_MAX_ADDR		((uint32_t)(RWWEE_ENC_CFG_ADDR+RWWEE_ENC_CFG_LEN))
#define RWWEE_EXTDAC_MAX_LEN		((uint8_t)(sizeof(uint16_t)))

#define RWWEE_INTDAC_LOW_ADDR		((uint32_t)(RWWEE_EXTDAC_MAX_ADDR+RWWEE_EXTDAC_MAX_LEN))
#define RWWEE_INTDAC_LOW_LEN		((uint8_t)(sizeof(uint16_t)))

#define RWWEE_INTDAC_LOWLS_ADDR		((uint32_t)(RWWEE_INTDAC_LOW_ADDR+RWWEE_INTDAC_LOW_LEN))
#define RWWEE_INTDAC_LOWLS_LEN		((uint8_t)(sizeof(uint16_t)))

#define RWWEE_INTDAC_HIGHLS_ADDR	((uint32_t)(RWWEE_INTDAC_LOWLS_ADDR+RWWEE_INTDAC_LOWLS_LEN))
#define RWWEE_INTDAC_HIGHLS_LEN		((uint8_t)(sizeof(uint16_t)))

#define RWWEE_FRACT_RANGE_ADDR      ((uint32_t)(RWWEE_INTDAC_HIGHLS_ADDR+RWWEE_INTDAC_HIGHLS_LEN))
#define RWWEE_FRACT_RANGE_LEN       ((uint8_t)(sizeof(uint16_t)))

#define RWWEE_FACT_OFFSET_ADDR		((uint32_t)(RWWEE_FRACT_RANGE_ADDR+RWWEE_FRACT_RANGE_LEN))
#define RWWEE_FACT_OFFSET_LEN		((uint8_t)1)
#define RWWEE_FACT_OFFSET_MAX		((uint8_t)199)

#define RWWEE_SCL_POS_L_ADDR		((uint32_t)(RWWEE_FACT_OFFSET_ADDR+RWWEE_FACT_OFFSET_LEN))
#define RWWEE_SCL_POS_L_LEN			((uint8_t)8)

#define RWWEE_SCL_POS_H_ADDR		((uint32_t)(RWWEE_SCL_POS_L_ADDR+RWWEE_SCL_POS_L_LEN))
#define RWWEE_SCL_POS_H_LEN			((uint8_t)8)

#define RWWEE_CFG_CRC_ADDR          ((uint32_t)(RWWEE_SCL_POS_H_ADDR+RWWEE_SCL_POS_H_LEN))
#define RWWEE_CFG_CRC_LEN           ((uint8_t)1)

#define RWWEE_ENC_CFG_TOTAL_LEN     (RWWEE_CFG_CRC_ADDR-RWWEE_ENC_CFG_ADDR+RWWEE_CFG_CRC_LEN)

//DAC60501 registers defines
#define DAC_REG_NOOP				((uint8_t)0)
#define DAC_REG_DEVID				((uint8_t)1)
#define DAC_REG_SYNC				((uint8_t)2)
#define DAC_REG_CONFIG				((uint8_t)3)
#define DAC_REG_GAIN				((uint8_t)4)
#define DAC_REG_TRIGGER				((uint8_t)5)
#define DAC_REG_STATUS				((uint8_t)7)
#define DAC_REG_DACVAL				((uint8_t)8)

typedef struct
{
	uint8_t Cmd;
	uint16_t Data;
}ExtDACType;
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

uint8_t IC_MHM_RegAccesTask();

uint8_t IC_MHM_Activate(uint8_t Data);
uint8_t IC_MHM_ReadPos(uint8_t* Data, uint8_t RxLength);
uint8_t IC_MHM_RdStatus(uint8_t* Data);
uint8_t IC_MHM_RegRdCtd(uint8_t Address, uint8_t* Data, uint8_t Length);
uint8_t IC_MHM_RegWrCtd(uint8_t Address, uint8_t* Data, uint8_t Length);
uint8_t IC_MHM_WrInstr(uint8_t* Data, uint8_t Length);
uint8_t IC_MHM_RegWr(uint8_t Address, uint8_t Data);
uint8_t IC_MHM_RegRd(uint8_t Address, uint8_t* Data);
uint8_t IC_MHM_SetFIO(uint8_t Data);
uint8_t IC_MHM_ClrFIO(uint8_t Data);
uint8_t IC_MHM_PresetPV();

void CalcDACsVal(void);
void CalcPosTransition(uint8_t ResoMT);
void CalcROverRange(uint8_t ResoMT);
void CalcPosRange(uint8_t ResoMT, int8_t UF_OF);
uint8_t CheckUserScaling(void);
void ComparePosition(uint8_t* pNewPos, uint8_t* pOldPos);
void CopyPosition (uint8_t* Source , uint8_t* Dest);
void BuildPosition (UsedScaleType Scaling);
void pPosSetUp (uint8_t ResoMT);
void SetScale(UsedScaleType Scaling);
uint8_t CalcMTResCode (uint8_t MHM_MT_Res);
void IC_MHM_BISS_Detection(void);
void IC_MHM_Task(void);

void ExtDACWrite(ExtDACType* ExtDacFrame);
void ExtDACInit(void);
void ExtDACTask(void);

uint8_t CalcCRC (uint16_t CRCPoly, uint8_t StartVal, uint8_t* pData, uint16_t Length);

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
