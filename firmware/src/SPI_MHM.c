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
static uint8_t		SPI1fsm;

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
	IC_MHM_SPIData->TxCnt = 0;
	IC_MHM_SPIData->RxCnt = 0;
	IC_MHM_SPIData->TxData = (uint8_t*)malloc(IC_MHM_SPIData->length * sizeof(*(IC_MHM_SPIData->TxData)));
	IC_MHM_SPIData->RxData = (uint8_t*)malloc(IC_MHM_SPIData->length * sizeof(*(IC_MHM_SPIData->RxData)));
}

void DeInit_IC_MHM_SPIData(SPI_IC_MHMType* IC_MHM_SPIData)
{
	IC_MHM_SPIData->length = 0;
	IC_MHM_SPIData->TxCnt = 0;
	IC_MHM_SPIData->RxCnt = 0;
	free(IC_MHM_SPIData->TxData);
	free(IC_MHM_SPIData->RxData);
}

void IC_MHM_SPIBufferInit(SPI_IC_MHMType** pIC_MHM_SPIData,uint8_t length)
{
	SPI_IC_MHMType* ptr = malloc(sizeof(SPI_IC_MHMType));

	ptr->length = length;
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
void SPI1Task()
{
	switch (SPI1fsm)
	{
		case 0:
			if(pSPI1Data != NULL)
			{
                //Enable SERCOM0
                SERCOM0_REGS->SPIM.SERCOM_CTRLA |= SERCOM_SPIM_CTRLA_ENABLE_Msk ;
                /* Wait for synchronization */
                while((SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY) != 0U)
                {
                    /* Do nothing */
                }
				//Enable SPI Reception Interupt
                SERCOM0_REGS->I2CM.SERCOM_INTENSET |= SERCOM_SPIM_INTENSET_RXC_Msk;
				//Enable SPI Data register Empty
                SERCOM0_REGS->SPIM.SERCOM_INTENSET |= SERCOM_SPIM_INTENSET_DRE_Msk;
				SPI1fsm++;
			}
		break;

		case 1:
			if(pSPI1Data->RxCnt == pSPI1Data->length)
			{
               //Disable SERCOM0
                SERCOM0_REGS->SPIM.SERCOM_CTRLA &= ~(SERCOM_SPIM_CTRLA_ENABLE_Msk);
                /* Wait for synchronization */
                while((SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY) != 0U)
                {
                    /* Do nothing */
                }
                //Disable SPI Reception Interupt
                SERCOM0_REGS->I2CM.SERCOM_INTENCLR |= SERCOM_SPIM_INTENSET_RXC_Msk;
				//Disable SPI Data register Empty
                SERCOM0_REGS->SPIM.SERCOM_INTENCLR |= SERCOM_SPIM_INTENSET_DRE_Msk;
				SPI1fsm++;
			}
		break;

		case 2:
			if(pSPI1Data == NULL) SPI1fsm = 0;
		break;
	}
}


/* *****************************************************************************
 End of File
 */
