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
/** \file exint.h
 **
 ** A detailed description is available at
 ** @link ExintGroup exint description @endlink
 ** 
 **   - 2017-08-10  1.0  WangM First version for Device Driver Library of exint
 **
 ******************************************************************************/

#ifndef __EXINT_H__
#define __EXINT_H__

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
 ** \defgroup ExintGroup External Interrupts (EXINT)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef enum en_exint_level
{
    ExIntLowLevel     = 0u,             ///< "L" level detection
    ExIntHighLevel    = 1u,             ///< "H" level detection
    ExIntRisingEdge   = 2u,             ///< Rising edge detection
    ExIntFallingEdge  = 3u,             ///< Falling edge detection
}en_exint_level_t;

/**
 *******************************************************************************
 ** \brief External Interrupt configuration
 ** 
 ** \note The EXINT configuration
 ******************************************************************************/
typedef struct stc_exint_config
{
    boolean_t         bEnable;          ///< TRUE: External Interrupt enable
    en_exint_level_t  enLevel;          ///< level detection, see #en_exint_level_t for details
    func_ptr_t        pfnExintCallback; ///< Callback pointers  
    boolean_t         bTouchNvic;       ///< TRUE: Enable NVIC, FALSE: don't enable NVIC, 
}stc_exint_config_t;

/**
 *******************************************************************************
 ** \brief NMI configuration
 ** 
 ** \note The NMI configuration
 ******************************************************************************/
typedef struct stc_exint_nmi_config
{ 
    func_ptr_t        pfnNmiCallback;   ///< NMI Callback pointers
}stc_exint_nmi_config_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define an index for EXINT
 ** 
 ** \note
 ******************************************************************************/
typedef enum en_exint_channel_index
{
    ExintInstanceIndexExint0 = 0,
    ExintInstanceIndexExint1,
    ExintInstanceIndexExint2,
    ExintInstanceIndexExint3,
    ExintInstanceIndexExint4,
    ExintInstanceIndexExint5,
    ExintInstanceIndexExint6,
    ExintInstanceIndexExint7,
    ExintInstanceIndexExint8,
    ExintInstanceIndexExint9,
    ExintInstanceIndexExint10,
    ExintInstanceIndexExint11,
    ExintInstanceIndexExint12,
    ExintInstanceIndexExint13,
    ExintInstanceIndexExint14,
    ExintInstanceIndexExint15,
    ExintInstanceIndexMax
}en_exint_channel_index_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DDL_EXINT_CHANNELS       (16u)  

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
 
/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
extern en_result_t Exint_Init(en_exint_channel_index_t enChannel,stc_exint_config_t* pstcConfig);
en_result_t Exint_DeInit(en_exint_channel_index_t enChannel, boolean_t bTouchNvic);

extern en_result_t ExintEnableNvic(en_exint_channel_index_t enChannel);
extern en_result_t ExintDisableNvic(en_exint_channel_index_t enChannel);

extern en_result_t Exint_EnableChannel(en_exint_channel_index_t enChannel);
extern en_result_t Exint_DisableChannel(en_exint_channel_index_t enChannel);

extern en_result_t Exint_SetDetectMode(en_exint_channel_index_t enChannel, en_exint_level_t enLevel);
extern en_exint_level_t Exint_GetDetectMode(en_exint_channel_index_t enChannel);

extern en_result_t Exint_Nmi_Init(stc_exint_nmi_config_t* pstcConfig);
extern en_result_t Exint_Nmi_DeInit(void);

extern en_result_t Exint_Nmi_IrqFlagClear(void);

//@} // ExintGroup

#ifdef __cplusplus
}
#endif

#endif /* __EXINT_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
