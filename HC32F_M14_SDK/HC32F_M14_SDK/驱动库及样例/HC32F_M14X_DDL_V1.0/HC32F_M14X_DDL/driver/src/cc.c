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
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
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
/** \file cc.c
 **
 ** cc driver define.
 ** @link CcGroup Cc description @endlink
 **
 **   - 2017-08-09  1.0  HeChun First version for Device Driver Library of cc.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "cc.h"

/**
 *******************************************************************************
 ** \addtogroup CcGroup
 ******************************************************************************/
//@{
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_CK(x)                  \
(   (Ck0 <= (x)) &&                     \
    (Ck3 >= (x)) )

#define IS_VALID_prescaler(x)           \
(   (Modulus0 <= (x)) &&                \
    (Modulus6 >= (x)) )

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void CcEnableNvic(void);
static void CcDisableNvic(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static func_ptr_t m_pfnIrqCallback = NULL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Enable device dependent initialization of transmit interrupts
 **
 ******************************************************************************/
static void CcEnableNvic(void)
{
    /* clear cc interrupt flag */
    NVIC_ClearPendingIRQ(OSC_CC_RP_IRQn);

    /* enable cc interrupt controller */
    NVIC_EnableIRQ(OSC_CC_RP_IRQn);

    /* set cc interrupt priority */
    NVIC_SetPriority(OSC_CC_RP_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief Disable device dependent initialization of transmit interrupts
 **
 ******************************************************************************/
static void CcDisableNvic(void)
{
    /* clear cc interrupt flag */
    NVIC_ClearPendingIRQ(OSC_CC_RP_IRQn);

    /*disenable cc interrupt controller */
    NVIC_DisableIRQ(OSC_CC_RP_IRQn);

    /*set cc interrupt priority */
    NVIC_SetPriority(OSC_CC_RP_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}
/**
 *******************************************************************************
 ** \brief cc transfer interrupt service routine.
 **
 ******************************************************************************/
void Cc_IrqHandler(uint8_t u8Param)
{
    /* ensure the interrupt had happen */
    if(TRUE == Cc_GetIrqFlag())
    {
        /* clear interrupt flag */
        Cc_ClrIrqFlag();

        /* if callback functiion is not none run function*/
        if(NULL != m_pfnIrqCallback)
        {
            m_pfnIrqCallback();
        }
    }

}


/**
 *******************************************************************************
 ** \brief Clear interrupt status
 **
 ** \retval Ok   the interrupt mark removed
 **
 ******************************************************************************/
en_result_t Cc_ClrIrqFlag(void)
{
    en_result_t enResult = Ok;

    M0P_CLOCK_COUNTER->CNTC_f.CNTIF = FALSE;

    return enResult;
}


/**
 *******************************************************************************
 ** \brief Enable clock Counter(clock start to work)
 **
 ** \retval Ok   set successfully
 **
 ******************************************************************************/
en_result_t Cc_EnableCount(void)
{
    en_result_t enResult = Ok;

    M0P_CLOCK_COUNTER->CNTC_f.CNTEN = TRUE;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Disable clock Counter(stop clock counter)
 **
 ** \retval Ok   set successfully
 **
 ******************************************************************************/
en_result_t Cc_DisableCount(void)
{
    en_result_t enResult = Ok;

    M0P_CLOCK_COUNTER->CNTC_f.CNTEN = FALSE;

    return enResult;
}


/**
 *******************************************************************************
 ** \brief Enable clock counter interrupt
 **
 ** \retval Ok   set successfully
 **
 ******************************************************************************/
en_result_t Cc_EnableIrq(void)
{
    en_result_t enResult = Ok;

    M0P_CLOCK_COUNTER->CNTC_f.CNTIE = TRUE;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Disable clock counter Interrupt
 **
 ** \retval Ok   set successfully
 **
 ******************************************************************************/
en_result_t Cc_DisableIrq(void)
{
    en_result_t enResult = Ok;

    M0P_CLOCK_COUNTER->CNTC_f.CNTIE = FALSE;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Get interrupt status
 **
 ** \retval Ok   set successfully
 **
 ******************************************************************************/
boolean_t Cc_GetIrqFlag(void)
{
    boolean_t bRetVal = TRUE;

    bRetVal = M0P_CLOCK_COUNTER->CNTC_f.CNTIF;

    return bRetVal;

}

/**
 *******************************************************************************
 ** \brief Write load value
 **
 ** \retval Ok   set successfully
 **
 ** \note The u32Val  can only be set 0~63 if u32Val>63
 **       the value will be set 63
 ******************************************************************************/
en_result_t Cc_WriteLoadVal(uint32_t u32Val)
{
    en_result_t enResult = Ok;

    /* the cc only can be set 6bit counter */
    M0P_CLOCK_COUNTER->CNTRL_f.RLC = u32Val&0x3F;

    return enResult;

}

/**
 *******************************************************************************
 ** \brief Read current count value
 **
 ** \retval uint32_t:current counter value
 **
 ******************************************************************************/
uint32_t Cc_ReadCurCntVal(void)
{
    uint32_t u32Val = 0;

    /* get current value */
    u32Val = M0P_CLOCK_COUNTER->CNTRL_f.RLC;

    return u32Val;
}

/**
 *******************************************************************************
 ** \brief De-Initialize Cc
 **
 ** This function de-initializes the specified clock counter.
 ** Cc-DeInit() accesses the Cc hardware register. They are reset.
 **
 ** \param [in]  bTouchNvic       Touch NVIC yes or not
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                 - u8Idx is invalid
 **
 ******************************************************************************/
en_result_t Cc_DeInit(boolean_t bTouchNvic)
{
    en_result_t enResult  = Ok;

    /* clear counter */
    M0P_CLOCK_COUNTER->CNTC = 0;
    M0P_CLOCK_COUNTER->CNTRL = 0;
    M0P_CLOCK_COUNTER->CNT_CLK1 = 0;

    if (TRUE == bTouchNvic)
    {
        CcDisableNvic();
    }

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Initialization of Cc
 **
 ** \param [in]   pstcConfig      Cc configuration
 **
 ** \retval Ok    Process successfully done.
 **
 ******************************************************************************/
en_result_t Cc_Init(stc_cc_config_t *pstcConfig)
{
    en_result_t enResult = Ok;

    /* set cc of prescaler */
    ASSERT(IS_VALID_CK(pstcConfig->CK));
    ASSERT(IS_VALID_prescaler(pstcConfig->modulus));

    M0P_CLOCK_COUNTER->CNT_CLK1_f.CKEN = pstcConfig->enRCK;
    M0P_CLOCK_COUNTER->CNT_CLK0_f.CKSEL = pstcConfig->enSource;
    M0P_CLOCK_COUNTER->CNT_CLK0_f.DIV = pstcConfig->modulus;

    switch(pstcConfig->CK)
    {
        case Ck0:
            M0P_CLOCK_COUNTER->CNTC_f.CCS = Ck0;
            break;
        case Ck1:
            M0P_CLOCK_COUNTER->CNTC_f.CCS = Ck1;
            break;
        case Ck2:
            M0P_CLOCK_COUNTER->CNTC_f.CCS = Ck2;
            break;
        case Ck3:
            M0P_CLOCK_COUNTER->CNTC_f.CCS = Ck3;
            break;
        default:
            break;
    }

    /* enable cc interrupt */
    if (TRUE == pstcConfig->bIrqEnable )
    {
        Cc_EnableIrq();
    }

    /* enable cc NVIC */
    if (TRUE == pstcConfig->bTouchNvic )
    {
        CcEnableNvic();
    }

    /* set interrupt callback function */
    m_pfnIrqCallback = pstcConfig->  pfnIrqCallback;
    return enResult;
}


//@} // CcGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
