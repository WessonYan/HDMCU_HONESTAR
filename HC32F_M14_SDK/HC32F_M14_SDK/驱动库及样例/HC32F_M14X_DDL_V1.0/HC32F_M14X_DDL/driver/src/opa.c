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
/** \file opa.c
 **
 ** A detailed description is available at
 ** @link OpaGroup Opa description @endlink
 **
 **   - 2017-08-07  1.0  HeChun First version for Device Driver Library of opa
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "opa.h"

/**
 *******************************************************************************
 ** \addtogroup OpaGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_OPA(x)                 \
(   (Opa0 == (x)) ||                    \
    (Opa1 == (x)))

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
 ** \brief Enable OPA
 **
 ** \param [in]  ch  if ch is Opa0_EN enable OPA0
 **                  if ch is Opa1_EN enable OPA1
 **
 ** \retval OK set successfully
 **
 ******************************************************************************/
en_result_t Opa_Enable(en_opa_ch_t ch)
{
    en_result_t ret = Ok;

    ASSERT(IS_VALID_OPA(ch));

    if(Opa0 == ch)
    {
        M0P_OPA0->OPA_CTL_f.OPA_EN = TRUE;
    }
    else if(Opa1 == ch)
    {
        M0P_OPA1->OPA_CTL_f.OPA_EN = TRUE;
    }
    else
    {
      
    }
    return ret;
}

/**
 *******************************************************************************
 ** \brief Disable OPA
 **
 ** \param [in]  ch  if ch is Opa0_EN enable OPA0
 **                  if ch is Opa1_EN enable OPA1
 **
 ** \retval OK Opa had be disabled
 **
 ******************************************************************************/
en_result_t Opa_Disable(en_opa_ch_t ch)
{
    en_result_t ret = Ok;

    ASSERT(IS_VALID_OPA(ch));

    if(Opa0 == ch)
    {
        M0P_OPA0->OPA_CTL_f.OPA_EN = FALSE;
    }
    else if(Opa1 == ch)
    {
        M0P_OPA1->OPA_CTL_f.OPA_EN = FALSE;
    }
    else
    {
      
    }
    return ret;
}

//@} // OpaGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
