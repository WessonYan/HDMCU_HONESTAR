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
 ** \brief This example demonstrates how to use the count up-down mode function
 **        of the timer4-adct.
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     timer4-cnt.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "timer4_cnt.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* LED I/O */
#define Led_Init() Gpio_InitIO(2, 4, GpioDirOut, FALSE, FALSE)
#define Led_On()   Gpio_SetIO(2, 4, FALSE)
#define Led_Off()  Gpio_SetIO(2, 4, TRUE)

#define CNT_CYCLE_VAL  (59999u)          /*< Timer4 counter cycle value */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void ZeroMatchIrqCb(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint32_t m_u32ZeroIrqCnt = 0;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief zero match interrupt handler
 ******************************************************************************/
static void ZeroMatchIrqCb(void)
{
   if(++m_u32ZeroIrqCnt % 2)
   {
       Led_On();   /* Led on */
   }
   else
   {
       Led_Off();  /* Led off */
   }
}

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_cnt_irq_en_t stcIrqEn;
    stc_cnt_irq_cb_t stcIrqCb;
    stc_timer4_cnt_config_t stcCntConfig;

    /* Clear structures */
    DDL_ZERO_STRUCT(stcIrqEn);
    DDL_ZERO_STRUCT(stcIrqCb);
    DDL_ZERO_STRUCT(stcCntConfig);

    /* Initialize I/O */
    Led_Init();

    /* Initialize CNT irq configuration structure */
    stcIrqEn.bCntPeakMatchIrq = FALSE;
    stcIrqEn.bCntZeroMatchIrq = TRUE;
    stcIrqCb.pfnCntPeakIrqCb = NULL;
    stcIrqCb.pfnCntZeroIrqCb = ZeroMatchIrqCb;

    /* Initialize CNT configuration structure */
    stcCntConfig.enCntClockDiv = CntPclkDiv4;  /* CNT clock divide */
    stcCntConfig.enCntMode = UpDownCnt;
    stcCntConfig.enCntClock = CntClockPclk;
    stcCntConfig.u8ZeroIntMsk = 0;
    stcCntConfig.u8PeakIntMsk = 0;
    stcCntConfig.bEnBuffer = FALSE;
    stcCntConfig.pstcIrqEn = &stcIrqEn;
    stcCntConfig.pstcIrqCb = &stcIrqCb;
    stcCntConfig.bTouchNvic = TRUE;
    Timer4_Cnt_Init(&stcCntConfig); /* Initialize CNT */

    Timer4_Cnt_SetCycleVal(CNT_CYCLE_VAL);   /* Set CNT cycle value */
    Timer4_Cnt_Start(CntStartFromZeroValue); /* Start CNT */

    while(1)
    {
        /* nop */
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
