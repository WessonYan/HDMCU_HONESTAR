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
 ** \brief The example for adc scan conversion demonstration.
 **        Get more details from readme text.
 **
 **   - 2017-8-9  1.0  Jason First version for Device Driver Library of adc.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "adc.h"
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

/**
 *******************************************************************************
 ** \brief The ADC conversion GPIO initialization configuration
 **
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
 ******************************************************************************
 ** \brief The ADC conversion initialization configuration
 **
 ** \return None.
 **
 ******************************************************************************/
void Adc_InitConvConfig(void)
{
    stc_adc_cfg_t stcAdcInitCfg;
    stc_adc_scan_cfg_t stcAdcScanCfg;
    stc_adc_samp_time_cfg_t stcAdcSampTimeCfg;

    DDL_ZERO_STRUCT(stcAdcInitCfg);
    DDL_ZERO_STRUCT(stcAdcScanCfg);
    DDL_ZERO_STRUCT(stcAdcSampTimeCfg);

    /* Configure scan conversion. */
    stcAdcScanCfg.bRepeat = TRUE;
    stcAdcScanCfg.bTimerTriggerEn = FALSE;
    stcAdcScanCfg.enFifoStages = AdcScanFifoStages2;
    Adc_ScanConvChnCfg(&stcAdcScanCfg, AdcScanChnModeOrder, CHSL(AdcChn8) | CHSL(AdcChn10));

    /* Sampling time configuration. */
    stcAdcSampTimeCfg.enFreqDiv = SampFreqSysTDiv8;
    stcAdcSampTimeCfg.enTimeReg0 = AdcSampTime6AdcClk;
    stcAdcSampTimeCfg.enTimeReg1 = AdcSampTime6AdcClk;
    Adc_ChnSampleTimeCfg(&stcAdcSampTimeCfg, ST1CHN(AdcChn10));

    /* Common configuration. */
    stcAdcInitCfg.enAdcRefVol = AdcRefVolInternal;
    stcAdcInitCfg.enAdcResultAlign = AdcResultAlignLeft;
    stcAdcInitCfg.pstcAdcSampTimeCfg = &stcAdcSampTimeCfg;
    stcAdcInitCfg.pstcAdcScanCfg = &stcAdcScanCfg;

    Adc_Init(ADC_UNIT, &stcAdcInitCfg);

    Adc_Enable(ADC_UNIT);

    return;
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
    uint16_t __attribute__((unused)) u16AdcValue = 0;

    /* GPIO configuration. */
    Adc_GpioConfig();
    /* The ADC conversion initialization configuration. */
    Adc_InitConvConfig();

    /* Start scan conversion. */
    Adc_ConvStart(ADC_UNIT, AdcConvScan);

    while (1)
    {
        u32Fifo = Adc_ReadFifo(ADC_UNIT, AdcConvScan);

        if(AdcFifoValidTrue == Adc_CheckConvDataValid(u32Fifo))
        {
            u8chn = Adc_GetConvChannel(u32Fifo);
            u16AdcValue = Adc_GetConvResult(ADC_UNIT, u32Fifo);
        }
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
