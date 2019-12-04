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
 ** \brief This example demonstrates how to use the delay mode trigger function of
 **      the timer4-adct.
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     timer4-adct.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "adc.h"
#include "timer4_cnt.h"
#include "timer4_adct.h"
#include "timer4_oco.h"
#include "timer4_pwm.h"


/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* CNT channel */
#define CNT_CYCLE_VAL    (20000u)               /*< Timer4 counter cycle value */

/* OCO channel */
#define OCO_CH           (TIMER4_OCO_CH0)       /*< OCO channel */
#define OCO_OCCR_VAL     (CNT_CYCLE_VAL*3/4)    /*< OCO cycle value */

/* ADCT channel */
#define ADCT_CH          (0u)
#define ADCT_DELAY_VAL   (OCO_OCCR_VAL/100)
#define ADCT_TRIG_ADC    (AdctTrigAdc0Prio)     /*< AdctTrigAdc0Prio */

/* ADC channel */
#define ADC_CH_SCAN      (AdcChn3)              /*< ADC channel for scan mode */
#define ADC_CH_PRIO      (AdcChn4)              /*< ADC channel for priority mode */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t u16AdcScanValue = 0;
uint16_t u16AdcPriValue = 0;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void ConfigAdc(void);
static void AdcScanConvIrqCallback(void);
static void AdcPriorityConvIrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t m_u16AdcScanCnt = 0;
static uint16_t m_u16AdcScanErr = 0;
static uint32_t m_u16AdcPriCnt = 0;
static uint32_t m_u16AdcPriErr = 0;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Adc scan convert interrupt handler
 **
 ******************************************************************************/
static void AdcScanConvIrqCallback(void)
{
    uint8_t u8Ch = 0;
    uint8_t u8Trigger = 0;
    uint16_t u16AdcValue = 0;
    uint32_t u32Fifo = 0;

    u32Fifo = Adc_ReadFifo(AdcUnit0, AdcConvScan);

    if(AdcFifoValidTrue == Adc_CheckConvDataValid(u32Fifo))
    {
        u8Ch = Adc_GetConvChannel(u32Fifo);
        u8Trigger = Adc_GetConvTrigger(u32Fifo, AdcConvScan);
        u16AdcValue = Adc_GetConvResult(AdcUnit0, u32Fifo);
        if((ADC_CH_SCAN == u8Ch) && (AdcConvTriggerTimer == u8Trigger))
        {
            u16AdcScanValue = u16AdcValue;
            m_u16AdcScanCnt++;
        }
        else
        {
            m_u16AdcScanErr++;
        }
    }
}

/**
 *******************************************************************************
 ** \brief Adc priority convert interrupt handler
 **
 ******************************************************************************/
static void AdcPriorityConvIrqCallback(void)
{
    uint8_t u8Ch = 0;
    uint8_t u8Trigger = 0;
    uint16_t u16AdcValue = 0;
    uint32_t u32Fifo = 0;

    u32Fifo = Adc_ReadFifo(AdcUnit0, AdcConvPriority);

    if(AdcFifoValidTrue == Adc_CheckConvDataValid(u32Fifo))
    {
        u8Ch = Adc_GetConvChannel(u32Fifo);
        u8Trigger = Adc_GetConvTrigger(u32Fifo, AdcConvPriority);
        u16AdcValue = Adc_GetConvResult(AdcUnit0, u32Fifo);
        if((ADC_CH_PRIO == u8Ch) && (AdcConvTriggerTimer == u8Trigger))
        {
            u16AdcPriValue = u16AdcValue;
            m_u16AdcPriCnt++;
        }
        else
        {
            m_u16AdcPriErr++;
        }
    }
}

/**
 *******************************************************************************
 ** \brief The ADC conversion initialization configuration
 **
 ******************************************************************************/
static void ConfigAdc(void)
{
    stc_adc_cfg_t stcAdcInitCfg;
    stc_adc_scan_cfg_t stcAdcScanCfg;
    stc_adc_prio_cfg_t stcAdcPrioCfg;
    stc_adc_intrpt_t stcAdcIntrpt;
    stc_adc_samp_time_cfg_t stcAdcSampTimeCfg;

    /* Clear structure. */
    DDL_ZERO_STRUCT(stcAdcInitCfg);
    DDL_ZERO_STRUCT(stcAdcScanCfg);
    DDL_ZERO_STRUCT(stcAdcPrioCfg);
    DDL_ZERO_STRUCT(stcAdcIntrpt);
    DDL_ZERO_STRUCT(stcAdcSampTimeCfg);

    /* Configure priority conversion. */
    stcAdcPrioCfg.bTimerTriggerEn = TRUE;
    stcAdcPrioCfg.enTimerTrigger = AdcTimerTriggerTimer4;
    stcAdcPrioCfg.enFifoStages = AdcPrioFifoStages1;
    Adc_PrioConvChnCfg(&stcAdcPrioCfg, DEFAULT_CHN, ADC_CH_PRIO);

    /* Configure scan conversion. */
    stcAdcScanCfg.bRepeat = FALSE;
    stcAdcScanCfg.bTimerTriggerEn = TRUE;
    stcAdcScanCfg.enTimerTrigger = AdcTimerTriggerTimer4;
    stcAdcScanCfg.enFifoStages = AdcScanFifoStages1;
    Adc_ScanConvChnCfg(&stcAdcScanCfg, AdcScanChnModeOrder, CHSL(ADC_CH_SCAN));

    /* Sampling time configuration. */
    stcAdcSampTimeCfg.enFreqDiv = SampFreqSysTDiv16;
    stcAdcSampTimeCfg.enTimeReg0 = AdcSampTime6AdcClk;
    stcAdcSampTimeCfg.enTimeReg1 = AdcSampTime12AdcClk;
    Adc_ChnSampleTimeCfg(&stcAdcSampTimeCfg, ST1CHN(ADC_CH_SCAN));

    /* interrupt configuration. */
    stcAdcIntrpt.bIrqEnPrio = TRUE;
    stcAdcIntrpt.bIrqEnScan = TRUE;
    stcAdcIntrpt.stcIsr.pfnIsrPrio = AdcPriorityConvIrqCallback;
    stcAdcIntrpt.stcIsr.pfnIsrScan = AdcScanConvIrqCallback;

    /* Common configuration. */
    stcAdcInitCfg.enAdcRefVol = AdcRefVolInternal;
    stcAdcInitCfg.enAdcResultAlign = AdcResultAlignLeft;
    stcAdcInitCfg.pstcAdcSampTimeCfg = &stcAdcSampTimeCfg;
    stcAdcInitCfg.pstcAdcPrioCfg = &stcAdcPrioCfg;
    stcAdcInitCfg.pstcAdcScanCfg = &stcAdcScanCfg;
    stcAdcInitCfg.pstcAdcIntrpt  = &stcAdcIntrpt;
    Adc_Init(AdcUnit0, &stcAdcInitCfg);

    Adc_Enable(AdcUnit0);
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
    stc_timer4_adct_config_t stcAdctConfig;
    stc_timer4_adct_trig_cond_t stcTrigCond;
    stc_timer4_oco_config_t stcOcoConfig;

    /* Clear structures */
    DDL_ZERO_STRUCT(stcIrqEn);
    DDL_ZERO_STRUCT(stcIrqCb);
    DDL_ZERO_STRUCT(stcCntConfig);
    DDL_ZERO_STRUCT(stcAdctConfig);
    DDL_ZERO_STRUCT(stcOcoConfig);

    /* Set Adc */
    Gpio_SetAnalog(4, 7, TRUE); /* Adc unit0 ch3: p47-A4 */
    ConfigAdc();

    /*  Timer4-CNT: Initialize CNT configuration structure */
    stcCntConfig.enCntClockDiv = CntPclkDiv1;    /* CNT clock divide */
    stcCntConfig.enCntMode = UpDownCnt;
    stcCntConfig.enCntClock = CntClockPclk;
    stcCntConfig.u8ZeroIntMsk = 0u;
    stcCntConfig.u8PeakIntMsk = 0u;
    stcCntConfig.bEnBuffer = FALSE;
    stcCntConfig.pstcIrqEn = &stcIrqEn;
    stcCntConfig.pstcIrqCb = &stcIrqCb;
    stcCntConfig.bTouchNvic = TRUE;
    Timer4_Cnt_Init(&stcCntConfig); /* Initialize CNT */
    Timer4_Cnt_SetCycleVal(CNT_CYCLE_VAL);

    /*  Timer4-OCO: Initialize OCO configuration structure */
    stcOcoConfig.enOccrBufMode = OccrBufDisable;
    stcOcoConfig.enOcmrBufMode = OcmrBufDisable;
    stcOcoConfig.enPinState = OpLowLevel;
    stcOcoConfig.bIrqEnable = FALSE;
    stcOcoConfig.pfnIrqCallback = NULL;
    stcOcoConfig.bTouchNvic = FALSE;
    Timer4_Oco_Init(OCO_CH, &stcOcoConfig);     /* Initialize OCO */
    Timer4_Oco_WriteOccr(OCO_CH, OCO_OCCR_VAL); /* Set OCO channel compare value */
    Timer4_Oco_EnableOperation(OCO_CH);         /* Enable OCO channel */

    /*  Timer4-ADCT: Initialize ADCT configuration structure */
    stcAdctConfig.enOccrSel = AdctSelOccr0;
    stcAdctConfig.enBuf = AdctBufDisable;
    stcAdctConfig.enMode = AdctDelayTrigMode;
    stcAdctConfig.enTrigAdcUnit = ADCT_TRIG_ADC;
    Timer4_Adct_Init(ADCT_CH, &stcAdctConfig);      /* Initialize ADCT */
    Timer4_Adct_WriteAtvr(ADCT_CH, ADCT_DELAY_VAL); /* Set ADCT delay value  */

    stcTrigCond.bUpEn = FALSE;
    stcTrigCond.bDownEn = TRUE;
    stcTrigCond.bZeroEn = TRUE;
    stcTrigCond.bPeakEn = TRUE;
    Timer4_Adct_EnableOperation(ADCT_CH, &stcTrigCond); /* Set ADCT operation condition */

    Timer4_Cnt_Start(CntStartFromZeroValue); /* Start CNT */

    while(1)
    {
        /* nop */
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
