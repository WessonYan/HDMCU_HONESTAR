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
/** \file lvd.c
 **
 ** A detailed description is available at
 ** @link LvdGroup Lvd description @endlink
 **
 **   - 2017-08-09  1.0  WangM First version for Device Driver Library of lvd
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "lvd.h"

/**
 *******************************************************************************
 ** \addtogroup LvdGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_CHANEL(x)              \
(   LvdChanel1 == (x) ||                \
    LvdChanel2 == (x)) 

#define IS_VALID_THRESHOLD_DOWN(x)      \
(   (x)<=Lvd_svhd_4_20V)

#define IS_VALID_THRESHOLD_RISE(x)      \
(   (x)<=Lvd_svhr_4_30V)

#define IS_VALID_IRQTYPE(x)             \
(   LvdTypeIrq == (x) ||                \
    LvdTypeRst == (x))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void LvdEnableNvic(void);
static void LvdDisableNvic(void);
static en_result_t LvdEnable(en_lvd_chanel_t enChanel, boolean_t bFlag);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static func_ptr_t pfnLvdIrqCbk[LvdTypeMax];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief ISR of the LVD module
 **
 ** \param [in]  u8Dummy Dummy parameter for function
 **
 ** \retval none
 **
 ******************************************************************************/
void Lvd_IrqHandler(uint8_t u8Dummy)
{
    if (TRUE == Lvd_GetIrqStat(LvdChanel1))
    {
        if (NULL != pfnLvdIrqCbk[LvdChanel1])
        {
            pfnLvdIrqCbk[LvdChanel1]();
        }
        M0P_WSUNIT->LVD_CLR_f.LVD1_CLR = 1u;
    }

    if (TRUE == Lvd_GetIrqStat(LvdChanel2))
    {
        if (NULL != pfnLvdIrqCbk[LvdChanel2])
        {
            pfnLvdIrqCbk[LvdChanel2]();
        }
        M0P_WSUNIT->LVD_CLR_f.LVD2_CLR = 1u;
    }
}

/**
 *******************************************************************************
 ** \brief Enable LVD NVIC function
 **
 ******************************************************************************/
static void LvdEnableNvic(void)
{
    NVIC_ClearPendingIRQ(LVD_IRQn);
    NVIC_SetPriority(LVD_IRQn, DDL_IRQ_LEVEL_LVD);
    NVIC_EnableIRQ(LVD_IRQn);
}

/**
 *******************************************************************************
 ** \brief Disable LVD NVIC function
 **
 ******************************************************************************/
static void LvdDisableNvic(void)
{
    NVIC_ClearPendingIRQ(LVD_IRQn);
    NVIC_DisableIRQ(LVD_IRQn);
    NVIC_SetPriority(LVD_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief LVD detect enable or disable
 **
 ** \param [in]  enChanel the LVD channel to be controlled
 **        [in]  bFlag    TRUE = Enable
 **                       FALSE = Disale
 **
 ** \retval en_result_t   Ok  LVD function set Done
 **
 ******************************************************************************/
static en_result_t LvdEnable(en_lvd_chanel_t enChanel, boolean_t bFlag)
{
    ASSERT(IS_VALID_CHANEL(enChanel));

    bFlag = bFlag & 1;

    switch (enChanel)
    {
        case LvdChanel1:
            M0P_WSUNIT->LVD1_SET_f.LVD1_EN = bFlag;
            break;
        case LvdChanel2:
            M0P_WSUNIT->LVD2_SET_f.LVD2_EN = bFlag;
            break;
        default:
            break;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set LVD event, interrupt or reset
 **
 ** \param [in]  enChanel The LVD channel to be controlled
 **        [in]  enType   The event type
 **
 ** \retval en_result_t   Ok:  Set Event done
 **                       ErrorInvalidParameter: Invalid parameter
 **
 ******************************************************************************/
en_result_t LvdEventTypeSet(en_lvd_chanel_t enChanel, en_lvd_type_t enType)
{
    en_result_t enRet = Ok;
    
    ASSERT(IS_VALID_CHANEL(enChanel));
    ASSERT(IS_VALID_IRQTYPE(enType));

    switch (enChanel)
    {
        case LvdChanel1:
            if(LvdTypeIrq == enType)
            {
                M0P_WSUNIT->LVD1_SET_f.LVD1_SRST = 0u;
            }
            else
            {
                M0P_WSUNIT->LVD1_SET_f.LVD1_SRST = 1u;
            }
            break;
        case LvdChanel2:
            if(LvdTypeIrq == enType)
            {
                M0P_WSUNIT->LVD2_SET_f.LVD2_SRST = 0u;
            }
            else
            {
                M0P_WSUNIT->LVD2_SET_f.LVD2_SRST = 1u;
            }
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize LVD function.
 **
 ** \param [in]  enChanel    The LVD channel
 **        [in]  pstcConfig  LVD module configuration
 **
 ** \retval en_result_t   Ok:  LVD initilation Done
 **
 ******************************************************************************/
en_result_t Lvd_Init(en_lvd_chanel_t enChanel, stc_lvd_config_t *pstcConfig)
{
    ASSERT(NULL != pstcConfig);
    
    ASSERT(IS_VALID_CHANEL(enChanel));
    ASSERT(IS_VALID_THRESHOLD_RISE(pstcConfig->enThresholdR));
    ASSERT(IS_VALID_THRESHOLD_DOWN(pstcConfig->enThresholdD));
    ASSERT(IS_VALID_IRQTYPE(pstcConfig->enIrqType));

    if(NULL == pstcConfig)
    {
        return ErrorInvalidParameter;
    }
    
    /* NEED to DISABLE first. */
    Lvd_Disable(enChanel);
    Lvd_ClearIrq(enChanel);
    if(LvdChanel1 == enChanel)
    {
        M0P_WSUNIT->LVD1_SET_f.LVD1_SRST = pstcConfig->enIrqType;
        M0P_WSUNIT->LVD1_SET_f.LVD1_SVHD = pstcConfig->enThresholdD;
        M0P_WSUNIT->LVD1_SET_f.LVD1_SVHR = pstcConfig->enThresholdR;
        
        /* enable irq */
        pfnLvdIrqCbk[LvdChanel1] = NULL;
        if (pstcConfig->enIrqType == LvdTypeIrq)
        {
            LvdEnableNvic();
            pfnLvdIrqCbk[LvdChanel1] = pstcConfig->pfnIrqCbk;
        }
    }
    else
    {
        M0P_WSUNIT->LVD2_SET_f.LVD2_SRST = pstcConfig->enIrqType;
        M0P_WSUNIT->LVD2_SET_f.LVD2_SVHD = pstcConfig->enThresholdD;
        M0P_WSUNIT->LVD2_SET_f.LVD2_SVHR = pstcConfig->enThresholdR;
        
        /* enable irq */
        pfnLvdIrqCbk[LvdChanel2] = NULL;
        if (pstcConfig->enIrqType == LvdTypeIrq)
        {
            LvdEnableNvic();
            pfnLvdIrqCbk[LvdChanel2] = pstcConfig->pfnIrqCbk;
        }
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Initialize the LVD module.
 **
 ** \param [in]  enChanel  The LVD channel
 **        [in]  bNvic     TRUE = De-Initialize the NVIC function meanwhile
 **                        FALSE = Don't consider the NVIC function
 **
 ** \retval en_result_t   Ok:  LVD De-init Done
 **
 ******************************************************************************/
en_result_t Lvd_DeInit(en_lvd_chanel_t enChanel, boolean_t bNvic)
{
    Lvd_ClearIrq(enChanel);
    pfnLvdIrqCbk[enChanel] = NULL;
    LvdEnable(enChanel, FALSE);

    if (TRUE == bNvic)
    {
        LvdDisableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief LVD detect enable
 **
 ** \param [in]  enChanel the LVD channel to be enabled
 **
 ** \retval en_result_t   Ok  LVD enable done
 **
 ******************************************************************************/
en_result_t Lvd_Enable(en_lvd_chanel_t enChanel)
{
    return LvdEnable(enChanel, TRUE);
}

/**
 *******************************************************************************
 ** \brief LVD detect disable
 **
 ** \param [in]  enChanel the LVD channel to be disabled
 **
 ** \retval en_result_t   Ok  LVD disable done
 **
 ******************************************************************************/
en_result_t Lvd_Disable(en_lvd_chanel_t enChanel)
{
    return LvdEnable(enChanel, FALSE);
}

/**
 *******************************************************************************
 ** \brief Get LVD interrupt flag
 **
 ** \param [in]  enChanel the LVD channel
 **
 ** \retval boolean_t   the requested type of LVD interrupt flag
 **
 ******************************************************************************/
boolean_t Lvd_GetIrqStat(en_lvd_chanel_t enChanel)
{
    boolean_t bFlag = FALSE;

    ASSERT(IS_VALID_CHANEL(enChanel));

    switch (enChanel)
    {
        case LvdChanel1:
            bFlag = M0P_WSUNIT->LVD_INT_STR_f.LVD1_IF;
            break;
        case LvdChanel2:
            bFlag = M0P_WSUNIT->LVD_INT_STR_f.LVD2_IF;
            break;
        default:
            break;
    }

    return bFlag;
}

/**
 *******************************************************************************
 ** \brief Clear LVD interrupt flag
 **
 ** \param [in]  enChanel the LVD channel
 **
 ** \retval en_result_t   Ok:  Interrupt flag cleared
 **                       ErrorInvalidParameter: Invalid channel
 **
 ******************************************************************************/
en_result_t Lvd_ClearIrq(en_lvd_chanel_t enChanel)
{
    en_result_t enRet = Ok;

    switch (enChanel)
    {
        case LvdChanel1:
            M0P_WSUNIT->LVD_CLR_f.LVD1_CLR = 1u;
            break;
        case LvdChanel2:
            M0P_WSUNIT->LVD_CLR_f.LVD2_CLR = 1u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }
    return enRet;
}

//@} // LvdGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
