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
/** \file adc.c
 **
 ** A detailed description is available at
 ** @link AdcGroup Adc description @endlink
 **
 **   - 2017-8-9  1.0  Jason First version for Device Driver Library of adc.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "adc.h"

/**
 *******************************************************************************
 ** \addtogroup AdcGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief ADC unit status
 ******************************************************************************/
typedef enum en_adc_unit_status
{
    AdcUnitInitialized          = 0,    ///< The adc unit has been initialized.
    AdcUnitUninitialized        = 1,    ///< The adc unit hasn't been initialized.
} en_adc_unit_status_t;

/**
 *******************************************************************************
 ** \brief ADC instance data typedef
 ******************************************************************************/
typedef struct stc_adc_instance_data
{
    en_adc_unit_t               enAdcUnit;          ///< Adc unit.

    en_adc_unit_status_t        enAdcUnitStatus;    ///< Adc unit status.

    stc_adc_regtypdef_t         *pstcAdcRegInst;    ///< Pointer to adc registers.

    stc_adc_isr_t               stcAdcIsr;          ///< The interrupt service routine.
} stc_adc_instance_data_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \name The macros related to sampling time configuration
 **       \ref stc_adc_samp_time_cfg_t
 ******************************************************************************/
//@{
/*! Sampling frequency division factor. */
#define SAMP_FREQ_DIV(timeCfg)          (timeCfg->enFreqDiv)
/*! Sampling clock for time register 0. */
#define SAMP_TIME0(timeCfg)             (timeCfg->enTimeReg0)
/*! Sampling clock for time register 1. */
#define SAMP_TIME1(timeCfg)             (timeCfg->enTimeReg1)
/*! Time register selection for each channel. */
#define CHN_SAMP_TIME(timeCfg)          (timeCfg->u16ChnSampTime)
//@} // stc_adc_samp_time_cfg_t

/**
 *******************************************************************************
 ** \name The macros related to conversion configuration
 **       \ref stc_adc_scan_cfg_t \ref stc_adc_prio_cfg_t
 ******************************************************************************/
//@{
#define TIMER_TRIGGER_EN                1u  ///< Enable timer trigger for scan or priority conversion.
/*! Enable timer trigger for scan or priority conversion.Default is disable.*/
#define TIMER_TRIGGER(convCfg)          (convCfg->bTimerTriggerEn)
/*! Timer channel for timer trigger*/
#define TIMER_TRIGGER_SEL(convCfg)      (convCfg->enTimerTrigger)

#define EXT_TRIGGER_EN                  1u  ///< Enable timer trigger for priority conversion.
/*! Enable external trigger for priority conversion.Default is disable. */
#define EXT_TRIGGER(adc)                (adc->pstcAdcPrioCfg->bExtTriggerEn)

/*! Convert single or repeated for scan conversion. */
#define SCAN_RPT(convCfg)               (convCfg->bRepeat)

#define CONV_CHN_INIT(convCfg)          (convCfg->stcChnCfg.bChnInit)
/*! Initilize the channel. */
#define CONV_CHN(convCfg)               (convCfg->stcChnCfg.u32Chn)

#define FIFO_CLEAR                      1u  ///< Clear fifo.
/*! Fifo stages of scan or priority. */
#define FIFO_STAGES(convCfg)            (convCfg->enFifoStages)
//@} // stc_adc_scan_cfg_t  stc_adc_prio_cfg_t

/**
 *******************************************************************************
 ** \name The macros related to result and range comparison configuration.
 **       \ref stc_adc_result_cmp_cfg_t \ref stc_adc_range_cmp_thr_t
 ******************************************************************************/
//@{
#define CMP_THR_INIT(cmpCfg)            (cmpCfg->stcCmpThrCfg.bCmpThrInit)
/*! Whethear compare all the channel. */
#define CMP_ALL_CHN(cmpCfg)             (cmpCfg->bCmpAllChn)
/*! The specified channel to be sampled. */
#define CMP_CHN(cmpCfg)                 (cmpCfg->enCmpChn)
/*! Comparison condition. */
#define CMP_CON(cmpCfg)                 (cmpCfg->enCmpCon)
/*! Threshold of result comparison. */
#define CMP_THR_RESULT(cmpCfg)          (cmpCfg->stcCmpThrCfg.u16CmpThr)
/*! Threshold of range comparison. */
#define CMP_THR_RANGE(cmpCfg, hl)       (cmpCfg->stcCmpThrCfg.u16CmpThr##hl)
/*! Times of range comparison. */
#define CMP_TIMES(cmpCfg)               (cmpCfg->enCmpTimes)
//@}

/**
 *******************************************************************************
 ** \name The macros related to interrupt configuration.
 **       \ref stc_adc_intrpt_t
 ******************************************************************************/
//@{
/*! Enable interrupt request.Default is disable. */
#define IRQ_EN(adcCfg, type)            (adcCfg->pstcAdcIntrpt->bIrqEn##type)
/*! Interrupt service routine.Default is null. */
#define ISR_PFN(adcCfg, type)           (adcCfg->pstcAdcIntrpt->stcIsr.pfnIsr##type)
//@}

/*! Interrupt service routine for the specified adc unit. */
#define ADC_ISR_PFN(inst, type)         (inst->stcAdcIsr.pfnIsr##type)
/*! Call the interrupt serice routine. */
#define ADC_ISR_ACTION(inst, type)      (inst->stcAdcIsr.pfnIsr##type())

/*! Parameter validity check for adc unit \a x. */
#define IS_VALID_UNIT(x)                \
(   AdcUnit0               <= (x) &&    \
    AdcUnitMax             >  (x)  )

/*! Parameter validity check for adc reference voltage \a x. */
#define IS_VALID_REFVOL(x)              \
(   AdcRefVolInternal      == (x) ||    \
    AdcRefVolExternal      == (x)  )

/*! Parameter validity check for adc conversion result alignment \a x. */
#define IS_VALID_ALIGNMENT(x)           \
(   AdcResultAlignLeft     == (x) ||    \
    AdcResultAlignRight    == (x) )

/*! Parameter validity check for adc sampling frequency division factor \a x. */
#define IS_VALID_SAMPFREQ(x)            \
(   SampFreqSysTDiv1        <= (x) &&   \
    SampFreqSysTDiv65       >= (x) )

/*! Parameter validity check for adc sampling clock selection \a x. */
#define IS_VALID_SAMPCLK(x)             \
(   AdcSampTime4AdcClk     <= (x) &&    \
    AdcSampTime12AdcClk    >= (x) )

/*! Parameter validity check for adc sampling time register selection \a x. */
#define IS_VALID_SAMPTIMEREG(x)         \
(   AdcSampTimeReg0        == (x) ||    \
    AdcSampTimeReg1        == (x) )

/*! Parameter validity check for adc sampling time \a x. */
#define IS_VALID_CHNSAMPTIME(x)         (   x <= 0xFFFF )

/*! Parameter validity check for adc timer trigger \a x. */
#define IS_VALID_TIMER_TRIGGER(x)       \
(   AdcTimerTriggerTimer4  <= (x) &&    \
    AdcTimerTriggerCT3     >= (x) )

/*! Parameter validity check for adc conversion type \a x. */
#define IS_VALID_CONV(x)                \
(   AdcConvScan            == (x) ||    \
    AdcConvPriority        == (x) )

/*! Parameter validity check for adc scan conversion fifo depth \a x. */
#define IS_VALID_SCAN_FIFOSTG(x)        \
(   AdcScanFifoStages1     <= (x) &&    \
    AdcScanFifoStages16    >= (x) )

/*! Parameter validity check for adc priority conversion fifo depth \a x. */
#define IS_VALID_PRIO_FIFOSTG(x)        \
(   AdcPrioFifoStages1     <= (x) &&    \
    AdcPrioFifoStages4     >= (x) )

/*! Parameter validity check for adc scan conversion channel mode \a x. */
#define IS_VALID_SCAN_CHNMODE(x)        \
(   AdcScanChnModeOrder    == (x) ||    \
    AdcScanChnModeDisorder == (x) )

/*! Parameter validity check for adc scan conversion channel \a x in channel mode 0. */
#define IS_VALID_SCAN_CHN_MODE0(x)      (   x <= 0xFFFF )

/*! Parameter validity check for adc scan conversion channel \a x in channel mode 1. */
#define IS_VALID_SCAN_CHN_MODE1(x)      (   x <= (0xFFFFFFFF >> 2) )

/*! Parameter validity check for adc comparison type \a x. */
#define IS_VALID_CMP(x)                 \
(   AdcCmpResult           == (x) ||    \
    AdcCmpRange            == (x) )

/*! Parameter validity check for adc result comparison condition \a x. */
#define IS_VALID_RESULT_CMP_CON(x)      \
(   AdcResultCmpCondLess   == (x) ||    \
    AdcResultCmpCondGreatOrEqual   == (x) )

/*! Parameter validity check for adc comparison threshold \a x. */
#define IS_VALID_THR(x)                 (   x <= 0xFFF )

/*! Parameter validity check for adc range comparison condition \a x. */
#define IS_VALID_RANGE_CMP_CON(x)       \
(   AdcRangeCmpCondOutRange    == (x) ||\
    AdcRangeCmpCondInRange     == (x) )

/*! Parameter validity check for adc range comparison times \a x. */
#define IS_VALID_RANGE_CMP_TIMES(x)     \
(   AdcRangeCmpTimes1      <= (x) &&    \
    AdcRangeCmpTimes7      >= (x) )

/*! Parameter validity check for adc sampling channel \a x. */
#define IS_VALID_CHN(x)                 \
(   AdcChn0                <= (x) &&    \
    AdcChn15               >= (x) )

/*! Parameter validity check for adc interrupt request \a x. */
#define IS_VALID_IRQ(x)                 \
(   AdcIrqPriorityConv     <= (x) &&    \
    AdcIrqRangeCmp         >= (x) )

/*! Parameter validity check for adc status \a x. */
#define IS_VALID_STATUS(x)              \
(   AdcStatusScanConv      <= (x) &&    \
    AdcStatusPrioFifoEmpty >= (x) )

/*! Enable interrupt register \a ref and hook isr callback \a type. */
#define ADC_INTRPT_INIT(adc, inst, reg, type)                                   \
do{                                                                             \
    if(ENABLE == IRQ_EN(adc, type)){                                            \
        ASSERT(NULL != ISR_PFN(adc, type));                                     \
        ADC_ISR_PFN(inst, type) = ISR_PFN(adc, type);                           \
        reg = ENABLE;                                                           \
    }                                                                           \
}while(0)

/*! Excute the isr callback \a type if the irq flag in \a reg is true. */
#define ADC_INTRPT_ACTION(inst, reg, type)                                      \
do{                                                                             \
    if(TRUE == reg){                                                            \
        reg = FALSE;                                                            \
        if(NULL != ADC_ISR_PFN(inst, type)){                                    \
            ADC_ISR_ACTION(inst, type);                                         \
        }                                                                       \
    }                                                                           \
}while(0)

/*! Check whether need to enable nvic or not. */
#define IS_ENABLE_NVIC(inst)                                                    \
(   (ADC_ISR_PFN(inst, Prio) != NULL) ||                                        \
    (ADC_ISR_PFN(inst, Scan) != NULL ) ||                                       \
    (ADC_ISR_PFN(inst, PrioFifoOvf) != NULL) ||                                 \
    (ADC_ISR_PFN(inst, ScanFifoOvf) != NULL ) ||                                \
    (ADC_ISR_PFN(inst, Result) != NULL ) ||                                     \
    (ADC_ISR_PFN(inst, Range) != NULL ))

/*! Check whether the adc instance has been initialized or not for debug mode. */
#define DEBUG_CHECK_INST(inst)                                                  \
do{                                                                             \
    ASSERT(NULL != inst);                                                       \
    ASSERT(AdcUnitUninitialized != inst->enAdcUnitStatus);                      \
    ASSERT(NULL != inst->pstcAdcRegInst);                                       \
}while(0)

#define FIFO_MASK_CHN                   0x1F        ///< Channel mask.
#define FIFO_MASK_VALID                 0x1         ///< Validity mask.
#define FIFO_MASK_SCAN_TRIGGER          0x300       ///< Scan trigger mask.
#define FIFO_MASK_PRIO_TRIGGER          0x700       ///< Prioriy trigger mask.
#define FIFO_MASK_DATA                  0xFFF       ///< Conversion result mask.


/*! Get conversion result. */
#define FIFO_DATA(fifo, align)          ((AdcResultAlignLeft == align ? (fifo >> 20u) : (fifo >> 16u)) & FIFO_MASK_DATA)
/*! Get the validation. */
#define FIFO_VALID(fifo)                ((fifo >> 12u) & FIFO_MASK_VALID)
/*! Get trigger. */
#define FIFO_TRIGGER(fifo, convType)    ((AdcConvScan == convType ? (fifo & FIFO_MASK_SCAN_TRIGGER) : (fifo & FIFO_MASK_PRIO_TRIGGER)) >> 8)

/*! Threshold \a thr. */
#define FORMAT_THR(thr)                 (((thr & 0xFFFu) >> 2u) << 6u)

/*! Choose sampling time register \a reg for channel \a chn.
*/
#define CHNSTSEL(chn, reg, x)           ((reg == AdcSampTimeReg0) ? (x &= (~(0x1u << chn))) : (x |= (0x1u << chn)) )
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/* Look-up table for all ADC instances. */
static stc_adc_instance_data_t m_astcAdcInstLut[] =
{
    {
        AdcUnit0,
        AdcUnitUninitialized,
        M0P_ADC,
        {NULL, NULL, NULL, NULL, NULL,NULL},
    },

};
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Return the Adc instance data by unit
 **
 ** \param [in] enAdcUnit               Unit of adc
 **
 ** \retval pstcAdcInst                 Pointer to adc instance data
 ** \retval NULL                        If instance is not enabled
 **
 ******************************************************************************/
static stc_adc_instance_data_t* GetAdcInst(en_adc_unit_t enAdcUnit)
{
    uint8_t i = 0;
    stc_adc_instance_data_t* pstcAdcInst = NULL;

    ASSERT(IS_VALID_UNIT(enAdcUnit));

    for (i = 0u; i < ARRAY_SZ(m_astcAdcInstLut); i++)
    {
        if (enAdcUnit == m_astcAdcInstLut[i].enAdcUnit)
        {
            pstcAdcInst = &m_astcAdcInstLut[i];
            break;
        }
    }

    /* If there is no specified instance data, check the look up table. */
    ASSERT(NULL != pstcAdcInst);

    return pstcAdcInst;
}

/**
 *******************************************************************************
 ** \brief Enable or Disable the nvic.
 **
 ** \param [in] bEnable                 Enable or disable
 ** \arg ENABLE                         Enable the nvic
 ** \arg DISABLE                        Disable the nvic
 **
 ******************************************************************************/
static void NvicEnable(boolean_t bEnable)
{
     /* ENABLE ADC interrupt. */
     if(bEnable == ENABLE)
     {
         NVIC_ClearPendingIRQ(ADC0_IRQn);
         NVIC_EnableIRQ(ADC0_IRQn);
         NVIC_SetPriority(ADC0_IRQn, DDL_IRQ_LEVEL_ADC);
     }
     /* DISABLE ADC interrupt. */
     if(bEnable == DISABLE)
     {
         NVIC_ClearPendingIRQ(ADC0_IRQn);
         NVIC_DisableIRQ(ADC0_IRQn);
         NVIC_SetPriority(DTIM_IRQn, DDL_IRQ_LEVEL_ADC);
     }
}

/**
 *******************************************************************************
 ** \brief Enable or disable comparison
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enCmpType               The adc comparison type
 ** \arg AdcCmpResult                   Result comparison
 ** \arg AdcCmpRange                    Range comparison
 ** \param [in] bSwitch                 Enable or disable
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcAdcInst == NULL
 **                                     - pstcAdcInst->pstcAdcRegInst == NULL
 **                                     - Adc channel hasn't been initialized
 **
 ******************************************************************************/
static en_result_t SwitchCmp(en_adc_unit_t enAdcUnit, en_adc_cmp_type_t enCmpType, boolean_t bSwitch)
{
    en_result_t enresult = Ok;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    ASSERT(IS_VALID_CMP(enCmpType));

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    enCmpType == AdcCmpResult ? (pADCReg->ADCMPCTL_f.CMPEN = bSwitch) : (pADCReg->ADRCMPCTL_f.RCMPEN = bSwitch);

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Enable or Disable the interrupt request
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enIrqType               The type of interrupt request
 ** \arg AdcIrqPriorityConv             Priority conversion irq
 ** \arg AdcIrqScanConv                 Scan conversion irq
 ** \arg AdcIrqPrioFifoOvf              Priority conversion fifo overflow irq
 ** \arg AdcIrqScanFifoOvf              Scan conversion fifo overflow irq
 ** \arg AdcIrqResultCmp                Result comparison irq
 ** \arg AdcIrqRangeCmp                 Range comparison irq
 ** \param [in] bSwitch                 Enable or disable
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcAdcInst == NULL
 **                                     - pstcAdcInst->pstcAdcRegInst == NULL
 **                                     - enIrqType is invalid
 **                                     - Adc channel hasn't been initialized
 **
 ******************************************************************************/
static en_result_t SwitchIrq(en_adc_unit_t enAdcUnit, en_adc_irq_type_t enIrqType, boolean_t bSwitch)
{
    en_result_t enresult = Ok;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    ASSERT(IS_VALID_IRQ(enIrqType));

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    switch(enIrqType)
    {
        case AdcIrqPriorityConv:
            pADCReg->ADCINT_f.ADINT0E = bSwitch;
            break;
        case AdcIrqScanConv:
            pADCReg->ADCINT_f.ADINT1E = bSwitch;
            break;
        case AdcIrqPrioFifoOvf:
            pADCReg->ADCPCTL_f.POV = bSwitch;
            break;
        case AdcIrqScanFifoOvf:
            pADCReg->ADCSCTL_f.SOV = bSwitch;
            break;
        case AdcIrqResultCmp:
            pADCReg->ADCINT_f.ADINT3E = bSwitch;
            break;
        case AdcIrqRangeCmp:
            pADCReg->ADCINT_f.ADINT4E = bSwitch;
            break;
        default:
            enresult = ErrorInvalidParameter;
            break;
    }

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Initializes an adc module
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 **
 ** \param [in] pstcAdcInitCfg          Pointer to adc config
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - enAdcUnit is invalid
 **                                     - pstcAdcInitCfg == NULL
 **                                     - pstcAdcInitCfg data is invalid
 **
 ******************************************************************************/
en_result_t Adc_Init(en_adc_unit_t enAdcUnit, stc_adc_cfg_t* pstcAdcInitCfg)
{
    en_result_t enresult = Error;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    /* Parameter detection. */

    /* Check parameter only in debug mode with assert. */
    ASSERT(NULL != pstcAdcInitCfg);
    ASSERT(IS_VALID_REFVOL(pstcAdcInitCfg->enAdcRefVol));
    ASSERT(IS_VALID_ALIGNMENT(pstcAdcInitCfg->enAdcResultAlign));

    ASSERT(NULL != pstcAdcInitCfg->pstcAdcSampTimeCfg);
    ASSERT(IS_VALID_SAMPFREQ(SAMP_FREQ_DIV(pstcAdcInitCfg->pstcAdcSampTimeCfg)));
    ASSERT(IS_VALID_SAMPCLK(SAMP_TIME0(pstcAdcInitCfg->pstcAdcSampTimeCfg)));
    ASSERT(IS_VALID_SAMPCLK(SAMP_TIME1(pstcAdcInitCfg->pstcAdcSampTimeCfg)));

    pstcAdcInst = GetAdcInst(enAdcUnit);

    if( (NULL == pstcAdcInst) || (NULL == pstcAdcInst->pstcAdcRegInst))
    {
        return ErrorInvalidParameter;
    }

    /* Check null pointer for config. */
    if((NULL == pstcAdcInitCfg) || (NULL == pstcAdcInitCfg->pstcAdcSampTimeCfg))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    /* Register setting. */

    /* Disable adc first. */
    pADCReg->ADEN_f.AD_EN = DISABLE;

    /* Reference voltage. */
    pADCReg->ADCTL_f.REFS = pstcAdcInitCfg->enAdcRefVol;

    /* Result alignment. */
    pADCReg->ADCTL_f.ALGN = pstcAdcInitCfg->enAdcResultAlign;

    /* The sampling time relevant config. */
    /* ADC sampling clock division factor. */
    pADCReg->ADCDIV = SAMP_FREQ_DIV(pstcAdcInitCfg->pstcAdcSampTimeCfg);
    /* ADC sampling clock for register 0. */
    pADCReg->STS0= SAMP_TIME0(pstcAdcInitCfg->pstcAdcSampTimeCfg);
    /* ADC sampling clock for register 1. */
    pADCReg->STS1 = SAMP_TIME1(pstcAdcInitCfg->pstcAdcSampTimeCfg);
    /* Set the ADC sampling time for each channel. */
    pADCReg->STSEL = CHN_SAMP_TIME(pstcAdcInitCfg->pstcAdcSampTimeCfg);

    /* Priority conversion. */
    if(pstcAdcInitCfg->pstcAdcPrioCfg != NULL)
    {
        ASSERT(FALSE != CONV_CHN_INIT(pstcAdcInitCfg->pstcAdcPrioCfg));
        ASSERT(IS_VALID_PRIO_FIFOSTG(FIFO_STAGES(pstcAdcInitCfg->pstcAdcPrioCfg)));


        /* Clear fifo first. */
        pADCReg->ADCPCTL_f.PCCLR = FIFO_CLEAR;

        /* Enable external trigger if needed. */
        if(ENABLE == EXT_TRIGGER(pstcAdcInitCfg))
        {
            /* Enable external trigger. */
            pADCReg->ADCPCTL_f.EXTEN = EXT_TRIGGER_EN;
        }

        /* Enable timer trigger if needed. */
        if(ENABLE == TIMER_TRIGGER(pstcAdcInitCfg->pstcAdcPrioCfg))
        {
            ASSERT(IS_VALID_TIMER_TRIGGER(TIMER_TRIGGER_SEL(pstcAdcInitCfg->pstcAdcPrioCfg)));

            /* Enable timer trigger. */
            pADCReg->ADCPCTL_f.PCTMEN = TIMER_TRIGGER_EN;

            /* Set timer channel for timer trigger. */
            pADCReg->ADCPTSL = TIMER_TRIGGER_SEL(pstcAdcInitCfg->pstcAdcPrioCfg);
        }

        /* Fifo stages/depth. */
        pADCReg->PFS = FIFO_STAGES(pstcAdcInitCfg->pstcAdcPrioCfg);

        /* Sampling channel. */
        pADCReg->ADPIN = CONV_CHN(pstcAdcInitCfg->pstcAdcPrioCfg);

    }

    /* Scan conversion. */
    if(pstcAdcInitCfg->pstcAdcScanCfg != NULL)
    {
        ASSERT(FALSE != CONV_CHN_INIT(pstcAdcInitCfg->pstcAdcScanCfg));
        ASSERT(IS_VALID_SCAN_FIFOSTG(FIFO_STAGES(pstcAdcInitCfg->pstcAdcScanCfg)));

        /* Clear fifo first. */
        pADCReg->ADCSCTL_f.SCCLR = FIFO_CLEAR;

        /* Scan repeat. */
        pADCReg->ADCSCTL_f.RPT = SCAN_RPT(pstcAdcInitCfg->pstcAdcScanCfg);

        /* Enable timer trigger if needed. */
        if(ENABLE == TIMER_TRIGGER(pstcAdcInitCfg->pstcAdcScanCfg))
        {
            ASSERT(IS_VALID_TIMER_TRIGGER(TIMER_TRIGGER_SEL(pstcAdcInitCfg->pstcAdcScanCfg)));

            /* Enable timer tigger. */
            pADCReg->ADCSCTL_f.SCTMEN = TIMER_TRIGGER_EN;

            /* Set timer channel for timer trigger. */
            pADCReg->ADCSTSL = TIMER_TRIGGER_SEL(pstcAdcInitCfg->pstcAdcScanCfg);
        }

        /* Fifo stages/depth. */
        pADCReg->SFS = FIFO_STAGES(pstcAdcInitCfg->pstcAdcScanCfg);

        /* Sampling channel. */
        pADCReg->ADSIN0 = CONV_CHN(pstcAdcInitCfg->pstcAdcScanCfg);

    }

    /* Result comparison. */
    if(pstcAdcInitCfg->pstcAdcResultCmpCfg != NULL)
    {
        ASSERT(IS_VALID_RESULT_CMP_CON(CMP_CON(pstcAdcInitCfg->pstcAdcResultCmpCfg)));
        ASSERT(FALSE != CMP_THR_INIT(pstcAdcInitCfg->pstcAdcResultCmpCfg));

        /* Channel mode. */
        pADCReg->ADCMPCTL_f.CMPMD0 = CMP_ALL_CHN(pstcAdcInitCfg->pstcAdcResultCmpCfg);

        /* Comparison channel. */
        if(CMP_ALL_CHN(pstcAdcInitCfg->pstcAdcResultCmpCfg) == FALSE)
        {
            ASSERT(IS_VALID_CHN(CMP_CHN(pstcAdcInitCfg->pstcAdcResultCmpCfg)));

            /* Result comparison channel. */
            pADCReg->ADCMPCTL_f.CMPCH = CMP_CHN(pstcAdcInitCfg->pstcAdcResultCmpCfg);
        }

        /* Condition. */
        pADCReg->ADCMPCTL_f.CMPMD1 = CMP_CON(pstcAdcInitCfg->pstcAdcResultCmpCfg);

        /* Threshold. */
        pADCReg->ADCMPD = CMP_THR_RESULT(pstcAdcInitCfg->pstcAdcResultCmpCfg);

        /* Enable. */
        pADCReg->ADCMPCTL_f.CMPEN = ENABLE;

    }

    /* Range comparison. */
    if(pstcAdcInitCfg->pstcAdcRangeCmpCfg != NULL)
    {
        ASSERT(IS_VALID_RANGE_CMP_CON(CMP_CON(pstcAdcInitCfg->pstcAdcRangeCmpCfg)));
        ASSERT(FALSE != CMP_THR_INIT(pstcAdcInitCfg->pstcAdcRangeCmpCfg));
        ASSERT(IS_VALID_RANGE_CMP_TIMES(CMP_TIMES(pstcAdcInitCfg->pstcAdcRangeCmpCfg)));

        /* Channel mode. */
        pADCReg->ADRCMPS_f.RCMPMD = CMP_ALL_CHN(pstcAdcInitCfg->pstcAdcRangeCmpCfg);

        /* Comparison channel. */
        if(CMP_ALL_CHN(pstcAdcInitCfg->pstcAdcRangeCmpCfg) == FALSE)
        {
            ASSERT(IS_VALID_CHN(CMP_CHN(pstcAdcInitCfg->pstcAdcRangeCmpCfg)));

            /* Range comparison channel. */
            pADCReg->ADRCMPS_f.RCMPCH = CMP_CHN(pstcAdcInitCfg->pstcAdcRangeCmpCfg);
        }

        /* Condition. */
        pADCReg->ADRCMPCTL_f.RCMPTRG = CMP_CON(pstcAdcInitCfg->pstcAdcRangeCmpCfg);

        /* Threshold. */
        pADCReg->ADRCMPL = CMP_THR_RANGE(pstcAdcInitCfg->pstcAdcRangeCmpCfg, L);
        pADCReg->ADRCMPH = CMP_THR_RANGE(pstcAdcInitCfg->pstcAdcRangeCmpCfg, H);

        /* Comparison times. */
        pADCReg->ADRCMPCTL_f.RCMPCNT = CMP_TIMES(pstcAdcInitCfg->pstcAdcRangeCmpCfg);

        /* Enable. */
        pADCReg->ADRCMPCTL_f.RCMPEN = ENABLE;

    }

    /* Interrupt. */
    if(pstcAdcInitCfg->pstcAdcIntrpt != NULL)
    {
        /* Enable interrupt and hook isr callback. */
        ADC_INTRPT_INIT(pstcAdcInitCfg, pstcAdcInst, pADCReg->ADCINT_f.ADINT0E, Prio);

        ADC_INTRPT_INIT(pstcAdcInitCfg, pstcAdcInst, pADCReg->ADCINT_f.ADINT1E, Scan);

        ADC_INTRPT_INIT(pstcAdcInitCfg, pstcAdcInst, pADCReg->ADCINT_f.ADINT2E, PrioFifoOvf);

        ADC_INTRPT_INIT(pstcAdcInitCfg, pstcAdcInst, pADCReg->ADCINT_f.ADINT2E, ScanFifoOvf);

        ADC_INTRPT_INIT(pstcAdcInitCfg, pstcAdcInst, pADCReg->ADCINT_f.ADINT3E, Result);

        ADC_INTRPT_INIT(pstcAdcInitCfg, pstcAdcInst, pADCReg->ADCINT_f.ADINT4E, Range);
    }

    /* Nvic. */

    (IS_ENABLE_NVIC(pstcAdcInst)) ? NvicEnable(ENABLE) : NvicEnable(DISABLE);

    /* The current adc unit has been initialized. */
    pstcAdcInst->enAdcUnitStatus = AdcUnitInitialized;

    enresult = Ok;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief De-initializes an adc module
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcAdcInst == NULL
 **                                     - pstcAdcInst->pstcAdcRegInst == NULL
 **
 ******************************************************************************/
en_result_t Adc_DeInit(en_adc_unit_t enAdcUnit)
{
    en_result_t enresult = Error;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst(enAdcUnit);

    if( (NULL == pstcAdcInst) || (NULL == pstcAdcInst->pstcAdcRegInst))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    /* Register default setting. */

    /* Disable adc first. */
    pADCReg->ADEN_f.AD_EN = DISABLE;

    /* Reference voltage. */
    pADCReg->ADCTL_f.REFS = 0u;

    /* Result alignment. */
    pADCReg->ADCTL_f.ALGN = 0u;

    /* The sampling time relevant config. */
    pADCReg->ADCDIV = 1u;
    pADCReg->STS0 = 2u;
    pADCReg->STS1 = 2u;
    pADCReg->STSEL = 0u;

    /* Priority conversion. */
    pADCReg->ADCPCTL_f.PCST = 0u;
    pADCReg->ADCPCTL_f.EXTEN = 0u;
    pADCReg->ADCPCTL_f.PCTMEN = 0u;
    pADCReg->PFS = 0u;
    pADCReg->ADPIN = 0x1f1f;
    pADCReg->ADCPTSL = 0u;

    /* Scan conversion. */
    pADCReg->ADCSCTL_f.SCST = 0u;
    pADCReg->ADCSCTL_f.SCTMEN = 0u;
    pADCReg->ADCSCTL_f.RPT = 0u;
    pADCReg->ADCSCTL_f.SMOD = 0u;
    pADCReg->SFS = 0u;
    pADCReg->ADSIN0= 0u;
    pADCReg->ADCSTSL = 0u;

    /* Result comparison. */
    pADCReg->ADCMPCTL_f.CMPCH = 0u;
    pADCReg->ADCMPCTL_f.CMPMD0 = 0u;
    pADCReg->ADCMPCTL_f.CMPMD1 = 0u;
    pADCReg->ADCMPCTL_f.CMPEN = 0u;
    pADCReg->ADCMPD = 0u;

    /* Range comparison. */
    pADCReg->ADRCMPCTL_f.RCMPEN = 0u;
    pADCReg->ADRCMPCTL_f.RCMPTRG = 0u;
    pADCReg->ADRCMPCTL_f.RCMPCNT = 1u;
    pADCReg->ADRCMPS_f.RCMPCH = 0u;
    pADCReg->ADRCMPS_f.RCMPMD = 0u;
    pADCReg->ADRCMPL = 0xffc0;
    pADCReg->ADRCMPH = 0xffc0;

    /* Interrupt. */
    ADC_ISR_PFN(pstcAdcInst, Prio) = NULL;
    ADC_ISR_PFN(pstcAdcInst, Scan) = NULL;
    ADC_ISR_PFN(pstcAdcInst, PrioFifoOvf) = NULL;
    ADC_ISR_PFN(pstcAdcInst, ScanFifoOvf) = NULL;
    ADC_ISR_PFN(pstcAdcInst, Result) = NULL;
    ADC_ISR_PFN(pstcAdcInst, Range) = NULL;

    pADCReg->ADCINT_f.ADINT0F = 0u;
    pADCReg->ADCINT_f.ADINT1F = 0u;
    pADCReg->ADCPCTL_f.POV = 0u;
    pADCReg->ADCSCTL_f.SOV = 0u;
    pADCReg->ADCINT_f.ADINT3F = 0u;
    pADCReg->ADCINT_f.ADINT4F = 0u;

    /* Nvic. */
    NvicEnable(DISABLE);

    /* The current adc unit has been uninitialized. */
    pstcAdcInst->enAdcUnitStatus = AdcUnitUninitialized;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Configure sampling time for each channel before
 **        calling \ref Adc_Init()
 **
 ** \param [in] pstcAdcSampTimeCfg      Pointer of sampling time config
 ** \param [in] u16ChnSampleTime        Choose time register for each channel
 **
 ** \retval Ok                          Process successfully done
 **
 ** \note Use time register 0 for each channel in default
 ** \note Set \ref ST1CHN(AdcChn0) ~ \ref ST1CHN(AdcChn15) to u16ChnSampleTime
 **       if you want to choose time register 1 for specified channel
 **
 ******************************************************************************/
en_result_t Adc_ChnSampleTimeCfg(stc_adc_samp_time_cfg_t* pstcAdcSampTimeCfg, uint16_t u16ChnSampleTime)
{
    en_result_t enresult = Error;

    ASSERT(NULL != pstcAdcSampTimeCfg);
    ASSERT(IS_VALID_CHNSAMPTIME(u16ChnSampleTime));

    CHN_SAMP_TIME(pstcAdcSampTimeCfg) = u16ChnSampleTime;

    enresult = Ok;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Configure sampling channel for priority conversion before
 **        calling \ref Adc_Init()
 **
 ** \param [in] pstcAdcPrioCfg          Pointer of priority conversion config
 ** \param [in] u32ChnL1                Sampling channel in level 1
 ** \param [in] u32ChnL2                Sampling channel in level 2
 **
 ** \retval Ok                          Process successfully done
 **
 ** \note If only use level 2 channel,set \ref DEFAULT_CHN to u32ChnL1
 ** \note If only use level 1 channel,set \ref DEFAULT_CHN to u32ChnL2
 **
 ******************************************************************************/
en_result_t Adc_PrioConvChnCfg(stc_adc_prio_cfg_t* pstcAdcPrioCfg, uint32_t u32ChnL1, uint32_t u32ChnL2)
{
    en_result_t enresult = Error;

    ASSERT(NULL != pstcAdcPrioCfg);

    CONV_CHN(pstcAdcPrioCfg) = 0;

    /* Check and set level 1 channel if needed. */
    if(DEFAULT_CHN != u32ChnL1)
    {
        ASSERT(IS_VALID_CHN(u32ChnL1));

        CONV_CHN(pstcAdcPrioCfg) |= L1CH(u32ChnL1);
    }

    /* Check and set level 2 channel if needed. */
    if(DEFAULT_CHN != u32ChnL2)
    {
        ASSERT(IS_VALID_CHN(u32ChnL2));

        CONV_CHN(pstcAdcPrioCfg) |= L2CH(u32ChnL2);
    }

    CONV_CHN_INIT(pstcAdcPrioCfg) = TRUE;

    enresult = Ok;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Configure sampling channel for scan conversion before
 **        calling \ref Adc_Init()
 **
 ** \param [in] pstcAdcScanCfg          Pointer of scan conversion config
 ** \param [in] enMode                  Channel mode of scan conversion
 ** \arg AdcScanChnModeOrder            Scan channels in order
 ** \arg AdcScanChnModeDisorder         Scan channels out of order
 ** \param [in] u32Chn                  Sampling channel according to enMode
 **
 ** \retval Ok                          Process successfully done
 **
 ** \note Set \ref CHSL(AdcChn0) ~ \ref CHSL(AdcChn15) to u32Chn
 **       if enMode is AdcScanChnModeOrder
 ** \note Set \ref SCHSL0(chn) ~ \ref SCHSL5(chn) to u32Chn
 **       if enMode is AdcScanChnModeDisorder
 **
 ******************************************************************************/
en_result_t Adc_ScanConvChnCfg(stc_adc_scan_cfg_t* pstcAdcScanCfg, en_adc_scan_chn_mode_t enMode, uint32_t u32Chn)
{
    en_result_t enresult = Error;

    ASSERT(NULL != pstcAdcScanCfg);
    ASSERT(IS_VALID_SCAN_CHNMODE(enMode));

    pstcAdcScanCfg->stcChnCfg.enScanChnMd = enMode;

    if(AdcScanChnModeOrder == enMode)
    {
        ASSERT(IS_VALID_SCAN_CHN_MODE0(u32Chn));
    }
    else
    {
        ASSERT(IS_VALID_SCAN_CHN_MODE1(u32Chn));
    }

    CONV_CHN(pstcAdcScanCfg) = u32Chn;

    CONV_CHN_INIT(pstcAdcScanCfg) = TRUE;

    enresult = Ok;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Configure threshold for result comparison before
 **        calling \ref Adc_Init()
 **
 ** \param [in] pstcAdcResultCmpCfg     Pointer of result comparison config
 ** \param [in] u16Thr                  Threshold.
 **
 ** \retval Ok                          Process successfully done
 **
 ** \note None
 **
 ******************************************************************************/
en_result_t Adc_ResultCmpThrCfg(stc_adc_result_cmp_cfg_t* pstcAdcResultCmpCfg, uint16_t u16Thr)
{
    en_result_t enresult = Error;

    ASSERT(NULL != pstcAdcResultCmpCfg);
    ASSERT(IS_VALID_THR(u16Thr));

    CMP_THR_RESULT(pstcAdcResultCmpCfg) = FORMAT_THR(u16Thr);
    CMP_THR_INIT(pstcAdcResultCmpCfg) = TRUE;

    enresult = Ok;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Configure threshold for range comparison before
 **        calling \ref Adc_Init()
 **
 ** \param [in] pstcAdcRangeCmpCfg      Pointer of range comparison config
 ** \param [in] u16ThrL                 Threshold lower limit
 ** \param [in] u16ThrH                 Threshold higher limit
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Adc_RangeCmpThrCfg(stc_adc_range_cmp_cfg_t* pstcAdcRangeCmpCfg, uint16_t u16ThrL, uint16_t u16ThrH)
{
    en_result_t enresult = Error;

    ASSERT(NULL != pstcAdcRangeCmpCfg);
    ASSERT(IS_VALID_THR(u16ThrL));
    ASSERT(IS_VALID_THR(u16ThrH));
    ASSERT(u16ThrH > u16ThrL);

    CMP_THR_RANGE(pstcAdcRangeCmpCfg,L) = FORMAT_THR(u16ThrL);
    CMP_THR_RANGE(pstcAdcRangeCmpCfg,H) = FORMAT_THR(u16ThrH);

    CMP_THR_INIT(pstcAdcRangeCmpCfg) = TRUE;

    enresult = Ok;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Enable adc function
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorTimeout                If one of following conditions are met:
 **                                     - adc doesn't ready in timeout
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcAdcInst == NULL
 **                                     - pstcAdcInst->pstcAdcRegInst == NULL
 **                                     - Adc channel hasn't been initialized
 **
 ******************************************************************************/
en_result_t Adc_Enable(en_adc_unit_t enAdcUnit)
{
    en_result_t enresult = Error;
    uint32_t u32TimeOut = ADC_READY_TIMEOUT_CNT;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    pADCReg->ADEN_f.AD_EN = ENABLE;

    /* Wait ready. */
    while(u32TimeOut)
    {
        if(TRUE == pADCReg->ADEN_f.ENRDY)
        {
            break;
        }

        u32TimeOut--;
    }

    enresult = (u32TimeOut == 0u ? ErrorTimeout :  Ok);

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Disnable adc function
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcAdcInst == NULL
 **                                     - pstcAdcInst->pstcAdcRegInst == NULL
 **                                     - Adc channel hasn't been initialized
 **
 ******************************************************************************/
en_result_t Adc_Disable(en_adc_unit_t enAdcUnit)
{
    en_result_t enresult = Ok;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    pADCReg->ADEN_f.AD_EN = DISABLE;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Enable comparison
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enCmpType               The adc comparison type
 ** \arg AdcCmpResult                   Result comparison
 ** \arg AdcCmpRange                    Range comparison
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Adc_CmpEnable(en_adc_unit_t enAdcUnit, en_adc_cmp_type_t enCmpType)
{
    return SwitchCmp(enAdcUnit, enCmpType, TRUE);
}

/**
 *******************************************************************************
 ** \brief Disable comparison
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enCmpType               The adc comparison type
 ** \arg AdcCmpResult                   Result comparison
 ** \arg AdcCmpRange                    Range comparison
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Adc_CmpDisable(en_adc_unit_t enAdcUnit, en_adc_cmp_type_t enCmpType)
{
    return SwitchCmp(enAdcUnit, enCmpType, FALSE);
}

/**
 *******************************************************************************
 ** \brief Enable the interrupt request
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enIrqType               The type of interrupt request
 ** \arg AdcIrqPriorityConv             Priority conversion irq
 ** \arg AdcIrqScanConv                 Scan conversion irq
 ** \arg AdcIrqPrioFifoOvf              Priority conversion fifo overflow irq
 ** \arg AdcIrqScanFifoOvf              Scan conversion fifo overflow irq
 ** \arg AdcIrqResultCmp                Result comparison irq
 ** \arg AdcIrqRangeCmp                 Range comparison irq
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Adc_IrqEnable(en_adc_unit_t enAdcUnit, en_adc_irq_type_t enIrqType)
{
    return SwitchIrq(enAdcUnit, enIrqType, TRUE);
}

/**
 *******************************************************************************
 ** \brief Disable the interrupt request
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enIrqType               The type of interrupt request
 ** \arg AdcIrqPriorityConv             Priority conversion irq
 ** \arg AdcIrqScanConv                 Scan conversion irq
 ** \arg AdcIrqPrioFifoOvf              Priority conversion fifo overflow irq
 ** \arg AdcIrqScanFifoOvf              Scan conversion fifo overflow irq
 ** \arg AdcIrqResultCmp                Result comparison irq
 ** \arg AdcIrqRangeCmp                 Range comparison irq
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Adc_IrqDisable(en_adc_unit_t enAdcUnit, en_adc_irq_type_t enIrqType)
{
    return SwitchIrq(enAdcUnit, enIrqType, FALSE);
}

/**
 *******************************************************************************
 ** \brief The interrupt service routine for priority conversion
 **
 ** \param [in] u8Param                 Index of adc unit
 **
 ** \retval None
 **
 ******************************************************************************/
void Adc_PriorityConvIrqHandler(uint8_t u8Param)
{
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst((en_adc_unit_t)u8Param);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) || (NULL == pstcAdcInst->pstcAdcRegInst) )
    {
        return;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    ADC_INTRPT_ACTION(pstcAdcInst, pADCReg->ADCINT_f.ADINT0F, Prio);
}

/**
 *******************************************************************************
 ** \brief The interrupt service routine for scan conversion
 **
 ** \param [in] u8Param                 Index of adc unit
 **
 ** \retval None
 **
 ******************************************************************************/
void Adc_ScanConvIrqHandler(uint8_t u8Param)
{
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst((en_adc_unit_t)u8Param);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) || (NULL == pstcAdcInst->pstcAdcRegInst) )
    {
        return;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    ADC_INTRPT_ACTION(pstcAdcInst, pADCReg->ADCINT_f.ADINT1F, Scan);
}

/**
 *******************************************************************************
 ** \brief The interrupt service routine for fifo overflow during conversion
 **
 ** \param [in] u8Param                 Index of adc unit
 **
 ** \retval None
 **
 ******************************************************************************/
void Adc_FifoOvfIrqHandler(uint8_t u8Param)
{
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst((en_adc_unit_t)u8Param);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) || (NULL == pstcAdcInst->pstcAdcRegInst) )
    {
        return;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    ADC_INTRPT_ACTION(pstcAdcInst, pADCReg->ADCSCTL_f.SOV, ScanFifoOvf);

    ADC_INTRPT_ACTION(pstcAdcInst, pADCReg->ADCPCTL_f.POV, PrioFifoOvf);
}

/**
 *******************************************************************************
 ** \brief The interrupt service routine for result comparison
 **
 ** \param [in] u8Param                 Index of adc unit
 **
 ** \retval None
 **
 ******************************************************************************/
void Adc_ResultCmpIrqHandler(uint8_t u8Param)
{
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst((en_adc_unit_t)u8Param);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) || (NULL == pstcAdcInst->pstcAdcRegInst) )
    {
        return;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    ADC_INTRPT_ACTION(pstcAdcInst, pADCReg->ADCINT_f.ADINT3F, Result);
}

/**
 *******************************************************************************
 ** \brief The interrupt service routine for range comparison
 **
 ** \param [in] u8Param                 Index of adc unit
 **
 ** \retval None
 **
 ******************************************************************************/
void Adc_RangeCmpIrqHandler(uint8_t u8Param)
{
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst((en_adc_unit_t)u8Param);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) || (NULL == pstcAdcInst->pstcAdcRegInst) )
    {
        return;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    ADC_INTRPT_ACTION(pstcAdcInst, pADCReg->ADCINT_f.ADINT4F, Range);
}

/**
 *******************************************************************************
 ** \brief Start conversion
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enConvType              The adc conversion type
 ** \arg AdcConvScan                    Scan conversion
 ** \arg AdcConvPriority                Priority conversion
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcAdcInst == NULL
 **                                     - pstcAdcInst->pstcAdcRegInst == NULL
 **                                     - Adc channel hasn't been initialized
 **
 ******************************************************************************/
en_result_t Adc_ConvStart(en_adc_unit_t enAdcUnit, en_adc_conv_type_t enConvType)
{
    en_result_t enresult = Ok;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    ASSERT(IS_VALID_CONV(enConvType));

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    enConvType == AdcConvScan ? (pADCReg->ADCSCTL_f.SCST = TRUE) : (pADCReg->ADCPCTL_f.PCST = TRUE);

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Stop conversion
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 **
 ** \retval Ok                          Process successfully done.
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcAdcInst == NULL
 **                                     - pstcAdcInst->pstcAdcRegInst == NULL
 **                                     - Adc channel hasn't been initialized
 **
 ******************************************************************************/
en_result_t Adc_ConvStop(en_adc_unit_t enAdcUnit)
{
    en_result_t enresult = Ok;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    pADCReg->ADCTL_f.SP = TRUE;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Repeat sampling or not for scan conversion
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] bRepeat                 Repeat or not
 ** \arg TRUE                           Repeated sampling
 ** \arg FALSE                          Single sampling
 **
 ** \retval Ok                          Process successfully done.
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcAdcInst == NULL
 **                                     - pstcAdcInst->pstcAdcRegInst == NULL
 **                                     - Adc channel hasn't been initialized
 **
 ******************************************************************************/
en_result_t Adc_ScanRepeat(en_adc_unit_t enAdcUnit, boolean_t bRepeat)
{
    en_result_t enresult = Ok;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    pADCReg->ADCSCTL_f.RPT = bRepeat;

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Read the conversion result from fifo
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enConvType              The adc conversion type
 ** \arg AdcConvScan                    Scan conversion
 ** \arg AdcConvPriority                Priority conversion
 **
 ** \retval uint32_t                    Fifo data
 **
 ** \note Call \ref Adc_GetConvDataValid() after read fifo
 **
 ******************************************************************************/
uint32_t Adc_ReadFifo(en_adc_unit_t enAdcUnit, en_adc_conv_type_t enConvType)
{
    uint32_t u32FifoData = 0u;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    ASSERT(IS_VALID_CONV(enConvType));

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return 0u;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    u32FifoData = ((AdcConvScan == enConvType) ? pADCReg->SFD : pADCReg->PFD);

    return u32FifoData;
}

/**
 *******************************************************************************
 ** \brief Clear fifo
 **
 ** \param [in] enAdcUnit             The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enConvType              The adc conversion type
 ** \arg AdcConvScan                    Scan conversion
 ** \arg AdcConvPriority                Priority conversion
 **
 ** \retval None
 **
 ******************************************************************************/
void Adc_ClearFifo(en_adc_unit_t enAdcUnit, en_adc_conv_type_t enConvType)
{
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    ASSERT(IS_VALID_CONV(enConvType));

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    AdcConvScan == enConvType ? (pADCReg->ADCSCTL_f.SCCLR = FIFO_CLEAR) : (pADCReg->ADCPCTL_f.PCCLR = FIFO_CLEAR);

    return;
}

/**
 *******************************************************************************
 ** \brief Get the channel which conversion completed from the fifo data.
 **
 ** \param [in] u32FifoData             Fifo data
 **
 ** \retval uint8_t                     The channel which conversion completed
 **
 ** \note Call \ref Adc_GetConvDataValid() before use this function
 **
 ******************************************************************************/
uint8_t Adc_GetConvChannel(uint32_t u32FifoData)
{
    return (uint8_t)(u32FifoData & FIFO_MASK_CHN);
}

/**
 *******************************************************************************
 ** \brief Check the validity from the fifo data
 **
 ** \param [in] u32FifoData             Fifo data
 **
 ** \retval AdcFifoValid_True           If the fifo data is valid
 ** \retval AdcFifoValid_False          If the fifo data is invalid
 **
 ******************************************************************************/
en_adc_fifo_valid_t Adc_CheckConvDataValid(uint32_t u32FifoData)
{
    return (FIFO_VALID(u32FifoData) == 0u ? AdcFifoValidTrue : AdcFifoValidFalse);
}

/**
 *******************************************************************************
 ** \brief Get the conversion trigger from the fifo data
 **
 ** \param [in] u32FifoData             Fifo data
 ** \param [in] enConvType              The adc conversion type
 ** \arg AdcConvScan                    Scan conversion
 ** \arg AdcConvPriority                Priority conversion
 **
 ** \retval AdcConvTrigger_Sw           If the trigger is software
 ** \retval AdcConvTrigger_Timer        If the trigger is timer
 ** \retval AdcConvTrigger_External     If the trigger is external
 **
 ** \note Call \ref Adc_GetConvDataValid() before use this function
 **
 ******************************************************************************/
en_adc_conv_trigger_t Adc_GetConvTrigger(uint32_t u32FifoData, en_adc_conv_type_t enConvType)
{
    ASSERT(IS_VALID_CONV(enConvType));

    return (en_adc_conv_trigger_t)FIFO_TRIGGER(u32FifoData, enConvType);
}

/**
 *******************************************************************************
 ** \brief Get the conversion result from the fifo data
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] u32FifoData             Fifo data
 **
 ** \retval uint16_t                    The conversion result
 **
 ** \note Call \ref Adc_GetConvDataValid() before use this function
 **
 ******************************************************************************/
uint16_t Adc_GetConvResult(en_adc_unit_t enAdcUnit, uint32_t u32FifoData)
{
    en_adc_result_align_sel_t enAlign;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return 0u;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    enAlign = (en_adc_result_align_sel_t)pADCReg->ADCTL_f.ALGN;

    return (uint16_t)FIFO_DATA(u32FifoData, enAlign);
}

/**
 *******************************************************************************
 ** \brief Get the interrupt request flag
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enIrqType               The type of adc interrupt request
 ** \arg AdcIrqPriorityConv             Priority conversion irq
 ** \arg AdcIrqScanConv                 Scan conversion irq
 ** \arg AdcIrqPrioFifoOvf              Priority conversion fifo overflow irq
 ** \arg AdcIrqScanFifoOvf              Scan conversion fifo overflow irq
 ** \arg AdcIrqResultCmp                Result comparison irq
 ** \arg AdcIrqRangeCmp                 Range comparison irq
 **
 ** \retval TRUE                        The enIrqType irq has been generated
 ** \retval FALSE                       The enIrqType irq hasn't been generated
 **
 ******************************************************************************/
boolean_t Adc_GetIrqFlag(en_adc_unit_t enAdcUnit, en_adc_irq_type_t enIrqType)
{
    boolean_t bFlag = FALSE;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    ASSERT(IS_VALID_IRQ(enIrqType));

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return FALSE;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    switch(enIrqType)
    {
        case AdcIrqPriorityConv:
            bFlag = ((1u == pADCReg->ADCINT_f.ADINT0F) ? TRUE : FALSE);
            break;
        case AdcIrqScanConv:
            bFlag = ((1u == pADCReg->ADCINT_f.ADINT1F) ? TRUE : FALSE);
            break;
        case AdcIrqPrioFifoOvf:
            bFlag = ((1u == pADCReg->ADCPCTL_f.POV) ? TRUE : FALSE);
            break;
        case AdcIrqScanFifoOvf:
            bFlag = ((1u == pADCReg->ADCSCTL_f.SOV) ? TRUE : FALSE);
            break;
        case AdcIrqResultCmp:
            bFlag = ((1u == pADCReg->ADCINT_f.ADINT3F) ? TRUE : FALSE);
            break;
        case AdcIrqRangeCmp:
            bFlag = ((1u == pADCReg->ADCINT_f.ADINT4F) ? TRUE : FALSE);
            break;
        default:
            break;
    }

    return bFlag;
}

/**
 *******************************************************************************
 ** \brief Clear the interrupt request flag
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enIrqType               The type of adc interrupt request
 ** \arg AdcIrqPriorityConv             Priority conversion irq
 ** \arg AdcIrqScanConv                 Scan conversion irq
 ** \arg AdcIrqPrioFifoOvf              Priority conversion fifo overflow irq
 ** \arg AdcIrqScanFifoOvf              Scan conversion fifo overflow irq
 ** \arg AdcIrqResultCmp                Result comparison irq
 ** \arg AdcIrqRangeCmp                 Range comparison irq
 **
 ** \retval None
 **
 ******************************************************************************/
void Adc_ClearIrqFlag(en_adc_unit_t enAdcUnit, en_adc_irq_type_t enIrqType)
{
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    ASSERT(IS_VALID_IRQ(enIrqType));

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    switch(enIrqType)
    {
        case AdcIrqPriorityConv:
            pADCReg->ADCINT_f.ADINT0F = FALSE;
            break;
        case AdcIrqScanConv:
            pADCReg->ADCINT_f.ADINT1F = FALSE;
            break;
        case AdcIrqPrioFifoOvf:
            pADCReg->ADCPCTL_f.POV = FALSE;
            break;
        case AdcIrqScanFifoOvf:
            pADCReg->ADCSCTL_f.SOV = FALSE;
            break;
        case AdcIrqResultCmp:
            pADCReg->ADCINT_f.ADINT3F = FALSE;
            break;
        case AdcIrqRangeCmp:
            pADCReg->ADCINT_f.ADINT4F = FALSE;
            break;
        default:
            break;
    }

    return;
}

/**
 *******************************************************************************
 ** \brief Get the adc status
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enStatus                The type of adc status
 ** \arg AdcStatusScanConv              Is scan conversion in process
 ** \arg AdcStatusPrioConv              Is priority conversion in process
 ** \arg AdcStatusPrioL2Pending         Is priority conversion(level 2) pending
 ** \arg AdcStatusScanFifoFull          Is fifo overflow for scan conversion
 ** \arg AdcStatusScanFifoEmpty         Is fifo empty for scan conversion
 ** \arg AdcStatusPrioFifoFull          Is fifo overflow for priority conversion
 ** \arg AdcStatusPrioFifoEmpty         Is fifo empty for priority conversion
 ** \arg AdcStatusRangeCmpExceedThr     More than high thr or less than low thr
 **
 ** \retval TRUE
 ** \retval FALSE
 **
 ******************************************************************************/
boolean_t Adc_GetStatus(en_adc_unit_t enAdcUnit, en_adc_status_t enStatus)
{
    boolean_t bFlag = FALSE;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    ASSERT(IS_VALID_STATUS(enStatus));

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return FALSE;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    switch(enStatus)
    {
        case AdcStatusScanConv:
            bFlag = ((1u == pADCReg->ADCTL_f.SCF) ? TRUE : FALSE);
            break;
        case AdcStatusPrioConv:
            bFlag = ((1u == pADCReg->ADCTL_f.PCF) ? TRUE : FALSE);
            break;
        case AdcStatusPrioL2Pending:
            bFlag = ((1u == pADCReg->ADCTL_f.PCP) ? TRUE : FALSE);
            break;
        case AdcStatusScanFifoFull:
            bFlag = ((1u == pADCReg->ADCSCTL_f.SCFUL) ? TRUE : FALSE);
            break;
        case AdcStatusScanFifoEmpty:
            bFlag = ((1u == pADCReg->ADCSCTL_f.SCEMP) ? TRUE : FALSE);
            break;
        case AdcStatusPrioFifoFull:
            bFlag = ((1u == pADCReg->ADCPCTL_f.PCFUL) ? TRUE : FALSE);
            break;
        case AdcStatusPrioFifoEmpty:
            bFlag = ((1u == pADCReg->ADCPCTL_f.PCEMP) ? TRUE : FALSE);
            break;
        case AdcStatusRangeCmpExceedThr:
            bFlag = ((1u == pADCReg->ADRCMPF_f.RCMPF) ? TRUE : FALSE);
            break;
        default:
            break;
    }

    return bFlag;
}

/**
 *******************************************************************************
 ** \brief Re-select the sample time for the specified channel
 **
 ** \param [in] enAdcUnit               The adc unit
 ** \arg AdcUnit0                       Unit 0
 ** \param [in] enAdcSampTimeReg        The sample time register
 ** \arg AdcSampTimeReg0                Use time register 0
 ** \arg AdcSampTimeReg1                Use time register 1
 ** \param [in] u16ReselChn             The specified channel
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcAdcInst == NULL
 **                                     - pstcAdcInst->pstcAdcRegInst == NULL
 **                                     - Adc channel hasn't been initialized
 **
 ** \note Set \ref AdcChn0 ~ \ref AdcChn15 to u16ReselChn
 **
 ******************************************************************************/
en_result_t Adc_ChnSampleTimeResel(en_adc_unit_t enAdcUnit, en_adc_samp_time_reg_t enAdcSampTimeReg, uint16_t u16ReselChn)
{
    en_result_t enresult = Ok;
    uint16_t u16RegStselBackup = 0;
    stc_adc_instance_data_t* pstcAdcInst = NULL;
    volatile stc_adc_regtypdef_t *pADCReg = NULL;

    ASSERT(IS_VALID_SAMPTIMEREG(enAdcSampTimeReg));
    ASSERT(IS_VALID_CHN(u16ReselChn));

    pstcAdcInst = GetAdcInst(enAdcUnit);
    DEBUG_CHECK_INST(pstcAdcInst);

    if( (NULL == pstcAdcInst) ||
        (NULL == pstcAdcInst->pstcAdcRegInst) ||
        (AdcUnitUninitialized == pstcAdcInst->enAdcUnitStatus))
    {
        return ErrorInvalidParameter;
    }

    pADCReg = pstcAdcInst->pstcAdcRegInst;

    u16RegStselBackup = pADCReg->STSEL;

    CHNSTSEL(u16ReselChn, enAdcSampTimeReg, u16RegStselBackup);

    pADCReg->STSEL = u16RegStselBackup;

    return enresult;
}

//@} // AdcGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
