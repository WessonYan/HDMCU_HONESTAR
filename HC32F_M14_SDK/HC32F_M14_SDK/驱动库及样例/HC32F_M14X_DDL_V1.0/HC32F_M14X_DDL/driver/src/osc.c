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
/** \file osc.c
 **
 ** A detailed description is available at
 ** @link OscGroup Osc description @endlink
 **
 **   - 2017-6-10  1.0  Zhangxl  First version for Device Driver Library of Osc
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "osc.h"

/**
 *******************************************************************************
 ** \addtogroup OscGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

 /******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! Parameter valid check for HICR division */
#define IS_VALID_HCRDIV(x)              \
(   HicrFreqDivBy4 == (x)   ||          \
    HicrFreqDivBy8 == (x)   ||          \
    HicrFreqDivBy16 == (x)  ||          \
    HicrFreqDivBy32 == (x)  ||          \
    HicrFreqDivBy64 == (x)  ||          \
    HicrFreqDivBy128 == (x) ||          \
    HicrFreqDivBy256 == (x) ||          \
    HicrFreqDivBy512 == (x))

/*! Parameter valid check for external osc input range */
#define IS_VALID_RANGE(x)               \
(   R4M6M == (x)    ||                  \
    R6M12M == (x)   ||                  \
    R12M16M == (x))

#define HICR_REG_UNLOCK_CODE    0x2BEC58A4u ///< HiCR control register unlock key

#define HICR_REG_LOCK_CODE      0u          ///< HiCR control register lock key

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
 ** \brief   Set the frequency division of HICR output to Composite timer
 **
 ** \param   [in]  enHicrDiv            HICR division
 ** \arg           HicrFreqDivBy4
 ** \arg           HicrFreqDivBy8
 ** \arg           HicrFreqDivBy16
 ** \arg           HicrFreqDivBy32
 ** \arg           HicrFreqDivBy64
 ** \arg           HicrFreqDivBy128
 ** \arg           HicrFreqDivBy256
 ** \arg           HicrFreqDivBy512
 **
 ** \retval  Ok                       Set the frequency division successfully
 ** \retval  ErrorInvalidParameter    enHicrDiv larger than HicrFreqDivBy512
 **
 **
 ******************************************************************************/
en_result_t Hicr_SetFreqDiv(en_hicr_freq_div_t enHicrDiv)
{
    ASSERT(IS_VALID_HCRDIV(enHicrDiv));

    switch (enHicrDiv)
    {
        case HicrFreqDivBy4:
            M0P_OSC_CTL->HICR_FDIV = 0u;
            break;
        case HicrFreqDivBy8:
            M0P_OSC_CTL->HICR_FDIV = 1u;
            break;
        case HicrFreqDivBy16:
            M0P_OSC_CTL->HICR_FDIV = 2u;
            break;
        case HicrFreqDivBy32:
            M0P_OSC_CTL->HICR_FDIV = 3u;
            break;
        case HicrFreqDivBy64:
            M0P_OSC_CTL->HICR_FDIV = 4u;
            break;
        case HicrFreqDivBy128:
            M0P_OSC_CTL->HICR_FDIV = 5u;
            break;
        case HicrFreqDivBy256:
            M0P_OSC_CTL->HICR_FDIV = 6u;
            break;
        case HicrFreqDivBy512:
            M0P_OSC_CTL->HICR_FDIV = 7u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}


/**
 *******************************************************************************
 ** \brief   Get the frequency division of HICR output to Composite timer
 **
 ** \param   none
 **
 ** \retval  en_hicr_freq_div_t         Get the frequency division of HICR
 **
 **
 ******************************************************************************/
en_hicr_freq_div_t Hicr_GetFreqDiv(void)
{
    return (en_hicr_freq_div_t)(M0P_OSC_CTL->HICR_FDIV);
}

/**
 *******************************************************************************
 ** \brief   Set 4MHz HICR frequency trimming register
 **
 ** \param   [in]  u8FTrimm             frequency trimming value
 **
 **
 ******************************************************************************/
void Hicr_Set4MFreqTrimmingData(uint8_t u8FTrimm)
{
    M0P_OSC_CTL->HICR_TRMKEY = HICR_REG_UNLOCK_CODE;
    M0P_OSC_CTL->HICR_FTRM_f.CR4MFTRM = u8FTrimm;
    M0P_OSC_CTL->HICR_TRMKEY = HICR_REG_LOCK_CODE;
}

/**
 *******************************************************************************
 ** \brief   Get 4MHz HICR frequency trimming register
 **
 ** \retval  frequency trimming value, only Bit[8:0] is valid.
 **
 ******************************************************************************/
uint16_t Hicr_Get4MFreqTrimmingData(void)
{
    return (uint16_t)(M0P_OSC_CTL->HICR_FTRM_f.CR4MFTRM & 0x01FFu);
}

/**
 *******************************************************************************
 ** \brief   Set 4MHz HICR temperature trimming register
 **
 ** \param   [in]  u16Data            temperature trimming value, only Bit[3:0]
 **                                   is valid.
 **
 ******************************************************************************/
void Hicr_Set4MTempTrimmingData(uint8_t u8TTrim)
{
    M0P_OSC_CTL->HICR_TRMKEY = HICR_REG_UNLOCK_CODE;
    M0P_OSC_CTL->HICR_TTRM_f.CR4MTTRM = u8TTrim & 0x0Fu;
    M0P_OSC_CTL->HICR_TRMKEY = HICR_REG_LOCK_CODE;
}

/**
 *******************************************************************************
 ** \brief   Get 4MHz HICR temperature trimming register
 **
 ** \retval  temperature trimming value, only Bit[3:0] is valid.
 **
 ******************************************************************************/
uint8_t Hicr_Get4MTempTrimmingData(void)
{
    return (uint8_t)(M0P_OSC_CTL->HICR_TTRM_f.CR4MTTRM & 0x0Fu);
}

/**
 *******************************************************************************
 ** \brief Initialize OSC
 **
 ** \param [in] pstcConfig   Pointer to Osc configuration
 **
 ** \retval Ok                      Setup successful
 ** \retval ErrorInvalidParameter   pstcConfig == NULL
 **
 ** \note This function only initializes the Osc configuration.
 **       Including frequency of HICR, settings of main OSC & Sub OSC.
 **
 ******************************************************************************/
en_result_t Osc_Init(stc_osc_config_t* pstcConfig)
{
    en_result_t enResult;
    enResult = ErrorInvalidParameter;

    ASSERT(pstcConfig != NULL);
    ASSERT(IS_VALID_RANGE(pstcConfig->enHxstRange));

    if (NULL != pstcConfig)
    {
        enResult = Ok;
        M0P_OSC_CTL->HSXT_PWRCTL_f.HIBS = pstcConfig->u8HxstBias;
        M0P_OSC_CTL->HSXT_PWRCTL_f.HXSEL = pstcConfig->enHxstRange;
        M0P_OSC_CTL->LSXT_PWRCTL_f.LIBS = pstcConfig->u8LxstBias;
    }
    return (enResult);
}

/**
 *******************************************************************************
 ** \brief De-Initialize OSC
 **
 ** Retval  Ok                      Process successful
 **
 ******************************************************************************/
en_result_t Osc_DeInit(void)
{
    M0P_OSC_CTL->HICR_TRMKEY = HICR_REG_UNLOCK_CODE;
    M0P_OSC_CTL->HICR_FTRM = 0x00000064u;
    M0P_OSC_CTL->HICR_TTRM = 0x00000005u;
    M0P_OSC_CTL->HICR_FDIV_f.FDIV = 1u;
    M0P_OSC_CTL->HSXT_PWRCTL_f.HIBS = 0u;
    M0P_OSC_CTL->HSXT_PWRCTL_f.HXSEL = 0u;
    M0P_OSC_CTL->LSXT_PWRCTL_f.LIBS = 3u;
    M0P_OSC_CTL->HICR_TRMKEY = HICR_REG_LOCK_CODE;
    return (Ok);
}

//@} // OscGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
