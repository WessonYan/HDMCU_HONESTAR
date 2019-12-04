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
 ** \brief The example for PWM function which use io mode 1 demonstration
 **
 **   - 2017-8-10  1.0  WangM First version for PWM mode sample of CT Module.
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
#define  CT_CH         CtChannel0
#define  CT_PWM_MODE   PwmContinuous

#define  CYCLE_VAL     (29u)
/*******************************************************************************
* HCR
*   3.84ms if Master Clock =  4MHz;   PCLK1 = 4MHz;    CT Prescalar = 512
* Ext HCR
*   1.92ms if Master Clock =  8MHz;   PCLK1 = 8MHz;    CT Prescalar = 512
*   0.96ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = 512
* LCR
*    480ms if Master Clock = 32000Hz; PCLK1 = 32000Hz; CT Prescalar = 512
* Ext LCR
*    469ms if Master Clock = 32768Hz; PCLK1 = 32768Hz; CT Prescalar = 512
* Main PLL(16Mhz for example)
*   0.96ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = 512
*******************************************************************************/

#define  DUTY_VAL      (19u)
/*******************************************************************************
* HCR
*   2.56ms if Master Clock =  4MHz;   PCLK1 = 4MHz;    CT Prescalar = 512
* Ext HCR
*   1.28ms if Master Clock =  8MHz;   PCLK1 = 8MHz;    CT Prescalar = 512
*   0.64ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = 512
* LCR
*    320ms if Master Clock = 32000Hz; PCLK1 = 32000Hz; CT Prescalar = 512
* Ext LCR
*    313ms if Master Clock = 32768Hz; PCLK1 = 32768Hz; CT Prescalar = 512
* Main PLL(16Mhz for example)
*   0.64ms if Master Clock = 16MHz;   PCLK1 = 16MHz;   CT Prescalar = 512
*******************************************************************************/
/* define for ct-rt function */
#define  CT_RT_CH           CtChannel2
#define  CT_RT_MODE         RtReload
#define  CT_RT_TIMER_SIZE   RtSize16Bit
#define  CT_RT_CYCLE        (99u)

#define  GPIO_PORT 0
#define  GPIO_PIN  0

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void PwmTrigIrqHandler(void);
static void PwmUnderflowIrqHandler(void);
static void PwmDutyMatchIrqHandler(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/** \brief Interrupt count value */
static volatile uint32_t m_u32CntIntTrg = 0u;
static volatile uint32_t m_u32CntIntDuty = 0u;
static volatile uint32_t m_u32CntIntUnder = 0u;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief PWM trigger interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void PwmTrigIrqHandler(void)
{
    m_u32CntIntTrg++;
}

/**
 *******************************************************************************
 ** \brief PWM underflow interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void PwmUnderflowIrqHandler(void)
{
    m_u32CntIntUnder++;
}

/**
 *******************************************************************************
 ** \brief PWM duty match interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void PwmDutyMatchIrqHandler(void)
{
    m_u32CntIntDuty++;
}

/**
 *******************************************************************************
 ** \brief  Main function of ct-pwm sample
 **
 ** \param [in]  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_pwm_irq_en_t stcIrqEn;
    stc_pwm_irq_cb_t stcIrqCb;
    stc_ct_pwm_config_t stcPwmConfig;
    stc_ct_rt_config_t stcRtConfig;

    /* GPIO for external trigger, Need connect P00 to TIOA1_0 before code execute*/
    Gpio_InitIO(GPIO_PORT, GPIO_PIN, GpioDirOut, TRUE, FALSE);
    Gpio_SetIO(GPIO_PORT, GPIO_PIN, 1);

    /*-------- Generate a clock via rt channel 2(start)---------------------- */
    /* Clear structures */
    DDL_ZERO_STRUCT(stcRtConfig);
    /* Set Composite Timer  IO mode */
    Ct_ConfigIOMode(CT_RT_CH, CtIoMode0);

    /* Set Composite Timer  IO port */
    Gpio_SetFunc_TIOA2_0_OUT(0u);

    /* Initialize Pointer to interrupt request structure */
    stcRtConfig.pstcRtIrqEn = NULL;
    stcRtConfig.pstcRtIrqCb = NULL;
    /* Initialize Composite Timer */
    stcRtConfig.enPres = RtPres1Div4;
    stcRtConfig.enSize = CT_RT_TIMER_SIZE;
    stcRtConfig.enMode = CT_RT_MODE;
    stcRtConfig.enExtTrig = RtExtTiggerDisable;
    stcRtConfig.enOutputPolarity = RtPolarityLow;
    stcRtConfig.pstcRtIrqEn->bRtTrigIrq = 0u;
    stcRtConfig.pstcRtIrqEn->bRtUnderflowIrq = 0u;
    stcRtConfig.pstcRtIrqCb->pfnRtTrigIrqCb = NULL ;
    stcRtConfig.pstcRtIrqCb->pfnRtUnderflowIrqCb = NULL;
    stcPwmConfig.bTouchNvic = TRUE;
    Ct_Rt_Init(CT_RT_CH, &stcRtConfig);

    /* Write cycle value */
    Ct_Rt_WriteCycleVal(CT_RT_CH, CT_RT_CYCLE);
    /* Start Reload timer by software trigger */
    Ct_Rt_EnableCount(CT_RT_CH);
    Ct_Rt_EnableSwTrig(CT_RT_CH);
    /*-------- Generate a clock via rt channel 2 (end)----------------------- */

    /* Clear structures */
    DDL_ZERO_STRUCT(stcPwmConfig);
    DDL_ZERO_STRUCT(stcIrqEn);
    DDL_ZERO_STRUCT(stcIrqCb);

    /* Set Composite Timer IO port */
    Gpio_SetFunc_TIOA0_0_OUT(0u);
    Gpio_SetFunc_TIOA1_0_IN(0u);
    Gpio_SetFunc_TIOB0_0_IN(0u);

    /* Set Composite Timer I/O mode */
    Ct_ConfigIOMode(CT_CH, CtIoMode1);

    /* Initialize Pointer to interrupt request structure */
    stcPwmConfig.pstcPwmIrqEn = &stcIrqEn;
    stcPwmConfig.pstcPwmIrqCb = &stcIrqCb;
    /* Initialize PWM timer */
    stcPwmConfig.enPres = PwmPres1ExtClkFalling; /* configurate external trigger by falling edge */
    stcPwmConfig.enMode = CT_PWM_MODE;
    stcPwmConfig.enExtTrig = PwmExtTrigBoth;
    stcPwmConfig.enOutputMask = PwmOutputNormal;
    stcPwmConfig.enOutputPolarity = PwmPolarityLow;
    stcPwmConfig.enRestartEn = PwmRestartDisable;
    stcPwmConfig.pstcPwmIrqEn->bPwmTrigIrq = 1u;
    stcPwmConfig.pstcPwmIrqEn->bPwmDutyMatchIrq = 1u;
    stcPwmConfig.pstcPwmIrqEn->bPwmUnderflowIrq = 1u;
    stcPwmConfig.pstcPwmIrqCb->pfnPwmTrigIrqCb = PwmTrigIrqHandler;
    stcPwmConfig.pstcPwmIrqCb->pfnPwmDutyMatchIrqCb = PwmDutyMatchIrqHandler;
    stcPwmConfig.pstcPwmIrqCb->pfnPwmUnderflowIrqCb = PwmUnderflowIrqHandler;
    stcPwmConfig.bTouchNvic = TRUE;
    Ct_Pwm_Init(CT_CH, &stcPwmConfig);

    /* Set cycle and duty value */
    Ct_Pwm_WriteCycleVal(CT_CH, CYCLE_VAL); /* Cycle = (1+m)*PWM clock cycle = 200us */
    Ct_Pwm_WriteDutyVal(CT_CH, DUTY_VAL);   /* Duty = (1+m)*PWM clock cycle = 40us */

    Ct_Pwm_EnableCount(CT_CH); /* Enable count operatoin */

    /* Here wait for falling edge at TIOA1_0 which as the trigger for PWM */
    Gpio_SetIO(GPIO_PORT, GPIO_PIN, 0);

    /* wait for ct-pwm enter underflow interrupt 5 times */
    while(m_u32CntIntUnder < 5UL)
    {
    }

    /* Disable Interrupt */
    Ct_Pwm_DisableIrq(CT_CH, PwmDutyMatchIrq);
    Ct_Pwm_DisableIrq(CT_CH, PwmUnderflowIrq);
    Ct_Pwm_DisableIrq(CT_CH, PwmTrigIrq);
    /* Disable PWM count */
    Ct_Pwm_DisableCount(CT_CH);

    while(1)
    {

    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
