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
 ** \brief This example demonstrates how to use the through mode function of
 **      the timer4-pwm.
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     timer4-pwm.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "timer4_cnt.h"
#include "timer4_oco.h"
#include "timer4_pwm.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Timer4 CNT */
#define CNT_CYCLE_VAL  (8000u)           /*< Timer4 counter cycle value */

/* Timer4 OCO */
#define OCO_CH_EVEN    (TIMER4_OCO_CH0)  /*< Only TIMER4_OCO_CH0  TIMER4_OCO_CH2  TIMER4_OCO_CH4 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
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
    uint8_t u8PwmCoupleCh;
    uint8_t u8OcoOddCh = OCO_CH_EVEN + 1;
    uint16_t u8OcoOccrVal = CNT_CYCLE_VAL/2;
    stc_timer4_cnt_config_t stcCntConfig;
    stc_timer4_oco_config_t stcOcoConfig;
    stc_oco_odd_compare_config_t stcOddCmpConfig;
    stc_oco_even_compare_config_t stcEvenCmpConfig;
    stc_timer4_pwm_config_t stcPwmConfig;

    /* Clear structures */
    DDL_ZERO_STRUCT(stcCntConfig);
    DDL_ZERO_STRUCT(stcOcoConfig);
    DDL_ZERO_STRUCT(stcOddCmpConfig);
    DDL_ZERO_STRUCT(stcEvenCmpConfig);
    DDL_ZERO_STRUCT(stcPwmConfig);

    /* Initialize I/O */
    Gpio_SetFunc_PWM00_0(x);
    Gpio_SetFunc_PWM01_0(x);
    Gpio_SetFunc_PWM02_0(x);
    Gpio_SetFunc_PWM03_0(x);
    Gpio_SetFunc_PWM04_0(x);
    Gpio_SetFunc_PWM05_0(x);

    /* Timer4 CNT : Initialize CNT configuration structure */
    stcCntConfig.enCntClockDiv = CntPclkDiv1;  /* CNT clock divide */
    stcCntConfig.enCntClock = CntClockPclk;
    stcCntConfig.bTouchNvic = FALSE;
    stcCntConfig.enCntMode = UpCnt;
    stcCntConfig.u8ZeroIntMsk = 0u;
    stcCntConfig.u8PeakIntMsk = 0u;
    stcCntConfig.bEnBuffer = FALSE;
    Timer4_Cnt_Init(&stcCntConfig); /* Initialize CNT */
    Timer4_Cnt_SetCycleVal(CNT_CYCLE_VAL);

    /* Timer4 OCO : Initialize OCO configuration structure */
    stcOcoConfig.enOccrBufMode = OccrBufDisable;
    stcOcoConfig.enOcmrBufMode = OcmrBufDisable;
    stcOcoConfig.enPinState = OpLowLevel;
    stcOcoConfig.bIrqEnable = FALSE;
    stcOcoConfig.pfnIrqCallback = NULL;
    stcOcoConfig.bTouchNvic = FALSE;
    Timer4_Oco_Init(OCO_CH_EVEN, &stcOcoConfig); /* Initialize OCO even channel */
    Timer4_Oco_Init(u8OcoOddCh, &stcOcoConfig);  /* Initialize OCO even channel */

    if ((TIMER4_OCO_CH0 == OCO_CH_EVEN) || (TIMER4_OCO_CH2 == OCO_CH_EVEN) || (TIMER4_OCO_CH4 == OCO_CH_EVEN))
    {
        /* ocmr[15:0] = 0x0FFF */
        stcEvenCmpConfig.enCntZeroEvenMatchEvenChOpStatus = OpOutputReverse;
        stcEvenCmpConfig.enCntZeroEvenNotMatchEvenChOpStatus = OpOutputHold;
        stcEvenCmpConfig.enCntUpCntEvenMatchEvenChOpStatus = OpOutputReverse;
        stcEvenCmpConfig.enCntPeakEvenMatchEvenChOpStatus = OpOutputReverse;
        stcEvenCmpConfig.enCntPeakEvenNotMatchEvenChOpStatus = OpOutputHold;
        stcEvenCmpConfig.enCntDownCntEvenMatchEvenChOpStatus = OpOutputReverse;

        stcEvenCmpConfig.enOcfWhenCntZeroEvenMatch = OcfSet;
        stcEvenCmpConfig.enOcfWhenCntUpCntEvenMatch = OcfSet;
        stcEvenCmpConfig.enOcfWhenCntPeakEvenMatch = OcfSet;
        stcEvenCmpConfig.enOcfWhenCntDownCntEvenMatch = OcfSet;

        stcEvenCmpConfig.bCntZeroPeakExtendMatchCondition = FALSE;

        Timer4_Oco_SetEvenChCompareMode(OCO_CH_EVEN, &stcEvenCmpConfig); /* Configure OCO even channel compare mode */
    }

    /*************Timer4 OCO ocmr1[31:0] = 0x0FF0 0FFF*****************************/
    if ((TIMER4_OCO_CH1 == u8OcoOddCh) || (TIMER4_OCO_CH3 == u8OcoOddCh) || (TIMER4_OCO_CH5 == u8OcoOddCh))
    {
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

        Timer4_Oco_SetOddChCompareMode(u8OcoOddCh, &stcOddCmpConfig); /* Configure OCO even channel compare mode */
    }

    /* set OCO compare value */
    Timer4_Oco_WriteOccr(u8OcoOddCh,  u8OcoOccrVal);
    Timer4_Oco_WriteOccr(OCO_CH_EVEN, u8OcoOccrVal);

    /* Enable oco function */
    Timer4_Oco_EnableOperation(u8OcoOddCh);
    Timer4_Oco_EnableOperation(OCO_CH_EVEN);

    /* Timer4 PWM: Get pwm couple channel */
    if((TIMER4_OCO_CH0 == OCO_CH_EVEN) && (TIMER4_OCO_CH1 == u8OcoOddCh))
    {
        u8PwmCoupleCh = TIMER4_PWM_COUPLE_CH10;
    }
    else if((TIMER4_OCO_CH2 == OCO_CH_EVEN) && (TIMER4_OCO_CH3 == u8OcoOddCh))
    {
        u8PwmCoupleCh = TIMER4_PWM_COUPLE_CH32;
    }
    else if((TIMER4_OCO_CH4 == OCO_CH_EVEN) && (TIMER4_OCO_CH5 == u8OcoOddCh))
    {
        u8PwmCoupleCh = TIMER4_PWM_COUPLE_CH54;
    }
    else
    {
        /* nop */
    }
    /* Timer4 PWM: Initialize OCO configuration structure */
    stcPwmConfig.bPwmTimerIrqMask = TRUE;
    stcPwmConfig.bTouchNvic = FALSE;
    stcPwmConfig.enClk = PwmPlckDiv1;
    stcPwmConfig.enLvlsBits = LvlsBits11B;  /* change: LvlsBits00B  LvlsBits01B  LvlsBits10B  LvlsBits11B */
    stcPwmConfig.enMode = PwmThroughMode;
    Timer4_Pwm_Init(u8PwmCoupleCh, &stcPwmConfig); /* Initialize timer4 pwm */

    /* Start CNT */
    Timer4_Cnt_Start(CntStartFromZeroValue);

    while(1)
    {
        /* nop */
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
