/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Hohner Automation S.L.

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
#include "push_buttons.h"
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
PBGroupType* pPushButtons;
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
//int ExampleInterfaceFunction(int param1, int param2) {
//    return 0;
//}
void InitPushButtonData (PBGroupType* pPBGroup)
{
    pPBGroup->pPBData = (PBType*)malloc(pPBGroup->length * sizeof(PBType));
}

void DenitPushButtonData (PBGroupType* pPBGroup)
{
    pPBGroup->length = 0;
    free(pPBGroup->pPBData);
}

void PBInit (PBGroupType** pPBGroup, uint8_t length)
{
    PBGroupType* ptr = malloc(sizeof(PBGroupType));
    
    ptr->length = length;
    ptr->pPBGroupInit = InitPushButtonData;
    ptr->pPBGroupDeInit = DenitPushButtonData;
    ptr->pPBGroupInit (ptr);
    *pPBGroup = ptr;
}

void PBFree (PBGroupType** pPBGroup)
{
    (*pPBGroup)->pPBGroupDeInit (*pPBGroup);
    free(*pPBGroup);
    *pPBGroup = NULL;
}

void PushButtonsTask()
{
    
}
/* *****************************************************************************
 End of File
 */
