/******************************************************************************
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
/** \file cc.h
 **
 ** A detailed description is available at
 ** @link CcGroup Cc description @endlink
 **
 **   - 2017-08-10  1.0  HeChun First version for Device Driver Library of cc.
 **
 ******************************************************************************/
#ifndef __CC_H__
#define __CC_H__

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
 ** \defgroup CcGroup Clock counter(Cc)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Cc clock prescaler coefficient
 ** 
 ** \note Select clock prescaler
 **
 ******************************************************************************/
typedef enum en_cc_prescaler_modulus
{
    Modulus0 = 0,
    Modulus1,
    Modulus2,
    Modulus3,
    Modulus4,
    Modulus5,
    Modulus6,
} en_cc_prescaler_modulus_t; 

/**
 *******************************************************************************
 ** \brief Cc counter clock selection
 ** 
 ** \note Select counter clock
 **
 ******************************************************************************/
typedef enum en_cc_ck_sel
{
    Ck0 = 0,
    Ck1,
    Ck2,
    Ck3,
} en_cc_ck_sel_t; 

/**
 *******************************************************************************
 ** \brief Cc counter clock soruce
 ** 
 ** \note Select counter clock soruce 
 **       if want to use Related clock you must enable corresponding clock
 **
 ******************************************************************************/
typedef enum cc_clk_src
{
    LSXT = 0,
    HSXT,
    HICR,
    LICR,
} cc_clk_src_t;

/**
 *******************************************************************************
 ** \brief Cc configuration structure
 ** 
 ** \note set your cc parameter
 **
 ******************************************************************************/
typedef struct stc_cc_config
{
    cc_clk_src_t               enSource;       ///< choose prescaler clock Source
    boolean_t                  enRCK;          ///< enable clock prescaler
    en_cc_prescaler_modulus_t  modulus;        ///< choose clock prescaler coefficient
    en_cc_ck_sel_t             CK;             ///< set counter clock selection
    boolean_t                  bIrqEnable;     ///< TRUE: enable interrupt, FALSE: disable interrupt
    func_ptr_t                 pfnIrqCallback; ///< Pointer to interrupt callback function
    boolean_t                  bTouchNvic;     ///< TRUE: enable MVIC, FALSE: don't enable NVIC
} stc_cc_config_t;


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/

en_result_t Cc_Init(stc_cc_config_t *pstcConfig);
en_result_t Cc_DeInit(boolean_t bTouchNvic);

en_result_t Cc_EnableCount(void);
en_result_t Cc_DisableCount(void);

en_result_t Cc_ClrIrqFlag(void);
boolean_t Cc_GetIrqFlag(void);

en_result_t Cc_WriteLoadVal(uint32_t u32Val);
uint32_t Cc_ReadCurCntVal(void);
//@} // CcGroup

#ifdef __cplusplus
}
#endif

#endif /* __CC_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
