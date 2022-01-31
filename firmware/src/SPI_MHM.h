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

typedef struct __SPI_IC_MHMType
{
	uint8_t 	TxLength;
	uint8_t*	TxData;
	uint8_t		RxLength;
	uint8_t*	RxData;
	void (*pInitSPIData) (struct __SPI_IC_MHMType* IC_MHM_SPIData);
	void (*pDeInitSPIData) (struct __SPI_IC_MHMType* IC_MHM_SPIData);
}SPI_IC_MHMType;

SPI_IC_MHMType* pSPI0Data;

typedef enum
{
    MHM_STARTUP_1 = (uint8_t)0,
    MHM_STARTUP_2,
    READ_POS_1,
    READ_POS_2,
    READ_STATUS_1,
    READ_STATUS_2,
    READ_REG_STAT_1,
    READ_REG_STAT_2
}IC_MHMfsmType;

enum MHM_OPCODE
{
    ACTIVATE = (uint8_t)0xB0,
    POS_READ = (uint8_t)0xA6,
    REG_RD_CTD = (uint8_t)0x8A,
    REG_WR_CTD = (uint8_t)0xCF,
    READ_STATUS = (uint8_t)0x9C,
    WR_INST = (uint8_t)0xD9,
    REG_RD = (uint8_t)0x97,
    REG_WR = (uint8_t)0xD2,
    READ_REG_STAT = (uint8_t)0xAD
};

#define IC_MHM_STAT_VALID_Msk       (uint8_t)0x01   //Data Valid
#define IC_MHM_STAT_BUSY_Msk        (uint8_t)0x02   //Slave Busy
#define IC_MHM_STAT_FAIL_Msk        (uint8_t)0x04   //Data Request Failed
#define IC_MHM_STAT_DISMISS_Msk     (uint8_t)0x08   //Illegal Address
#define IC_MHM_STAT_ERROR_Msk       (uint8_t)0x80   //Invalid opcode
                
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
void Init_IC_MHM_SPIData(SPI_IC_MHMType* IC_MHM_SPIData);
void DeInit_IC_MHM_SPIData(SPI_IC_MHMType* IC_MHM_SPIData);
void IC_MHM_SPIBufferInit(SPI_IC_MHMType** pIC_MHM_SPIData, uint8_t TxLength, uint8_t RxLength);
void IC_MCB_SPIBufferFree(SPI_IC_MHMType** pIC_MHM_SPIData);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _SPI_MHM */

/* *****************************************************************************
 End of File
 */
