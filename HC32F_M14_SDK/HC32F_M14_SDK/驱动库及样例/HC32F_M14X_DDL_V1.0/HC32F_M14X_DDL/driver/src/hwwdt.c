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
/** \file hwwdt.c
 **
 ** A detailed description is available at
 ** @link HwwdtGroup Hwwdt description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Hwwdt
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hwwdt.h"

/**
 *******************************************************************************
 ** \addtogroup HwwdtGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define HWWDT_CTL_INTEN       (0x01u)
#define HWWDT_CTL_INTDIS      (0xFEu)
#define HWWDT_CTL_RESEN       (0x02u)
#define HWWDT_CTL_RESDIS      (0xFDu)
#define HWWDT_CTL_CLKSRC_EXT  (0x04u)
#define HWWDT_CTL_CLKSRC_LCR  (0xFBu)

#define HWWDT_MAGIC_WORD_CHK1        (0x38D1AE5Cu)
#define HWWDT_MAGIC_WORD_CHK2        (0x7624D1BCu)
#define HWWDT_MAGIC_WORD_CHK_RESULT  (0xFFFFFFFFu)

#define HWWDT_REG_UNLOCK_1  (0x1ACCE551u)
#define HWWDT_REG_UNLOCK_2  (0xE5331AAEu)

#define IS_VALID_POINTER(x)  (NULL != (x))  ///< Parameter validity check for pointer

/*! Parameter validity check for enable flag  */
#define IS_VALID_EN(x)                  \
(   (TRUE == (x)) ||                    \
    (FALSE == (x)))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static func_ptr_t m_pfnHwwdtCallback = NULL; /* callback function pointer for HW-Wdt Irq */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Hardware Watchdog Timer Interrupt Handler
 **
 ** \param [in] u8Dummy                 Dummy parameter
 **
 ** \retval                             None
 **
 ******************************************************************************/
void Hwwdt_IrqHandler(uint8_t u8Dummy)
{
    /* Check hardware interrupt status */
    if (TRUE == M0P_HWWDT->WDT_STA_f.STA)
    {
        if (NULL != m_pfnHwwdtCallback)
        {
            m_pfnHwwdtCallback();
        }
    }
}

/**
 *******************************************************************************
 ** \brief Initialize Hardware Watchdog Timer
 **
 ** \param [in] pstcConfig        Pointer to Hardware Watchdog Timer configuration
 **
 ** \retval Ok                    Setup successful
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - pstcConfig == NULL
 **
 ** \note This function only set the Hardware Watchdog Timer configuration.
 **       If Hwwdt_Start() is called, MCU start the Hardware Watchdog Timer.
 **
 ******************************************************************************/
en_result_t Hwwdt_Init(stc_hwwdt_config_t* pstcConfig)
{
    uint8_t u8WdogControl = 0u;      /* Preset register */

    ASSERT(IS_VALID_POINTER(pstcConfig));
    ASSERT(IS_VALID_EN(pstcConfig->bResetEnable));

    if(NULL == pstcConfig)
    {
        return ErrorInvalidParameter;
    }

    /* Check for NULL Pointer */
    if (NULL != pstcConfig)
    {
        /* Release Lock */
        M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_1;
        M0P_HWWDT->WDT_LD = pstcConfig->u32LoadValue;

        if (TRUE == pstcConfig->bResetEnable)
        {
            /* RESEN bit */
            u8WdogControl |= HWWDT_CTL_RESEN;
        }
        else
        {
            u8WdogControl &= HWWDT_CTL_RESDIS;
        }

        /* HW Watchdog Control Register unlock sequence */
        M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_1;
        M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_2;

        /* Setup HW-Watchdog and start */
        M0P_HWWDT->WDT_CTL = u8WdogControl;

        /* Initialize callback to NULL */
        m_pfnHwwdtCallback = pstcConfig->pfnHwwdtIrqCb;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Initialize Hardware Watchdog Timer
 **
 ** \param [in] u32MagicWord1     1st Magic word for disabling (0xC72E51A3)
 ** \param [in] u32MagicWord2     2nd Magic word for disabling (0x89DB2E43)
 **
 ** \retval Ok                    Disable sucessful
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                               - not diabled => magic word is wrong!
 **
 ** \note This function disables and un-initializes the Hardware Watchdog Timer,
 **       when the first argument is 0xC72E51A3 and the second agrument
 **       0x89DB2E43. The magic words are together 64-bit wide and have a
 **       balanced entropy.(32 zero and 32 one bits)
 **
 ******************************************************************************/
en_result_t Hwwdt_DeInit(uint32_t u32MagicWord1, uint32_t u32MagicWord2)
{
    en_result_t enResult = ErrorInvalidParameter;

    /* Inverted magic word check is done to avoid "plain text magic word" in ROM. */
    if ((HWWDT_MAGIC_WORD_CHK_RESULT == (u32MagicWord1 ^ HWWDT_MAGIC_WORD_CHK1)) &&
        (HWWDT_MAGIC_WORD_CHK_RESULT == (u32MagicWord2 ^ HWWDT_MAGIC_WORD_CHK2))
       )
    {
        /* HW Watchdog Control Register unlock sequence */
        M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_1;
        M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_2;

        /* HW Watchdog stop */
        M0P_HWWDT->WDT_CTL = 0;

        /* Initialize callback to NULL */
        m_pfnHwwdtCallback = NULL;

        enResult = Ok;
    }

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief Start the Hardware Watchdog Timer
 **
 ** \param [in]                   None
 **
 ** \retval Ok                    Setup successful
 ** \retval ErrorOperationInProgress  Hardware Watchdog is active now
 **
 ******************************************************************************/
en_result_t Hwwdt_Start(void)
{
    en_result_t enResult;

    /* If hardware watchdog is active, error is returned. */
    if (TRUE == M0P_HWWDT->WDT_CTL_f.IE)
    {
        enResult = ErrorOperationInProgress;
    }
    else
    {
        /* HW Watchdog Control Register unlock sequence */
        M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_1;
        M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_2;

        /* Enable interrupt and count */
        M0P_HWWDT->WDT_CTL_f.IE = TRUE;

        enResult = Ok;
    }

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief Stop the Hardware Watchdog Timer
 **
 ** \param [in]                   None
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Hwwdt_Stop(void)
{
    if (TRUE == M0P_HWWDT->WDT_CTL_f.IE)
    {
        /* HW Watchdog Control Register unlock sequence */
        M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_1;
        M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_2;

        /* Disable interrupt and count */
        M0P_HWWDT->WDT_CTL_f.IE = FALSE;
    }
}

/**
 *******************************************************************************
 ** \brief Write the load value for Hardware Watchdog Timer
 **
 ** \param [in] u32LoadValue      Load value
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Hwwdt_WriteWdtLoad(uint32_t u32LoadValue)
{
    M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_1;   /* Release Lock */
    M0P_HWWDT->WDT_LD = u32LoadValue;          /* Write load value */
}

/**
 *******************************************************************************
 ** \brief Read the count value for Hardware Watchdog Timer
 **
 ** \param [in]                   None
 **
 ** \retval Value of value register
 **
 ******************************************************************************/
uint32_t Hwwdt_ReadWdtValue(void)
{
    return (M0P_HWWDT->WDT_VL);
}

/**
 *******************************************************************************
 ** \brief Feed Hardware Watchdog Timer(Call function)
 **
 ** \param [in] u8ClearPattern1   Pattern of arbitrary value
 ** \param [in] u8ClearPattern2   Inverted arbitrary value
 **
 ** \retval                       None
 **
 ** \note This function feeds the Hardware Watchdog Timer with the unlock, feed,
 **       and lock sequence. Take care of the arbitrary values, because there
 **       are not checked for plausibility!
 **
 ******************************************************************************/
void Hwwdt_Feed(uint8_t u8ClearPattern1, uint8_t u8ClearPattern2)
{
    M0P_HWWDT->WDT_LCK = HWWDT_REG_UNLOCK_1;    /* Release Lock */
    M0P_HWWDT->WDT_CLR = u8ClearPattern1;       /* Clear possible interrupt and reload value, step 1 */
    M0P_HWWDT->WDT_CLR = u8ClearPattern2;       /* Clear possible interrupt and reload value, step 2 */
}

/**
 *******************************************************************************
 ** \brief Enable Debug Break Hardware Watchdog Timer Control
 **
 ** \param [in]                   None
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Hwwdt_EnableDbgBrkWdtCtl(void)
{
    stc_clock_dbwdt_ctrl_field_t stcDBWDT_CTRL;

    stcDBWDT_CTRL = M0P_CLOCK->DBWDT_CTRL_f;
    stcDBWDT_CTRL.HWWDT_BRK = TRUE;
    M0P_CLOCK->DBWDT_CTRL_f = stcDBWDT_CTRL;
}

/**
 *******************************************************************************
 ** \brief Disable Debug Break Hardware Watchdog Timer Control
 **
 ** \param [in]                   None
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Hwwdt_DisableDbgBrkWdtCtl(void)
{
    stc_clock_dbwdt_ctrl_field_t stcDBWDT_CTRL;

    stcDBWDT_CTRL = M0P_CLOCK->DBWDT_CTRL_f;
    stcDBWDT_CTRL.HWWDT_BRK  = FALSE;
    M0P_CLOCK->DBWDT_CTRL_f = stcDBWDT_CTRL;
}

//@} // HwwdtGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
