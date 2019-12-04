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
/** \file timer4_oco.c
 **
 ** A detailed description is available at
 ** @link Timer4OcoGroup Timer4Oco description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Oco
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "timer4_oco.h"

/**
 *******************************************************************************
 ** \addtogroup Timer4OcoGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_POINTER(x)  (NULL != (x))  ///< Parameter validity check for pointer

/*! Parameter validity check for oco channel  */
#define IS_VALID_CH(x)                  \
(   (TIMER4_OCO_CH0 == (x)) ||          \
    (TIMER4_OCO_CH1 == (x)) ||          \
    (TIMER4_OCO_CH2 == (x)) ||          \
    (TIMER4_OCO_CH3 == (x)) ||          \
    (TIMER4_OCO_CH4 == (x)) ||          \
    (TIMER4_OCO_CH5 == (x)))

/*! Parameter validity check for oco odd channel  */
#define IS_VALID_ODD_CH(x)              \
(   (IS_VALID_CH(x)) &&                 \
    ((x)%2))

/*! Parameter validity check for even odd channel  */
#define IS_VALID_EVEN_CH(x)             \
(   (IS_VALID_CH(x)) &&                 \
    (!IS_VALID_ODD_CH(x)))

/*! Parameter validity check for occr buffer mode  */
#define IS_VALID_OCCR_BUF_MODE(x)               \
(   (OccrBufDisable == (x))               ||    \
    (OccrBufTrsfByCntZero == (x))         ||    \
    (OccrBufTrsfByCntPeak == (x))         ||    \
    (OccrBufTrsfByCntZeroPeak == (x))     ||    \
    (OccrBufTrsfByCntZeroMszcZero == (x)) ||    \
    (OccrBufTrsfByCntPeakMspcZero == (x)) ||    \
    (OccrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero == (x)))

/*! Parameter validity check for ocmr buffer mode  */
#define IS_VALID_OCMR_BUF_MODE(x)               \
(   (OcmrBufDisable == (x))               ||    \
    (OcmrBufTrsfByCntZero == (x))         ||    \
    (OcmrBufTrsfByCntPeak == (x))         ||    \
    (OcmrBufTrsfByCntZeroPeak == (x))     ||    \
    (OcmrBufTrsfByCntZeroMszcZero == (x)) ||    \
    (OcmrBufTrsfByCntPeakMspcZero == (x)) ||    \
    (OcmrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero == (x)))

/*! Parameter validity check for output level type  */
#define IS_VALID_OP_OUT_STATE(x)        \
(   (OpLowLevel == (x)) ||              \
    (OpHighLevel == (x)))

/*! Parameter validity check for enable flag  */
#define IS_VALID_EN(x)                  \
(   (TRUE == (x)) ||                    \
    (FALSE == (x)))

/*! Parameter validity check for oco even channel compare mode  */
#define IS_VALID_EVEN_CH_CMP_MODE(x)                                    \
(   (NULL != (x))                                                 &&    \
    ((x->enCntZeroEvenNotMatchEvenChOpStatus) <= OpOutputReverse) &&    \
    ((x->enCntZeroEvenMatchEvenChOpStatus) <= OpOutputReverse)    &&    \
    ((x->enCntUpCntEvenMatchEvenChOpStatus) <= OpOutputReverse)   &&    \
    ((x->enCntPeakEvenMatchEvenChOpStatus) <= OpOutputReverse)    &&    \
    ((x->enCntPeakEvenNotMatchEvenChOpStatus) <= OpOutputReverse) &&    \
    ((x->enCntDownCntEvenMatchEvenChOpStatus) <= OpOutputReverse) &&    \
    ((x->enOcfWhenCntZeroEvenMatch) <= OcfSet)                    &&    \
    ((x->enOcfWhenCntUpCntEvenMatch) <= OcfSet)                   &&    \
    ((x->enOcfWhenCntPeakEvenMatch) <= OcfSet)                    &&    \
    ((x->enOcfWhenCntDownCntEvenMatch) <= OcfSet))

/*! Parameter validity check for oco odd channel compare mode  */
#define IS_VALID_ODD_CH_CMP_MODE(x)                                             \
(   (NULL != (x))                                                           &&  \
    ((x->enCntZeroOddMatchEvenMatchOddChOpStatus) <= OpOutputReverse)       &&  \
    ((x->enCntZeroOddMatchEvenNotMatchOddChOpStatus) <= OpOutputReverse)    &&  \
    ((x->enCntZeroOddNotMatchEvenMatchOddChOpStatus) <= OpOutputReverse)    &&  \
    ((x->enCntZeroOddNotMatchEvenNotMatchOddChOpStatus) <= OpOutputReverse) &&  \
    ((x->enCntUpCntOddMatchEvenMatchOddChOpStatus) <= OpOutputReverse)      &&  \
    ((x->enCntUpCntOddMatchEvenNotMatchOddChOpStatus) <= OpOutputReverse)   &&  \
    ((x->enCntUpCntOddNotMatchEvenMatchOddChOpStatus) <= OpOutputReverse)   &&  \
    ((x->enCntPeakOddMatchEvenMatchOddChOpStatus) <= OpOutputReverse)       &&  \
    ((x->enCntPeakOddMatchEvenNotMatchOddChOpStatus) <= OpOutputReverse)    &&  \
    ((x->enCntPeakOddNotMatchEvenMatchOddChOpStatus) <= OpOutputReverse)    &&  \
    ((x->enCntPeakOddNotMatchEvenNotMatchOddChOpStatus) <= OpOutputReverse) &&  \
    ((x->enCntDownOddMatchEvenMatchOddChOpStatus) <= OpOutputReverse)       &&  \
    ((x->enCntDownOddMatchEvenNotMatchOddChOpStatus) <= OpOutputReverse)    &&  \
    ((x->enCntDownOddNotMatchEvenMatchOddChOpStatus) <= OpOutputReverse)    &&  \
    ((x->enOcfWhenCntZeroOddMatch) <= OcfSet)                               &&  \
    ((x->enOcfWhenCntUpCntOddMatch) <= OcfSet)                              &&  \
    ((x->enOcfWhenCntPeakOddMatch) <= OcfSet)                               &&  \
    ((x->enOcfWhenCntDownCntOddMatch) <= OcfSet))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Timer4OcoEnableNvic(void);
static void Timer4OcoDisableNvic(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_oco_irq_cb_t m_stcOcoIrqCb = {NULL, NULL, NULL, NULL, NULL, NULL};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
*******************************************************************************
** \brief Device dependent initialization of interrupts according CMSIS with
**        level defined in interrupt_xxx.h
**
******************************************************************************/
static void Timer4OcoEnableNvic(void)
{
    NVIC_ClearPendingIRQ(TIM4OCO_IRQn);
    NVIC_EnableIRQ(TIM4OCO_IRQn);
    NVIC_SetPriority(TIM4OCO_IRQn, DDL_IRQ_LEVEL_OCO05);
}

/**
 *******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ******************************************************************************/
static void Timer4OcoDisableNvic(void)
{
    NVIC_ClearPendingIRQ(TIM4OCO_IRQn);
    NVIC_DisableIRQ(TIM4OCO_IRQn);
    NVIC_SetPriority(TIM4OCO_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief  OCO interrupt handler function
 **
 ** \param [in] u8Ch              timer4 oco channel
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Oco_IrqHandler(uint8_t u8Ch)
{
    func_ptr_t *ppfnCallback = NULL;
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return;
    }

    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);
    if(0u == (u8Ch % 2u)) /* channel 0, 2, 4 */
    {
        pstcOCSR->OCF0 = 0u;
    }
    else
    {
         pstcOCSR->OCF1 = 0u;
    }

    ppfnCallback = &m_stcOcoIrqCb.pfnOco0IrqCallback + u8Ch;

    if(*ppfnCallback)
    {
        (*ppfnCallback)();
    }
}

/**
 *******************************************************************************
 ** \brief  Initialize OCO module
 **
 ** \param [in] u8Ch              channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 ** \param [in] pstcOcoConfig configure of OCO
 ** \arg see #stc_timer4_oco_config_t
 **
 ** \retval Ok                    OCO module initialized
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcOcuConfig == NULL
 **                               - u8Ch > TIMER4_OCO_MAX_CH
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Timer4_Oco_Init(uint8_t u8Ch, stc_timer4_oco_config_t* pstcOcoConfig)
{
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;
    volatile stc_timer4_ocer10_field_t* pstcOCER;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));
    ASSERT(IS_VALID_POINTER(pstcOcoConfig));
    ASSERT(IS_VALID_OCMR_BUF_MODE(pstcOcoConfig->enOcmrBufMode));
    ASSERT(IS_VALID_OCCR_BUF_MODE(pstcOcoConfig->enOccrBufMode));
    ASSERT(IS_VALID_OP_OUT_STATE(pstcOcoConfig->enPinState));
    ASSERT(IS_VALID_EN(pstcOcoConfig->bIrqEnable));
    ASSERT(IS_VALID_EN(pstcOcoConfig->bTouchNvic));

    if((u8Ch > TIMER4_OCO_MAX_CH) ||
       (NULL == pstcOcoConfig))
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);
    pstcOCER = (volatile stc_timer4_ocer10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCER10 + (u8Ch/2u)*4u);

    /* Set OCMR and OCCR buffer mode */
    if(0u == (u8Ch % 2u)) //channel 0, 2, 4 */
    {
        pstcOCSR->OCE0 = 0u;
        /* OCMR buffer */
        switch (pstcOcoConfig->enOcmrBufMode)
        {
            case OcmrBufDisable:
                pstcOCER->LMM0 = 0u;
                pstcOCER->M0BUFEN = 0u;
                break;
            case OcmrBufTrsfByCntZero:
                pstcOCER->LMM0 = 0u;
                pstcOCER->M0BUFEN = 1u;
                break;
            case OcmrBufTrsfByCntPeak:
                pstcOCER->LMM0 = 0u;
                pstcOCER->M0BUFEN = 2u;
                break;
            case OcmrBufTrsfByCntZeroPeak:
                pstcOCER->LMM0 = 0u;
                pstcOCER->M0BUFEN = 3u;
                break;
            case OcmrBufTrsfByCntZeroMszcZero:
                pstcOCER->LMM0 = 1u;
                pstcOCER->M0BUFEN = 1u;
                break;
            case OcmrBufTrsfByCntPeakMspcZero:
                pstcOCER->LMM0 = 1u;
                pstcOCER->M0BUFEN = 2u;
                break;
            case OcmrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero:
                pstcOCER->LMM0 = 1u;
                pstcOCER->M0BUFEN = 3u;
                break;
            default:
                return ErrorInvalidParameter;
        }

        /* OCCR buffer */
        switch (pstcOcoConfig->enOccrBufMode)
        {
            case OccrBufDisable:
                pstcOCER->LMC0 = 0u;
                pstcOCER->C0BUFEN = 0u;
                break;
            case OccrBufTrsfByCntZero:
                pstcOCER->LMC0 = 0u;
                pstcOCER->C0BUFEN = 1u;
                break;
            case OccrBufTrsfByCntPeak:
                pstcOCER->LMC0 = 0u;
                pstcOCER->C0BUFEN = 2u;
                break;
            case OccrBufTrsfByCntZeroPeak:
                pstcOCER->LMC0 = 0u;
                pstcOCER->C0BUFEN = 3u;
                break;
            case OccrBufTrsfByCntZeroMszcZero:
                pstcOCER->LMC0 = 1u;
                pstcOCER->C0BUFEN = 1u;
                break;
            case OccrBufTrsfByCntPeakMspcZero:
                pstcOCER->LMC0 = 1u;
                pstcOCER->C0BUFEN = 2u;
                break;
            case OccrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero:
                pstcOCER->LMC0 = 1u;
                pstcOCER->C0BUFEN = 3u;
                break;
            default:
                return ErrorInvalidParameter;
        }

        /* Set initial OP level */
        pstcOCSR->OCP0 = ((pstcOcoConfig->enPinState == OpHighLevel) ? 1u : 0u);
    }
    else/* channel 1, 3, 5 */
    {
        pstcOCSR->OCE1 = 0u;
        /* OCMR buffer */
        switch (pstcOcoConfig->enOcmrBufMode)
        {
            case OcmrBufDisable:
                pstcOCER->LMM1 = 0u;
                pstcOCER->M1BUFEN = 0u;
                break;
            case OcmrBufTrsfByCntZero:
                pstcOCER->LMM1 = 0u;
                pstcOCER->M1BUFEN = 1u;
                break;
            case OcmrBufTrsfByCntPeak:
                pstcOCER->LMM1 = 0u;
                pstcOCER->M1BUFEN = 2u;
                break;
            case OcmrBufTrsfByCntZeroPeak:
                pstcOCER->LMM1 = 0u;
                pstcOCER->M1BUFEN = 3u;
                break;
            case OcmrBufTrsfByCntZeroMszcZero:
                pstcOCER->LMM1 = 1u;
                pstcOCER->M1BUFEN = 1u;
                break;
            case OcmrBufTrsfByCntPeakMspcZero:
                pstcOCER->LMM1 = 1u;
                pstcOCER->M1BUFEN = 2u;
                break;
            case OcmrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero:
                pstcOCER->LMM1 = 1u;
                pstcOCER->M1BUFEN = 3u;
                break;
            default:
                return ErrorInvalidParameter ;
        }

        /* OCCR buffer */
        switch (pstcOcoConfig->enOccrBufMode)
        {
            case OccrBufDisable:
                pstcOCER->LMC1 = 0u;
                pstcOCER->C1BUFEN = 0u;
                break;
            case OccrBufTrsfByCntZero:
                pstcOCER->LMC1 = 0u;
                pstcOCER->C1BUFEN = 1u;
                break;
            case OccrBufTrsfByCntPeak:
                pstcOCER->LMC1 = 0u;
                pstcOCER->C1BUFEN = 2u;
                break;
            case OccrBufTrsfByCntZeroPeak:
                pstcOCER->LMC1 = 0u;
                pstcOCER->C1BUFEN = 3u;
                break;
            case OccrBufTrsfByCntZeroMszcZero:
                pstcOCER->LMC1 = 1u;
                pstcOCER->C1BUFEN = 1u;
                break;
            case OccrBufTrsfByCntPeakMspcZero:
                pstcOCER->LMC1 = 1u;
                pstcOCER->C1BUFEN = 2u;
                break;
            case OccrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero:
                pstcOCER->LMC1 = 1u;
                pstcOCER->C1BUFEN = 3u;
                break;
            default:
                return ErrorInvalidParameter;
        }

        /* Set initial OP level */
        pstcOCSR->OCP1 = ((pstcOcoConfig->enPinState == OpHighLevel) ? 1u : 0u);
    }

    /* set interrupt enable */
    if(0u == (u8Ch%2u)) /* channel 0,2,4 */
    {
        pstcOCSR->OCIE0 = (TRUE == pstcOcoConfig->bIrqEnable) ? 1u:0u;
    }
    else /* channel 1,3,5 */
    {
        pstcOCSR->OCIE1 = (TRUE == pstcOcoConfig->bIrqEnable) ? 1u:0u;
    }

    /* set interrupt callback funciton */
    *(&(m_stcOcoIrqCb.pfnOco0IrqCallback) + u8Ch) = pstcOcoConfig->pfnIrqCallback;

    /* set NVIC */
    if(TRUE == pstcOcoConfig->bTouchNvic)
    {
        Timer4OcoEnableNvic();
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  De-Initialize OCO module
 **
 ** \param [in] u8Ch              channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 ** \param [in] bTouchNvic        Disable NVIC or not
 ** \arg FALSE                    Don't disable NVIC
 ** \arg TRUE                     Disable NVIC
 **
 ** \retval Ok                    OCO module initialized
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch > TIMER4_OCO_MAX_CH
 **
 ******************************************************************************/
en_result_t Timer4_Oco_DeInit(uint8_t u8Ch, boolean_t bTouchNvic)
{
    volatile uint16_t* pu16OCCR;
    volatile uint32_t* pu32OCMR;
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;
    volatile stc_timer4_ocer10_field_t* pstcOCER;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));
    ASSERT(IS_VALID_EN(bTouchNvic));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    pu16OCCR = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->OCCR0 + u8Ch*4);
    pu32OCMR = (volatile uint32_t*)((volatile uint8_t*)&M0P_TIMER4->OCMR0 + u8Ch*4);
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);
    pstcOCER = (volatile stc_timer4_ocer10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    /* Set default value */
    if(0u == (u8Ch % 2u)) /* channel 0, 2, 4 */
    {
        pstcOCSR->OCE0 = 0u;
        pstcOCSR->OCF0 = 0u;
        pstcOCSR->OCIE0 = 0u;
        pstcOCSR->OCP0 = 0u;
        pstcOCER->LMM0 = 0u;
        pstcOCER->M0BUFEN = 0u;
        pstcOCER->LMC0 = 0u;
        pstcOCER->C0BUFEN = 0u;
        pstcOCER->MCEC0 = 0u;
    }
    else
    {
        pstcOCSR->OCE1 = 0u;
        pstcOCSR->OCF1 = 0u;
        pstcOCSR->OCIE1 = 0u;
        pstcOCSR->OCP1 = 0u;
        pstcOCER->LMM1 = 0u;
        pstcOCER->M1BUFEN = 0u;
        pstcOCER->LMC1 = 0u;
        pstcOCER->C1BUFEN = 0u;
        pstcOCER->MCEC1 = 0u;
    }

    *pu16OCCR = 0u;
    *pu32OCMR = 0u;

    /* set interrupt callback funciton */
    *(&(m_stcOcoIrqCb.pfnOco0IrqCallback) + u8Ch) = NULL;

    if(TRUE == bTouchNvic)
    {
        Timer4OcoDisableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Set occr buffer mode
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 ** \param [in] enOccrBufMode     occr buffer mode
 ** \arg OccrBufDisable ~ OccrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero
 **
 ** \retval Ok                    OCO occr buffer mode initialized
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch > TIMER4_OCO_MAX_CH
 **                               - enOccrBufMode is invalid
 **
 ******************************************************************************/
en_result_t Timer4_Oco_SetOccrBuf(uint8_t u8Ch, en_oco_occr_buf_t enOccrBufMode)
{
    volatile stc_timer4_ocer10_field_t* pstcOCER;

    ASSERT(IS_VALID_CH(u8Ch));
    ASSERT(IS_VALID_OCCR_BUF_MODE(enOccrBufMode));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pstcOCER = (volatile stc_timer4_ocer10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    /* Set OCCR buffer mode */
    if(0u == (u8Ch % 2u)) /* channel 0, 2, 4 */
    {
        /* OCCR buffer */
        switch (enOccrBufMode)
        {
            case OccrBufDisable:
                pstcOCER->LMC0 = 0u;
                pstcOCER->C0BUFEN = 0u;
                break;
            case OccrBufTrsfByCntZero:
                pstcOCER->LMC0 = 0u;
                pstcOCER->C0BUFEN = 1u;
                break;
            case OccrBufTrsfByCntPeak:
                pstcOCER->LMC0 = 0u;
                pstcOCER->C0BUFEN = 2u;
                break;
            case OccrBufTrsfByCntZeroPeak:
                pstcOCER->LMC0 = 0u;
                pstcOCER->C0BUFEN = 3u;
                break;
            case OccrBufTrsfByCntZeroMszcZero:
                pstcOCER->LMC0 = 1u;
                pstcOCER->C0BUFEN = 1u;
                break;
            case OccrBufTrsfByCntPeakMspcZero:
                pstcOCER->LMC0 = 1u;
                pstcOCER->C0BUFEN = 2u;
                break;
            case OccrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero:
                pstcOCER->LMC0 = 1u;
                pstcOCER->C0BUFEN = 3u;
                break;
            default:
                return ErrorInvalidParameter;
        }
    }
    else/* channel 1, 3, 5 */
    {
        /* OCCR buffer */
        switch (enOccrBufMode)
        {
            case OccrBufDisable:
                pstcOCER->LMC1 = 0u;
                pstcOCER->C1BUFEN = 0u;
                break;
            case OccrBufTrsfByCntZero:
                pstcOCER->LMC1 = 0u;
                pstcOCER->C1BUFEN = 1u;
                break;
            case OccrBufTrsfByCntPeak:
                pstcOCER->LMC1 = 0u;
                pstcOCER->C1BUFEN = 2u;
                break;
            case OccrBufTrsfByCntZeroPeak:
                pstcOCER->LMC1 = 0u;
                pstcOCER->C1BUFEN = 3u;
                break;
            case OccrBufTrsfByCntZeroMszcZero:
                pstcOCER->LMC1 = 1u;
                pstcOCER->C1BUFEN = 1u;
                break;
            case OccrBufTrsfByCntPeakMspcZero:
                pstcOCER->LMC1 = 1u;
                pstcOCER->C1BUFEN = 2u;
                break;
            case OccrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero:
                pstcOCER->LMC1 = 1u;
                pstcOCER->C1BUFEN = 3u;
                break;
            default:
                return ErrorInvalidParameter;
        }
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Set occr buffer mode
 **
 ** \param [in] u8Ch              channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 ** \param [in] enOcmrBufMode     ocmr buffer mode
 ** \arg OcmrBufDisable ~ OcmrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero
 **
 ** \retval Ok                    OCO ocmr buffer mode initialized
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch > TIMER4_OCO_MAX_CH
 **                               - enOcmrBufMode is invalid.
 **
 ******************************************************************************/
en_result_t Timer4_Oco_SetOcmrBuf(uint8_t u8Ch, en_oco_ocmr_buf_t enOcmrBufMode)
{
    volatile stc_timer4_ocer10_field_t* pstcOCER;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));
    ASSERT(IS_VALID_OCMR_BUF_MODE(enOcmrBufMode));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pstcOCER = (volatile stc_timer4_ocer10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    /* Set OCMR buffer mode */
    if(0u == (u8Ch % 2u)) /* channel 0, 2, 4 */
    {
        /* OCMR buffer */
        switch (enOcmrBufMode)
        {
            case OcmrBufDisable:
                pstcOCER->LMM0 = 0u;
                pstcOCER->M0BUFEN = 0u;
                break;
            case OcmrBufTrsfByCntZero:
                pstcOCER->LMM0 = 0u;
                pstcOCER->M0BUFEN = 1u;
                break;
            case OcmrBufTrsfByCntPeak:
                pstcOCER->LMM0 = 0u;
                pstcOCER->M0BUFEN = 2u;
                break;
            case OcmrBufTrsfByCntZeroPeak:
                pstcOCER->LMM0 = 0u;
                pstcOCER->M0BUFEN = 3u;
                break;
            case OcmrBufTrsfByCntZeroMszcZero:
                pstcOCER->LMM0 = 1u;
                pstcOCER->M0BUFEN = 1u;
                break;
            case OcmrBufTrsfByCntPeakMspcZero:
                pstcOCER->LMM0 = 1u;
                pstcOCER->M0BUFEN = 2u;
                break;
            case OcmrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero:
                pstcOCER->LMM0 = 1u;
                pstcOCER->M0BUFEN = 3u;
                break;
            default:
                return ErrorInvalidParameter ;
        }
    }
    else/* channel 1, 3, 5 */
    {
       /* OCMR buffer */
        switch (enOcmrBufMode)
        {
            case OcmrBufDisable:
                pstcOCER->LMM1 = 0u;
                pstcOCER->M1BUFEN = 0u;
                break;
            case OcmrBufTrsfByCntZero:
                pstcOCER->LMM1 = 0u;
                pstcOCER->M1BUFEN = 1u;
                break;
            case OcmrBufTrsfByCntPeak:
                pstcOCER->LMM1 = 0u;
                pstcOCER->M1BUFEN = 2u;
                break;
            case OcmrBufTrsfByCntZeroPeak:
                pstcOCER->LMM1 = 0u;
                pstcOCER->M1BUFEN = 3u;
                break;
            case OcmrBufTrsfByCntZeroMszcZero:
                pstcOCER->LMM1 = 1u;
                pstcOCER->M1BUFEN = 1u;
                break;
            case OcmrBufTrsfByCntPeakMspcZero:
                pstcOCER->LMM1 = 1u;
                pstcOCER->M1BUFEN = 2u;
                break;
            case OcmrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero:
                pstcOCER->LMM1 = 1u;
                pstcOCER->M1BUFEN = 3u;
                break;
            default:
                return ErrorInvalidParameter;
        }
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Extend the matching determination conditions of OCO channel
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval Ok                    OCO channel is set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch > TIMER4_OCO_MAX_CH
 **
 ******************************************************************************/
en_result_t Timer4_Oco_EnableExtMatchCond(uint8_t u8Ch)
{
    volatile stc_timer4_ocer10_field_t* pstcOCER;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address  */
    pstcOCER = (volatile stc_timer4_ocer10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCER10 + (u8Ch/2u)*4u);

    (0u == (u8Ch%2)) ? (pstcOCER->MCEC0 = 1u) : (pstcOCER->MCEC1 = 1u);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Don't extend the matching determination conditions of OCO channel
 **
 ** \param [in] u8Ch              channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval Ok                    OCO channel is set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch out of range
 **
 ******************************************************************************/
en_result_t Timer4_Oco_DisableExtMatchCond(uint8_t u8Ch)
{
    volatile stc_timer4_ocer10_field_t* pstcOCER;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address  */
    pstcOCER = (volatile stc_timer4_ocer10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCER10 + (u8Ch/2u)*4u);

    (0u == (u8Ch%2)) ? (pstcOCER->MCEC0 = 1u) : (pstcOCER->MCEC1 = 1u);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Compare congifuration of even OCO channel
 **
 ** \param [in] u8EvenCh          even channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \param [in] pstcConfig pointer to structure of compare mode
 **
 ** \retval Ok                    Even OCO channel compare mode is set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8EvenCh out of range
 **                               - pstcConfig pointer is NULL
 **
 ******************************************************************************/
en_result_t Timer4_Oco_SetEvenChCompareMode(uint8_t u8EvenCh,
                                      stc_oco_even_compare_config_t* pstcConfig)
{
    uint16_t u16OCMR0 = 0;
    volatile uint16_t* pu16OCMR0;
    volatile stc_timer4_ocer10_field_t* pstcOCER;

    /* Check parameters */
    ASSERT(IS_VALID_EVEN_CH(u8EvenCh));
    ASSERT(IS_VALID_EVEN_CH_CMP_MODE(pstcConfig));

    if((u8EvenCh%2) ||
       (u8EvenCh > TIMER4_OCO_MAX_CH) ||
       (NULL == pstcConfig))
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pu16OCMR0 = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->OCMR0 + u8EvenCh*4u);
    pstcOCER  = (volatile stc_timer4_ocer10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCER10 + (u8EvenCh/2u)*4u);

    pstcOCER->MCEC0 = (pstcConfig->bCntZeroPeakExtendMatchCondition == TRUE) ? 1u:0u;
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enCntZeroEvenMatchEvenChOpStatus    << 10u);
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enCntZeroEvenNotMatchEvenChOpStatus << 14u);
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enCntUpCntEvenMatchEvenChOpStatus   << 8u);
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enCntPeakEvenMatchEvenChOpStatus    << 6u);
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enCntPeakEvenNotMatchEvenChOpStatus << 12u);
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enCntDownCntEvenMatchEvenChOpStatus << 4u);
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enOcfWhenCntZeroEvenMatch     << 3u);
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enOcfWhenCntUpCntEvenMatch    << 2u);
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enOcfWhenCntPeakEvenMatch     << 1u);
    u16OCMR0 |= (uint16_t)((uint16_t)pstcConfig->enOcfWhenCntDownCntEvenMatch  << 0u);

    *pu16OCMR0 = u16OCMR0;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Compare congifuration of odd OCO channel
 **
 ** \param [in] u8OddCh           odd channel of OCO
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 ** \param [in] pstcConfig pointer to structure of compare mode
 **
 ** \retval Ok                    Odd OCO channel compare mode is set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8OddCh out of range
 **                               - pstcConfig pointer is NULL
 **
 ******************************************************************************/
en_result_t Timer4_Oco_SetOddChCompareMode (uint8_t u8OddCh,
                                         stc_oco_odd_compare_config_t* pstcConfig)
{
    uint32_t u32OCMR1 = 0;
    volatile uint32_t* pu32OCMR1;
    volatile stc_timer4_ocer10_field_t* pstcOCER;

    /* Check parameters */
    ASSERT(IS_VALID_ODD_CH(u8OddCh));
    ASSERT(IS_VALID_ODD_CH_CMP_MODE(pstcConfig));

    if((!(u8OddCh%2)) ||
       (u8OddCh > TIMER4_OCO_MAX_CH) ||
       (NULL == pstcConfig))
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pu32OCMR1 = (volatile uint32_t*)((volatile uint8_t*)&M0P_TIMER4->OCMR0 + u8OddCh*4u);
    pstcOCER = (volatile stc_timer4_ocer10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCER10 + (u8OddCh/2u)*4u);

    pstcOCER->MCEC1 = (pstcConfig->bCntZeroPeakExtendMatchCondition == TRUE) ? 1u:0u;
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntZeroOddMatchEvenMatchOddChOpStatus        << 26u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntZeroOddMatchEvenNotMatchOddChOpStatus     << 10u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntZeroOddNotMatchEvenMatchOddChOpStatus     << 30u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntZeroOddNotMatchEvenNotMatchOddChOpStatus  << 14u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntUpCntOddMatchEvenMatchOddChOpStatus       << 24u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntUpCntOddMatchEvenNotMatchOddChOpStatus    << 8u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntUpCntOddNotMatchEvenMatchOddChOpStatus    << 18u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntPeakOddMatchEvenMatchOddChOpStatus        << 22u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntPeakOddMatchEvenNotMatchOddChOpStatus     << 6u) ;
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntPeakOddNotMatchEvenMatchOddChOpStatus     << 28u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntPeakOddNotMatchEvenNotMatchOddChOpStatus  << 12u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntDownOddMatchEvenMatchOddChOpStatus        << 20u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntDownOddMatchEvenNotMatchOddChOpStatus     << 4u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enCntDownOddNotMatchEvenMatchOddChOpStatus     << 16u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enOcfWhenCntZeroOddMatch     << 3u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enOcfWhenCntUpCntOddMatch    << 2u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enOcfWhenCntPeakOddMatch     << 1u);
    u32OCMR1 |= (uint32_t)((uint32_t)pstcConfig->enOcfWhenCntDownCntOddMatch  << 0u);

    *pu32OCMR1 = u32OCMR1;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Enable OCO operation
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval Ok                    OCO operation enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch out of range
 **
 ******************************************************************************/
en_result_t Timer4_Oco_EnableOperation(uint8_t u8Ch)
{
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;

    /* Check OCO channel */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    /* set register */
    (0u == (u8Ch%2)) ? (pstcOCSR->OCE0 = 1u) : (pstcOCSR->OCE1 = 1u);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Disable OCO operation
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval Ok                    OCO operation disabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch out of range
 **
 ******************************************************************************/
en_result_t Timer4_Oco_DisableOperation(uint8_t u8Ch)
{
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;

    /* Check OCO channel */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    /* set register */
    (0u == (u8Ch%2)) ? (pstcOCSR->OCE0 = 0u) : (pstcOCSR->OCE1 = 0u);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Enable OCO interrupt
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 ** \param [in]  pfnCallback  callback function
 **
 ** \retval Ok                    OCO interrupt enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch out of range
 **
 ******************************************************************************/
en_result_t Timer4_Oco_EnableIrq(uint8_t u8Ch, func_ptr_t pfnCallback)
{
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Ch));
    ASSERT(IS_VALID_POINTER(pfnCallback));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    /* set register */
    (0u == (u8Ch%2)) ? (pstcOCSR->OCIE0 = 1u) : (pstcOCSR->OCIE1 = 1u);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Disable OCO interrupt
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval Ok                    OCO interrupt disabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch out of range
 **
 ******************************************************************************/
en_result_t Timer4_Oco_DisableIrq(uint8_t u8Ch)
{
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;

    /* Check OCO channel */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    /* set register */
    (0u == (u8Ch%2)) ? (pstcOCSR->OCIE0 = 0u): (pstcOCSR->OCIE1 = 0u);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Get OCO interrupt flag
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval DdlClr                OCO interrupt flag is clear
 ** \retval DdlSet                OCO interrupt flag is set
 **
 ******************************************************************************/
en_irq_flag_t Timer4_Oco_GetIrqFlag(uint8_t u8Ch)
{
    en_irq_flag_t enFlag = DdlClr;
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;

    /* Check OCO channel */
    ASSERT(IS_VALID_CH(u8Ch));

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    /* set return value */
    if(0u == (u8Ch%2)) /* channel 0,2,4 */
    {
        enFlag = ((1u == pstcOCSR->OCF0) ? DdlSet: DdlClr);
    }
    else /* channel 1,3,5 */
    {
        enFlag = ((1u == pstcOCSR->OCF1) ? DdlSet: DdlClr);
    }

    return enFlag;
}

/**
 *******************************************************************************
 ** \brief  Clear OCO interrupt flag
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval Ok                    OCO interrupt flag is clear
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch out of range
 **
 ******************************************************************************/
en_result_t Timer4_Oco_ClrIrqFlag(uint8_t u8Ch)
{
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;

    /* Check OCO channel */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    /* set return value */
    (0u == (u8Ch%2)) ? (pstcOCSR->OCF0 = 0u) : (pstcOCSR->OCF1 = 0u);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Set OP pin level of OCO
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 ** \param [in]  enLevel          OP pin level of OCO
 ** \arg  OpLowlevel              OP pin is low level
 ** \arg  OpHighlevel             OP pin is high level
 **
 ** \retval Ok                    OCO interrupt flag is clear
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch out of range
 **
 ******************************************************************************/
en_result_t Timer4_Oco_SetOpPinLevel(uint8_t u8Ch,
                                               en_oco_op_out_state_t enLevel)
{
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;

    /* Check OCO channel */
    ASSERT(IS_VALID_CH(u8Ch));
    ASSERT(IS_VALID_OP_OUT_STATE(enLevel));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    if(0u == (u8Ch%2))
    {
        pstcOCSR->OCP0 = (OpHighLevel == enLevel) ? 1u:0u;
    }
    else
    {
        pstcOCSR->OCP1 = (OpHighLevel == enLevel) ? 1u:0u;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Get OP pin level of OCO
 **
 ** \param [in]  u8Ch             channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval OpLowlevel            OP pin is low level
 ** \retval OpHighlevel           OP pin is high level
 **
 ******************************************************************************/
en_oco_op_out_state_t Timer4_Oco_GetOpPinLevel(uint8_t u8Ch)
{
    en_oco_op_out_state_t enLevel = OpLowLevel;
    volatile stc_timer4_ocsr10_field_t* pstcOCSR;

    /* Check OCO channel */
    ASSERT(IS_VALID_CH(u8Ch));

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (volatile stc_timer4_ocsr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->OCSR10 + (u8Ch/2u)*4u);

    if(0u == (u8Ch%2))
    {
        enLevel = ((1u == pstcOCSR->OCP0) ? OpHighLevel:OpLowLevel);
    }
    else
    {
        enLevel = ((1u == pstcOCSR->OCP1) ? OpHighLevel:OpLowLevel);
    }

    return enLevel;
}

/**
 *******************************************************************************
 ** \brief  Write occr register
 **
 ** \param [in] u8Ch              channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 ** \param [in]  u16Occr  the value of occr
 ** \arg  16bit value
 **
 ** \retval Ok                    OCCR written
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8Ch out of range
 **
 ******************************************************************************/
en_result_t Timer4_Oco_WriteOccr(uint8_t u8Ch, uint16_t u16Occr)
{
    volatile uint16_t* pu16OCCR;

    /* Check OCO channel */
    ASSERT(IS_VALID_CH(u8Ch));

    if(u8Ch > TIMER4_OCO_MAX_CH)
    {
        return ErrorInvalidParameter;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }

    /* Get pointer of current channel OCO register address */
    pu16OCCR = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->OCCR0 + u8Ch*4u);

    /* set register */
    *pu16OCCR = u16Occr;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Get occr register value
 **
 ** \param [in] u8Ch              channel of OCO
 ** \arg TIMER4_OCO_CH0           OCO channel 0
 ** \arg TIMER4_OCO_CH1           OCO channel 1
 ** \arg TIMER4_OCO_CH2           OCO channel 2
 ** \arg TIMER4_OCO_CH3           OCO channel 3
 ** \arg TIMER4_OCO_CH4           OCO channel 4
 ** \arg TIMER4_OCO_CH5           OCO channel 5
 **
 ** \retval 16bit occr value
 **
 ******************************************************************************/
uint16_t Timer4_Oco_ReadOccr(uint8_t u8Ch)
{
    volatile uint16_t* pu16OCCR;

    /* Check OCO channel */
    ASSERT(IS_VALID_CH(u8Ch));

    /* Get pointer of current channel OCO register address */
    pu16OCCR = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->OCCR0 + u8Ch*4u);

    return (*pu16OCCR);
}

//@} // Timer4OcoGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
