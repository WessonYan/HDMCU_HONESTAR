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
/** \file timer4_adct.c
 **
 ** A detailed description is available at
 ** @link Timer4AdctGroup Timer4Adct description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Adct
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "timer4_adct.h"

/**
 *******************************************************************************
 ** \addtogroup Timer4AdctGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_POINTER(x)  (NULL != (x))  ///< Parameter validity check for pointer

/*! Parameter validity check for adct channel  */
#define IS_VALID_CH(x)                  \
(   (TIMER4_ADCT_CH0 == (x)) ||         \
    (TIMER4_ADCT_CH1 == (x)) ||         \
    (TIMER4_ADCT_CH2 == (x)) ||         \
    (TIMER4_ADCT_CH3 == (x)) ||         \
    (TIMER4_ADCT_CH4 == (x)) ||         \
    (TIMER4_ADCT_CH5 == (x)))

/*! Parameter validity check for adct couple channel  */
#define IS_VALID_COUPLE_CH(x)           \
(   (TIMER4_ADCT_COUPLE_CH10 == (x)) || \
    (TIMER4_ADCT_COUPLE_CH32 == (x)) || \
    (TIMER4_ADCT_COUPLE_CH54 == (x)))

/*! Parameter validity check for adct buffer mode  */
#define IS_VALID_BUF_MODE(x)            \
(   (AdctBufDisable == (x))     ||      \
    (AdctBufCntZero == (x))     ||      \
    (AdctBufCntZeroPeak == (x)) ||      \
    (AdctBufCntPeak == (x))     ||      \
    (AdctBufCntZicZero  == (x)) ||      \
    (AdctBufCntPicZero  == (x)) ||      \
    (AdctBufCntZicZeroOrCntPicZero == (x)))

/*! Parameter validity check for adct trigger unit  */
#define IS_VALID_TRIGGER_UNIT(x)        \
(   (AdctTrigAdc0Scan == (x)) ||        \
    (AdctTrigAdc0Prio == (x)) ||        \
    (AdctTrigAdc1Scan == (x)) ||        \
    (AdctTrigAdc1Prio == (x)) ||        \
    (AdctTrigAdc2Scan == (x)) ||        \
    (AdctTrigAdc2Prio == (x)))

/*! Parameter validity check for adct mode  */
#define IS_VALID_ADCT_MODE(x)           \
(   (AdctCmpTrigMode == (x)) ||         \
    (AdctDelayTrigMode == (x)))

/*! Parameter validity check for adct occr select  */
#define IS_VALID_OCCR_SEL(x)            \
(   (AdctSelOccr0 == (x)) ||            \
    (AdctSelOccr1 == (x)))

/*! Parameter validity check for enable flag  */
#define IS_VALID_EN(x)                  \
(   (TRUE == (x)) ||                    \
    (FALSE == (x)))

#define IS_VALID_AMC_VAL(x)  ((x) < 16u)    ///< Parameter validity check for adct compare value

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
 ** \brief Device dependent initialization of Timer4 adct module
 **
 ** \param [in] u8Ch              Timer4 adct channel
 ** \arg TIMER4_ADCT_CH0          ADCT channel 0
 ** \arg TIMER4_ADCT_CH1          ADCT channel 1
 ** \arg TIMER4_ADCT_CH2          ADCT channel 2
 ** \arg TIMER4_ADCT_CH3          ADCT channel 3
 ** \arg TIMER4_ADCT_CH4          ADCT channel 4
 ** \arg TIMER4_ADCT_CH5          ADCT channel 5
 ** \param [in] pstcConfig        Pointer to Timer4 adct config
 ** \arg see #stc_timer4_adct_config_t
 **
 ** \retval Ok                    Adct initialized
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcConfig == NULL
 **                               - u8Ch > TIMER4_ADCT_CH5
 **
 ******************************************************************************/
en_result_t Timer4_Adct_Init(uint8_t u8Ch,
                            stc_timer4_adct_config_t* pstcConfig)
{
    volatile stc_timer4_atcr0_field_t* pstcATCR_f;
    volatile stc_timer4_ater0_field_t* pstcATER_f;
    volatile stc_timer4_atmr0_field_t* pstcATMR_f;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));
    ASSERT(IS_VALID_POINTER(pstcConfig));
    ASSERT(IS_VALID_BUF_MODE(pstcConfig->enBuf));
    ASSERT(IS_VALID_TRIGGER_UNIT(pstcConfig->enTrigAdcUnit));
    ASSERT(IS_VALID_ADCT_MODE(pstcConfig->enMode));
    ASSERT(IS_VALID_OCCR_SEL(pstcConfig->enOccrSel));
    ASSERT(IS_VALID_EN(pstcConfig->bCompareCntZeroMaskCntVal));
    ASSERT(IS_VALID_EN(pstcConfig->bCompareCntPeakMaskCntVal));
    ASSERT(IS_VALID_AMC_VAL(pstcConfig->u8CompareVal));

    if((NULL == pstcConfig) ||
       (u8Ch > TIMER4_ADCT_CH5))
    {
        return ErrorInvalidParameter;
    }

    /* Get actual address of register list of current channel */
    pstcATCR_f = (volatile stc_timer4_atcr0_field_t*)((volatile uint8_t*)&M0P_TIMER4->ATCR0 + u8Ch * 4u);
    pstcATER_f = (volatile stc_timer4_ater0_field_t*)((volatile uint8_t*)&M0P_TIMER4->ATER0 + u8Ch * 4u);
    pstcATMR_f = (volatile stc_timer4_atmr0_field_t*)((volatile uint8_t*)&M0P_TIMER4->ATMR0 + u8Ch * 4u);

    /* Configure default parameter */
    pstcATCR_f->ADTRGS = 0u;
    pstcATCR_f->BUFEN = 0u;
    pstcATCR_f->LMC = 0u;
    pstcATER_f->DEN = 0u;
    pstcATER_f->PEN = 0u;
    pstcATER_f->UEN = 0u;
    pstcATER_f->ZEN = 0u;
    pstcATER_f->OCS = 0u;
    pstcATER_f->TRGMD = 0u;
    pstcATMR_f->AMC =0u;
    pstcATMR_f->MPCE = 0u;
    pstcATMR_f->MZCE = 0u;

    switch (pstcConfig->enBuf)
    {
        case AdctBufDisable:
            pstcATCR_f->BUFEN = 0u;
            pstcATCR_f->LMC = 0u;
            break;
        case AdctBufCntZero:
            pstcATCR_f->BUFEN = 1u;
            pstcATCR_f->LMC = 0u;
            break;
        case AdctBufCntPeak:
            pstcATCR_f->BUFEN = 2u;
            pstcATCR_f->LMC = 0u;
            break;
        case AdctBufCntZeroPeak:
            pstcATCR_f->BUFEN = 3u;
            pstcATCR_f->LMC = 0u;
            break;
        case AdctBufCntZicZero:
            pstcATCR_f->BUFEN = 1u;
            pstcATCR_f->LMC = 1u;
            break;
        case AdctBufCntPicZero:
            pstcATCR_f->BUFEN = 2u;
            pstcATCR_f->LMC = 1u;
            break;
        case AdctBufCntZicZeroOrCntPicZero:
            pstcATCR_f->BUFEN = 3u;
            pstcATCR_f->LMC = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* Configure start trigger output channel number */
    switch (pstcConfig->enTrigAdcUnit)
    {
        case AdctTrigAdc0Scan:
            pstcATCR_f->ADTRGS = 0u;
            break;
        case AdctTrigAdc0Prio:
            pstcATCR_f->ADTRGS = 1u;
            break;
        case AdctTrigAdc1Scan:
            pstcATCR_f->ADTRGS = 2u;
            break;
        case AdctTrigAdc1Prio:
            pstcATCR_f->ADTRGS = 3u;
            break;
        case AdctTrigAdc2Scan:
            pstcATCR_f->ADTRGS = 4u;
            break;
        case AdctTrigAdc2Prio:
            pstcATCR_f->ADTRGS = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* Select ADCT running mode */
    switch (pstcConfig->enMode)
    {
        case AdctCmpTrigMode:
            pstcATER_f->TRGMD = 0u;
            break;
        case AdctDelayTrigMode:
            pstcATER_f->TRGMD = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* select OCO OCCR register: OCCR(x) */
    switch (pstcConfig->enOccrSel)
    {
        case AdctSelOccr0:
            pstcATER_f->OCS = 0u;
            break;
        case AdctSelOccr1:
            pstcATER_f->OCS = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    pstcATMR_f->AMC =0u;

    /* Set the comparison with CNT interrupt mask counter */
    pstcATMR_f->MZCE = ((TRUE == pstcConfig->bCompareCntZeroMaskCntVal) ? 1u : 0u);
    pstcATMR_f->MPCE = ((TRUE == pstcConfig->bCompareCntPeakMaskCntVal) ? 1u : 0u);
    pstcATMR_f->AMC = pstcConfig->u8CompareVal;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Device dependent De-initialization of Timer4 adct module
 **
 ** \param [in] pstcTimer4Adct    Pointer to Timer4 instance
 ** \param [in] u8Ch              Timer4 adct channel
 ** \arg TIMER4_ADCT_CH0          ADCT channel 0
 ** \arg TIMER4_ADCT_CH1          ADCT channel 1
 ** \arg TIMER4_ADCT_CH2          ADCT channel 2
 ** \arg TIMER4_ADCT_CH3          ADCT channel 3
 ** \arg TIMER4_ADCT_CH4          ADCT channel 4
 ** \arg TIMER4_ADCT_CH5          ADCT channel 5
 **
 ** \retval Ok                    Adct De-initialized
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                               - u8Ch > TIMER4_ADCT_CH5
 **
 ******************************************************************************/
en_result_t Timer4_Adct_DeInit(uint8_t u8Ch)
{
    volatile stc_timer4_atcr0_field_t* pstcATCR_f;
    volatile stc_timer4_ater0_field_t* pstcATER_f;
    volatile stc_timer4_atmr0_field_t* pstcATMR_f;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_ADCT_CH5)
    {
        return ErrorInvalidParameter;
    }

    /* Get actual address of register list of current channel */
    pstcATCR_f = (volatile stc_timer4_atcr0_field_t*)((volatile uint8_t*)&M0P_TIMER4->ATCR0 + u8Ch * 4u);
    pstcATER_f = (volatile stc_timer4_ater0_field_t*)((volatile uint8_t*)&M0P_TIMER4->ATER0 + u8Ch * 4u);
    pstcATMR_f = (volatile stc_timer4_atmr0_field_t*)((volatile uint8_t*)&M0P_TIMER4->ATMR0 + u8Ch * 4u);

    /* Configure default parameter */
    pstcATCR_f->ADTRGS = 0u;
    pstcATCR_f->BUFEN = 0u;
    pstcATCR_f->LMC = 0u;
    pstcATER_f->DEN = 0u;
    pstcATER_f->PEN = 0u;
    pstcATER_f->UEN = 0u;
    pstcATER_f->ZEN = 0u;
    pstcATER_f->OCS = 0u;
    pstcATER_f->TRGMD = 0u;
    pstcATMR_f->AMC = 0u;
    pstcATMR_f->MPCE = 0u;
    pstcATMR_f->MZCE = 0u;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable Timer4 Adct operations
 **
 ** \param [in] u8Ch              Timer4 adct channel
 ** \arg TIMER4_ADCT_CH0          ADCT channel 0
 ** \arg TIMER4_ADCT_CH1          ADCT channel 1
 ** \arg TIMER4_ADCT_CH2          ADCT channel 2
 ** \arg TIMER4_ADCT_CH3          ADCT channel 3
 ** \arg TIMER4_ADCT_CH4          ADCT channel 4
 ** \arg TIMER4_ADCT_CH5          ADCT channel 5
 ** \param [in] pstcTrigCond      Pointer to Timer4 Adct function structure
 **
 ** \retval Ok                    Functions enabled
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                               - pstcTrigCond == NULL
 **                               - u8Ch >  TIMER4_ADCT_CH5 ADCT channel 5
 **
 ******************************************************************************/
en_result_t Timer4_Adct_EnableOperation(uint8_t u8Ch,
                                      stc_timer4_adct_trig_cond_t* pstcTrigCond)
{
    volatile stc_timer4_ater0_field_t* pstcATER_f;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));
    ASSERT(IS_VALID_POINTER(pstcTrigCond));

    if((u8Ch > TIMER4_ADCT_CH5) ||
       (NULL == pstcTrigCond))
    {
        return ErrorInvalidParameter;
    }

    /* Get actual address of register list of current channel */
    pstcATER_f = (volatile stc_timer4_ater0_field_t*)((volatile uint8_t*)&M0P_TIMER4->ATER0 + u8Ch * 4u);

    pstcATER_f->PEN = (TRUE == pstcTrigCond->bPeakEn) ? 1u:0u;
    pstcATER_f->ZEN = (TRUE == pstcTrigCond->bZeroEn) ? 1u:0u;
    pstcATER_f->UEN = (TRUE == pstcTrigCond->bUpEn)   ? 1u:0u;
    pstcATER_f->DEN = (TRUE == pstcTrigCond->bDownEn) ? 1u:0u;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable Timer4 Adct operations
 **
 ** \param [in] pstcTimer4Adct    Pointer to Timer4 instance
 ** \param [in] u8Ch              Timer4 adct channel
 ** \arg TIMER4_ADCT_CH0          ADCT channel 0
 ** \arg TIMER4_ADCT_CH1          ADCT channel 1
 ** \arg TIMER4_ADCT_CH2          ADCT channel 2
 ** \arg TIMER4_ADCT_CH3          ADCT channel 3
 ** \arg TIMER4_ADCT_CH4          ADCT channel 4
 ** \arg TIMER4_ADCT_CH5          ADCT channel 5
 ** \param [in] pstcTrigCond      Pointer to Timer4 Adct function structure
 **
 ** \retval Ok                    Timer4 Adct operations disabled
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - pstcTrigCond == NULL
 **                               - u8Ch > TIMER4_ADCT_CH5 ADCT channel 5
 **
 ******************************************************************************/
en_result_t Timer4_Adct_DisableOperation(uint8_t u8Ch,
                                       stc_timer4_adct_trig_cond_t* pstcTrigCond)
{
    volatile stc_timer4_ater0_field_t* pstcATER_f;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));
    ASSERT(IS_VALID_POINTER(pstcTrigCond));

    if((u8Ch > TIMER4_ADCT_CH5) ||
       (NULL == pstcTrigCond))
    {
        return ErrorInvalidParameter;
    }

    /* Get actual address of register list of current channel */
    pstcATER_f = (volatile stc_timer4_ater0_field_t*)((volatile uint8_t*)&M0P_TIMER4->ATER0 + u8Ch * 4u);

    if(TRUE == pstcTrigCond->bPeakEn)
    {
        pstcATER_f->PEN = 0u;
    }
    if(TRUE == pstcTrigCond->bZeroEn)
    {
        pstcATER_f->ZEN = 0u;
    }
    if(TRUE == pstcTrigCond->bUpEn)
    {
        pstcATER_f->UEN = 0u;
    }
    if(TRUE == pstcTrigCond->bDownEn)
    {
        pstcATER_f->DEN = 0u;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Write compare or delay value to ADCT
 **
 ** \param [in] u8Ch              Timer4 adct channel
 ** \arg TIMER4_ADCT_CH0          ADCT channel 0
 ** \arg TIMER4_ADCT_CH1          ADCT channel 1
 ** \arg TIMER4_ADCT_CH2          ADCT channel 2
 ** \arg TIMER4_ADCT_CH3          ADCT channel 3
 ** \arg TIMER4_ADCT_CH4          ADCT channel 4
 ** \arg TIMER4_ADCT_CH5          ADCT channel 5
 ** \param [in] u16AdctVal ADCT compare value
 **
 ** \retval Ok                    Compare or delay value to ADCT is set
 ** \retval ErrorInvalidParameter     If one of following condiitons are met:
 **                               - u8Ch > TIMER4_ADCT_CH5 ADCT channel 5
 **
 ******************************************************************************/
en_result_t Timer4_Adct_WriteAtvr(uint8_t u8Ch, uint16_t u16AdctVal)
{
    volatile uint16_t* pu16Atvr;

    /* check parameters */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_ADCT_CH5)
    {
        return ErrorInvalidParameter;
    }

    pu16Atvr = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->ATVR0 + u8Ch*4u);
    *pu16Atvr =  u16AdctVal;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Read compare value or delay value of ATVR
 **
 ** \param [in] u8Ch              Timer4 adct channel
 ** \arg TIMER4_ADCT_CH0          ADCT channel 0
 ** \arg TIMER4_ADCT_CH1          ADCT channel 1
 ** \arg TIMER4_ADCT_CH2          ADCT channel 2
 ** \arg TIMER4_ADCT_CH3          ADCT channel 3
 ** \arg TIMER4_ADCT_CH4          ADCT channel 4
 ** \arg TIMER4_ADCT_CH5          ADCT channel 5
 **
 ** \retval  Value of register ATVR
 **
 ******************************************************************************/
uint16_t Timer4_Adct_ReadAtvr(uint8_t u8Ch)
{
    uint16_t u16Data;
    volatile uint16_t* pu16Atvr;

    /* check parameters */
    ASSERT(IS_VALID_CH(u8Ch));

    pu16Atvr =  (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->ATVR0 + u8Ch*4u);
    u16Data = *pu16Atvr;

    return u16Data;
}

//@} // Timer4AdctGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
