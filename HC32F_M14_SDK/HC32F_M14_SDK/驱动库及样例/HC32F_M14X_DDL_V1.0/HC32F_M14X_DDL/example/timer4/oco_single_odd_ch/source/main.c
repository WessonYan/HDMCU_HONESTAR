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
 ** \brief This example demonstrates how to use the single odd channel of the
 **      timer4-oco.
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     timer4-oco.
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
#define  CNT_CYCLE_VAL        (8000u)           /*< Timer4 counter cycle value */

/* Timer4 OCO */
#define OCO_CH                (TIMER4_OCO_CH1)  /*< Only select TIMER4_OCO_CH1 TIMER4_OCO_CH3 TIMER4_OCO_CH5 */

/* FAST I/O */
#define FAST_IO_PORT          (0)
#define FAST_IO_PIN           (0)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void ToggleFastIo(void);
static void OcoIrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Toggle fast gpio.
 **
 ** \param [in]                   None
 **
 ** \retval                       None
 **
 ******************************************************************************/
static void ToggleFastIo(void)
{
    if(FALSE == FGpio_GetIO(FAST_IO_PORT, FAST_IO_PIN))
    {
        FGpio_SetIO(FAST_IO_PORT, FAST_IO_PIN, TRUE);
    }
    else
    {
        FGpio_SetIO(FAST_IO_PORT, FAST_IO_PIN, FALSE);
    }

    if(FALSE == FGpio_GetIO(FAST_IO_PORT, FAST_IO_PIN))
    {
        FGpio_SetIO(FAST_IO_PORT, FAST_IO_PIN, TRUE);
    }
    else
    {
        FGpio_SetIO(FAST_IO_PORT, FAST_IO_PIN, FALSE);
    }
}

/**
 *******************************************************************************
 ** \brief oco match interrupt handler
 **
 ******************************************************************************/
static void OcoIrqCallback(void)
{
    ToggleFastIo();
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
    uint16_t u16OcoCmpVal = CNT_CYCLE_VAL/2;
    stc_timer4_cnt_config_t stcCntConfig;
    stc_timer4_oco_config_t stcOcoConfig;
    stc_oco_odd_compare_config_t stcOddCmpConfig;

    /* Clear structures */
    DDL_ZERO_STRUCT(stcCntConfig);
    DDL_ZERO_STRUCT(stcOcoConfig);
    DDL_ZERO_STRUCT(stcOddCmpConfig);

    /* Initialize Fast I/O */
    FGpio_InitIO(FAST_IO_PORT, FAST_IO_PIN, GpioDirOut);
    FGpio_SetIO(FAST_IO_PORT, FAST_IO_PIN, FALSE);

    /* Timer4 CNT : Initialize CNT configuration structure */
    stcCntConfig.enCntClockDiv = CntPclkDiv1;   /* CNT clock divide */
    stcCntConfig.enCntClock = CntClockPclk;
    stcCntConfig.bTouchNvic = FALSE;
    stcCntConfig.enCntMode = UpCnt;
    stcCntConfig.u8ZeroIntMsk = 0u;
    stcCntConfig.u8PeakIntMsk = 0u;
    stcCntConfig.bEnBuffer = FALSE;
    Timer4_Cnt_Init(&stcCntConfig);             /* Initialize CNT */
    Timer4_Cnt_SetCycleVal(CNT_CYCLE_VAL);      /* Set CNT Cycle value */

    /* Timer4 OCO : Initialize OCO configuration structure */
    stcOcoConfig.enOccrBufMode = OccrBufDisable;
    stcOcoConfig.enOcmrBufMode = OcmrBufDisable;
    stcOcoConfig.enPinState = OpLowLevel;
    stcOcoConfig.bIrqEnable = TRUE;
    stcOcoConfig.pfnIrqCallback = OcoIrqCallback;
    stcOcoConfig.bTouchNvic = TRUE;
    Timer4_Oco_Init(OCO_CH, &stcOcoConfig);     /* Initialize OCO */

    if ((TIMER4_OCO_CH1 == OCO_CH) || (TIMER4_OCO_CH3 == OCO_CH) || (TIMER4_OCO_CH5 == OCO_CH))
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

        Timer4_Oco_SetOddChCompareMode(OCO_CH, &stcOddCmpConfig); /* Set OCO odd channel compare mode */
    }

    Timer4_Oco_WriteOccr(OCO_CH, u16OcoCmpVal); /* Set OCO compare value */
    Timer4_Oco_EnableOperation(OCO_CH);         /* Enable OCO */

    /* Start CNT */
    Timer4_Cnt_Start(CntStartFromZeroValue);

    /* Toggle I/O */
    ToggleFastIo();

    while(1)
    {
        /* nop */
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
