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
IC_MHMfsmType IC_MHMfsm = READ_REG_STAT_1;
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

void IC_MHM_Task()
{
    if(BISS_MASTER_Get())
    {
        NCS_MHM_Set();
        IC_MHMfsm = MHM_STARTUP_1;
    }
    else if(!SERCOM0_SPI_IsBusy())
    {
        switch (IC_MHMfsm)
        {
            //sends SPI POSITION READ command.
            case MHM_STARTUP_1:
                IC_MHM_SPIBufferInit(&pSPI0Data, (uint8_t)1, (uint8_t)8);
                pSPI0Data->TxData[0] = POS_READ;
                NCS_MHM_Clear();
                if(SERCOM0_SPI_WriteRead (pSPI0Data->TxData, (uint8_t)1, pSPI0Data->RxData, 8)) IC_MHMfsm = MHM_STARTUP_2;
                else
                {
                    NCS_MHM_Set();
                    IC_MCB_SPIBufferFree(&pSPI0Data);
                }
                break;
            //Checks if received opcode is POSTION READ and nERR and nWARN bits
            //During Start up, MISO is stuck high so No data will be received
            case MHM_STARTUP_2:
                NCS_MHM_Set();
                //Check recived opcode, if not POSITION READ then repeat process.
                if(pSPI0Data->RxData[0] !=  POS_READ) IC_MHMfsm = MHM_STARTUP_1;
                //POSITION READ Opcode Received
                else
                {
                    //Check for any error bit or warning bit, repeat process to wait start up process end.
                    //During Start Up, nWARN and nERR are reset. 
                    if(pSPI0Data->RxData[6]) IC_MHMfsm = MHM_STARTUP_1;
                    //if none error or warning bit active, start up complete.
                    else IC_MHMfsm = READ_STATUS_1;
                }
                IC_MCB_SPIBufferFree(&pSPI0Data);
                break;

            case READ_POS_1:
                IC_MHM_SPIBufferInit(&pSPI0Data, (uint8_t)1, (uint8_t)8);
                pSPI0Data->TxData[0] = POS_READ;
                NCS_MHM_Clear();
                if(SERCOM0_SPI_WriteRead (pSPI0Data->TxData, (uint8_t)1, pSPI0Data->RxData, 8)) IC_MHMfsm = READ_POS_2;
                else
                {
                    NCS_MHM_Set();
                    IC_MCB_SPIBufferFree(&pSPI0Data);
                }
                break;
                
            case READ_POS_2:
                NCS_MHM_Set();
                //Check for nWARN bit
                if(pSPI0Data->RxData[7] & IC_MHM_SPI_nWARN_Msk) 
                {
                    //nWARN bit detectec, excessive rotor speed
                }
                //Check for nERR bit 
                if(pSPI0Data->RxData[7] & IC_MHM_SPI_nERR_Msk) 
                {
                    //nERR bit detectec
                    IC_MHMfsm = READ_STATUS_1;                        
                }
                //Read MT Position from pSPI0Data->RxData[1] to pSPI0Data->RxData[4]
                //Read ST Position from pSPI0Data->RxData[5] to pSPI0Data->RxData[6]
                IC_MCB_SPIBufferFree(&pSPI0Data);
                break;
                
            case READ_STATUS_1:
                IC_MHM_SPIBufferInit(&pSPI0Data, (uint8_t)1, (uint8_t)5);
                pSPI0Data->TxData[0] = READ_STATUS;
                NCS_MHM_Clear();
                if(SERCOM0_SPI_WriteRead (pSPI0Data->TxData, (uint8_t)1, pSPI0Data->RxData, 5))IC_MHMfsm = READ_STATUS_2;
                else
                {
                    NCS_MHM_Set();
                    IC_MCB_SPIBufferFree(&pSPI0Data);
                }
                break;
                
            case READ_STATUS_2:
                NCS_MHM_Set();
                if(pSPI0Data->RxData[1])
                {
                    //error detected
                }
                else IC_MHMfsm = READ_POS_1;
                IC_MCB_SPIBufferFree(&pSPI0Data);
                break;
                
            case READ_REG_STAT_1:
                IC_MHM_SPIBufferInit(&pSPI0Data, (uint8_t)1, (uint8_t)3);
                pSPI0Data->TxData[0] = READ_REG_STAT;
                if(SERCOM0_SPI_WriteRead (pSPI0Data->TxData, (uint8_t)1, pSPI0Data->RxData, 3)) IC_MHMfsm = READ_REG_STAT_2;
                break;
                
            case READ_REG_STAT_2:
                if(pSPI0Data->RxData[1] & IC_MHM_STAT_BUSY_Msk) IC_MHMfsm = READ_REG_STAT_1;
                else IC_MHMfsm = READ_STATUS_1;
                IC_MCB_SPIBufferFree(&pSPI0Data);
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
