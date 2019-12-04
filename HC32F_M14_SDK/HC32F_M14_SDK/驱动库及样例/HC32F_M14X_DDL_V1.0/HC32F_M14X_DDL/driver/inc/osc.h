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
/** \file osc.h
 **
 ** A detailed description is available at
 ** @link OscGroup Osc description @endlink
 **
 **   - 2017-6-10  1.0  Zhangxl First version for Device Driver Library of Osc.
 **
 ******************************************************************************/

#ifndef __OSC_H_
#define __OSC_H_

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
 ** \defgroup OscGroup Oscillator (OSC)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief HICR frequency division values
 **
 ** \note Set HICR frequency division
 ******************************************************************************/
typedef enum en_hicr_freq_div
{
    HicrFreqDivBy4 = 0,                 ///< Hicr frequency divided by 4.
    HicrFreqDivBy8 = 1,                 ///< Hicr frequency divided by 8.
    HicrFreqDivBy16 = 2,                ///< Hicr frequency divided by 16.
    HicrFreqDivBy32 = 3,                ///< Hicr frequency divided by 32.
    HicrFreqDivBy64 = 4,                ///< Hicr frequency divided by 64.
    HicrFreqDivBy128 = 5,               ///< Hicr frequency divided by 128.
    HicrFreqDivBy256 = 6,               ///< Hicr frequency divided by 256.
    HicrFreqDivBy512 = 7                ///< Hicr frequency divided by 512.
} en_hicr_freq_div_t;

/**
 *******************************************************************************
 ** \brief External OSC input frequency range
 **
 ** \note Selcet the range of external OSC frequency
 ******************************************************************************/
typedef enum en_hxst_range
{
    R4M6M = 0,                          ///< External OSC freq. is 4MHz ~ 6MHz
    R6M12M = 1,                         ///< External OSC freq. is 6MHz ~ 12MHz
    R12M16M = 2                         ///< External OSC freq. is 12MHz ~ 16MHz
} en_hxst_range_t;

/**
 *******************************************************************************
 ** \brief Main & Sub External OSC parameter settings
 **
 ** \note Set the bias of main & sub external osc, input range select of main
 **       osc
 ******************************************************************************/
typedef struct stc_osc_config
{
    uint8_t u8HxstBias;                 ///< External High-Speed Crystal bias current setting.
    en_hxst_range_t enHxstRange;        ///< Input range of external OSC.
    uint8_t u8LxstBias;                 ///< External Low-Speed Crystal bias current setting.
} stc_osc_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
en_result_t Hicr_SetFreqDiv(en_hicr_freq_div_t enHicrDiv);
en_hicr_freq_div_t Hicr_GetFreqDiv(void);
void Hicr_Set4MFreqTrimmingData(uint8_t u8FTrim);
uint16_t Hicr_Get4MFreqTrimmingData(void);
void Hicr_Set4MTempTrimmingData(uint8_t u8TTrim);
uint8_t Hicr_Get4MTempTrimmingData(void);
en_result_t Osc_Init(stc_osc_config_t* pstcConfig);
en_result_t Osc_DeInit(void);

//@} // OscGroup

#ifdef __cplusplus
}
#endif

#endif /*__OSC_H_*/

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
