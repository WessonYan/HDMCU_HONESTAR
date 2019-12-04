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
/** \file timer4_emi.h
 **
 ** A detailed description is available at
 ** @link Timer4EmiGroup Timer4Emi description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Emi
 **
 ******************************************************************************/

#ifndef __TIMER4_EMI_H__
#define __TIMER4_EMI_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f_m14x.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup Timer4EmiGroup Timer4 motor EMergency stop Input(Timer4Emi)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/*******************************************************************************
 * brief noise canceling width for a digital noise-canceler
 ******************************************************************************/
typedef enum en_emi_filter_width
{
    EmiNoFilter      = 0u,              ///< no noise-canceling
    EmiWidth4Cycle   = 1u,              ///< 4 PCLK cycles
    EmiWidth8Cycle   = 2u,              ///< 8 PCLK cycles
    EmiWidth16Cycle  = 3u,              ///< 16 PCLK cycles
    EmiWidth32Cycle  = 4u,              ///< 32 PCLK cycles
    EmiWidth64Cycle  = 5u,              ///< 64 PCLK cycles
    EmiWidth128Cycle = 6u,              ///< 128 PCLK cycles
    EmiWidth256Cycle = 7u,              ///< 256 PCLK cycles
}en_emi_filter_width_t;

/**
 *******************************************************************************
 ** \brief EMI interrupt callback function
 ******************************************************************************/
typedef struct stc_emi_irq_cb
{
    func_ptr_t pfnDigitalPathIrqCb;     ///< digital filter interrupt callback function
    func_ptr_t pfnAnalogPathIrqCb;      ///< analog filter interrupt callback function
} stc_emi_irq_cb_t;

/*******************************************************************************
 * brief EMI configure
 ******************************************************************************/
typedef struct stc_timer4_emi_config
{
    boolean_t             bDigitalPathEnable;       ///< Enable digital path
    en_emi_filter_width_t enDigitalFilterWidth;     ///< Digital filter width
    boolean_t             bAnalogPathEnable;        ///< Enable analog path
    boolean_t             bHoldPwm;                 ///< TRUE: hold PWM when EMIF interrupt occurs, FALSE: don't hold PWM when EMIF interrupt occurs
    boolean_t             bSwitchToGpio;            ///< Switch to GPIO

    boolean_t             bDigitalPathIrqMskEnable; ///< Digital path interrupt mask
    boolean_t             bAnalogPathIrqMskEnable;  ///< Analog path interrupt mask
    stc_emi_irq_cb_t      stcIrqCb;                 ///< Emi irq callback function

    boolean_t             bTouchNvic;               ///< TRUE: enable NVIC, FALSE: disable NVIC
}stc_timer4_emi_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/


/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/


/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* 1. Timer4-EMI configuration */
extern en_result_t Timer4_Emi_Init(stc_timer4_emi_config_t* pstcEmiConfig);
extern en_result_t Timer4_Emi_DeInit(boolean_t bTouchNvic);

/* 2. Timer4-EMI trigger */
extern en_result_t Timer4_Emi_SwTiggerEmi(void);

/* 2. Timer4-EMI status get/clear  */
extern en_irq_flag_t Timer4_Emi_GetDigitalPathIrqFlag(void);
extern en_result_t Timer4_Emi_ClrDigitalPathIrqFlag(void);

extern en_irq_flag_t Timer4_Emi_GetAnalogPathIrqFlag(void);
extern en_result_t Timer4_Emi_ClrAnalogPathIrqFlag(void);

//@} // Timer4EmiGroup

#ifdef __cplusplus
}
#endif

#endif /* __TIMER4_EMI_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
