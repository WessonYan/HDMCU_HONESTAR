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
/** \file adc.h
 **
 ** A detailed description is available at
 ** @link AdcGroup Adc description @endlink
 **
 **   - 2017-8-9  1.0  Jason First version for Device Driver Library of adc.
 **
 ******************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f_m14x.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup AdcGroup Analog to digital converter(ADC)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Redefinition of ADC register structure
 *****************************************************************************/
typedef  M0P_ADC_TypeDef  stc_adc_regtypdef_t;

/**
 *******************************************************************************
 ** \brief The ADC unit.
 **
 ** \note Choose ADC unit.
 **
 ******************************************************************************/
typedef enum en_adc_unit
{
    AdcUnit0                    = 0u,   ///< The adc unit 0.
    AdcUnitMax,                         ///< The initial value.
} en_adc_unit_t;

/**
 *******************************************************************************
 ** \brief ADC conversion type.
 **
 ** \note Set ADC conversion.
 **
 ******************************************************************************/
typedef enum en_adc_conv_type
{
    AdcConvScan                 = 0u,   ///< The scan conversion.
    AdcConvPriority             = 1u,   ///< The priority conversion.
} en_adc_conv_type_t;

/**
 *******************************************************************************
 ** \brief ADC comparison type.
 **
 ** \note Set ADC comparison.
 ******************************************************************************/
typedef enum en_adc_cmp_type
{
    AdcCmpResult                = 0u,   ///< The result comparison.
    AdcCmpRange                 = 1u,   ///< The range comparison.
} en_adc_cmp_type_t;

/**
 *******************************************************************************
 ** \brief ADC interrupt request type.
 **
 ** \note Enable or disable interrupt request.Get or clear interrupt request.
 **
 ******************************************************************************/
typedef enum en_adc_irq_type
{
    AdcIrqPriorityConv          = 0u,   ///< The priority conversion interrupt request.
    AdcIrqScanConv              = 1u,   ///< The scan conversion interrupt request.
    AdcIrqPrioFifoOvf           = 2u,   ///< The fifo overflow interrupt request during priority conversion.
    AdcIrqScanFifoOvf           = 3u,   ///< The fifo overflow interrupt request during scan conversion.
    AdcIrqResultCmp             = 4u,   ///< The complete interrupt request of conversion result compare.
    AdcIrqRangeCmp              = 5u,   ///< The complete interrupt request of conversion result range compare.
} en_adc_irq_type_t;

/**
 *******************************************************************************
 ** \brief ADC status type.
 **
 ** \note Get theres adc status.
 **
 ******************************************************************************/
typedef enum en_adc_status
{
    AdcStatusScanConv           = 0u,   ///< The scan conversion is in process or not.
    AdcStatusPrioConv           = 1u,   ///< The priority conversion is in process or not.
    AdcStatusPrioL2Pending      = 2u,   ///< The level 2 priority conversion is pending or not.
    AdcStatusScanFifoFull       = 3u,   ///< The fifo is full or not during scan conversion.
    AdcStatusScanFifoEmpty      = 4u,   ///< The fifo is empty or not during scan conversion.
    AdcStatusPrioFifoFull       = 5u,   ///< The fifo is full or not during priority conversion.
    AdcStatusPrioFifoEmpty      = 6u,   ///< The fifo is empty or not during priority conversion.
    AdcStatusRangeCmpExceedThr  = 7u,   ///< The result of range comparison exceed the high threshold or not in out of range mode.
} en_adc_status_t;

/**
 *******************************************************************************
 ** \brief ADC sampling time register type.
 **
 ** \note Set time register for adc sampling channel.The default is reg0.
 **
 ******************************************************************************/
typedef enum en_adc_samp_time_reg
{
    AdcSampTimeReg0             = 0u,   ///< The adc samp time register 0.
    AdcSampTimeReg1             = 1u,   ///< The adc samp time register 1.
} en_adc_samp_time_reg_t;

/**
 *******************************************************************************
 ** \brief ADC sampling time.
 **
 ** \note Set sampling time for time register.
 **
 ******************************************************************************/
typedef enum en_adc_samp_time
{
    AdcSampTime4AdcClk          = 0u,   ///< Take 4 adc conversion clock to sample.
    AdcSampTime6AdcClk          = 1u,   ///< Take 6 adc conversion clock to sample.
    AdcSampTime8AdcClk          = 2u,   ///< Take 8 adc conversion clock to sample.
    AdcSampTime12AdcClk         = 3u,   ///< Take 12 adc conversion clock to sample.
} en_adc_samp_time_t;

/**
 *******************************************************************************
 ** \brief ADC conversion channel.
 **
 ** \note The number of adc channel.
 **
 ******************************************************************************/
typedef enum en_adc_channel
{
    AdcChn0                     = 0u,   ///< The ADC conversion channel 0.
    AdcChn1                     = 1u,   ///< The ADC conversion channel 1.
    AdcChn2                     = 2u,   ///< The ADC conversion channel 2.
    AdcChn3                     = 3u,   ///< The ADC conversion channel 3.
    AdcChn4                     = 4u,   ///< The ADC conversion channel 4.
    AdcChn5                     = 5u,   ///< The ADC conversion channel 5.
    AdcChn6                     = 6u,   ///< The ADC conversion channel 6.
    AdcChn7                     = 7u,   ///< The ADC conversion channel 7.
    AdcChn8                     = 8u,   ///< The ADC conversion channel 8.
    AdcChn9                     = 9u,   ///< The ADC conversion channel 9.
    AdcChn10                    = 10u,  ///< The ADC conversion channel 10.
    AdcChn11                    = 11u,  ///< The ADC conversion channel 11.
    AdcChn12                    = 12u,  ///< The ADC conversion channel 12.
    AdcChn13                    = 13u,  ///< The ADC conversion channel 13.
    AdcChn14                    = 14u,  ///< The ADC conversion channel 14.
    AdcChn15                    = 15u,  ///< The ADC conversion channel 15.
} en_adc_channel_t;

/**
 *******************************************************************************
 ** \brief The validity of result fifo for scan or priority conversion.
 **
 ** \note Return the validity of result fifo.
 **
 ******************************************************************************/
typedef enum en_adc_fifo_valid
{
    AdcFifoValidFalse           = 0u,   ///< The scan or priority conversion results fifo is invalid.
    AdcFifoValidTrue            = 1u,   ///< The scan or priority conversion results fifo is valid.
} en_adc_fifo_valid_t;

/**
 *******************************************************************************
 ** \brief ADC conversion trigger.
 **
 ** \note Set adc conversion trigger.The default is software trigger.
 **
 ******************************************************************************/
typedef enum en_adc_conv_trigger
{
    AdcConvTriggerSw            = 1u,   ///< The ADC conversion trigger is software.
    AdcConvTriggerTimer         = 2u,   ///< The ADC conversion trigger is timer.
    AdcConvTriggerExternal      = 4u,   ///< The ADC conversion trigger is external.
} en_adc_conv_trigger_t;

/**
 *******************************************************************************
 ** \brief ADC conversion timer trigger.
 **
 ** \note Set the timer channel for timer trigger.
 **
 ******************************************************************************/
typedef enum en_adc_timer_trigger
{
    AdcTimerTriggerNone         = 0u,   ///< No selected timer trigger.
    AdcTimerTriggerTimer4       = 1u,   ///< Start scan or prioprity conversion with timer4.
    AdcTimerTriggerCT0          = 2u,   ///< Start scan or prioprity conversion with composite timer channel 0.
    AdcTimerTriggerCT1          = 3u,   ///< Start scan or prioprity conversion with composite timer channel 1.
    AdcTimerTriggerCT2          = 4u,   ///< Start scan or prioprity conversion with composite timer channel 2.
    AdcTimerTriggerCT3          = 5u,   ///< Start scan or prioprity conversion with composite timer channel 3.
} en_adc_timer_trigger_t;

/**
 *******************************************************************************
 ** \brief ADC Sampling frequency selection.
 **
 ** \note Set the adc sampling frequency.Divide from system frequency.
 **
 ******************************************************************************/
typedef enum en_adc_samp_freq_sel
{
    SampFreqSysTDiv1            = 0u,   ///< The ADC sampling frequency for the system sample frequency, regardless of frequency*/
    SampFreqSysTDiv2            = 1u,   ///< The ADC sampling frequency for the system frequency dividing 2.
    SampFreqSysTDiv3            = 2u,   ///< The ADC sampling frequency for the system frequency dividing 3.
    SampFreqSysTDiv4            = 3u,   ///< The ADC sampling frequency for the system frequency dividing 4.
    SampFreqSysTDiv5            = 4u,   ///< The ADC sampling frequency for the system frequency dividing 5.
    SampFreqSysTDiv6            = 5u,   ///< The ADC sampling frequency for the system frequency dividing 6.
    SampFreqSysTDiv7            = 6u,   ///< The ADC sampling frequency for the system frequency dividing 7.
    SampFreqSysTDiv8            = 7u,   ///< The ADC sampling frequency for the system frequency dividing 8.
    SampFreqSysTDiv9            = 8u,   ///< The ADC sampling frequency for the system frequency dividing 9.
    SampFreqSysTDiv10           = 9u,   ///< The ADC sampling frequency for the system frequency dividing 10.
    SampFreqSysTDiv11           = 10u,  ///< The ADC sampling frequency for the system frequency dividing 11.
    SampFreqSysTDiv12           = 11u,  ///< The ADC sampling frequency for the system frequency dividing 12.
    SampFreqSysTDiv13           = 12u,  ///< The ADC sampling frequency for the system frequency dividing 13.
    SampFreqSysTDiv14           = 13u,  ///< The ADC sampling frequency for the system frequency dividing 14.
    SampFreqSysTDiv15           = 14u,  ///< The ADC sampling frequency for the system frequency dividing 15.
    SampFreqSysTDiv16           = 15u,  ///< The ADC sampling frequency for the system frequency dividing 16.
    SampFreqSysTDiv17           = 16u,  ///< The ADC sampling frequency for the system frequency dividing 17.
    SampFreqSysTDiv18           = 17u,  ///< The ADC sampling frequency for the system frequency dividing 18.
    SampFreqSysTDiv19           = 18u,  ///< The ADC sampling frequency for the system frequency dividing 19.
    SampFreqSysTDiv20           = 19u,  ///< The ADC sampling frequency for the system frequency dividing 20.
    SampFreqSysTDiv21           = 20u,  ///< The ADC sampling frequency for the system frequency dividing 21.
    SampFreqSysTDiv22           = 21u,  ///< The ADC sampling frequency for the system frequency dividing 22.
    SampFreqSysTDiv23           = 22u,  ///< The ADC sampling frequency for the system frequency dividing 23.
    SampFreqSysTDiv24           = 23u,  ///< The ADC sampling frequency for the system frequency dividing 24.
    SampFreqSysTDiv25           = 24u,  ///< The ADC sampling frequency for the system frequency dividing 25.
    SampFreqSysTDiv26           = 25u,  ///< The ADC sampling frequency for the system frequency dividing 26.
    SampFreqSysTDiv27           = 26u,  ///< The ADC sampling frequency for the system frequency dividing 27.
    SampFreqSysTDiv28           = 27u,  ///< The ADC sampling frequency for the system frequency dividing 28.
    SampFreqSysTDiv29           = 28u,  ///< The ADC sampling frequency for the system frequency dividing 29.
    SampFreqSysTDiv30           = 29u,  ///< The ADC sampling frequency for the system frequency dividing 30.
    SampFreqSysTDiv31           = 30u,  ///< The ADC sampling frequency for the system frequency dividing 31.
    SampFreqSysTDiv32           = 31u,  ///< The ADC sampling frequency for the system frequency dividing 32.
    SampFreqSysTDiv33           = 32u,  ///< The ADC sampling frequency for the system frequency dividing 33.
    SampFreqSysTDiv34           = 33u,  ///< The ADC sampling frequency for the system frequency dividing 34.
    SampFreqSysTDiv35           = 34u,  ///< The ADC sampling frequency for the system frequency dividing 35.
    SampFreqSysTDiv36           = 35u,  ///< The ADC sampling frequency for the system frequency dividing 36.
    SampFreqSysTDiv37           = 36u,  ///< The ADC sampling frequency for the system frequency dividing 37.
    SampFreqSysTDiv38           = 37u,  ///< The ADC sampling frequency for the system frequency dividing 38.
    SampFreqSysTDiv39           = 38u,  ///< The ADC sampling frequency for the system frequency dividing 39.
    SampFreqSysTDiv40           = 39u,  ///< The ADC sampling frequency for the system frequency dividing 40.
    SampFreqSysTDiv41           = 40u,  ///< The ADC sampling frequency for the system frequency dividing 41.
    SampFreqSysTDiv42           = 41u,  ///< The ADC sampling frequency for the system frequency dividing 42.
    SampFreqSysTDiv43           = 42u,  ///< The ADC sampling frequency for the system frequency dividing 43.
    SampFreqSysTDiv44           = 43u,  ///< The ADC sampling frequency for the system frequency dividing 44.
    SampFreqSysTDiv45           = 44u,  ///< The ADC sampling frequency for the system frequency dividing 45.
    SampFreqSysTDiv46           = 45u,  ///< The ADC sampling frequency for the system frequency dividing 46.
    SampFreqSysTDiv47           = 46u,  ///< The ADC sampling frequency for the system frequency dividing 47.
    SampFreqSysTDiv48           = 47u,  ///< The ADC sampling frequency for the system frequency dividing 48.
    SampFreqSysTDiv49           = 48u,  ///< The ADC sampling frequency for the system frequency dividing 49.
    SampFreqSysTDiv50           = 49u,  ///< The ADC sampling frequency for the system frequency dividing 50.
    SampFreqSysTDiv51           = 50u,  ///< The ADC sampling frequency for the system frequency dividing 51.
    SampFreqSysTDiv52           = 51u,  ///< The ADC sampling frequency for the system frequency dividing 52.
    SampFreqSysTDiv53           = 52u,  ///< The ADC sampling frequency for the system frequency dividing 53.
    SampFreqSysTDiv54           = 53u,  ///< The ADC sampling frequency for the system frequency dividing 54.
    SampFreqSysTDiv55           = 54u,  ///< The ADC sampling frequency for the system frequency dividing 55.
    SampFreqSysTDiv56           = 55u,  ///< The ADC sampling frequency for the system frequency dividing 56.
    SampFreqSysTDiv57           = 56u,  ///< The ADC sampling frequency for the system frequency dividing 57.
    SampFreqSysTDiv58           = 57u,  ///< The ADC sampling frequency for the system frequency dividing 58.
    SampFreqSysTDiv59           = 58u,  ///< The ADC sampling frequency for the system frequency dividing 59.
    SampFreqSysTDiv60           = 59u,  ///< The ADC sampling frequency for the system frequency dividing 60.
    SampFreqSysTDiv61           = 60u,  ///< The ADC sampling frequency for the system frequency dividing 61.
    SampFreqSysTDiv62           = 61u,  ///< The ADC sampling frequency for the system frequency dividing 62.
    SampFreqSysTDiv63           = 62u,  ///< The ADC sampling frequency for the system frequency dividing 63.
    SampFreqSysTDiv64           = 63u,  ///< The ADC sampling frequency for the system frequency dividing 64.
    SampFreqSysTDiv65           = 64u,  ///< The ADC sampling frequency for the system frequency dividing 65.
} en_adc_samp_freq_sel_t;

/**
 *******************************************************************************
 ** \brief Reference voltage selection.
 **
 ** \note Set the reference voltage.
 **
 ******************************************************************************/
typedef enum en_adc_ref_vol_sel
{
    AdcRefVolInternal           = 0u,   ///< The ADC reference voltage is internal voltage.
    AdcRefVolExternal           = 1u,   ///< The ADC reference voltage is external voltage.
} en_adc_ref_vol_sel_t;

/**
 *******************************************************************************
 ** \brief The alignment of the conversion result.
 **
 ** \note Set the alignment of the conversion result in fifo.
 **
 ******************************************************************************/
typedef enum en_adc_result_align_sel
{
    AdcResultAlignLeft          = 0u,   ///< The ADC conversion result is left-aliged.
    AdcResultAlignRight         = 1u,   ///< The ADC conversion result is right-aliged.
} en_adc_result_align_sel_t;

/**
 *******************************************************************************
 ** \brief Channel mode of scan conversion.
 **
 ** \note Set the channel in order of out of order for scan conversion.
 **
 ******************************************************************************/
typedef enum en_adc_scan_chn_mode
{
    AdcScanChnModeOrder        = 0u,    ///< Scan from chn0 to chn15 in order if the channel has been chosen.
    AdcScanChnModeDisorder     = 1u,    ///< Scan from schsl0 to schsl5 in order.Each schsl can be config from chn0 to chn15.
} en_adc_scan_chn_mode_t;

/**
 *******************************************************************************
 ** \brief Fifo stages of scan conversion.
 **
 ** \note Scan conversion interrupt will be generated
 **       after the conversion result has stored in the specified fifo stage.
 **
 ******************************************************************************/
typedef enum en_adc_scan_fifo_stages
{
    AdcScanFifoStages1          = 0u,   ///< Fifo stage of scan conversion is 1.
    AdcScanFifoStages2          = 1u,   ///< Fifo stage of scan conversion is 2.
    AdcScanFifoStages3          = 2u,   ///< Fifo stage of scan conversion is 3.
    AdcScanFifoStages4          = 3u,   ///< Fifo stage of scan conversion is 4.
    AdcScanFifoStages5          = 4u,   ///< Fifo stage of scan conversion is 5.
    AdcScanFifoStages6          = 5u,   ///< Fifo stage of scan conversion is 6.
    AdcScanFifoStages7          = 6u,   ///< Fifo stage of scan conversion is 7.
    AdcScanFifoStages8          = 7u,   ///< Fifo stage of scan conversion is 8.
    AdcScanFifoStages9          = 8u,   ///< Fifo stage of scan conversion is 9.
    AdcScanFifoStages10         = 9u,   ///< Fifo stage of scan conversion is 10.
    AdcScanFifoStages11         = 10u,  ///< Fifo stage of scan conversion is 11.
    AdcScanFifoStages12         = 11u,  ///< Fifo stage of scan conversion is 12.
    AdcScanFifoStages13         = 12u,  ///< Fifo stage of scan conversion is 13.
    AdcScanFifoStages14         = 13u,  ///< Fifo stage of scan conversion is 14.
    AdcScanFifoStages15         = 14u,  ///< Fifo stage of scan conversion is 15.
    AdcScanFifoStages16         = 15u,  ///< Fifo stage of scan conversion is 16.
} en_adc_scan_fifo_stages_t;

/**
 *******************************************************************************
 ** \brief The fifo stages for priority conversion.
 **
 ** \note Priority conversion interrupt will be generated
 **       after the conversion result has stored in the specified fifo stage.
 **
 ******************************************************************************/
typedef enum en_adc_priority_fifo_stages
{
    AdcPrioFifoStages1          = 0u,   ///< Fifo stage of priority conversion is 1.
    AdcPrioFifoStages2          = 1u,   ///< Fifo stage of priority conversion is 2.
    AdcPrioFifoStages3          = 2u,   ///< Fifo stage of priority conversion is 3.
    AdcPrioFifoStages4          = 3u,   ///< Fifo stage of priority conversion is 4.
} en_adc_priority_fifo_stages_t;

/**
 *******************************************************************************
 ** \brief Conditions to generate interrupt request/flag for result comparison.
 **
 ** \note Set the condition to generate interrupt request/flag.
 **
 ******************************************************************************/
typedef enum en_adc_result_cmp_condition
{
    AdcResultCmpCondLess        = 0u,   ///< Conversion result is less than compare value.
    AdcResultCmpCondGreatOrEqual = 1u,  ///< Conversion result is equal or great than compare value.

} en_adc_result_cmp_condition_t;

/**
 *******************************************************************************
 ** \brief Conditions to generate interrupt request/flag for range comparison.
 **
 ** \note Set the condition to generate interrupt request/flag.
 **
 ******************************************************************************/
typedef enum en_adc_range_cmp_condition
{
    AdcRangeCmpCondOutRange     = 0u,   ///< Conversion result is out of compare range.
    AdcRangeCmpCondInRange      = 1u,   ///< Conversion result is in compare range.
} en_adc_range_cmp_condition_t;

/**
 *******************************************************************************
 ** \brief Range comparison times.
 **
 ** \note Set the range comparison times.
 **
 ******************************************************************************/
typedef enum en_adc_range_cmp_times
{
    AdcRangeCmpTimesProhibit    = 0u,   ///< Setting prohibit.
    AdcRangeCmpTimes1           = 1u,   ///< Range comparison times 1.
    AdcRangeCmpTimes2           = 2u,   ///< Range comparison times 2.
    AdcRangeCmpTimes3           = 3u,   ///< Range comparison times 3.
    AdcRangeCmpTimes4           = 4u,   ///< Range comparison times 4.
    AdcRangeCmpTimes5           = 5u,   ///< Range comparison times 5.
    AdcRangeCmpTimes6           = 6u,   ///< Range comparison times 6.
    AdcRangeCmpTimes7           = 7u,   ///< Range comparison times 7.
} en_adc_range_cmp_times_t;

/**
 *******************************************************************************
 ** \brief Configuration structure of adc sampling time.
 **
 ** \note Configure the sampling time if needed.
 **
 ******************************************************************************/
typedef struct stc_adc_samp_time_cfg
{
    en_adc_samp_freq_sel_t          enFreqDiv;      ///< Set sampling frequency division factor.

    en_adc_samp_time_t              enTimeReg0;     ///< Set sampling clock for time register 0.

    en_adc_samp_time_t              enTimeReg1;     ///< Set sampling clock for time register 1.

    uint16_t                        u16ChnSampTime; ///< Recored time register selection for each channel.
} stc_adc_samp_time_cfg_t;

/**
 *******************************************************************************
 ** \brief Channel configuration structure of adc scan conversion.
 **
 ** \note Configure the channel for scan conversion.
 **
 ******************************************************************************/
typedef struct stc_adc_scan_chn_cfg
{
    boolean_t                       bChnInit;       ///< Record channels is already initialized by channel initialize function first.

    en_adc_scan_chn_mode_t          enScanChnMd;    ///< Record channel mode.

    uint32_t                        u32Chn;         ///< Record value of the channel to be sampled.
} stc_adc_scan_chn_cfg_t;

/**
 *******************************************************************************
 ** \brief Configuration structure of adc scan conversion.
 **
 ** \note Configure the adc scan conversion.
 **
 ******************************************************************************/
typedef struct stc_adc_scan_cfg
{
    boolean_t                       bTimerTriggerEn;///< Set whether to enable timer trigger.

    boolean_t                       bRepeat;        ///< Set whether to convert repeatly.

    en_adc_timer_trigger_t          enTimerTrigger; ///< Set timer channel if enable timer trigger.

    en_adc_scan_fifo_stages_t       enFifoStages;   ///< Set fifo stages for scan conversion.

    stc_adc_scan_chn_cfg_t          stcChnCfg;      ///< Channel configuration structure.
} stc_adc_scan_cfg_t;

/**
 *******************************************************************************
 ** \brief Channel configuration structure of adc priority conversion.
 **
 ** \note Configure the channel for priority conversion.
 **
 ******************************************************************************/
typedef struct stc_adc_prio_chn_cfg
{
    boolean_t                       bChnInit;       ///< Record channels is already initialized by channel initialize function first.

    uint32_t                        u32Chn;         ///< Record value of the channel to be sampled.
} stc_adc_prio_chn_cfg_t;

/**
 *******************************************************************************
 ** \brief Configuration structure of adc priority conversion.
 **
 ** \note Configure the adc priority conversion.
 **
 ******************************************************************************/
typedef struct stc_adc_prio_cfg
{
    boolean_t                       bTimerTriggerEn;///< Set whether to enable timer trigger.

    boolean_t                       bExtTriggerEn;  ///< Set whether to enable external trigger.

    en_adc_timer_trigger_t          enTimerTrigger; ///< Set timer channel if enable timer trigger.

    en_adc_priority_fifo_stages_t   enFifoStages;   ///< Set fifo stages for priority conversion.

    stc_adc_prio_chn_cfg_t          stcChnCfg;      ///< Channel configuration structure.
} stc_adc_prio_cfg_t;

/**
 *******************************************************************************
 ** \brief Configuration structure of adc result comparison threshold.
 **
 ** \note Configure the threshold for result comparison.
 **
 ******************************************************************************/
typedef struct stc_adc_result_cmp_thr
{
    boolean_t                       bCmpThrInit;    ///< Record threshold is already initialized by threshold initialize function first.

    uint16_t                        u16CmpThr;      ///< Record threshold.
} stc_adc_result_cmp_thr_t;

/**
 *******************************************************************************
 ** \brief Configuration structure of adc result comparison.
 **
 ** \note Configure the result comparison.
 **
 ******************************************************************************/
typedef struct stc_adc_result_cmp_cfg
{
    boolean_t                       bCmpAllChn;     ///< Set whether to compare all channels.

    en_adc_channel_t                enCmpChn;       ///< Set the result comparison channel.

    en_adc_result_cmp_condition_t   enCmpCon;       ///< Set the result comparison condition.

    stc_adc_result_cmp_thr_t        stcCmpThrCfg;   ///< Threshold configuration structure.
} stc_adc_result_cmp_cfg_t;

/**
 *******************************************************************************
 ** \brief Configuration structure of adc range comparison threshold.
 **
 ** \note Configure the threshold for range comparison.
 **
 ******************************************************************************/
typedef struct stc_adc_range_cmp_thr
{
    boolean_t                       bCmpThrInit;    ///< Record threshold is already initialized by threshold initialize function first.

    uint16_t                        u16CmpThrL;     ///< Record lower threshold.
    uint16_t                        u16CmpThrH;     ///< Record higher threshold.
} stc_adc_range_cmp_thr_t;

/**
 *******************************************************************************
 ** \brief Configuration structure of adc range comparison.
 **
 ** \note Configure the range comparison.
 **
 ******************************************************************************/
typedef struct stc_adc_range_cmp_cfg
{
    boolean_t                       bCmpAllChn;     ///< Set whether to compare all channels.

    en_adc_channel_t                enCmpChn;       ///< Set the range comparison channel.

    en_adc_range_cmp_times_t        enCmpTimes;     ///< Set the range comparison times.

    en_adc_range_cmp_condition_t    enCmpCon;       ///< Set the range comparison condition.

    stc_adc_range_cmp_thr_t         stcCmpThrCfg;   ///< Threshold configuration structure.
} stc_adc_range_cmp_cfg_t;

/**
 *******************************************************************************
 ** \brief Adc interrupt service routine structure.
 **
 ** \note Hook the interrupt service routine.
 **
 ******************************************************************************/
typedef struct stc_adc_isr
{
    func_ptr_t                      pfnIsrPrio;         ///< Interrupt service routine of priority conversion.

    func_ptr_t                      pfnIsrScan;         ///< Interrupt service routine of scan conversion.

    func_ptr_t                      pfnIsrPrioFifoOvf;  ///< Interrupt service routine of fifo overflow during priority conversion.

    func_ptr_t                      pfnIsrScanFifoOvf;  ///< Interrupt service routine of fifo overflow during scan conversion.

    func_ptr_t                      pfnIsrResult;       ///< Interrupt service routine of result comparison.

    func_ptr_t                      pfnIsrRange;        ///< Interrupt service routine of range comparison.
} stc_adc_isr_t;

/**
 *******************************************************************************
 ** \brief Adc interrupt request structure.
 **
 ** \note Enable or disable interrupt request structure.Default is disable.
 **
 ******************************************************************************/
typedef struct stc_adc_intrpt
{
    boolean_t                       bIrqEnPrio;         ///< Set whether to enable interrupt request for priority conversion.

    boolean_t                       bIrqEnScan;         ///< Set whether to enable interrupt request for scan conversion.

    boolean_t                       bIrqEnPrioFifoOvf;  ///< Set whether to enable interrupt request for fifo overflow during priority conversion.

    boolean_t                       bIrqEnScanFifoOvf;  ///< Set whether to enable interrupt request for fifo overflow during scan conversion.

    boolean_t                       bIrqEnResult;       ///< Set whether to enable interrupt request for result comparison.

    boolean_t                       bIrqEnRange;        ///< Set whether to enable interrupt request for range comparison.

    stc_adc_isr_t                   stcIsr;             ///< Interrupt service routine configuration structure.
} stc_adc_intrpt_t;

/**
 *******************************************************************************
 ** \brief Configuration structure of adc module.
 **
 ** \note Configure the these function for adc module if needed.
 **
 ******************************************************************************/
typedef struct stc_adc_cfg
{
    en_adc_ref_vol_sel_t            enAdcRefVol;        ///< Set the reference voltage.

    en_adc_result_align_sel_t       enAdcResultAlign;   ///< Set the result alignment for adc conversion.

    stc_adc_samp_time_cfg_t*        pstcAdcSampTimeCfg; ///< Configure the sampling time if needed.

    stc_adc_scan_cfg_t*             pstcAdcScanCfg;     ///< Configure the scan conversion if needed.

    stc_adc_prio_cfg_t*             pstcAdcPrioCfg;     ///< Configure the priority conversion if needed.

    stc_adc_result_cmp_cfg_t*       pstcAdcResultCmpCfg;///< Configure the result comparison if needed.

    stc_adc_range_cmp_cfg_t*        pstcAdcRangeCmpCfg; ///< Configure the range comparison if needed.

    stc_adc_intrpt_t*               pstcAdcIntrpt;      ///< Configure the interrupt if needed.
} stc_adc_cfg_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#ifndef ENABLE
    #define ENABLE TRUE                 ///< Enable definition. */
#endif

#ifndef DISABLE
    #define DISABLE FALSE               ///< Disable definition. */
#endif

#define ADC_READY_TIMEOUT_CNT   10000u  ///< Time-out counter value for read the adc ready bit. */

#define DEFAULT_CHN             0x1Fu   ///< The default channel value,it's a invalid channel value. */

/**
 *******************************************************************************
 ** \name The channel(s) to sample in order for scan conversion.
 **
 ** \brief Sample channel is \ref AdcChn10.
 **        Eg:chn = CHSL(AdcChn10);
 **
 ** \brief Sample channels are \ref AdcChn0, \ref AdcChn1, \ref AdcChn9.
 **        Eg:chn = CHSL(AdcChn0) | CHSL(AdcChn1) | CHSL(AdcChn9);
 **
 ******************************************************************************/
//@{
/*! Sample channel \a chn in order for scan conversion. */
#define CHSL(chn)       (1u << chn)
//@} // CHSL(chn)

/**
 *******************************************************************************
 ** \name The channel(s) to sample out of order for scan conversion.
 **
 ** \brief Sample channel is \ref AdcChn10.
 **        Eg:chn = SCHSL0(AdcChn10);
 **
 ** \brief Sample channel are \ref AdcChn10, \ref AdcChn9, \ref AdcChn8.
 **        Eg:chn = SCHSL0(AdcChn_10) | SCHSL1(AdcChn_9) | SCHSL2(AdcChn_8);
 **
 ******************************************************************************/
//@{
/*! The first chnnel \a chn to be sampled for scan conversion. */
#define SCHSL0(chn)     (chn << 0u)
/*! The second chnnel \a chn to be sampled for scan conversion. */
#define SCHSL1(chn)     (chn << 5u)
/*! The third chnnel \a chn to be sampled for scan conversion. */
#define SCHSL2(chn)     (chn << 10u)
/*! The fourth chnnel \a chn to be sampled for scan conversion. */
#define SCHSL3(chn)     (chn << 15u)
/*! The fifth chnnel \a chn to be sampled for scan conversion. */
#define SCHSL4(chn)     (chn << 20u)
/*! The sixth chnnel \a chn to be sampled for scan conversion. */
#define SCHSL5(chn)     (chn << 25u)
//@} // SCHSLx(chn)

/**
 *******************************************************************************
 ** \name The channel(s) to sample for priority conversion.
 **
 ** \brief Sample level1 channel \ref AdcChn10 and level2 channel \ref AdcChn9.
 **        Eg:chn = L1CH(AdcChn10) | L2CH(AdcChn9);
 **
 ******************************************************************************/
//@{
/*! Sample channel \a chn in level 1 for priority conversion. */
#define L1CH(chn)       (chn << 0u)
/*! Sample channel \a chn in level 2 for priority conversion. */
#define L2CH(chn)       (chn << 8u)
//@} // LxCH(chn)

/**
 *******************************************************************************
 ** \name The sample time register reg1 for each channel.Default is reg0.
 **
 ** \brief Choose time register reg1 for channel \ref AdcChn10.
 **        Eg:ST1CHN(AdcChn10);
 **
 ** \brief Choose time register reg1 for channel \ref AdcChn9 and \ref AdcChn10.
 **        Eg:ST1CHN(AdcChn9) | ST1CHN(AdcChn10);
 **
 ******************************************************************************/
//@{
/*! Choose time register reg1 for channel \a chn. */
#define ST1CHN(chn)     (1u << chn)
//@} // ST1CHN(chn)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/

/* Init and de-init. */
en_result_t Adc_Init(en_adc_unit_t enAdcUnit, stc_adc_cfg_t* pstcAdcInitCfg);

en_result_t Adc_DeInit(en_adc_unit_t enAdcUnit);


/* Configure sample time for each channel. */
en_result_t Adc_ChnSampleTimeCfg(stc_adc_samp_time_cfg_t* pstcAdcSampTimeCfg, uint16_t u16ChnSampleTime);


/* Configure scan or priority conversion channel before init. */
en_result_t Adc_PrioConvChnCfg(stc_adc_prio_cfg_t* pstcAdcPrioCfg, uint32_t u32ChnL1, uint32_t u32ChnL2);

en_result_t Adc_ScanConvChnCfg(stc_adc_scan_cfg_t* pstcAdcScanCfg, en_adc_scan_chn_mode_t enMode, uint32_t u32Chn);


/* Configure result or range comparison threshold before init. */
en_result_t Adc_ResultCmpThrCfg(stc_adc_result_cmp_cfg_t* pstcAdcResultCmpCfg, uint16_t u16Thr);

en_result_t Adc_RangeCmpThrCfg(stc_adc_range_cmp_cfg_t* pstcAdcRangeCmpCfg, uint16_t u16ThrL, uint16_t u16ThrH);


/* Enable(wait ready) and disable adc. */
en_result_t Adc_Enable(en_adc_unit_t enAdcUnit);

en_result_t Adc_Disable(en_adc_unit_t enAdcUnit);

/* Enable comparison. */
en_result_t Adc_CmpEnable(en_adc_unit_t enAdcUnit, en_adc_cmp_type_t enCmpType);

/* Disable comparison. */
en_result_t Adc_CmpDisable(en_adc_unit_t enAdcUnit, en_adc_cmp_type_t enCmpType);


/* Enable interrupr request. */
en_result_t Adc_IrqEnable(en_adc_unit_t enAdcUnit, en_adc_irq_type_t enIrqType);

/* Disable interrupr request. */
en_result_t Adc_IrqDisable(en_adc_unit_t enAdcUnit, en_adc_irq_type_t enIrqType);


/* Get and clear interrupr request flag. */
boolean_t Adc_GetIrqFlag(en_adc_unit_t enAdcUnit, en_adc_irq_type_t enIrqType);

void Adc_ClearIrqFlag(en_adc_unit_t enAdcUnit, en_adc_irq_type_t enIrqType);


/* Start and stop scan or priority conversion. */
en_result_t Adc_ConvStart(en_adc_unit_t enAdcUnit, en_adc_conv_type_t enConvType);

en_result_t Adc_ConvStop(en_adc_unit_t enAdcUnit);


/* Set scan repeat or single. */
en_result_t Adc_ScanRepeat(en_adc_unit_t enAdcUnit, boolean_t bRepeat);


/* Read fifo. */
uint32_t Adc_ReadFifo(en_adc_unit_t enAdcUnit, en_adc_conv_type_t enConvType);

/* Check fifo data is valid or not. */
en_adc_fifo_valid_t Adc_CheckConvDataValid(uint32_t u32FifoData);

/* Get trigger type after check fifo data is valid. */
en_adc_conv_trigger_t Adc_GetConvTrigger(uint32_t u32FifoData, en_adc_conv_type_t enConvType);

/* Get channel number after check fifo data is valid. */
uint8_t Adc_GetConvChannel(uint32_t u32FifoData);

/* Get result after check fifo data is valid. */
uint16_t Adc_GetConvResult(en_adc_unit_t enAdcUnit, uint32_t u32FifoData);

/* Clear fifo. */
void Adc_ClearFifo(en_adc_unit_t enAdcUnit, en_adc_conv_type_t enConvType);


/* Get adc status. */
boolean_t Adc_GetStatus(en_adc_unit_t enAdcUnit, en_adc_status_t enStatus);


/* Reselect the sample time register by channel. */
en_result_t Adc_ChnSampleTimeResel(en_adc_unit_t enAdcUnit, en_adc_samp_time_reg_t enAdcSampTimeReg, uint16_t u16ReselChn);

//@} // AdcGroup
#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
