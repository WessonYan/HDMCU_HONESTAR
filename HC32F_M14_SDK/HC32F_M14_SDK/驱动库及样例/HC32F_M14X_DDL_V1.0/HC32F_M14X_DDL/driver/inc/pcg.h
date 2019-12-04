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
/** \file pcg.h
 **
 ** A detailed description is available at
 ** @link PcgGroup PCG description @endlink
 **
 **   - 2018-08-09  1.0 chengy First version for Device Driver Library of PCG.
 **
 ******************************************************************************/

#ifndef __PCG_H__
#define __PCG_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup PcgGroup Peripheral Clock Gate(PCG)
 **
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief peripheral selected
 **
 ** \note To determine which peripheral will be selected
 **
 ******************************************************************************/
typedef enum
{
    MSC4          = 0u,       ///< Multi-functional Serial Channel 4.
    MSC5          = 1u,       ///< Multi-functional Serial Channel 5.
    MSC6          = 2u,       ///< Multi-functional Serial Channel 6.
    GPIO          = 3u,       ///< General Purpose Input/Output.
    DMA           = 4u,       ///< Direct Memory Access.
    ADC           = 5u,       ///< Analog to Digital Converter.
    OPA_0         = 6u,       ///< Operational Amplifier Channel 0.
    OPA_1         = 7u,       ///< Operational Amplifier Channel 1.
    TIM4          = 8u,       ///< Timer4.
    VC            = 9u,       ///< Voltage Comparator.
    CT            = 10u,      ///< Composite Timer(channels 0-3).
    LCDC          = 11u,      ///< LCD Controller.
    DT            = 12u,      ///< Dual timer.
    HWWDT         = 13u,      ///< Hardware watchdog timer.
    SWWDT         = 14u,      ///< Software watchdog timer.
} en_peripheral_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/


/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/


/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* Function CLK Stop/Recovery */
extern en_result_t Pcg_Stop(en_peripheral_t enPeri);
extern en_result_t Pcg_Recovery(en_peripheral_t enPeri);

/* Function  CLK Enable/Disable */
extern en_result_t Pcg_EnableCLK(en_peripheral_t enPeri);
extern en_result_t Pcg_DisableCLK(en_peripheral_t enPeri);

/* Function  RST Enable/Disable */
extern en_result_t Pcg_EnableRST(en_peripheral_t enPeri);
extern en_result_t Pcg_DisableRST(en_peripheral_t enPeri);

//@} // PcgGroup

#ifdef __cplusplus
}
#endif

#endif /* __PCG_H_ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
