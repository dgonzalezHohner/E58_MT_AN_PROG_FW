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
#include <stdlib.h>
#include "push_buttons.h"
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
volatile uint16_t ScaleCfgTimer = 0;
volatile uint16_t DefCfgTimer = 0;
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
    uint8_t i;
    static PBfsmType  PBfsm = WAIT_PB_CT;
    
    //Read Current Input value and store in push buttons data
    PB1_CURRENT_VAL = PB1_Get();
    PB2_CURRENT_VAL = PB2_Get();
    SET1_CURRENT_VAL = SET1_DIR_Get();
    SET2_CURRENT_VAL = SET2_PRESET_Get();
    
    for(i=0; i<NUMBER_PUSH_BUTTONS ; i++)
    {
        //if pin value is equal to debounce value
        if(pPushButtons->pPBData[i].PBStatus.Bitfield.PinStat == pPushButtons->pPBData[i].PBStatus.Bitfield.DebStat)
        {
            //Stop Debouncing Timer
            pPushButtons->pPBData[i].DebTimer = 0;
        }
        //if a pin value change is detected
        else
        {
            //If debounce timer not started, start debounce timer
            if(!pPushButtons->pPBData[i].DebTimer) pPushButtons->pPBData[i].DebTimer = DEB_TIMER_set;
            //If debounce timer ended
            else if (pPushButtons->pPBData[i].DebTimer == 1)
            {
                //set pin status as new debounce value
                pPushButtons->pPBData[i].PBStatus.Bitfield.DebStat = pPushButtons->pPBData[i].PBStatus.Bitfield.PinStat;
            }
        }
    }
    if(USR_SCL_EN == SCALABLE)
    {
        switch (PBfsm)
        {
            case WAIT_PB_CT:
                //set led configuration none
                //Check both PBs are pressed, start timers
                if(CommonVars.Scaling == NO_SCALE)
                {
                    if(!PB1_DEB_VAL && !PB2_DEB_VAL)
                    {
                        ScaleCfgTimer = PB_SCALE_CFG_T_set;
                        DefCfgTimer = PB_DEF_CFG_T_set;
                        CommonVars.Scaling = PB_SCALE;
                        PBfsm = BOTH_PBS_ON;
                    }
                    //
                    else if (SET1_DEB_VAL && SET2_DEB_VAL)
                    {
                        PBfsm = BOTH_CTS_ON; //CT Scaling
                    }
                    //
                    else
                    {
                        ScaleCfgTimer = 0;
                        DefCfgTimer = 0;
                    }
                }
                break;

            case BOTH_PBS_ON:
                //set both leds solid
                //Check if Scale configuration timer not end
                if(ScaleCfgTimer != 1)
                {
                    //Check if any PB is released
                    if(PB1_DEB_VAL || PB2_DEB_VAL)
                    {
                        CommonVars.Scaling = NO_SCALE;
                        PBfsm = WAIT_PB_CT;
                    }
                }
                //Scale configuration timer end
                else
                {
                    //Check if both PBs are released
                    if(PB1_DEB_VAL && PB2_DEB_VAL) PBfsm = BOTH_PBS_OFF;
                    //Check if restore default config timer end
                    else if(!PB1_DEB_VAL && !PB2_DEB_VAL && (DefCfgTimer == 1)) PBfsm = RESTORE_DEF;
                }
                break;

            case BOTH_PBS_OFF:
                //Both leds blinking
                if(!PB1_DEB_VAL)
                {
                    ScaleCfgTimer = PB_SET_CFG_T_set;
                    PBfsm = PB1_ON_FIRST;
                }
                else if(!PB2_DEB_VAL)
                {
                    ScaleCfgTimer = PB_SET_CFG_T_set;
                    PBfsm = PB2_ON_FIRST;
                }
                break;

            case PB1_ON_FIRST:
            case PB2_ON_FIRST:
                //both leds blinking
                //Before timer end
                if(ScaleCfgTimer != 1)
                {
                    //Check abort conditiion
                    if(!PB1_DEB_VAL && !PB2_DEB_VAL)
                    {
                        CommonVars.Scaling = NO_SCALE;
                        PBfsm = WAIT_PB_CT;
                    }
                    else
                    {
                        //Check PB1 pressed first and released or PB2 pressed first and released
                        if(((PBfsm == PB1_ON_FIRST) && PB1_DEB_VAL) || ((PBfsm == PB2_ON_FIRST) && PB2_DEB_VAL)) PBfsm = BOTH_PBS_OFF;
                    }
                }
                else
                {
                    if((PBfsm == PB1_ON_FIRST) && PB1_DEB_VAL)
                    {
                        //To Do, Get current position as lowest output
                        PBfsm = PB1_OFF_FIRST;
                    }
                    else if((PBfsm == PB2_ON_FIRST) && PB2_DEB_VAL)
                    {
                        //To Do, Get current position as highest output
                        PBfsm = PB2_OFF_FIRST;
                    }
                }
                break;

            case PB1_OFF_FIRST:
            case PB2_OFF_FIRST:
                if(PBfsm == PB1_OFF_FIRST)
                {
                    //Green led solid, Yellow led blink
                    if(!PB2_DEB_VAL)
                    {
                        ScaleCfgTimer = PB_SET_CFG_T_set;
                        PBfsm = PB2_ON_LAST;
                    }
                }
                else
                {
                    //Green led blink, Yellow led solid
                    if(!PB1_DEB_VAL)
                    {
                        ScaleCfgTimer = PB_SET_CFG_T_set;
                        PBfsm = PB1_ON_LAST;
                    }
                }
                break;

            case PB2_ON_LAST:
            case PB1_ON_LAST:
                if(PBfsm == PB2_ON_LAST)
                {
                    //Green led solid, Yellow led blink
                    if(ScaleCfgTimer != 1)
                    {
                        if(PB2_DEB_VAL) PBfsm = PB1_OFF_FIRST;
                    }
                    else
                    {
                        if(PB2_DEB_VAL)
                        {
                            //To Do, Get current position as highest position
                            //To Do, Apply user scaling if possible or default scaling
                            ScaleCfgTimer = PB_SET_CFG_T_set;
                            PBfsm = PB2_OFF_LAST;
                        }
                    }
                }
                else
                {
                    //Green led blink, Yellow solid
                    if(ScaleCfgTimer != 1)
                    {
                        if(PB1_DEB_VAL) PBfsm = PB2_OFF_FIRST;
                    }
                    else
                    {
                        if(PB1_DEB_VAL)
                        {
                            //To Do, Get current position as lowest position
                            //To Do, Apply user scaling if possible or default scaling
                            ScaleCfgTimer = PB_SET_CFG_T_set;
                            PBfsm = PB1_OFF_LAST;
                        }
                    }
                }
                break;

            case PB2_OFF_LAST:
            case PB1_OFF_LAST:
                //both leds solid
                if(ScaleCfgTimer == 1) 
                {
                    CommonVars.Scaling = NO_SCALE;
                    PBfsm = WAIT_PB_CT;
                }
                break;

            case BOTH_CTS_ON:

                break;

            case RESTORE_DEF:

                break;
        }
    }
    //Encoder no scalable, use SET1 as DIR input and SET2 as preset
    else
    {
        CommonVars.Scaling = PB_SCALE;
        PBfsm = BOTH_PBS_ON;
        
    }
}

void PBDebTimerTask()
{
    uint8_t i;
    //Loop all debounce timers to update them
    for(i=0; i<NUMBER_PUSH_BUTTONS ; i++)
    {
        if( pPushButtons->pPBData[i].DebTimer > 1)  pPushButtons->pPBData[i].DebTimer--;
    }
    if(ScaleCfgTimer > 1) ScaleCfgTimer--;
    if(DefCfgTimer > 1) DefCfgTimer--;
}
/* *****************************************************************************
 End of File
 */
