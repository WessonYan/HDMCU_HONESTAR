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
/** \file clk.c
 **
 ** A detailed description is available at
 ** @link ClkGroup Clk description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of Clock
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "clk.h"

/**
 *******************************************************************************
 ** \addtogroup ClkGroup
 *****************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#ifndef CLK_HSXT_VAL
#warning "HSXT default frequency: 4MHz "
#define CLK_HSXT_VAL  (4*1000*1000)     ///< HSXT default frequency = 4M if not coding  macro definition CLK_HSXT_VAL
#endif

#define CLK_TIMEOUT  (1000000u)

#define IS_VALID_POINTER(x)  (NULL != (x))  ///< Parameter validity check for pointer

/*! Parameter validity check for clock type */
#define IS_VALID_SRC(x)                 \
(   (ClkHICR == (x)) ||                 \
    (ClkHSXT == (x)) ||                 \
    (ClkMPLL == (x)) ||                 \
    (ClkLICR == (x)) ||                 \
    (ClkLSXT == (x)))

/*! Parameter validity check for external high cr wait time */
#define IS_VALID_HSXT_WATI_TIME(x)      \
(   (ClkCswtMain64us == (x))  ||        \
    (ClkCswtMain256us == (x)) ||        \
    (ClkCswtMain1ms == (x))   ||        \
    (ClkCswtMain4ms == (x))   ||        \
    (ClkCswtMain8ms == (x))   ||        \
    (ClkCswtMain16ms == (x))  ||        \
    (ClkCswtMain32ms == (x))  ||        \
    (ClkCswtMain65ms == (x))  ||        \
    (ClkCswtMain131ms == (x)) ||        \
    (ClkCswtMain262ms == (x)) ||        \
    (ClkCswtMain524ms == (x)) ||        \
    (ClkCswtMain1s == (x))    ||        \
    (ClkCswtMain2s == (x))    ||        \
    (ClkCswtMain1us == (x))   ||        \
    (ClkCswtMain4us == (x))   ||        \
    (ClkCswtMain16us == (x)))

/*! Parameter validity check for external sub cr wait time */
#define IS_VALID_LSXT_WATI_TIME(x)      \
(   (ClkCswtSub01ms == (x))  ||         \
    (ClkCswtSub05ms == (x))  ||         \
    (ClkCswtSub2ms == (x))   ||         \
    (ClkCswtSub8ms == (x))   ||         \
    (ClkCswtSub16ms == (x))  ||         \
    (ClkCswtSub32ms == (x))  ||         \
    (ClkCswtSub64ms == (x))  ||         \
    (ClkCswtSub128ms == (x)) ||         \
    (ClkCswtSub256ms == (x)) ||         \
    (ClkCswtSub512ms == (x)) ||         \
    (ClkCswtSub1s == (x))    ||         \
    (ClkCswtSub2s == (x))    ||         \
    (ClkCswtSub4s == (x))    ||         \
    (ClkCswtSub8s == (x))    ||         \
    (ClkCswtSub16s == (x))   ||         \
    (ClkCswtSub32s == (x)))

/*! Parameter validity check for pll clock wait time */
#define IS_VALID_PLL_WATI_TIME(x)       \
(   (ClkPswtPll85us == (x))  ||         \
    (ClkPswtPll170us == (x)) ||         \
    (ClkPswtPll340us == (x)) ||         \
    (ClkPswtPll680us == (x)))

/*! Parameter validity check for  pll source clock */
#define IS_VALID_PLL_CLK_SRC(x)         \
(   (MAIN2PLL == (x)) ||                \
    (HICR2PLL == (x)))

/*! Parameter validity check for base clock division */
#define IS_VALID_CLK_BASE_DIV(x)        \
(   (ClkBaseDiv1 == (x)) ||             \
    (ClkBaseDiv2 == (x)) ||             \
    (ClkBaseDiv3 == (x)) ||             \
    (ClkBaseDiv4 == (x)) ||             \
    (ClkBaseDiv6 == (x)) ||             \
    (ClkBaseDiv8 == (x)) ||             \
    (ClkBaseDiv16 == (x)))

/*! Parameter validity check for apb0 clock division */
#define IS_VALID_CLK_APB0_DIV(x)        \
(   (ClkApb0Div1 == (x)) ||             \
    (ClkApb0Div2 == (x)) ||             \
    (ClkApb0Div4 == (x)) ||             \
    (ClkApb0Div8 == (x)))

/*! Parameter validity check for apb1 clock division */
#define IS_VALID_CLK_APB1_DIV(x)        \
(   (ClkApb1Div1 == (x)) ||             \
    (ClkApb1Div2 == (x)) ||             \
    (ClkApb1Div4 == (x)) ||             \
    (ClkApb1Div8 == (x)))

/*! Parameter validity check for interrupt type */
#define IS_VALID_IRQ(x)                 \
(   (ClkIrqAfd == (x))     ||           \
    (ClkIrqPLLRdy == (x))  ||           \
    (ClkIrqLSXTRdy == (x)) ||           \
    (ClkIrqHSXTRdy == (x)))

/*! Parameter validity check for func type */
#define IS_VALID_FUNC(x)                \
(   (ClkFuncAfd == (x))         ||      \
    (ClkFuncAfdReset == (x))    ||      \
    (ClkFuncLSXTCfd == (x))     ||      \
    (ClkFuncHSXTCfd == (x))     ||      \
    (ClkFuncLSXTCfdRest == (x)) ||      \
    (ClkFuncHSXTCfdRest == (x)))

/*! Parameter validity check for interrupt type */
#define IS_VALID_CLK_SWWDT_DIV(x)       \
(   (ClkSwwdtDiv1 == (x)) ||            \
    (ClkSwwdtDiv2 == (x)) ||            \
    (ClkSwwdtDiv4 == (x)) ||            \
    (ClkSwwdtDiv8 == (x)))

/*! Parameter validity check for afd division */
#define IS_VALID_AFDDIV(x)              \
(   (ClkAfdDiv256 == (x))  ||           \
    (ClkAfdDiv512 == (x))  ||           \
    (ClkAfdDiv1024 == (x)))

/*! Parameter validity check for cfd type */
#define IS_VALID_CFDSTAT(x)             \
(   (ClkCfdLSXT == (x))  ||             \
    (ClkCfdHSXT == (x)))

/*! Parameter validity check for enable flag */
#define IS_VALID_EN(x)                  \
(   (TRUE == (x)) ||                    \
    (FALSE == (x)))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void ClkEnableNvic(void);
static void ClkDisableNvic(void);
static void ClkAfdEnableNvic(void);
static void ClkAfdDisableNvic(void);
static en_result_t ClkSetHICR(void);
static en_result_t ClkSetHSXT(void);
static en_result_t ClkSetLICR(void);
static en_result_t ClkSetLSXT(void);
static en_result_t ClkSetMPLL(void);
static en_result_t ClkSetFunc(en_clk_func_t enFunc, boolean_t bFlag);
static en_result_t ClkSetIrq(en_clk_irq_sel_t enSel, boolean_t bFlag);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static func_ptr_t m_pfnAfdCallback = NULL;
static func_ptr_t m_pfnHSXTCallback = NULL;
static func_ptr_t m_pfnLSXTCallback = NULL;
static func_ptr_t m_pfnPLLCallback = NULL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ******************************************************************************/
static void ClkDisableNvic(void)
{
    NVIC_ClearPendingIRQ(OSC_CC_RP_IRQn);
    NVIC_DisableIRQ(OSC_CC_RP_IRQn);
    NVIC_SetPriority(OSC_CC_RP_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
********************************************************************************
** \brief Device dependent initialization of interrupts according CMSIS with
**        level defined in interrupt_xxx.h
**
******************************************************************************/
static void ClkEnableNvic(void)
{
    NVIC_ClearPendingIRQ(OSC_CC_RP_IRQn);
    NVIC_EnableIRQ(OSC_CC_RP_IRQn);
    NVIC_SetPriority(OSC_CC_RP_IRQn, DDL_IRQ_LEVEL_OSCCCRP);
}

/**
 *******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ******************************************************************************/
static void ClkAfdDisableNvic(void)
{
    NVIC_ClearPendingIRQ(CSV_IRQn);
    NVIC_DisableIRQ(CSV_IRQn);
    NVIC_SetPriority(CSV_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
********************************************************************************
** \brief Device dependent initialization of interrupts according CMSIS with
**        level defined in interrupt_xxx.h
**
******************************************************************************/
static void ClkAfdEnableNvic(void)
{
    NVIC_ClearPendingIRQ(CSV_IRQn);
    NVIC_EnableIRQ(CSV_IRQn);
    NVIC_SetPriority(CSV_IRQn, DDL_IRQ_LEVEL_CSV);
}

/**
 *******************************************************************************
 ** \brief Set HICR as master clock
 **
 ** \param [in]                   None
 **
 ** \retval Ok                    Functions enabled
 **
 ******************************************************************************/
static en_result_t ClkSetHICR(void)
{
    en_clk_source_t enSrc;

    M0P_CLOCK->CTRL_f.HC_EN = TRUE;

    enSrc = (en_clk_source_t)M0P_CLOCK->CLK_STB_f.SRC;

    if ( (ClkLICR == enSrc) || (ClkLSXT == enSrc) )
    {
        M0P_CLOCK->CTRL_f.INPUT = ClkHICR;
        while (FALSE == M0P_CLOCK->CLK_STB_f.HC_RDY)
        {
            ;
        }
    }
    else
    {
        while (FALSE == M0P_CLOCK->CLK_STB_f.HC_RDY)
        {
            ;
        }
        M0P_CLOCK->CTRL_f.INPUT = ClkHICR;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set HSXT as master clock
 **
 ** \param [in]                   None
 **
 ** \retval Ok                    Functions enabled
 **
 ******************************************************************************/
static en_result_t ClkSetHSXT(void)
{
    en_clk_source_t enSrc;

    M0P_CLOCK->CTRL_f.MAIN_EN = TRUE;

    enSrc = (en_clk_source_t)M0P_CLOCK->CLK_STB_f.SRC;

    if ( (ClkLICR == enSrc) || (ClkLSXT == enSrc) )
    {
        M0P_CLOCK->CTRL_f.INPUT = ClkHSXT;
        /* after the stable time, the rdy flg must be true. */
        while (FALSE == M0P_CLOCK->CLK_STB_f.MAIN_RDY)
        {
            ;
        }
    }
    else
    {
        /* after the stable time, the rdy flg must be true. */
        while (FALSE == M0P_CLOCK->CLK_STB_f.MAIN_RDY)
        {
            ;
        }
        M0P_CLOCK->CTRL_f.INPUT = ClkHSXT;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set LICR as master clock
 **
 ** \param [in]                   None
 **
 ** \retval Ok                    Functions enabled
 **
 ******************************************************************************/
static en_result_t ClkSetLICR(void)
{
    M0P_CLOCK->CTRL_f.INPUT = ClkLICR;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set LSXT as master clock
 **
 ** \param [in]                   None
 **
 ** \retval Ok                    Functions enabled
 **
 ******************************************************************************/
static en_result_t ClkSetLSXT(void)
{
    M0P_CLOCK->CTRL_f.SUB_EN = TRUE;

    /* after the stable time, the rdy flg must be true. */
    while (FALSE == M0P_CLOCK->CLK_STB_f.SUB_RDY)
    {
        ;
    }

    M0P_CLOCK->CTRL_f.INPUT = ClkLSXT;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set PLL as master clock
 **
 ** \param [in]                   None
 **
 ** \retval Ok                    Functions enabled
 **
 ******************************************************************************/
static en_result_t ClkSetMPLL(void)
{
    en_clk_source_t enSrc;

    M0P_CLOCK->CTRL_f.PLL_EN = TRUE;

    enSrc = (en_clk_source_t)M0P_CLOCK->CLK_STB_f.SRC;

    if ( (ClkLICR == enSrc) || (ClkLSXT == enSrc) )
    {
        M0P_CLOCK->CTRL_f.INPUT = ClkMPLL;
        /* after the stable time, the rdy flg must be true. */
        while (FALSE == M0P_CLOCK->CLK_STB_f.PLL_RDY)
        {
            ;
        }
    }
    else
    {
        /* after the stable time, the rdy flg must be true. */
        while (FALSE == M0P_CLOCK->CLK_STB_f.PLL_RDY)
        {
            ;
        }
        M0P_CLOCK->CTRL_f.INPUT = ClkMPLL;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable/disalbe clock function
 **
 ** \param [in] enFunc            peripheral clock function enum
 ** \param [in] bFlag             Setting clock function flag
 ** \arg TRUE                     Enable peripheral clock function
 ** \arg FALSE                    Diable peripheral clock function
 ** \retval Ok                    Functions enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - enFunc is invalid
 **
 ******************************************************************************/
static en_result_t ClkSetFunc(en_clk_func_t enFunc, boolean_t bFlag)
{
    en_result_t enRet = Ok;

    ASSERT(IS_VALID_FUNC(enFunc));
    ASSERT(IS_VALID_EN(bFlag));

    switch (enFunc)
    {
        case ClkFuncAfd:
            M0P_CLOCK->FCM_CTL_f.AFDE = bFlag;
            break;
        case ClkFuncAfdReset:
            M0P_CLOCK->FCM_CTL_f.AFDRE = bFlag;
            break;
        case ClkFuncLSXTCfd:
            M0P_CLOCK->FCM_CTL_f.SFCME = bFlag;
            break;
        case ClkFuncHSXTCfd:
            M0P_CLOCK->FCM_CTL_f.MFCME = bFlag;
            break;
        case ClkFuncLSXTCfdRest:
            M0P_CLOCK->FCM_CTL_f.SFCMRE = bFlag;
            break;
        case ClkFuncHSXTCfdRest:
            M0P_CLOCK->FCM_CTL_f.MFCMRE = bFlag;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable/disable clock interrupt
 **
 ** \param [in] enSel             clock interrupt type
 ** \param [in] bFlag             Setting interrupt flag
 ** \arg TRUE                     Disable clock NVIC interrupt
 ** \arg FALSE                    Don't set clock NVIC interrupt
 **
 ** \retval Ok                    Functions enabled
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enSel is invalid
 **
 ******************************************************************************/
static en_result_t ClkSetIrq(en_clk_irq_sel_t enSel, boolean_t bFlag)
{
    en_result_t enRet = Ok;

    ASSERT(IS_VALID_IRQ(enSel));
    ASSERT(IS_VALID_EN(bFlag));

    switch (enSel)
    {
        case ClkIrqAfd:
            M0P_CLOCK->INT_CTRL_f.AFDEN = bFlag;
            break;
        case ClkIrqPLLRdy:
            M0P_CLOCK->INT_CTRL_f.PLLEN = bFlag;
            break;
        case ClkIrqHSXTRdy:
            M0P_CLOCK->INT_CTRL_f.MAINEN = bFlag;
            break;
        case ClkIrqLSXTRdy:
            M0P_CLOCK->INT_CTRL_f.SUBEN = bFlag;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  PLL ready interrupt handler function
 **
 ** \param [in] u8Dummy                 Dummy parameter
 **
 ** \retval                             None
 **
 ******************************************************************************/
void Clk_PLLRdy_IrqHandler(uint8_t u8Dummy)
{
    if (NULL != m_pfnPLLCallback)
    {
        m_pfnPLLCallback();
    }

    M0P_CLOCK->INT_CLR_f.PLL = TRUE;
}

/**
 *******************************************************************************
 ** \brief  HSXT ready interrupt handler function
 **
 ** \param [in] u8Dummy           Dummy parameter
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Clk_HSXTRdy_IrqHandler(uint8_t u8Dummy)
{
    if (NULL != m_pfnHSXTCallback)
    {
        m_pfnHSXTCallback();
    }

    M0P_CLOCK->INT_CLR_f.MAIN = TRUE;
}

/**
 *******************************************************************************
 ** \brief  LSXT ready interrupt handler function
 **
 ** \param [in] u8Dummy           Dummy parameter
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Clk_LSXTRdy_IrqHandler(uint8_t u8Dummy)
{
    if (NULL != m_pfnLSXTCallback)
    {
        m_pfnLSXTCallback();
    }

    M0P_CLOCK->INT_CLR_f.SUB = TRUE;
}

/**
 *******************************************************************************
 ** \brief CSV interrupt handler function
 **
 ** \param [in] u8Dummy           Dummy parameter
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Clk_CsvIrqHandler(uint8_t u8Dummy)
{
    if (TRUE == M0P_CLOCK->INT_PEND_f.AFD)
    {
        if (NULL != m_pfnAfdCallback)
        {
            m_pfnAfdCallback();
        }

        M0P_CLOCK->INT_CLR_f.AFD = TRUE;
    }
}

/**
 *******************************************************************************
 ** \brief Set chip clock input source.
 **
 ** \param [in] enSource          clock input source
 ** \arg ClkHICR                  Internal High CR Clock Oscillator
 ** \arg ClkHSXT                  External High CR Clock Oscillator
 ** \arg ClkMPLL                  Main PLL Clock
 ** \arg ClkLICR                  Internal Low CR Clock Oscillator
 ** \arg ClkLSXT                  external low CR Clock Oscillator
 **
 ** \retval Ok                    switch clock successfully.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enSource is invalid
 ** \retval Error  If one of following conditions are met:
 **                               - switch stable timeout
 **
 ******************************************************************************/
en_result_t Clk_SwitchTo(en_clk_source_t enSource)
{
    uint32_t u32To = 0;
    uint8_t u8val1 = 0;
    volatile uint8_t u8val2 = 0;

    ASSERT(IS_VALID_SRC(enSource));

    switch (enSource)
    {
        case ClkHICR:
            ClkSetHICR();
            break;

        case ClkHSXT:
            ClkSetHSXT();
            break;

        case ClkLICR:
            ClkSetLICR();
            break;

        case ClkLSXT:      /* Always possible */
            ClkSetLSXT();
            break;

        case ClkMPLL:
            ClkSetMPLL();
            break;

        default:
            return ErrorInvalidParameter;
    }

    /* Wait until switch stable */
    u32To = CLK_TIMEOUT;
    u8val1 = M0P_CLOCK->CTRL_f.INPUT;

    while (--u32To > 0)
    {
        u8val2 = M0P_CLOCK->CLK_STB_f.SRC;
        if (u8val1 == u8val2)
        {
            break;
        }
    }

    if (u8val1 == u8val2)
    {
        return Ok;
    }

    return Error;
}

/**
 *******************************************************************************
 ** \brief Get the indicated clock source frequence.
 **
 ** \param [in] enSource          clock input source
 ** \arg ClkHICR                  Internal High CR Clock Oscillator
 ** \arg ClkHSXT                  External High CR Clock Oscillator
 ** \arg ClkMPLL                  Main PLL Clock
 ** \arg ClkLICR                  Internal Low CR Clock Oscillator
 ** \arg ClkLSXT                  external low CR Clock Oscillato
 **
 ** \retval base clock frequency
 **
 ** \note if enSource is invalid, this retval value will is zero
 **
 ******************************************************************************/
uint32_t Clk_GetSourceFreq(en_clk_source_t enSource)
{
    /* update systemCoreClock value. */
    uint32_t u32Val = 0;

    ASSERT(IS_VALID_SRC(enSource));

    switch (enSource)
    {
        case ClkHICR:
            u32Val = CLK_HICR_VAL;
            break;
        case ClkHSXT:
            u32Val = CLK_HSXT_VAL;
            break;
        case ClkMPLL:
            if(M0P_CLOCK->PLL_CTL2_f.PRS)
            {
                u32Val = CLK_HICR_VAL;
            }
            else
            {
                u32Val = CLK_HSXT_VAL;
            }
            u32Val *= M0P_CLOCK->PLL_CTL2_f.PLLN;
            break;
        case ClkLICR:
            u32Val = CLK_LICR_VAL;
            break;
        case ClkLSXT:
            u32Val = CLK_LSXT_VAL;
            break;
        default:
            break;
    }

    return u32Val;
}

/**
 *******************************************************************************
 ** \brief get the HCLK and Update the System Core Clock with current core Clock
 **      retrieved from cpu registers.
 **
 ** \param [in]                   None
 **
 ** \retval the current clock frequency
 **
 ******************************************************************************/
uint32_t Clk_GetSystemClk(void)
{
    uint8_t u8Val = 0;
    uint32_t u32Val = 0;

    u8Val = M0P_CLOCK->CLK_STB_f.SRC;
    u32Val = Clk_GetSourceFreq((en_clk_source_t)u8Val);

    /* base div */
    u8Val = M0P_CLOCK->BC_DIV_f.DIV;

    switch (u8Val)
    {
        case ClkBaseDiv1:
            break;
        case ClkBaseDiv2:
            u32Val = u32Val / 2;
            break;
        case ClkBaseDiv3:
            u32Val = u32Val / 3;
            break;
        case ClkBaseDiv4:
            u32Val = u32Val / 4;
            break;
        case ClkBaseDiv6:
            u32Val = u32Val / 6;
            break;
        case ClkBaseDiv8:
            u32Val = u32Val / 8;
            break;
        case ClkBaseDiv16:
            u32Val = u32Val / 16;
            break;
        default:
            break;
    }

    SystemCoreClock = u32Val;

    return u32Val;
}

/**
 *******************************************************************************
 ** \brief get requested peripheral clock freqency.
 **
 ** \param [in] enIdx             peripheral index
 **
 ** \retval requested peripheral clock freqency.
 **
 ** \note if enIdx is invalid or equal ClkPeripheralFlash, this retval value will is zero
 **
 ******************************************************************************/
uint32_t Clk_GetPeripheralClk(en_clk_periph_idx_t enIdx)
{
    uint32_t u32Val = 0;
    uint8_t u8div = 0;
    const uint8_t mu8apbdiv[] = { 1, 2, 4, 8 };

    ASSERT(enIdx < ClkPeripheralMax);

    u32Val = Clk_GetSystemClk();

    switch (enIdx)
    {
        case ClkPeripheralClock:
        case ClkPeripheralReset:
        case ClkPeripheralHwwdt:
        case ClkPeripheralSwwdt:
        case ClkPeripheralDt:
            u8div = M0P_CLOCK->APB0_CTRL_f.DIV;
            break;
        case ClkPeripheralCt:
        case ClkPeripheralAdc:
        case ClkPeripheralExti:
        case ClkPeripheralLcd:
        case ClkPeripheralGpio:
        case ClkPeripheralLvd:
        case ClkPeripheralMsc:
        case ClkPeripheralVc:
            u8div = M0P_CLOCK->APB1_CTRL_f.DIV;
            break;
        case ClkPeripheralFlash:
            u8div = 0;
            break;
        default:
            return 0;
    }

    u8div = u8div & 0x3;
    u8div = mu8apbdiv[u8div];
    u32Val = u32Val / u8div;

    return u32Val;
}

/**
 *******************************************************************************
 ** \brief Initialize clock.
 **
 ** \param [in] pstcCfg           pointer to clock config structure
 **
 ** \retval Ok                    Initialize successfully.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - pstcCfg == NULL
 **                               - pstcCfg has invalid param
 **
 ******************************************************************************/
en_result_t Clk_Init(stc_clk_config_t *pstcCfg)
{
    uint32_t u32MainOSCFreq = CLK_HSXT_VAL;
    uint32_t u32PllFreq;

    ASSERT(IS_VALID_POINTER(pstcCfg));
    ASSERT(IS_VALID_CLK_BASE_DIV(pstcCfg->enBaseClkDiv));
    ASSERT(IS_VALID_CLK_APB0_DIV(pstcCfg->enAPB0Div));
    ASSERT(IS_VALID_CLK_APB1_DIV(pstcCfg->enAPB1Div));
    ASSERT(IS_VALID_HSXT_WATI_TIME(pstcCfg->enClkWaitMain));
    ASSERT(IS_VALID_LSXT_WATI_TIME(pstcCfg->enClkWaitSub));
    ASSERT(IS_VALID_PLL_WATI_TIME(pstcCfg->enClkWaitPll));
    ASSERT(IS_VALID_PLL_CLK_SRC(pstcCfg->enPllSrc));
    ASSERT(IS_VALID_EN(pstcCfg->bAPB1En));
    ASSERT(IS_VALID_EN(pstcCfg->bHicrEn));
    ASSERT(IS_VALID_EN(pstcCfg->bMainEn));
    ASSERT(IS_VALID_EN(pstcCfg->bSubEn));
    ASSERT(IS_VALID_EN(pstcCfg->bPllEn));
    ASSERT(IS_VALID_EN(pstcCfg->bPLLIrq));
    ASSERT(IS_VALID_EN(pstcCfg->bHSXTIrq));
    ASSERT(IS_VALID_EN(pstcCfg->bLSXTIrq));
    ASSERT(IS_VALID_SRC(pstcCfg->enClkSrc));

    if(NULL == pstcCfg)
    {
        return ErrorInvalidParameter;
    }

    M0P_CLOCK->BC_DIV_f.DIV = pstcCfg->enBaseClkDiv;
    M0P_CLOCK->APB0_CTRL_f.DIV = pstcCfg->enAPB0Div;

    if (TRUE == pstcCfg->bAPB1En)
    {
        M0P_CLOCK->APB1_CTRL_f.DIV = pstcCfg->enAPB1Div;
        M0P_CLOCK->APB1_CTRL_f.EN = TRUE;
    }

    /* Configure interrupt */
    if (TRUE == pstcCfg->bPLLIrq)
    {
        m_pfnPLLCallback = pstcCfg->pfnPLLRdyCb;
        M0P_CLOCK->INT_CTRL_f.PLLEN = TRUE;
        ClkSetIrq(ClkIrqPLLRdy, TRUE);
    }

    if (TRUE == pstcCfg->bHSXTIrq)
    {
        m_pfnHSXTCallback = pstcCfg->pfnHSXTRdyCb;
        M0P_CLOCK->INT_CTRL_f.MAINEN = TRUE;
        ClkSetIrq(ClkIrqHSXTRdy, TRUE);
    }

    if (TRUE == pstcCfg->bLSXTIrq)
    {
        m_pfnLSXTCallback = pstcCfg->pfnLSXTRdyCb;
        M0P_CLOCK->INT_CTRL_f.SUBEN = TRUE;
        ClkSetIrq(ClkIrqLSXTRdy, TRUE);
    }

    if (TRUE == pstcCfg->bHicrEn)
    {
        M0P_CLOCK->CTRL_f.HC_EN = TRUE;
    }

    if (TRUE == pstcCfg->bMainEn)
    {
        if (u32MainOSCFreq < 4*1000*1000)
        {
            return ErrorInvalidParameter;
        }
        else if (u32MainOSCFreq <= 6*1000*1000)
        {
            M0P_OSC_CTL->HSXT_PWRCTL_f.HXSEL = 0;
        }
        else if (u32MainOSCFreq <= 12*1000*1000)
        {
            M0P_OSC_CTL->HSXT_PWRCTL_f.HXSEL = 1;
        }
        else if (u32MainOSCFreq <= 16*1000*1000)
        {
            M0P_OSC_CTL->HSXT_PWRCTL_f.HXSEL = 2;
        }
        else
        {
            return ErrorInvalidParameter;
        }

        M0P_CLOCK->CSWT_f.MAINWT = pstcCfg->enClkWaitMain & 0xF;
        M0P_CLOCK->CTRL_f.MAIN_EN = TRUE;
    }
    if (TRUE == pstcCfg->bSubEn)
    {
        M0P_CLOCK->CSWT_f.SUBWT = pstcCfg->enClkWaitSub & 0xF;
        M0P_CLOCK->CTRL_f.SUB_EN = TRUE;
    }
    if (TRUE == pstcCfg->bPllEn)
    {
        M0P_CLOCK->PSWT_f.PLLWT = pstcCfg->enClkWaitPll & 0x3;
        M0P_CLOCK->PLL_CTL2_f.PRS = pstcCfg->enPllSrc;
        if(MAIN2PLL == pstcCfg->enPllSrc)
        {
            u32MainOSCFreq = CLK_HSXT_VAL;
        }
        else if(HICR2PLL == pstcCfg->enPllSrc)
        {
            u32MainOSCFreq = CLK_HICR_VAL;
        }
        else
        {

        }

        if (u32MainOSCFreq < 4*1000*1000u)
        {
            return Error;
        }
        else if (u32MainOSCFreq <= 6*1000*1000u)
        {
            M0P_CLOCK->PLL_CTL1_f.PLLFR = 0;
        }
        else if (u32MainOSCFreq <= 12*1000*1000u)
        {
            M0P_CLOCK->PLL_CTL1_f.PLLFR = 1;
        }
        else if (u32MainOSCFreq <= 16*1000*1000)
        {
            M0P_CLOCK->PLL_CTL1_f.PLLFR = 2;
        }
        else
        {
            return ErrorInvalidParameter;
        }

        u32PllFreq = pstcCfg->u8PllMulti * u32MainOSCFreq;
        if (u32PllFreq > (48*1000*1000u))
        {
            return Error;
        }
        else if (u32PllFreq <= (24*1000*1000u))  /* if ( 8MHz, 24MHz], set flash read 0 wait cycle */
        {
            M0P_FLASH_IF->FWCICR_f.FWTC = 1;
        }
        else    /* if (24MHz, 48MHz], set flash read 1 wait cycle */
        {
            M0P_FLASH_IF->FWCICR_f.FWTC = 0;
        }

        if (u32PllFreq >= (36*1000*1000u))
        {
            M0P_CLOCK->PLL_CTL1_f.PLLFO = 4;
        }
        else if (u32PllFreq >= (24*1000*1000u))
        {
            M0P_CLOCK->PLL_CTL1_f.PLLFO = 3;
        }
        else if (u32PllFreq >= (18*1000*1000u))
        {
            M0P_CLOCK->PLL_CTL1_f.PLLFO = 2;
        }
        else if (u32PllFreq >= (12*1000*1000u))
        {
            M0P_CLOCK->PLL_CTL1_f.PLLFO = 1;
        }
        else if (u32PllFreq >= (8*1000*1000u))
        {
            M0P_CLOCK->PLL_CTL1_f.PLLFO = 0;
        }
        else
        {
            return ErrorInvalidParameter;
        }

        M0P_CLOCK->PLL_CTL2_f.PLLN = pstcCfg->u8PllMulti;
        M0P_CLOCK->CTRL_f.PLL_EN = TRUE;
    }

    Clk_SwitchTo(pstcCfg->enClkSrc);

     if ((TRUE == pstcCfg->bLSXTIrq) ||
         (TRUE == pstcCfg->bHSXTIrq) ||
         (TRUE == pstcCfg->bPLLIrq))
    {
        ClkEnableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-initialize clock.
 **
 ** \param [in] bTouchNvic        Set clock NVIC interrupt
 ** \arg TRUE                     Disable clock NVIC interrupt
 ** \arg FALSE                    Don't set clock NVIC interrupt
 **
 ** \retval Ok                    De-nitialize successfully.
 **
 ******************************************************************************/
en_result_t Clk_DeInit(boolean_t bTouchNvic)
{
    ASSERT(IS_VALID_EN(bTouchNvic));

    /* Set clock to HCR 4Mhz */
    Clk_SwitchTo(ClkHICR);

    if (TRUE == bTouchNvic)
    {
        ClkDisableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief enable/disalbe to a paticular clock input.
 **
 ** \param [in] enSource          clock input source
 ** \arg ClkHICR                  Internal High CR Clock Oscillator
 ** \arg ClkHSXT                  External High CR Clock Oscillator
 ** \arg ClkMPLL                  Main PLL Clock
 ** \arg ClkLICR                  Internal Low CR Clock Oscillator
 ** \arg ClkLSXT                  External low CR Clock Oscillator
 ** \param [in] bFlag             Setting clock flag
 ** \arg TRUE                     Disable clock NVIC interrupt
 ** \arg FALSE                    Don't set clock NVIC interrupt
 **
 ** \retval Ok                    Enable successfully.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enSource is invalid source type.
 **
 ******************************************************************************/
en_result_t Clk_Enable(en_clk_source_t enSource, boolean_t bFlag)
{
    en_result_t enRet = Ok;

    ASSERT(IS_VALID_SRC(enSource));
    ASSERT(IS_VALID_EN(bFlag));

    switch (enSource)
    {
        case ClkHICR:
            M0P_CLOCK->CTRL_f.HC_EN = bFlag;
            break;
        case ClkHSXT:
            M0P_CLOCK->CTRL_f.MAIN_EN = bFlag;
            break;
        case ClkLSXT:
            M0P_CLOCK->CTRL_f.SUB_EN = bFlag;
            break;
        case ClkMPLL:
            M0P_CLOCK->CTRL_f.PLL_EN = bFlag;
            break;
        case ClkLICR:
        default:
            enRet = ErrorInvalidParameter;
            break;
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief set target clock source that switch to.
 **
 ** \param [in] enSource          clock input source
 ** \arg ClkHICR                  Internal High CR Clock Oscillator
 ** \arg ClkHSXT                  External High CR Clock Oscillator
 ** \arg ClkMPLL                  Main PLL Clock
 ** \arg ClkLICR                  Internal Low CR Clock Oscillator
 ** \arg ClkLSXT                  External low CR Clock Oscillator
 **
 ** \retval Ok                    Set successfully.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enSource is invalid source type.
 **
 ******************************************************************************/
en_result_t Clk_SetSource(en_clk_source_t enSource)
{
    en_result_t enRet = Ok;

    ASSERT(IS_VALID_SRC(enSource));

    switch (enSource)
    {
        case ClkHSXT:
        case ClkHICR:
        case ClkMPLL:
        case ClkLICR:
        case ClkLSXT:
            M0P_CLOCK->CTRL_f.INPUT = enSource;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get clock status.
 **
 ** \param [in] enSource          clock input source
 ** \arg ClkHICR                  Internal High CR Clock Oscillator
 ** \arg ClkHSXT                  External High CR Clock Oscillator
 ** \arg ClkMPLL                  Main PLL Clock
 ** \arg ClkLICR                  Internal Low CR Clock Oscillator
 ** \arg ClkLSXT                  External low CR Clock Oscillator
 **
 ** \retval target clock source status.
 **
 ** \note this retval valid is FALSE will when enSource is invalid clock source type or
 **      the target clock source isn't ready.
 **
 ******************************************************************************/
boolean_t Clk_GetClkRdy(en_clk_source_t enSource)
{
    boolean_t bFlag = FALSE;

    ASSERT(IS_VALID_SRC(enSource));

    switch (enSource)
    {
        case ClkHICR:
            bFlag = M0P_CLOCK->CLK_STB_f.HC_RDY;
            break;
        case ClkHSXT:
            bFlag = M0P_CLOCK->CLK_STB_f.MAIN_RDY;
            break;
        case ClkLSXT:
            bFlag = M0P_CLOCK->CLK_STB_f.SUB_RDY;
            break;
        case ClkLICR:
            /* dummy case */
            bFlag = TRUE;
            break;
        default:
            break;
    }

    return bFlag;
}

/**
 *******************************************************************************
 ** \brief Get current clock source type.
 **
 ** \param [in]                   None
 **
 ** \retval ClkHICR               Internal High CR Clock Oscillator
 ** \retval ClkHSXT               External High CR Clock Oscillator
 ** \retval ClkMPLL               Main PLL Clock
 ** \retval ClkLICR               Internal Low CR Clock Oscillator
 ** \retval ClkLSXT               External low CR Clock Oscillator
 **
 ** \note before use the new clock source, check the new clock soruce is valid.
 **
 ******************************************************************************/
en_clk_source_t Clk_GetCurSource(void)
{
    return (en_clk_source_t)M0P_CLOCK->CLK_STB_f.SRC;
}

/**
 *******************************************************************************
 ** \brief enable/disable apb1 bus.
 **
 ** \param [in] bFlag             Setting apb1 bus flag
 ** \arg TRUE                     Enable apb1 bus
 ** \arg FALSE                    Diable apb1 bus
 **
 ** \retval Ok                    Enable successfully.
 **
 ******************************************************************************/
en_result_t Clk_EnableAPB1(boolean_t bFlag)
{
    ASSERT(IS_VALID_EN(bFlag));

    M0P_CLOCK->APB1_CTRL_f.EN = bFlag;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get the chip reset cause.
 **
 ** \param [in] pstcData          Pointer to return reset cause structure.
 **
 ** \retval Ok                    Get successfully.
 **
 ******************************************************************************/
en_result_t Clk_GetResetCause(stc_clk_rstcause_t *pstcData)
{
    uint16_t u16RstCause = 0;
    stc_clock_stat_field_t *STAT_f = NULL;

    ASSERT(IS_VALID_POINTER(pstcData));

    if(NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    u16RstCause = M0P_CLOCK->STAT;
    STAT_f = (stc_clock_stat_field_t *)(&u16RstCause);

    pstcData->bSoftware = STAT_f->SW;
    pstcData->bAfd = STAT_f->AF;
    pstcData->bCfd = STAT_f->CF;
    pstcData->bHwwdt = STAT_f->HWWDT;
    pstcData->bSwwdt = STAT_f->SWWDT;
    pstcData->bLvd = STAT_f->LVD;
    pstcData->bRstPin = STAT_f->RSTB;
    pstcData->bPowerOn = STAT_f->PWR;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable a clk peripheral funciton.
 **
 ** \param [in] enFunc            funciton event enumeration
 **
 ** \retval Ok                    Functions enabled
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enFunc is invalid
 **
 ******************************************************************************/
en_result_t Clk_EnableFunc(en_clk_func_t enFunc)
{
    ASSERT(IS_VALID_FUNC(enFunc));

    return ClkSetFunc(enFunc, TRUE);
}

/**
 *******************************************************************************
 ** \brief Disable a clk peripheral funciton.
 **
 ** \param [in] enFunc            clock peripheral funciton enum
 **
 ** \retval Ok                    Functions disabled.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enFunc is invalid
 **
 ******************************************************************************/
en_result_t Clk_DisableFunc(en_clk_func_t enFunc)
{
    ASSERT(IS_VALID_FUNC(enFunc));

    return ClkSetFunc(enFunc, FALSE);
}

/**
 *******************************************************************************
 ** \brief Set software watchdog timer clock frequency division.
 **
 ** \param [in] enDiv             input clock division
 **
 ** \retval Ok                    Set successfully.
 **
 ******************************************************************************/
en_result_t Clk_SetSwwdtDiv(en_clk_swwdt_div_t enDiv)
{
    ASSERT(IS_VALID_CLK_SWWDT_DIV(enDiv));

    M0P_CLOCK->SWDG_CTRL_f.DIV = enDiv;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Initialize clock csv function.
 **
 ** \param [in] pstcCfg           Pointer to clock csv configure structure
 **
 ** \retval Ok                    Initialize successfully.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCfg == NULL
 **
 ******************************************************************************/
en_result_t Clk_CsvInit(stc_clk_csv_config_t *pstcCfg)
{
    stc_clock_fcm_ctl_field_t stcCfg;

    ASSERT(NULL != pstcCfg);
    ASSERT(IS_VALID_AFDDIV(pstcCfg->enAfdDiv));

    if(NULL == pstcCfg)
    {
        return ErrorInvalidParameter;
    }

    DDL_ZERO_STRUCT(stcCfg);

    stcCfg.AFDCC = pstcCfg->enAfdDiv;
    stcCfg.AFDRE = pstcCfg->bAfdResetEn;
    stcCfg.AFDE = pstcCfg->bAfdEn;
    stcCfg.SFCMRE = pstcCfg->bLSXTFailResetEn;
    stcCfg.MFCMRE = pstcCfg->bHSXTFailResetEn;
    stcCfg.SFCME = pstcCfg->bLSXTFailDetectEn;
    stcCfg.MFCME = pstcCfg->bHSXTFailDetectEn;

    M0P_CLOCK->FCM_CTL_f = stcCfg;

    if (NULL != pstcCfg->pstcAfdIrqCb)
    {
        m_pfnAfdCallback = pstcCfg->pstcAfdIrqCb->pfnAfdIrqCb;
    }

    if (NULL != pstcCfg->pstcAfdIrqSel)
    {
        M0P_CLOCK->INT_CTRL_f.AFDEN = pstcCfg->pstcAfdIrqSel->bAfdIrqEn;
        ClkAfdEnableNvic();
    }
    else
    {
        M0P_CLOCK->INT_CTRL_f.AFDEN = FALSE;
        ClkAfdDisableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-initialize clock csv function.
 **
 ** \param [in] bTouchNvic        Set clock NVIC interrupt
 ** \arg TRUE                     Disable clock NVIC interrupt
 ** \arg FALSE                    Don't set clock NVIC interrupt
 **
 ** \retval Ok                    Set successfully.
 **
 ******************************************************************************/
en_result_t Clk_CsvDeInit(boolean_t bTouchNvic)
{
    if(TRUE == bTouchNvic)
    {
        ClkAfdDisableNvic();
    }

    M0P_CLOCK->FCM_CTL = 0u;
    M0P_CLOCK->INT_CTRL_f.AFDEN = FALSE;
    m_pfnAfdCallback = NULL;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief set afd input clock division
 **
 ** \param [in] enDiv             division value enum
 **
 ** \retval Ok                    Set successfully.
 **
 ******************************************************************************/
en_result_t Clk_SetAfdDiv(en_clk_afd_div_t enDiv)
{
    ASSERT(IS_VALID_AFDDIV(enDiv));

    M0P_CLOCK->FCM_CTL_f.AFDCC = enDiv;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief get cfd status.
 **
 ** \param [in] enType            clock cfd type
 **
 ** \retval TRUE                  Detected cfd.
 ** \retval FALSE                 Don't detected cfd.
 **
 ******************************************************************************/
boolean_t Clk_Csv_GetCfdStat(en_clk_cfd_stat_t enType)
{
    boolean_t bFlag = FALSE;

    ASSERT(IS_VALID_CFDSTAT(enType));

    switch (enType)
    {
        case ClkCfdHSXT:
            bFlag = M0P_CLOCK->FCM_STR_f.MCFDF;
            break;
        case ClkCfdLSXT:
            bFlag = M0P_CLOCK->FCM_STR_f.SCFDF;
            break;
        default:
            break;
    }

    return bFlag;
}

/**
 *******************************************************************************
 ** \brief set afd detect range.
 **
 ** \param [in]  u16High          window upper value
 ** \param [in]  u16Low           window lower value
 **
 ** \retval Ok                    Set done
 **
 ******************************************************************************/
en_result_t Clk_SetAfdWindow(uint16_t u16Low, uint16_t u16High)
{
    M0P_CLOCK->AFDWH = u16High;
    M0P_CLOCK->AFDWL = u16Low;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get afd frequency.
 **
 ** \param [in]  None
 **
 ** \retval afd frequency
 **
 ******************************************************************************/
uint16_t Clk_GetAfdFreq(void)
{
    return M0P_CLOCK->AFDC;
}

/**
 *******************************************************************************
 ** \brief Enable clock interrupt
 **
 ** \param [in] enSel             clock interrupt type
 **
 ** \retval Ok                    Functions enabled
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enSel is invalid
 **
 ******************************************************************************/
en_result_t Clk_EnableIrq(en_clk_irq_sel_t enSel)
{
    ASSERT(IS_VALID_IRQ(enSel));

    return ClkSetIrq(enSel, TRUE);
}

/**
 *******************************************************************************
 ** \brief Enable clock interrupt
 **
 ** \param [in] enSel             clock interrupt type
 **
 ** \retval Ok                    Functions disabled
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enSel is invalid
 **
 ******************************************************************************/
en_result_t Clk_DisableIrq(en_clk_irq_sel_t enSel)
{
    ASSERT(IS_VALID_IRQ(enSel));

    return ClkSetIrq(enSel, FALSE);
}

/**
 *******************************************************************************
 ** \brief Get clock interrupt flag
 **
 ** \param [in] enSel             clock interrupt type
 **
 ** \retval Ok                    Get successfully.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enSel is invalid
 **
 ******************************************************************************/
en_result_t Clk_GetIrqStat(en_clk_irq_sel_t enSel, boolean_t *pbIrqStat)
{
    en_result_t enRet = Ok;

    ASSERT(IS_VALID_IRQ(enSel));

    switch (enSel)
    {
        case ClkIrqAfd:
            *pbIrqStat = M0P_CLOCK->INT_PEND_f.AFD;
            break;
        case ClkIrqPLLRdy:
            *pbIrqStat = M0P_CLOCK->INT_PEND_f.PLL;
            break;
        case ClkIrqHSXTRdy:
            *pbIrqStat = M0P_CLOCK->INT_PEND_f.MAIN;
            break;
        case ClkIrqLSXTRdy:
            *pbIrqStat = M0P_CLOCK->INT_PEND_f.SUB;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Clear clock interrupt flag
 **
 ** \param [in] enSel             clock interrupt type
 **
 ** \retval Ok                    Clear successfully.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - enSel is invalid
 **
 ******************************************************************************/
en_result_t Clk_ClearIrq(en_clk_irq_sel_t enSel)
{
    en_result_t enRet = Ok;

    ASSERT(IS_VALID_IRQ(enSel));

    switch (enSel)
    {
        case ClkIrqAfd:
            M0P_CLOCK->INT_CLR_f.AFD = TRUE;
            break;
        case ClkIrqPLLRdy:
            M0P_CLOCK->INT_CLR_f.PLL = TRUE;
            break;
        case ClkIrqHSXTRdy:
            M0P_CLOCK->INT_CLR_f.MAIN = TRUE;
            break;
        case ClkIrqLSXTRdy:
            M0P_CLOCK->INT_CLR_f.SUB = TRUE;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

//@} // ClkGroup

/*******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
