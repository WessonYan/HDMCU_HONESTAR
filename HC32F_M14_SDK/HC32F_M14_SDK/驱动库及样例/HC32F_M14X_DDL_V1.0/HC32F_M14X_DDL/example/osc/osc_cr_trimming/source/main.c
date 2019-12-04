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
 ** \brief This sample demonstrates how to trimming internal high-speed CR freq.
 **
 **   - 2017-5-31  1.0  Zhangxl First version for HiCR trim.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "ct.h"
#include "osc.h"
#include "flash.h"
#include "math.h"
#include "stdlib.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief State of high CR trim procedure
 **
 ** \note State of high CR trim procedure
 **
 ******************************************************************************/
typedef enum en_TrimState
{
    TRIM_PROC_INIT = 0,
    TRIM_PROC_START,
    TRIM_PROC_ONGOING_BIT6,
    TRIM_PROC_ONGOING_BIT5,
    TRIM_PROC_ONGOING_BIT4,
    TRIM_PROC_ONGOING_BIT3,
    TRIM_PROC_ONGOING_BIT2,
    TRIM_PROC_ONGOING_BIT1,
    TRIM_PROC_ONGOING_BIT0,
    TRIM_PROC_FINISH
}en_TrimState_t;

/**
 *******************************************************************************
 ** \brief High CR trim value info. matrix
 **
 ** \note Record 8 times trim data
 **
 ******************************************************************************/
typedef struct stc_Cr_Trim_Matrix
{
    uint8_t u8Idx;
    uint8_t u8CrFreqTrimVal;
    uint32_t u32CalAbs;
}stc_Cr_Trim_Matrix_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* PWC  */
#define  CT_PWC_CH             CtChannel0   ///< Use CT Ch.0
#define  CT_PWC_TIMER_SIZE     PwcSize32Bit ///< Use 32bit PWC function

#define  MEASURE_CNT_MAX       (8)      ///< The times of trimming process

/*External OSC Freq. (Mhz) */
#define EXT_OSC_FREQ_MHZ        4
#define PLLMULTX                8       ///< Main PLL Multiply factor
#define HICR_FREQ               4       ///< Target Trimming frequency
#define HCR_FREQ_INIT_VAL       0x80    ///< Default value of freq. trimming data
#define HCR_TEMP_DEFAULT_VAL    0x06    ///< Default value of temp. trimming data

#define HICR_OUTPUT_DEBUG               ///< Output HiCR Clock via ICRO_0

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/* Array to store the measure result */
uint32_t m_aMeasureResult[MEASURE_CNT_MAX];

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void PwcOverflowIntHandler(void);
static void PwcMeasCmpIrqHandler(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static volatile uint32_t m_u32CntIntMeasure;
static volatile uint32_t m_u32CntIntOverflow;
static volatile uint32_t m_u32CntMeasureErr;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief PWC overflow interrupt handler
 **
 ******************************************************************************/
static void PwcOverflowIntHandler(void)
{
    m_u32CntIntOverflow++;
}

/**
 *******************************************************************************
 **  \brief PWC measure complete interrupt handler
 **
 ******************************************************************************/
static void PwcMeasCmpIrqHandler(void)
{
    uint32_t u32CntIntOverflow;
    Ct_Pwc_DisableCount(CT_PWC_CH);
    /*<<< user code here  */
    if (m_u32CntIntMeasure < MEASURE_CNT_MAX)
    {
        if (CT_PWC_TIMER_SIZE == PwcSize16Bit)
        {
            u32CntIntOverflow = (m_u32CntIntOverflow * 0xffff);
            m_aMeasureResult[m_u32CntIntMeasure] = Ct_Pwc_Get16BitMeasureData(CT_PWC_CH) + u32CntIntOverflow;
        }else /*CT_32IT_TIMER  */
        {
            m_aMeasureResult[m_u32CntIntMeasure] = Ct_Pwc_Get32BitMeasureData(CT_PWC_CH);
        }
        m_u32CntIntMeasure++;
    }else
    {
        if (CT_PWC_TIMER_SIZE == PwcSize16Bit)
        {
            Ct_Pwc_Get16BitMeasureData(CT_PWC_CH);
        }else
        {
            Ct_Pwc_Get32BitMeasureData(CT_PWC_CH);
        }
    }
    m_u32CntIntOverflow = 0;
}

/**
 *******************************************************************************
 ** \brief  High CR trimming process
 **
 ** \param  [in] u8CrTempTrimDefaultVal Temperature trimming data
 **                                     default value: 0x05
 ** \param  [out] u8pCrFreqTrimVal      Pointer to final Freq. trimming data
 **
 ** \retval Ok
 **
 ******************************************************************************/
static uint8_t Osc_CrTrimProcess(uint8_t u8CrTempTrimDefaultVal,
                                 uint8_t *u8pCrFreqTrimVal)
{
    static en_TrimState_t enTrimState = TRIM_PROC_INIT;
    stc_ct_pwc_config_t stcPwcConfig;
    stc_pwc_irq_en_t stcPwcIrqEn;
    stc_pwc_irq_cb_t stcPwcIrqCallback;

    uint8_t u8Count = 0;
    uint8_t u8TmpIdx;
    uint32_t u32TmpAbs;
    uint8_t u8CrFreqTrimInitValue = HCR_FREQ_INIT_VAL;
    uint32_t u32ExpPwcVal = 0;
    stc_Cr_Trim_Matrix_t a_stcCrTrimMatrix[8];
    a_stcCrTrimMatrix[0].u8Idx = 0;
    a_stcCrTrimMatrix[0].u8CrFreqTrimVal = u8CrFreqTrimInitValue;
    while (1)
    {
        switch (enTrimState)
        {
            case TRIM_PROC_INIT:
                Hicr_SetFreqDiv(HicrFreqDivBy512);  /* Divide HCR */
                M0P_GPIO->FN_SEL02_f.TIOB0S = 7u;   /* HCR output to CT ch.0 */

                Ct_ConfigIOMode(CT_PWC_CH, CtIoMode0);

                /* Clear structures */
                DDL_ZERO_STRUCT(stcPwcConfig);
                DDL_ZERO_STRUCT(stcPwcIrqEn);
                DDL_ZERO_STRUCT(stcPwcIrqCallback);

                /* PWC register initialization */
                stcPwcConfig.enPres = PwcPresNone;
                stcPwcConfig.enMode = PwcContinuous;
                stcPwcConfig.enMeasureEdge = PwcMeasureRisingToRising;
                stcPwcConfig.enSize = CT_PWC_TIMER_SIZE;
                stcPwcConfig.bTouchNvic = TRUE;

                /* enable PWC interrupt */
                stcPwcIrqEn.bPwcMeasureCompleteIrq = TRUE;
                stcPwcIrqEn.bPwcMeasureOverflowIrq = TRUE;
                stcPwcIrqCallback.pfnPwcMeasureCompleteIrqCb = PwcMeasCmpIrqHandler;
                stcPwcIrqCallback.pfnPwcMeasureOverflowIrqCb = PwcOverflowIntHandler;

                /* Initialize Pointer to interrupt request structure */
                stcPwcConfig.pstcPwcIrqEn = &stcPwcIrqEn;
                stcPwcConfig.pstcPwcIrqCb = &stcPwcIrqCallback;

                /* PWC initalize */
                Ct_Pwc_Init(CT_PWC_CH, &stcPwcConfig);
                enTrimState++;
                break;
            case TRIM_PROC_START:
                /* Set default CR temperature trimming value */
                Hicr_Set4MTempTrimmingData(u8CrTempTrimDefaultVal);
                /* Set Initial CR freq. trimming value */
                Hicr_Set4MFreqTrimmingData(u8CrFreqTrimInitValue);
                /* freq. mult * HICR div */
                u32ExpPwcVal = ((PLLMULTX * EXT_OSC_FREQ_MHZ) / HICR_FREQ) * (2 << (Hicr_GetFreqDiv() + 1));
                delay1ms(1000);
                /* Enable count operatoin */
                Ct_Pwc_EnableCount(CT_PWC_CH);
                delay1ms(1000);
                enTrimState++;
                break;
            case TRIM_PROC_ONGOING_BIT6:
            case TRIM_PROC_ONGOING_BIT5:
            case TRIM_PROC_ONGOING_BIT4:
            case TRIM_PROC_ONGOING_BIT3:
            case TRIM_PROC_ONGOING_BIT2:
            case TRIM_PROC_ONGOING_BIT1:
            case TRIM_PROC_ONGOING_BIT0:
                if (m_aMeasureResult[u8Count] < u32ExpPwcVal)       /* CR freq. < the expected value */
                {
                    u8CrFreqTrimInitValue |= 1u << (6u - u8Count);
                }else if (m_aMeasureResult[u8Count] > u32ExpPwcVal) /* CR freq. > the expected value */
                {
                    u8CrFreqTrimInitValue &= ~(1u << (7u - u8Count));
                    u8CrFreqTrimInitValue |= 1u << (6u - u8Count);
                }else
                {
                    __ASM("nop");
                }
                /* Record current value */
                a_stcCrTrimMatrix[u8Count + 1].u8Idx = u8Count + 1;
                a_stcCrTrimMatrix[u8Count + 1].u8CrFreqTrimVal = u8CrFreqTrimInitValue;
                /* Calculate ABS of last time */
                a_stcCrTrimMatrix[u8Count].u32CalAbs = abs(u32ExpPwcVal - m_aMeasureResult[u8Count]);
                /* Set trim register */
                Hicr_Set4MFreqTrimmingData(u8CrFreqTrimInitValue);
                /* Enable count operatoin   */
                Ct_Pwc_EnableCount(CT_PWC_CH);
                delay1ms(1000);
                u8Count++;
                enTrimState++;
                break;
            case TRIM_PROC_FINISH:
                break;
            default:
                break;
        }
        if (TRIM_PROC_FINISH == enTrimState)
        {
            a_stcCrTrimMatrix[u8Count].u32CalAbs = abs(u32ExpPwcVal - m_aMeasureResult[u8Count]);
            u32TmpAbs = a_stcCrTrimMatrix[0].u32CalAbs;
            for (u8Count = 0; u8Count < 7; u8Count++)
            {
                if (a_stcCrTrimMatrix[u8Count].u32CalAbs < u32TmpAbs)
                {
                    u32TmpAbs = a_stcCrTrimMatrix[u8Count].u32CalAbs;
                    u8TmpIdx = a_stcCrTrimMatrix[u8Count].u8Idx;
                }
            }
            *u8pCrFreqTrimVal = a_stcCrTrimMatrix[u8TmpIdx].u8CrFreqTrimVal;
            return Ok;
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Main function of CR trim sample for HC32F_M14X series.
 **
 ** \note This sample uses CT PWC function to trimming internal HiCR frequency
 **       precision into +-2%
 **
 ******************************************************************************/
int32_t main(void)
{
    uint8_t u8FreqTrimVal;
    stc_clk_config_t stcClkCfg;
    uint8_t u8CrTempTrimDefaultVal = HCR_TEMP_DEFAULT_VAL;
    uint32_t u32Udr0;

    m_u32CntIntMeasure = 0;
    m_u32CntIntOverflow = 0;
    m_u32CntMeasureErr = 0;

    Clk_DisableFunc(ClkFuncHSXTCfdRest);
    Clk_DisableFunc(ClkFuncHSXTCfd);
    DDL_ZERO_STRUCT(stcClkCfg);
    stcClkCfg.bAPB1En = TRUE;
    stcClkCfg.bMainEn = TRUE;
    stcClkCfg.bPllEn = TRUE;
    stcClkCfg.enClkSrc = ClkMPLL;
    stcClkCfg.enClkWaitPll = ClkPswtPll680us;
    stcClkCfg.enPllSrc = MAIN2PLL;
    stcClkCfg.u8PllMulti = PLLMULTX;
    stcClkCfg.bPLLIrq = FALSE;

    Clk_Init(&stcClkCfg);

    SystemCoreClock = Clk_GetSystemClk();

    Osc_CrTrimProcess(u8CrTempTrimDefaultVal, &u8FreqTrimVal);

#ifdef HICR_OUTPUT_DEBUG
    /* write to NVR */
    Flash_PgmTimerConfig(4u);
    Flash_PageErase(0x100000u);
    Flash_UdrRead(FUDR0, (uint32_t*)&u32Udr0);
    u32Udr0 &= 0xFF0000FFu;
    u32Udr0 |= (uint32_t)u8FreqTrimVal << 16u;
    u32Udr0 |= HCR_TEMP_DEFAULT_VAL << 8u;
    Flash_WriteWord(0x100004u, u32Udr0);

    /* set trim reg and output */
    Hicr_Set4MFreqTrimmingData(u8FreqTrimVal);
    Gpio_SetFunc_IRCO_0(0u);
#endif
    while (1u)
    {
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
