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
 ** \brief The example for adc priority conversion demonstration.
 **        Get more details from readme text.
 **
 **   - 2017-8-9  1.0  Jason First version for Device Driver Library of adc.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "adc.h"
#include "ct.h"
#include "gpio.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

#define T1_PORT             7
#define T1_PIN              0

#define T2_PORT             6
#define T2_PIN              0

#define ADC_UNIT            AdcUnit0
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
static void RtTrigIrqHandler(void)
{
    ;
}
/**
 *******************************************************************************
 ** \brief The ADC conversion GPIO initialization configuration
 **
 ** \return None.
 **
 ******************************************************************************/
void Adc_GpioConfig(void)
{
    /* Set the I/O to analog input port. */
    Gpio_SetAnalog( T1_PORT, T1_PIN, TRUE);
    Gpio_SetAnalog( T2_PORT, T2_PIN, TRUE);
}

/**
 *******************************************************************************
 ** \brief The ADC conversion initialization configuration
 **
 **
 ** \return None.
 **
 ** This sample
 **
 ******************************************************************************/
void Adc_InitConvConfig(void)
{
    stc_adc_cfg_t stcAdcInitCfg;
    stc_adc_scan_cfg_t stcAdcScanCfg;
    stc_adc_prio_cfg_t stcAdcPrioCfg;
    stc_adc_samp_time_cfg_t stcAdcSampTimeCfg;

    DDL_ZERO_STRUCT(stcAdcInitCfg);
    DDL_ZERO_STRUCT(stcAdcScanCfg);
    DDL_ZERO_STRUCT(stcAdcPrioCfg);
    DDL_ZERO_STRUCT(stcAdcSampTimeCfg);

    /* Configure priority conversion. */
    stcAdcPrioCfg.bExtTriggerEn = FALSE;
    stcAdcPrioCfg.bTimerTriggerEn = TRUE;
    stcAdcPrioCfg.enTimerTrigger = AdcTimerTriggerCT0;
    stcAdcPrioCfg.enFifoStages = AdcPrioFifoStages1;
    Adc_PrioConvChnCfg(&stcAdcPrioCfg, DEFAULT_CHN, AdcChn10);

    /* Configure scan conversion. */
    stcAdcScanCfg.bRepeat = TRUE;
    stcAdcScanCfg.bTimerTriggerEn = FALSE;
    stcAdcScanCfg.enFifoStages = AdcScanFifoStages1;
    Adc_ScanConvChnCfg(&stcAdcScanCfg, AdcScanChnModeOrder, CHSL(AdcChn8));

    /* Sampling time configuration. */
    stcAdcSampTimeCfg.enFreqDiv = SampFreqSysTDiv16;
    stcAdcSampTimeCfg.enTimeReg0 = AdcSampTime6AdcClk;
    stcAdcSampTimeCfg.enTimeReg1 = AdcSampTime12AdcClk;
    Adc_ChnSampleTimeCfg(&stcAdcSampTimeCfg, ST1CHN(AdcChn10));

    /* Common configuration. */
    stcAdcInitCfg.enAdcRefVol = AdcRefVolInternal;
    stcAdcInitCfg.enAdcResultAlign = AdcResultAlignLeft;
    stcAdcInitCfg.pstcAdcSampTimeCfg = &stcAdcSampTimeCfg;
    stcAdcInitCfg.pstcAdcPrioCfg = &stcAdcPrioCfg;
    stcAdcInitCfg.pstcAdcScanCfg = &stcAdcScanCfg;

    Adc_Init(ADC_UNIT, &stcAdcInitCfg);

    Adc_Enable(ADC_UNIT);

    return;
}

/**
 *******************************************************************************
 ** \brief The CT initialization configuration
 **
 ** \return None.
 **
 ******************************************************************************/
void Ct_InitRtConfig(void)
{
    stc_rt_irq_en_t    stcIrqEn;
    stc_rt_irq_cb_t    stcIrqCb;
    stc_ct_rt_config_t stcRtConfig;

    DDL_ZERO_STRUCT(stcRtConfig);

    stcRtConfig.pstcRtIrqEn = &stcIrqEn;
    stcRtConfig.pstcRtIrqCb = &stcIrqCb;

    stcRtConfig.enPres = RtPres1Div2048;
    stcRtConfig.enSize = RtSize16Bit;
    stcRtConfig.enMode = RtOneshot;
    stcRtConfig.enExtTrig = RtExtTiggerDisable;
    stcRtConfig.enOutputPolarity = RtPolarityLow;
    stcRtConfig.pstcRtIrqEn->bRtTrigIrq = 1;
    stcRtConfig.pstcRtIrqCb->pfnRtTrigIrqCb = RtTrigIrqHandler ;
    stcRtConfig.bTouchNvic = TRUE;
    Ct_Rt_Init(CtChannel0, &stcRtConfig);

    Ct_Rt_WriteCycleVal(CtChannel0, 99u);

    Ct_Rt_EnableCount(CtChannel0);
}
/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \return int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    uint32_t u32Fifo = 0;
    uint8_t __attribute__((unused)) u8chn = 0;
    uint8_t __attribute__((unused)) u8Trigger = 0;
    uint16_t __attribute__((unused)) u16AdcValue = 0;

    /* GPIO configuration. */
    Adc_GpioConfig();
    /* The ADC conversion initialization configuration. */
    Adc_InitConvConfig();

    Ct_InitRtConfig();

    /* Start priority conversion. */
    Adc_ConvStart(ADC_UNIT, AdcConvScan);

    Ct_Rt_EnableSwTrig(CtChannel0);

    while (1)
    {
        if(TRUE == Adc_GetIrqFlag(ADC_UNIT, AdcIrqPriorityConv))
        {
            Adc_ClearIrqFlag(ADC_UNIT, AdcIrqPriorityConv);

            u32Fifo = Adc_ReadFifo(ADC_UNIT, AdcConvPriority);

            if(AdcFifoValidTrue == Adc_CheckConvDataValid(u32Fifo))
            {
                u8Trigger = Adc_GetConvTrigger(u32Fifo, AdcConvPriority);
                u8chn = Adc_GetConvChannel(u32Fifo);
                u16AdcValue = Adc_GetConvResult(ADC_UNIT, u32Fifo);
            }
        }
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
