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
 ** \brief The example for RT function which use io mode5 demonstration
 **
 **   - 2017-8-10  1.0  WangM First version for RT mode sample of CT Module.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "ct.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define  CT_CH           CtChannel2
#define  CT_RT_MODE      RtReload
#define  CT_TIMER_SIZE   RtSize16Bit

#define  CT_RT_CYCLE     (29u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void RtTrigIrqHandler(void);
static void RtUnderflowIrqHandler(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static volatile uint32_t m_u32CntIrqTrg = 0u;
static volatile uint32_t m_u32CntIrqUnder = 0u;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 /**
 *******************************************************************************
 ** \brief RT trigger interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void RtTrigIrqHandler(void)
{
    m_u32CntIrqTrg++;
}

/**
 *******************************************************************************
 ** \brief RT underflow interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void RtUnderflowIrqHandler(void)
{
    m_u32CntIrqUnder++;
}

/**
 *******************************************************************************
 ** \brief  Main function of ct-rt sample
 **
 ** \param [in]  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_rt_irq_en_t    stcIrqEn;
    stc_rt_irq_cb_t    stcIrqCb;
    stc_ct_rt_config_t stcRtConfig;

    stc_clk_config_t stcCfg;
    en_result_t enRet = Ok;

    /* Set System clock */
    DDL_ZERO_STRUCT(stcCfg);
    stcCfg.u8PllMulti = 4u;
    stcCfg.enPllSrc = HICR2PLL;

    stcCfg.enClkSrc = ClkMPLL;
    stcCfg.enBaseClkDiv = ClkBaseDiv1;
    stcCfg.enAPB0Div = ClkApb0Div1;
    stcCfg.enAPB1Div = ClkApb1Div1;
    stcCfg.bAPB1En = TRUE;
    stcCfg.enClkWaitMain = ClkCswtMain524ms;
    stcCfg.enClkWaitSub = ClkCswtSub512ms;
    stcCfg.enClkWaitPll = ClkPswtPll680us;

    stcCfg.bHicrEn = TRUE;
    stcCfg.bMainEn = TRUE;
    stcCfg.bSubEn  = FALSE;
    stcCfg.bPllEn  = TRUE;

    enRet = Clk_Init(&stcCfg);
    if(Ok != enRet)
    {
        while(1)
        {
        }
    }
    else
    {
    }

    /* Clear structures */
    DDL_ZERO_STRUCT(stcIrqEn);
    DDL_ZERO_STRUCT(stcIrqCb);
    DDL_ZERO_STRUCT(stcRtConfig);

    /* Set Composite Timer  IO mode */
    Ct_ConfigIOMode(CT_CH, CtIoMode5);

    /* Set Composite Timer  IO port */
    Gpio_SetFunc_TIOA2_0_OUT(0u);
    Gpio_SetFunc_TIOA3_1_OUT(0u);

    /* Initialize Pointer to interrupt request structure */
    stcRtConfig.pstcRtIrqEn = &stcIrqEn;
    stcRtConfig.pstcRtIrqCb = &stcIrqCb;
    /* Initialize Composite Timer */
    stcRtConfig.enPres = RtPres1Div2048;
    stcRtConfig.enSize = CT_TIMER_SIZE;
    stcRtConfig.enMode = CT_RT_MODE;
    stcRtConfig.enExtTrig = RtExtTiggerFallingEdge;;
    stcRtConfig.enOutputPolarity = RtPolarityLow;
    stcRtConfig.pstcRtIrqEn->bRtTrigIrq = 1u;
    stcRtConfig.pstcRtIrqEn->bRtUnderflowIrq = 1u;
    stcRtConfig.pstcRtIrqCb->pfnRtTrigIrqCb = RtTrigIrqHandler ;
    stcRtConfig.pstcRtIrqCb->pfnRtUnderflowIrqCb = RtUnderflowIrqHandler;
    stcRtConfig.bTouchNvic = TRUE;
    Ct_Rt_Init(CT_CH, &stcRtConfig);

    /* Initialize Composite Timer for channel CT_CH+1*/
    stcRtConfig.enPres = RtPres1Div128;
    stcRtConfig.pstcRtIrqEn->bRtTrigIrq = FALSE;
    stcRtConfig.pstcRtIrqEn->bRtUnderflowIrq = FALSE;
    stcRtConfig.pstcRtIrqCb->pfnRtTrigIrqCb = NULL ;
    stcRtConfig.pstcRtIrqCb->pfnRtUnderflowIrqCb = NULL;
    Ct_Rt_Init(CT_CH + 1u, &stcRtConfig);

    /* Write cycle value */
    Ct_Rt_WriteCycleVal(CT_CH, CT_RT_CYCLE);
    /* Write cycle value for channel CT_CH+1 */
    Ct_Rt_WriteCycleVal(CT_CH + 1u, CT_RT_CYCLE);

    /* Start Reload timer by software trigger */
    Ct_Rt_EnableCount(CT_CH);
    /* Start Reload timer by software trigger for channel CT_CH+1 */
    Ct_Rt_EnableCount(CT_CH + 1u);

    /* Software trigger channel 2 and 3 Simultaneous */
    Ct_SetSimultaneousStart(0x0C);

    /* Start Reload timer by software trigger */
    while(m_u32CntIrqUnder < 10ul)
    {
    }

    /* Disable RT count */
    Ct_Rt_DisableCount(CT_CH);
    Ct_Rt_DisableCount(CT_CH + 1);
    /* Disable Interrupt */
    Ct_Rt_DisableIrq(CT_CH, RtTrigIrq);
    Ct_Rt_DisableIrq(CT_CH, RtUnderflowIrq);

    while(1)
    {

    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
