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
/** \file hwwdt.h
 **
 ** A detailed description is available at
 ** @link HwwdtGroup Hwwdt description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Hwwdt
 **
 ******************************************************************************/

#ifndef __HWWDT_H__
#define __HWWDT_H__

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
 ** \defgroup HwwdtGroup HardWare WatchDog Timer(Hwwdt)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Hardware Watchdog Timer configuration
 ******************************************************************************/
typedef struct stc_hwwdt_config
{
    uint32_t   u32LoadValue;            ///< Timer interval
    boolean_t  bResetEnable;            ///< TRUE:  Enables Hardware Watchdog Timer reset
    func_ptr_t pfnHwwdtIrqCb;           ///< Pointer to Hardware Watchdog Timer interrupt callback function
} stc_hwwdt_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/


/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* Setup(Initialize)/Disable(Un-initialize) */
extern en_result_t Hwwdt_Init(stc_hwwdt_config_t* pstcConfig);
extern en_result_t Hwwdt_DeInit(uint32_t u32MagicWord1, uint32_t u32MagicWord2 );

/* Start/Stop */
extern en_result_t Hwwdt_Start(void);
extern void Hwwdt_Stop(void);

/* Write/Read for counter */
extern void Hwwdt_WritewdtLoad(uint32_t u32LoadValue);
extern uint32_t Hwwdt_ReadwdtValue(void);

/* Feed watchdog */
extern void Hwwdt_Feed(uint8_t u8ClearPattern1, uint8_t u8ClearPattern2);

extern void Hwwdt_EnableDbgBrkWdtCtl(void);
extern void Hwwdt_DisableDbgBrkWdtCtl(void);

//@} // HwwdtGroup

#ifdef __cplusplus
}
#endif

#endif /* __HWWDT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
