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
 ** \brief The example for PPG io mode8 function demonstration
 **
 **   - 2017-8-10  1.0  WangM First version for PPG mode sample of CT Module.
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
#define  CT_CH          CtChannel2
#define  CT_PPG_MODE    PpgContinuous

#define  LOW_WIDTH      (199u)
/*******************************************************************************
* HCR
*   12.8ms if Master Clock =  4MHz;   PCLK1 = 4MHz;    CT Prescalar = 256
* Ext HCR
*    6.4ms if Master Clock =  8MHz;   PCLK1 = 8MHz;    CT Prescalar = 256
*    3.2ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = 256
* LCR
*     1.6s if Master Clock = 32000Hz; PCLK1 = 32000Hz; CT Prescalar = 256
* Ext LCR
*   1562ms if Master Clock = 32768Hz; PCLK1 = 32768Hz; CT Prescalar = 256
* Main PLL(16Mhz for example)
*    3.2ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = 256
*******************************************************************************/

#define  HIGH_WIDTH     (99u)
/*******************************************************************************
* HCR
*   6.4ms if Master Clock =  4MHz;   PCLK1 = 4MHz;    CT Prescalar = 256
* Ext HCR
*    3.2ms if Master Clock =  8MHz;   PCLK1 = 8MHz;    CT Prescalar = 256
*    1.6ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = 256
* LCR
*     0.8s if Master Clock = 32000Hz; PCLK1 = 32000Hz; CT Prescalar = 256
* Ext LCR
*   781ms if Master Clock = 32768Hz; PCLK1 = 32768Hz; CT Prescalar = 256
* Main PLL(16Mhz for example)
*    1.6ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = 256
*******************************************************************************/

#define  GPIO_PORT 0
#define  GPIO_PIN  0

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void PpgTrigIrqHandler(void);
static void PpgUnderflowIrqHandler(void);

/******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static volatile uint32_t m_u32CntIntTrg = 0;
static volatile uint32_t m_u32CntIntUnder = 0;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief PPG trigger interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void PpgTrigIrqHandler(void)
{
    m_u32CntIntTrg++;
}

/**
 *******************************************************************************
 ** \brief PPG underflow interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void PpgUnderflowIrqHandler(void)
{
    m_u32CntIntUnder++;
}

/**
 ******************************************************************************
 ** \brief  Main function of ct-ppg sample
 **
 ** \param [in]  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_ppg_irq_en_t    stcIrqEn;
    stc_ppg_irq_cb_t    stcIrqCb;
    stc_ct_ppg_config_t stcPpgConfig;
    stc_clk_config_t stcCfg;
    en_result_t enRet = Ok;

    /* GPIO for external trigger, Need connect P00 to TIOB2_0 before code execute*/
    Gpio_InitIO(GPIO_PORT, GPIO_PIN, GpioDirOut, TRUE, FALSE);
    Gpio_SetIO(GPIO_PORT, GPIO_PIN, 0);

    /* Set System clock */
    DDL_ZERO_STRUCT(stcCfg);
    stcCfg.u8PllMulti = 4;
    stcCfg.enPllSrc = MAIN2PLL;

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

    /* Set Main PLL as system clock, when exit osc = 4000000, pclk1 = 16000000 */
    enRet = Clk_Init(&stcCfg);
    if(Ok != enRet)
    {
        while(1)  /* Clock initialize failed */
        {
        }
    }
    else
    {
    }

    /* Clear structures */
    DDL_ZERO_STRUCT(stcPpgConfig);
    DDL_ZERO_STRUCT(stcIrqEn);
    DDL_ZERO_STRUCT(stcIrqCb);

    /* Set Composite Timer IO port */
    Gpio_SetFunc_TIOA2_0_OUT(0u);
    Gpio_SetFunc_TIOA3_1_OUT(0u);

    Gpio_SetFunc_TIOB0_0_IN(0u);  /* for exteral trigger */

    /* Set Composite Timer I/O mode */
    Ct_ConfigIOMode(CT_CH, CtIoMode8);
    /* The channel 0 need set to io mode 2 to input trigger as CIN signal */
    Ct_ConfigIOMode(CtChannel0, CtIoMode2);

    /* Initialize Pointer to interrupt request structure */
    stcPpgConfig.pstcPpgIrqEn = &stcIrqEn;
    stcPpgConfig.pstcPpgIrqCb = &stcIrqCb;
    /* Initialize PPG timer */
    stcPpgConfig.enPres = PpgPres1Div256;
    stcPpgConfig.enMode = CT_PPG_MODE;
    stcPpgConfig.enExtTrig = PpgExtTrigRising;
    stcPpgConfig.enOutputMask = PpgOutputNormal;
    stcPpgConfig.enOutputPolarity = PpgPolarityLow;
    stcPpgConfig.enRestartEn = PpgRestartDisable;
    stcPpgConfig.pstcPpgIrqEn->bPpgTrigIrq = TRUE;
    stcPpgConfig.pstcPpgIrqEn->bPpgUnderflowIrq = TRUE;
    stcPpgConfig.pstcPpgIrqCb->pfnPpgTrigIrqCb = PpgTrigIrqHandler;
    stcPpgConfig.pstcPpgIrqCb->pfnPpgUnderflowIrqCb = PpgUnderflowIrqHandler;
    stcPpgConfig.bTouchNvic = TRUE;
    Ct_Ppg_Init(CT_CH, &stcPpgConfig);

    /* Initialize PPG for channel CT_CH+1, Don't enable interrupt */
    stcPpgConfig.pstcPpgIrqEn->bPpgTrigIrq = FALSE;
    stcPpgConfig.pstcPpgIrqEn->bPpgUnderflowIrq = FALSE;
    stcPpgConfig.pstcPpgIrqCb->pfnPpgTrigIrqCb = NULL;
    stcPpgConfig.pstcPpgIrqCb->pfnPpgUnderflowIrqCb = NULL;
    Ct_Ppg_Init(CT_CH + 1u, &stcPpgConfig);

    /* Set cycle and duty value */
    Ct_Ppg_WriteLowWidthVal(CT_CH, LOW_WIDTH);   /* Cycle = (1+m)*PPG clock cycle */
    Ct_Ppg_WriteHighWidthVal(CT_CH, HIGH_WIDTH); /* Duty = (1+m)*PPG clock cycle */
    /* Set cycle and duty value for CT_CH+1 */
    Ct_Ppg_WriteLowWidthVal(CT_CH + 1u, (LOW_WIDTH+1u)/2u - 1u);
    Ct_Ppg_WriteHighWidthVal(CT_CH + 1u, (HIGH_WIDTH+1u)/2u - 1u);

    Ct_Ppg_EnableCount(CT_CH);  /* Enable count operatoin */
    Ct_Ppg_EnableCount(CT_CH + 1u);  /* Enable count operatoin */

    /* Here Generate rising edge from TIOB0_0 via P00*/
    Gpio_SetIO(GPIO_PORT, GPIO_PIN, 1u);

    /* wait for PPG channel 0 enter underflow interrupt 5 times */
    while(m_u32CntIntUnder < 5UL)
    {
    }

    /* Here Generate falling edge from TIOB0_0 via P00*/
    Gpio_SetIO(GPIO_PORT, GPIO_PIN, 0u);
    /* delay 10ms */
    delay1ms(10);
    /* Here Generate falling edge from TIOB0_0 via P00*/
    Gpio_SetIO(GPIO_PORT, GPIO_PIN, 0u);

    /* wait for PPG channel 0 enter underflow interrupt 5 times */
    while(m_u32CntIntUnder < 10UL)
    {
    }

    /* Disable PPG count */
    Ct_Ppg_DisableCount(CT_CH);
    Ct_Ppg_DisableCount(CT_CH + 1u);
    /* Disable Interrupt */
    Ct_Ppg_DisableIrq(CT_CH, PpgTrigIrq);
    Ct_Ppg_DisableIrq(CT_CH, PpgUnderflowIrq);

    while(1)
    {
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
