/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file main.c
 **
 ** \brief This example demonstrates how to use the software trigger function
 **       of the timer4-emi.
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     timer4-emi.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "timer4_cnt.h"
#include "timer4_oco.h"
#include "timer4_pwm.h"
#include "timer4_emi.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Timer4 CNT */
#define CNT_CYCLE_VAL   (8000u)      /*< Timer4 counter cycle value */

/* Timer4 OCO */
#define OCO_OCCR_VAL    (CNT_CYCLE_VAL/4)

/* User key */
#define USER_KEY_PORT   (6)
#define USER_KEY_PIN    (2)

#define ISKeyPressed()  (FALSE == Gpio_GetIO(USER_KEY_PORT, USER_KEY_PIN))
#define ISKeyReleased() (TRUE == Gpio_GetIO(USER_KEY_PORT, USER_KEY_PIN))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static boolean_t PollingKey(void);
static void EmiDigitalPathIrqCb(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Polling key status
 **
 ** \param [in] none
 **
 ** \retval TRUE                        key is pressed and released.
 ** \retval FALSE                       key isn't pressed or is pressed.
 **
 ******************************************************************************/
static boolean_t PollingKey(void)
{
    boolean_t bKeyStatus = FALSE;
    static boolean_t bPressed = FALSE;

    if((FALSE == bPressed) && (ISKeyPressed()))
    {
        bPressed = TRUE;    /* Key is pressed */
    }

    if((TRUE == bPressed) && (ISKeyReleased()))
    {
        bPressed = FALSE;
        bKeyStatus = TRUE;  /* Released key after pressed */
    }

    return bKeyStatus;
}

/**
 *******************************************************************************
 ** \brief emi digital path interrupt handler callback
 **
 ******************************************************************************/
static void EmiDigitalPathIrqCb(void)
{
    if(TRUE == PollingKey())
    {
        Timer4_Emi_ClrDigitalPathIrqFlag();
    }
}

/**
 *******************************************************************************
 ** \brief  Main function of project.
 **
 ** \param [in] none
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_timer4_cnt_config_t stcCntConfig;
    stc_timer4_oco_config_t stcOcoConfig;
    stc_oco_odd_compare_config_t stcOddCmpConfig;
    stc_oco_even_compare_config_t stcEvenCmpConfig;
    stc_timer4_pwm_config_t stcPwmConfig;
    stc_timer4_emi_config_t stcEmiConfig;

    /* Clear structures */
    DDL_ZERO_STRUCT(stcCntConfig);
    DDL_ZERO_STRUCT(stcOcoConfig);
    DDL_ZERO_STRUCT(stcOddCmpConfig);
    DDL_ZERO_STRUCT(stcEvenCmpConfig);
    DDL_ZERO_STRUCT(stcPwmConfig);
    DDL_ZERO_STRUCT(stcEmiConfig);

    /* Initialize I/O */
    Gpio_InitIO(USER_KEY_PORT, USER_KEY_PIN, GpioDirIn, FALSE, FALSE);
    Gpio_SetFunc_PWM01_0(x);  /* P71 */
    bM0P_GPIO_DIR7_P71 = 1u;
    Gpio_SetFunc_EMII_EX(x);  /* P07 */

    /* Timer4 CNT : Initialize CNT configuration structure */
    stcCntConfig.enCntClockDiv = CntPclkDiv1;  /* CNT clock divide */
    stcCntConfig.enCntClock = CntClockPclk;
    stcCntConfig.pstcIrqEn = NULL;
    stcCntConfig.pstcIrqCb = NULL;
    stcCntConfig.bTouchNvic = FALSE;
    stcCntConfig.enCntMode = UpCnt;
    stcCntConfig.u8ZeroIntMsk = 0u;
    stcCntConfig.u8PeakIntMsk = 0u;
    stcCntConfig.bEnBuffer = FALSE;
    Timer4_Cnt_Init(&stcCntConfig);   /* Initialize CNT */
    Timer4_Cnt_SetCycleVal(CNT_CYCLE_VAL);

    /* Timer4 OCO : Initialize OCO channel configuration structure */
    stcOcoConfig.enOccrBufMode = OccrBufDisable;
    stcOcoConfig.enOcmrBufMode = OcmrBufDisable;
    stcOcoConfig.enPinState = OpHighLevel;
    stcOcoConfig.bIrqEnable = FALSE;
    stcOcoConfig.pfnIrqCallback = NULL;
    stcOcoConfig.bTouchNvic = FALSE;
    Timer4_Oco_Init(TIMER4_OCO_CH1, &stcOcoConfig);   /* Initialize OCO odd channel */

    /*************Timer4 OCO ocmr1[31:0] = 0x0FF0 0FFF*****************************/
    /* OCMR[31:0] Ox 0FF0 0FFF    0000 1111 1111 0000   0000 1111 1111 1111 */
    stcOddCmpConfig.enCntZeroOddMatchEvenMatchOddChOpStatus = OpOutputReverse;         /* bit[27:26]  11 */
    stcOddCmpConfig.enCntZeroOddMatchEvenNotMatchOddChOpStatus = OpOutputReverse;      /* bit[11:10]  11 */
    stcOddCmpConfig.enCntZeroOddNotMatchEvenMatchOddChOpStatus = OpOutputHold;         /* bit[31:30]  00 */
    stcOddCmpConfig.enCntZeroOddNotMatchEvenNotMatchOddChOpStatus = OpOutputHold;      /* bit[15:14]  00 */

    stcOddCmpConfig.enCntUpCntOddMatchEvenMatchOddChOpStatus = OpOutputReverse;        /* bit[25:24]  11 */
    stcOddCmpConfig.enCntUpCntOddMatchEvenNotMatchOddChOpStatus = OpOutputReverse;     /* bit[9:8]    11 */
    stcOddCmpConfig.enCntUpCntOddNotMatchEvenMatchOddChOpStatus = OpOutputHold;        /* bit[19:18]  00 */

    stcOddCmpConfig.enCntPeakOddMatchEvenMatchOddChOpStatus = OpOutputReverse;         /* bit[23:22]  11 */
    stcOddCmpConfig.enCntPeakOddMatchEvenNotMatchOddChOpStatus = OpOutputReverse;      /* bit[7:6]    11 */
    stcOddCmpConfig.enCntPeakOddNotMatchEvenMatchOddChOpStatus = OpOutputHold;         /* bit[29:28]  00 */
    stcOddCmpConfig.enCntPeakOddNotMatchEvenNotMatchOddChOpStatus = OpOutputHold;      /* bit[13:12]  00 */

    stcOddCmpConfig.enCntDownOddMatchEvenMatchOddChOpStatus = OpOutputReverse;         /* bit[21:20]  11 */
    stcOddCmpConfig.enCntDownOddMatchEvenNotMatchOddChOpStatus = OpOutputReverse;      /* bit[5:4]    11 */
    stcOddCmpConfig.enCntDownOddNotMatchEvenMatchOddChOpStatus = OpOutputHold;         /* bit[17:16]  00 */

    stcOddCmpConfig.enOcfWhenCntZeroOddMatch = OcfSet;    /* bit[3] 1 */
    stcOddCmpConfig.enOcfWhenCntUpCntOddMatch = OcfSet;   /* bit[2] 1 */
    stcOddCmpConfig.enOcfWhenCntPeakOddMatch = OcfSet;    /* bit[1] 1 */
    stcOddCmpConfig.enOcfWhenCntDownCntOddMatch = OcfSet; /* bit[0] 1 */

    /* Set oco odd channel compare mode */
    Timer4_Oco_SetOddChCompareMode(TIMER4_OCO_CH1, &stcOddCmpConfig);

    /* Set oco odd channel compare value */
    Timer4_Oco_WriteOccr(TIMER4_OCO_CH1, OCO_OCCR_VAL);

    /* Enable oco odd channel */
    Timer4_Oco_EnableOperation(TIMER4_OCO_CH1);

    /* Timer4 PWM: Initialize OCO configuration structure */
    stcPwmConfig.bPwmTimerIrqMask = TRUE;
    stcPwmConfig.bTouchNvic = FALSE;
    stcPwmConfig.enClk = PwmPlckDiv1;
    stcPwmConfig.enLvlsBits = LvlsBits00B;
    stcPwmConfig.enMode = PwmThroughMode;
    Timer4_Pwm_Init(TIMER4_PWM_COUPLE_CH10, &stcPwmConfig);   /* Initialize timer4 pwm */

    /* Timer4 EMI: Initialize emi configuration structure */
    stcEmiConfig.bDigitalPathIrqMskEnable = FALSE;
    stcEmiConfig.bAnalogPathIrqMskEnable = TRUE;
    stcEmiConfig.bDigitalPathEnable = FALSE;
    stcEmiConfig.bAnalogPathEnable = FALSE;
    stcEmiConfig.stcIrqCb.pfnDigitalPathIrqCb = EmiDigitalPathIrqCb;
    stcEmiConfig.stcIrqCb.pfnAnalogPathIrqCb = NULL;
    stcEmiConfig.bHoldPwm = TRUE;
    stcEmiConfig.bSwitchToGpio = TRUE;
    stcEmiConfig.bTouchNvic = TRUE;
    Timer4_Emi_Init(&stcEmiConfig); /* Initialize timer4 emi */

    /* Start CNT */
    Timer4_Cnt_Start(CntStartFromZeroValue);

    while(1)
    {
        if(TRUE == PollingKey())
        {
            Timer4_Emi_SwTiggerEmi();
        }
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
