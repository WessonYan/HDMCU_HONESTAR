
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
 ** \brief This example demonstrates how to use the link double channels of the
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
#define  CNT_CYCLE_VAL        (8000u)           // Timer4 counter cycle value

/* Timer4 OCO */
#define OCO_CH_EVEN           (TIMER4_OCO_CH0)   // only TIMER4_OCO_CH0  TIMER4_OCO_CH2  TIMER4_OCO_CH4

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
static uint8_t m_OcoOddCh = OCO_CH_EVEN + 1;
en_oco_op_out_state_t m_enOcoOddChLastOpOutState = OpLowLevel;

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
    en_oco_op_out_state_t enOcoOpOutState;

    enOcoOpOutState = Timer4_Oco_GetOpPinLevel(m_OcoOddCh);
    if(m_enOcoOddChLastOpOutState != enOcoOpOutState)
    {
        ToggleFastIo();
        m_enOcoOddChLastOpOutState = enOcoOpOutState;
    }
    else
    {
        /* nop */
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
    uint16_t OcoEvenChOccrVal = CNT_CYCLE_VAL / 4;
    uint16_t OcoOddChOccrVal  = CNT_CYCLE_VAL * 3 /4;
    stc_timer4_cnt_config_t stcCntConfig;
    stc_timer4_oco_config_t stcOcoChConfig;
    stc_oco_odd_compare_config_t stcOddCmpConfig;
    stc_oco_even_compare_config_t stcEvenCmpConfig;

    /* Clear structures */
    DDL_ZERO_STRUCT(stcCntConfig);
    DDL_ZERO_STRUCT(stcOcoChConfig);
    DDL_ZERO_STRUCT(stcOddCmpConfig);
    DDL_ZERO_STRUCT(stcEvenCmpConfig);
    m_OcoOddCh = OCO_CH_EVEN + 1;

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

    /* Timer4 OCO : Initialize OCO channel configuration structure */
    stcOcoChConfig.enOccrBufMode = OccrBufTrsfByCntZero;
    stcOcoChConfig.enOcmrBufMode = OcmrBufDisable;
    stcOcoChConfig.enPinState = OpLowLevel;
    stcOcoChConfig.bIrqEnable = TRUE;
    stcOcoChConfig.pfnIrqCallback = OcoIrqCallback;
    stcOcoChConfig.bTouchNvic = TRUE;
    Timer4_Oco_Init(OCO_CH_EVEN, &stcOcoChConfig);  /* Initialize OCO even channel */
    Timer4_Oco_Init(m_OcoOddCh, &stcOcoChConfig);   /* Initialize OCO odd channel */

    if ((TIMER4_OCO_CH0 == OCO_CH_EVEN) || (TIMER4_OCO_CH2 == OCO_CH_EVEN) || (TIMER4_OCO_CH4 == OCO_CH_EVEN))
    {
        /* ocmr[15:0] = 0x000F     0000 0000 0000 1111   */
        stcEvenCmpConfig.enCntZeroEvenMatchEvenChOpStatus = OpOutputHold;     /* Bit[11:10]  00 */
        stcEvenCmpConfig.enCntZeroEvenNotMatchEvenChOpStatus = OpOutputHold;  /* Bit[15:14]  00 */
        stcEvenCmpConfig.enCntUpCntEvenMatchEvenChOpStatus = OpOutputHold;    /* Bit[9:8]    00 */
        stcEvenCmpConfig.enCntPeakEvenMatchEvenChOpStatus = OpOutputHold;     /* Bit[7:6]    00 */
        stcEvenCmpConfig.enCntPeakEvenNotMatchEvenChOpStatus = OpOutputHold;  /* Bit[13:12]  00 */
        stcEvenCmpConfig.enCntDownCntEvenMatchEvenChOpStatus = OpOutputHold;  /* Bit[5:4]    00 */

        stcEvenCmpConfig.enOcfWhenCntZeroEvenMatch = OcfSet;     /* bit[3] 1 */
        stcEvenCmpConfig.enOcfWhenCntUpCntEvenMatch = OcfSet;    /* bit[2] 1 */
        stcEvenCmpConfig.enOcfWhenCntPeakEvenMatch = OcfSet;     /* bit[1] 1 */
        stcEvenCmpConfig.enOcfWhenCntDownCntEvenMatch = OcfSet;  /* bit[0] 1 */

        stcEvenCmpConfig.bCntZeroPeakExtendMatchCondition = FALSE;

        Timer4_Oco_SetEvenChCompareMode(OCO_CH_EVEN, &stcEvenCmpConfig);  /* Set OCO even channel compare mode */
    }

    if ((TIMER4_OCO_CH1 == m_OcoOddCh) || (TIMER4_OCO_CH3 == m_OcoOddCh) || (TIMER4_OCO_CH5 == m_OcoOddCh))
    {
        /* OCMR[31:0] Ox FFFF 0FFF    1111 1111 1111 1111  0000 1111 1111 1111 */
        stcOddCmpConfig.enCntZeroOddMatchEvenMatchOddChOpStatus = OpOutputReverse;         /* bit[27:26]  11  */
        stcOddCmpConfig.enCntZeroOddMatchEvenNotMatchOddChOpStatus = OpOutputReverse;      /* bit[11:10]  11  */
        stcOddCmpConfig.enCntZeroOddNotMatchEvenMatchOddChOpStatus = OpOutputReverse;      /* bit[31:30]  11 */
        stcOddCmpConfig.enCntZeroOddNotMatchEvenNotMatchOddChOpStatus = OpOutputHold;      /* bit[15:14]  00 */

        stcOddCmpConfig.enCntUpCntOddMatchEvenMatchOddChOpStatus = OpOutputReverse;        /* bit[25:24]  11 */
        stcOddCmpConfig.enCntUpCntOddMatchEvenNotMatchOddChOpStatus = OpOutputReverse;     /* bit[9:8]    11 */
        stcOddCmpConfig.enCntUpCntOddNotMatchEvenMatchOddChOpStatus = OpOutputReverse;     /* bit[19:18]  11 */

        stcOddCmpConfig.enCntPeakOddMatchEvenMatchOddChOpStatus = OpOutputReverse;         /* bit[23:22]  11 */
        stcOddCmpConfig.enCntPeakOddMatchEvenNotMatchOddChOpStatus = OpOutputReverse;      /* bit[7:6]    11 */
        stcOddCmpConfig.enCntPeakOddNotMatchEvenMatchOddChOpStatus = OpOutputReverse;      /* bit[29:28]  11 */
        stcOddCmpConfig.enCntPeakOddNotMatchEvenNotMatchOddChOpStatus = OpOutputHold;      /* bit[13:12]  00 */

        stcOddCmpConfig.enCntDownOddMatchEvenMatchOddChOpStatus = OpOutputReverse;         /* bit[21:20]  11 */
        stcOddCmpConfig.enCntDownOddMatchEvenNotMatchOddChOpStatus = OpOutputReverse;      /* bit[5:4]    11 */
        stcOddCmpConfig.enCntDownOddNotMatchEvenMatchOddChOpStatus = OpOutputReverse;      /* bit[17:16]  11 */

        stcOddCmpConfig.enOcfWhenCntZeroOddMatch = OcfSet;    /* bit[3] 1 */
        stcOddCmpConfig.enOcfWhenCntUpCntOddMatch = OcfSet;   /* bit[2] 1 */
        stcOddCmpConfig.enOcfWhenCntPeakOddMatch = OcfSet;    /* bit[1] 1 */
        stcOddCmpConfig.enOcfWhenCntDownCntOddMatch = OcfSet; /* bit[0] 1 */

        Timer4_Oco_SetOddChCompareMode(m_OcoOddCh, &stcOddCmpConfig);  /* Set OCO odd channel compare mode */
    }

    /* Set OCO compare value */
    Timer4_Oco_WriteOccr(OCO_CH_EVEN, OcoEvenChOccrVal);
    Timer4_Oco_WriteOccr(m_OcoOddCh,  OcoOddChOccrVal);

    /* Enable OCO */
    Timer4_Oco_EnableOperation(OCO_CH_EVEN);
    Timer4_Oco_EnableOperation(m_OcoOddCh);

    m_enOcoOddChLastOpOutState = Timer4_Oco_GetOpPinLevel(m_OcoOddCh);

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
