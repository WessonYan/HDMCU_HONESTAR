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
/** \file vc.c
 **
 ** A detailed description is available at
 ** @link VcGroup Vc description @endlink
 **
 **   - 2017-08-10  1.0  WangM First version for Device Driver Library of vc.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "vc.h"

/**
 *******************************************************************************
 ** \addtogroup VcGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_STAT(x)                \
(   VcCmpResult==(x) ||                 \
    VcCmpIrqResult == (x))
#define IS_VALID_FILTERTIME(x)   ((x) <= VcFilter256cycle )
#define IS_VALID_CompSelN(x)     ((x) <= AvccDiv2 )
#define IS_VALID_INPUT(x)        ((x) <= VcAvcc )
#define IS_VALID_EDGE(x)         ((x) <= VcIrqCmpIrqHigh )
#define IS_VALID_VCFUNCTION(x)   ((x) <= VcFuncFilter )

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static en_result_t VcEnableFunc(en_vc_func_sel_t enFunc, boolean_t bFlag);
static void VcEnableNvic(void);
static void VcDisableNvic(void);
static en_result_t Vc_ConfigIrqEdge(en_vc_irq_sel_t enSel, boolean_t bFlag );
static en_result_t VcEnableIrq(boolean_t bFlag);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static func_ptr_t m_pfnAnalogCmpCb = NULL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief VC funciton enable or disable
 **
 ** \param [in]  enFunc  Include VcFuncACvt or VcFuncFilter
 ** \param [in]  bFlag   TRUE = enable  FALSE = disable
 **
 ** \retval Ok                    = Function set done
 **         ErrorInvalidParameter = parameter error
 **
 ******************************************************************************/
static en_result_t VcEnableFunc(en_vc_func_sel_t enFunc, boolean_t bFlag)
{
    en_result_t enRet = Ok;
    ASSERT(IS_VALID_VCFUNCTION(enFunc));

    switch (enFunc)
    {
        case VcFuncACvt:
            M0P_VC->VC_CTL1_f.VC_EN = bFlag;
            break;
        case VcFuncFilter:
            M0P_VC->VC_CTL2_f.VC_FILTER_EN = bFlag;
            break;
        default:
            enRet= ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief VC funciton interrupt edge config
 **
 ** \param [in]  enSel   interrupt edge
 ** \param [in]  bFlag   TRUE = enable irq  FALSE = disable irq
 **
 ** \retval Ok                    = set done
 **         ErrorInvalidParameter = parameter error
 **
 ******************************************************************************/
static en_result_t Vc_ConfigIrqEdge(en_vc_irq_sel_t enSel, boolean_t bFlag )
{
    en_result_t enRet = Ok;
    ASSERT(IS_VALID_EDGE(enSel));

    switch (enSel)
    {
        case VcIrqCmpIrqRise:
            M0P_VC->VC_CTL2_f.VCINT_EDGE = (uint16_t)bFlag << 1u;
            if(TRUE == bFlag)
            {
                M0P_VC->VC_CTL2_f.VCINT_HIGH = FALSE;
            }
            else
            {
            }
            break;
        case VcIrqCmpIrqFall:
            M0P_VC->VC_CTL2_f.VCINT_EDGE = bFlag;
            if(TRUE == bFlag)
            {
                M0P_VC->VC_CTL2_f.VCINT_HIGH = FALSE;
            }
            else
            {

            }
            break;
        case VcIrqCmpIrqEdge:
            M0P_VC->VC_CTL2_f.VCINT_EDGE = (((uint16_t)bFlag << 1u) + bFlag);
            if(TRUE == bFlag)
            {
                M0P_VC->VC_CTL2_f.VCINT_HIGH = FALSE;
            }
            else
            {
            }
            break;
        case VcIrqCmpIrqHigh:
            M0P_VC->VC_CTL2_f.VCINT_HIGH = bFlag;
            break;

        default:
            enRet= ErrorInvalidParameter;
            break;
    }

    return enRet;
}


/**
 *******************************************************************************
 ** \brief VC interrupt enbable
 **
 ** \param [in]  bFlag   TRUE = enable irq  FALSE = disable irq
 **
 ** \retval Ok   = set done
 **
 ******************************************************************************/
static en_result_t VcEnableIrq(boolean_t bFlag)
{
    M0P_VC->VC_CTL2_f.VCIE = bFlag;
    if (TRUE == bFlag)
    {
        VcEnableNvic();
    }
    else
    {
        VcDisableNvic();
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief VC NVIC enable
 **
 ** \param [in]  None
 **
 ** \retval Ok  = set done
 **
 ******************************************************************************/
static void VcEnableNvic(void)
{
    NVIC_ClearPendingIRQ(VC_IRQn);
    NVIC_EnableIRQ(VC_IRQn);
    NVIC_SetPriority(VC_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief VC NVIC disable
 **
 ** \param [in]  None
 **
 ** \retval Ok  = set done
 **
 ******************************************************************************/
static void VcDisableNvic(void)
{
    NVIC_ClearPendingIRQ(VC_IRQn);
    NVIC_DisableIRQ(VC_IRQn);
    NVIC_SetPriority(VC_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief ISR for VC function
 **
 ** \param [in]  u8Param reseve
 **
 ** \retval None
 **
 ******************************************************************************/
void Vc_IrqHandler(uint8_t u8Param)
{
    stc_vc_vc_ctl2_field_t stcCtl2;

    DDL_ZERO_STRUCT(stcCtl2);

    stcCtl2 = M0P_VC->VC_CTL2_f;

    if (TRUE == stcCtl2.VCIF)
    {
        if (NULL != m_pfnAnalogCmpCb)
        {
            m_pfnAnalogCmpCb();
        }
        M0P_VC->VC_CTL2_f.VCIF = 0;
    }
}

/**
 *******************************************************************************
 ** \brief VC function status return
 **
 ** \param [in]  enStat Include VC_OUT flag and VC irq flag
 **
 ** \retval TRUE = flag is set   FALSE = flag is not set
 **
 ******************************************************************************/
boolean_t Vc_GetStatus(en_vc_stat_t enStat)
{
    boolean_t bFlag = FALSE;

    ASSERT( IS_VALID_STAT(enStat));

    switch (enStat)
    {
        case VcCmpResult:
            bFlag = M0P_VC->VC_CTL1_f.VC_OUT;
            break;
        case VcCmpIrqResult:
            bFlag = M0P_VC->VC_CTL2_f.VCIF;
            break;
        default:
            break;
    }

    return bFlag;
}

/**
 *******************************************************************************
 ** \brief VC status clear
 **
 ** \param [in]  enStat Include VC irq flag
 **
 ** \retval Ok                    = set done
 **         ErrorInvalidParameter = parameter error
 **
 ******************************************************************************/
en_result_t Vc_ClearStat(en_vc_stat_t enStat)
{
    en_result_t enRet = Ok;
    ASSERT(IS_VALID_STAT(enStat));

    switch (enStat)
    {
        case VcCmpIrqResult:
            M0P_VC->VC_CTL2_f.VCIF = FALSE;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief VC interrupt enable
 **
 ** \param [in]  None
 **
 ** \retval Ok   = set done
 **
 ******************************************************************************/
en_result_t Vc_EnableIrq(void)
{
    return VcEnableIrq(TRUE);
}

/**
 *******************************************************************************
 ** \brief VC interrupt disable
 **
 ** \param [in]  None
 **
 ** \retval Ok   = set done
 **
 ******************************************************************************/
en_result_t Vc_DisableIrq(void)
{
    return VcEnableIrq(FALSE);
}

/**
 *******************************************************************************
 ** \brief Initialize VC function
 **
 ** This function initializes the VC module according the given configuration.
 **
 ** \param [in]  pstcConfig  VC configuration parameters
 **
 ** \retval  Ok                       VC initialized
 **          ErrorInvalidParameter    pstcConfig == NULL or Illegal mode
 **
 ******************************************************************************/
en_result_t Vc_Init(stc_vc_config_t *pstcConfig)
{
    en_result_t enRet = Ok;

    ASSERT(NULL != pstcConfig);
    ASSERT(IS_VALID_FILTERTIME(pstcConfig->enVcFilterTime));
    ASSERT(IS_VALID_CompSelN(pstcConfig->enCompSelN));
    ASSERT(IS_VALID_INPUT(pstcConfig->enVcAnalogInputN));

    switch (pstcConfig->enVcAnalogInputN)
    {
        case VcExtDirect:
            /* Set VC_INM as reference voltage */
            M0P_VC->VC_CTL1_f.VC_NMODE = 0u;
            break;
        case VcAvcc:
            /* Set Voltage from AVCC as reference voltage */
            M0P_VC->VC_CTL1_f.VC_NMODE = 1u;
            /* Set the prescaler of AVCC */
            M0P_VC->VC_CTL1_f.VC_COMPSL = pstcConfig->enCompSelN;
            break;
        default:
            break;
    }

    /* Hysteresis function set */
    M0P_VC->VC_CTL2_f.VC_HYS_SEL = pstcConfig->bVcCmpDly;

    /* Set interrupt edge for VC function */
    Vc_ConfigIrqEdge(pstcConfig->enIrqEdgeSel, TRUE);

    /* if compare result reverse */
    M0P_VC->VC_CTL2_f.VC2PT = pstcConfig->bACmpOutputReverse;

    /* Filter time configurate */
    M0P_VC->VC_CTL2_f.VC_RESPONSE = pstcConfig->enVcFilterTime;
    M0P_VC->VC_CTL2_f.VC_FILTER_EN = pstcConfig->bVcFilterEn;

    /* Set if trigger the CT0 or CT2 by the output of the VC function */
    M0P_VC->VC_CTL3_f.VC_TM0G = pstcConfig->bTimer0Rev;
    M0P_VC->VC_CTL3_f.VC_TM2G = pstcConfig->bTimer2Rev;

    /* Set the callback function for interrupt */
    m_pfnAnalogCmpCb = pstcConfig->pfnAnalogCmpCb;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Initialate VC module
 **
 ** \param [in]  None
 **
 ** \retval Ok   = configurate done
 **
 ******************************************************************************/
en_result_t Vc_DeInit(void)
{
    Vc_DisableFunc(VcFuncACvt);
    Vc_DisableIrq();

    m_pfnAnalogCmpCb = NULL;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable VC function
 **
 ** \param [in]  Include VcFuncACvt or VcFuncFilter
 **
 ** \retval Ok                    = Function set done
 **         ErrorInvalidParameter = parameter error
 **
 ******************************************************************************/
en_result_t Vc_EnableFunc(en_vc_func_sel_t enFunc)
{
    return VcEnableFunc(enFunc, TRUE);
}

/**
 *******************************************************************************
 ** \brief Disable VC function
 **
 ** \param [in]  Include VcFuncACvt or VcFuncFilter
 **
 ** \retval Ok                    = Function set done
 **         ErrorInvalidParameter = parameter error
 **
 ******************************************************************************/
en_result_t Vc_DisableFunc(en_vc_func_sel_t enFunc)
{
    return VcEnableFunc(enFunc, FALSE);
}

//@} // VcGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
