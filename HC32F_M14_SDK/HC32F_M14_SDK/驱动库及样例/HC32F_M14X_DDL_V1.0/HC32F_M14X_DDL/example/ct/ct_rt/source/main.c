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
 ** \brief The example for RT function demonstration
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
#define  CT_CH           CtChannel0
#define  CT_RT_MODE      RtReload
#define  CT_TIMER_SIZE   RtSize32Bit

#define  CT_RT_CYCLE     (0x7FFFFu)
/*******************************************************************************
* HCR
*    131ms if Master Clock =  4MHz;   PCLK1 = 4MHz;    CT Prescalar = none
* Ext HCR
*   65.6ms if Master Clock =  8MHz;   PCLK1 = 8MHz;    CT Prescalar = none
*   32.8ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = none
* LCR
*    16.4s if Master Clock = 32000Hz; PCLK1 = 32000Hz; CT Prescalar = none
* Ext LCR
*    15.9s if Master Clock = 32768Hz; PCLK1 = 32768Hz; CT Prescalar = none
* Main PLL(16Mhz for example)
*   32.8ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = none
*******************************************************************************/

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
    Ct_ConfigIOMode(CT_CH, CtIoMode0);

    /* Set Composite Timer  IO port */
    Gpio_SetFunc_TIOA0_0_OUT(0u);

    /* Initialize Pointer to interrupt request structure */
    stcRtConfig.pstcRtIrqEn = &stcIrqEn;
    stcRtConfig.pstcRtIrqCb = &stcIrqCb;
    /* Initialize Composite Timer */
    stcRtConfig.enPres = RtPresNone;
    stcRtConfig.enSize = CT_TIMER_SIZE;
    stcRtConfig.enMode = CT_RT_MODE;
    stcRtConfig.enExtTrig = RtExtTiggerDisable;
    stcRtConfig.enOutputPolarity = RtPolarityLow;
    stcRtConfig.pstcRtIrqEn->bRtTrigIrq = 1u;
    stcRtConfig.pstcRtIrqEn->bRtUnderflowIrq = 1u;
    stcRtConfig.pstcRtIrqCb->pfnRtTrigIrqCb = RtTrigIrqHandler ;
    stcRtConfig.pstcRtIrqCb->pfnRtUnderflowIrqCb = RtUnderflowIrqHandler;
    stcRtConfig.bTouchNvic = TRUE;
    Ct_Rt_Init(CT_CH, &stcRtConfig);

    /* Write cycle value */
    Ct_Rt_WriteCycleVal(CT_CH+1, (uint16_t)((uint32_t)CT_RT_CYCLE >>16));
    Ct_Rt_WriteCycleVal(CT_CH, (uint16_t)CT_RT_CYCLE);

    /* Start Reload timer by software trigger */
    Ct_Rt_EnableCount(CT_CH+1);
    Ct_Rt_EnableCount(CT_CH);

    Ct_Rt_EnableSwTrig(CT_CH);

    while(m_u32CntIrqUnder < 10ul)
    {
    }

    /* Disable RT count */
    Ct_Rt_DisableCount(CT_CH);
    /* Disable Interrupt */
    Ct_Rt_DisableIrq(CT_CH, RtTrigIrq);
    Ct_Rt_DisableIrq(CT_CH, RtUnderflowIrq);

    m_u32CntIrqTrg = 0u;
    m_u32CntIrqUnder = 0u;

    Ct_Rt_EnableCount(CT_CH);  /* Disable RT count */
    Ct_Rt_EnableSwTrig(CT_CH); /* Restart triggered by software */

    /* Interrupt query mode */
    do
    {
        if (1u == Ct_Rt_GetIrqFlag(CT_CH, RtTrigIrq))
        {
            Ct_Rt_ClrIrqFlag(CT_CH, RtTrigIrq);
            m_u32CntIrqTrg++;
        }
        else if (1u == Ct_Rt_GetIrqFlag(CT_CH, RtUnderflowIrq))
        {
            Ct_Rt_ClrIrqFlag(CT_CH, RtUnderflowIrq);
            m_u32CntIrqUnder++;
        }
        else
        {
        }
    }while(m_u32CntIrqUnder < 10ul);

    Ct_Rt_DisableCount(CT_CH); /* Disable RT count */
    Ct_Rt_DeInit(CT_CH, TRUE); /* De-Initialize RT */

    while(1)
    {

    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
