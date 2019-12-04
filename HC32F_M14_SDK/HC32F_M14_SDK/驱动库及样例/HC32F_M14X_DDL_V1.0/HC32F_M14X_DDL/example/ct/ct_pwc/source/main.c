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
 ** \brief The example for PWC function demonstration
 **
 **   - 2017-8-10  1.0  WangM First version for PWC mode sample of CT Module.
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
/* PWM */
#define  CT_PWM_CH             CtChannel0
#define  CT_PWM_CYCLE          (999)
#define  CT_PWM_DUTY           (199)

/* PWC */
#define  CT_PWC_CH             CtChannel1
#define  CT_PWC_TIMER_SIZE     PwcSize16Bit

#define  MEASURE_CNT_MAX       (10)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint32_t m_aMeasureResult[MEASURE_CNT_MAX];

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void PwcOverflowIntHandler(void);
static void PwcMeasCmpIrqHandler(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/* \brief Interrupt count value */
static volatile uint32_t m_u32CntIntMeasure;
static volatile uint32_t m_u32CntIntOverflow;
static volatile uint32_t m_u32CntMeasureErr;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief PWC overflow interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void PwcOverflowIntHandler(void)
{
    m_u32CntIntOverflow++;
}

/**
 *******************************************************************************
 ** \brief PWC measure complete interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void PwcMeasCmpIrqHandler(void)
{
    uint32_t u32CntIntOverflow;

    if (m_u32CntIntMeasure < MEASURE_CNT_MAX)
    {
        if (CT_PWC_TIMER_SIZE == PwcSize16Bit)
        {
            u32CntIntOverflow = (m_u32CntIntOverflow * 0xffff);
            m_aMeasureResult[m_u32CntIntMeasure] = Ct_Pwc_Get16BitMeasureData(CT_PWC_CH) + u32CntIntOverflow;
        }
        else /*CT_32IT_TIMER */
        {
            m_aMeasureResult[m_u32CntIntMeasure] = Ct_Pwc_Get32BitMeasureData(CT_PWC_CH);
        }
        m_u32CntIntMeasure++;
    }
    else
    {
        if (CT_PWC_TIMER_SIZE == PwcSize16Bit)
        {
            Ct_Pwc_Get16BitMeasureData(CT_PWC_CH);
        }
        else
        {
            Ct_Pwc_Get32BitMeasureData(CT_PWC_CH);
        }
    }
    m_u32CntIntOverflow = 0;
}

/**
 *******************************************************************************
 ** \brief  Main function of ct-pwc sample
 **
 ** \return int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    uint32_t u32Cnt;
    uint32_t u32CntIntOverflow;
    stc_ct_pwm_config_t stcPwmConfig;
    stc_ct_pwc_config_t stcPwcConfig;
    stc_pwc_irq_en_t stcPwcIrqEn;
    stc_pwc_irq_cb_t stcPwcIrqCallback;

    stc_clk_config_t stcCfg;
    en_result_t enRet = Ok;

    /* Set System clock */
    DDL_ZERO_STRUCT(stcCfg);
    stcCfg.u8PllMulti = 4;
    stcCfg.enPllSrc = MAIN2PLL;

    stcCfg.enClkSrc = ClkHSXT;
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
    DDL_ZERO_STRUCT(stcPwmConfig);
    DDL_ZERO_STRUCT(stcPwcConfig);
    DDL_ZERO_STRUCT(stcPwcIrqEn);
    DDL_ZERO_STRUCT(stcPwcIrqCallback);

    /*------------- PWM Generation -----------------*/
    /* Set Composite Timer  IO port */
    Gpio_SetFunc_TIOA0_0_OUT(0u);

    /* PWM configuration  */
    Ct_ConfigIOMode(CT_PWM_CH, CtIoMode0);
    stcPwmConfig.enPres = PwmPres1Div128;
    stcPwmConfig.enMode = PwmContinuous;
    stcPwmConfig.enExtTrig = PwmExtTrigDisable;
    stcPwmConfig.enOutputMask = PwmOutputNormal;
    stcPwmConfig.enOutputPolarity = PwmPolarityLow;
    stcPwmConfig.enRestartEn = PwmRestartEnable;
    Ct_Pwm_Init(CT_PWM_CH , &stcPwmConfig);
    Ct_Pwm_WriteCycleVal(CT_PWM_CH, CT_PWM_CYCLE);
    Ct_Pwm_WriteDutyVal(CT_PWM_CH,  CT_PWM_DUTY);
    /* Start PWM  */
    Ct_Pwm_EnableCount(CT_PWM_CH);
    Ct_Pwm_EnableSwTrig(CT_PWM_CH);
    /*---------------------------------------------*/

    /* Set IO port   */
    Gpio_SetFunc_TIOB1_1_IN(0u);

    /* PWC register initialization */
    stcPwcConfig.enPres = PwcPres1Div128;
    stcPwcConfig.enMode = PwcContinuous;
    stcPwcConfig.enMeasureEdge = PwcMeasureEitherToEither;
    stcPwcConfig.enSize = CT_PWC_TIMER_SIZE;
    stcPwcConfig.bTouchNvic =  TRUE;
    /* Initialize Pointer to interrupt request structure */
    stcPwcConfig.pstcPwcIrqEn = &stcPwcIrqEn;
    stcPwcConfig.pstcPwcIrqCb = &stcPwcIrqCallback;
    /* Enable Interrupt */
    stcPwcConfig.pstcPwcIrqEn->bPwcMeasureCompleteIrq = TRUE;
    stcPwcConfig.pstcPwcIrqEn->bPwcMeasureOverflowIrq = TRUE;
    stcPwcConfig.pstcPwcIrqCb->pfnPwcMeasureCompleteIrqCb = PwcMeasCmpIrqHandler;
    stcPwcConfig.pstcPwcIrqCb->pfnPwcMeasureOverflowIrqCb = PwcOverflowIntHandler;
    Ct_Pwc_Init(CT_PWC_CH, &stcPwcConfig);

    /* Enable count operatoin */
    Ct_Pwc_EnableCount(CT_PWC_CH);

    /* Waiting for mesaurment compeleted */
    while(m_u32CntIntMeasure < MEASURE_CNT_MAX)
    {
    }

    /* Disable Interrupt */
    Ct_Pwc_DisableIrq(CT_PWC_CH, PwcMeasureCompleteIrq);
    Ct_Pwc_DisableIrq(CT_PWC_CH, PwcMeasureOverflowIrq);
    /* Disable Pwc count */
    Ct_Pwc_DisableCount(CT_PWC_CH);

    m_u32CntIntMeasure = 0;
    m_u32CntIntOverflow = 0;
    m_u32CntMeasureErr = 0;

    for(u32Cnt = 0; u32Cnt < MEASURE_CNT_MAX; u32Cnt++)
    {
        m_aMeasureResult[u32Cnt] = 0;
    }

    /* Enable count operatoin */
    Ct_Pwc_EnableCount(CT_PWC_CH);

    /* Interrupt query mode */
    do
    {
        if (Ct_Pwc_GetIrqFlag(CT_PWC_CH, PwcMeasureCompleteIrq) == TRUE)
        {
            if (stcPwcConfig.enSize == PwcSize16Bit)
            {
                u32CntIntOverflow = (m_u32CntIntOverflow * 0xffff);
                m_aMeasureResult[m_u32CntIntMeasure] = Ct_Pwc_Get16BitMeasureData(CT_PWC_CH) +
                                                       u32CntIntOverflow;
            }
            else /*CT_32IT_TIMER */
            {
                /*Comment this line if testing measure error feature */
        m_aMeasureResult[m_u32CntIntMeasure] = Ct_Pwc_Get32BitMeasureData(CT_PWC_CH);
            }

            m_u32CntIntOverflow = 0;
            m_u32CntIntMeasure++;

            if (Ct_Pwc_GetErrorFlag(CT_PWC_CH) == TRUE)
            {
                m_u32CntMeasureErr++;
            }
        }
        else if (Ct_Pwc_GetIrqFlag(CT_PWC_CH, PwcMeasureOverflowIrq) == TRUE)
        {
            Ct_Pwc_ClrIrqFlag(CT_PWC_CH, PwcMeasureOverflowIrq);
            m_u32CntIntOverflow++;
        }
        else
        {
        }

    }while(m_u32CntIntMeasure < MEASURE_CNT_MAX);

    Ct_Pwm_DisableCount(CT_PWC_CH); /* Disable Pwm count */
    Ct_Pwm_DeInit(CT_PWC_CH, TRUE); /* De-Initialize Pwm */
    Ct_Pwc_DisableCount(CT_PWC_CH); /* Disable Pwc count */
    Ct_Pwc_DeInit(CT_PWC_CH, TRUE); /* De-Initialize Pwc */

    while(1)
    {
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
