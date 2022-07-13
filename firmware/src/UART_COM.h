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

#ifndef _UART_COM_H    /* Guard against multiple inclusion */
#define _UART_COM_H


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
#define SERCOM3_RXDATA_LENGTH	((uint8_t)1)
volatile uint8_t Sercom3RxData;
//volatile uint8_t Sercom3TxData;

#define RX3_BUFF_LEN			((uint8_t)20)
#define RX3_FRAME_TOUT_ms		500
#define RX3_FRAME_TOUT_SET		(((RX3_FRAME_TOUT_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)
#define RX3_MAX_REG_DIGIT		((uint8_t)3)
#define RX3_MAX_REG_DIGIT_POS	((uint8_t)RX3_MAX_REG_DIGIT+1)

#define RX3_MAX_VAL_DIGIT		((uint8_t)5)
#define RX3_MAX_VAL_DIGIT_POS	((uint8_t)RX3_MAX_VAL_DIGIT+1)
typedef struct _UARTRxBuffType
{
	uint8_t Data[RX3_BUFF_LEN];
	uint8_t RxCnt;
	uint8_t WrIndex;
	uint8_t RxTimeout;
}UARTRxBuffType;
volatile UARTRxBuffType UART3RxBuffer;

#define RX3_CMD_NUMBER		3
#define RX3_CMD_LENGTH		4
#define RX3_REG_NUMBER		11
#define RX3_REG_LENGTH		8

#define RX3_CMD_BUFF_LEN	((uint8_t)5)
typedef struct __UARTRxCmdBuffType
{
	char* CmdPtr[RX3_CMD_BUFF_LEN];
	uint8_t	WrIndex;
	uint8_t RdIndex;
	uint8_t CmdCnt;
}UARTRxCmdBuffType;
volatile UARTRxCmdBuffType UART3RxCmdBuffer;

//#define UART_RX3_WITH_CRC
#ifdef UART_RX3_WITH_CRC
	#define UART_RX3_END_SIZE	2
#else
	#define UART_RX3_END_SIZE	1
#endif
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
//	typedef struct _example_struct_t {
//		/* Describe structure member. */
//		int some_number;
//
//		/* Describe structure member. */
//		bool some_flag;
//
//	} example_struct_t;


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
//	int ExampleFunction(int param1, int param2);



void UARTRxBufferInit(UARTRxBuffType* Buff);
void UARTRxCmdBufferInit(UARTRxCmdBuffType* Buff);
bool UARTRxCmdBufferAdd (char* RxCmdPtr);
void UARTRxDataBufferAdd(uintptr_t testparam);
bool UART3CmdWRParse(char *pFirst,char *pLast,int* RegVal);
void UART3Task(void);
	/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _UART_COM_H */

/* *****************************************************************************
 End of File
 */
