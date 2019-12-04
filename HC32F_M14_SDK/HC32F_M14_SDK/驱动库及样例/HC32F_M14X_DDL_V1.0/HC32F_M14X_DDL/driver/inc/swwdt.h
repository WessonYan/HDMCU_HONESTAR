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
/** \file swwdt.h
 **
 ** A detailed description is available at
 ** @link SwwdtGroup Swwdt description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Swwdt
 **
 ******************************************************************************/

#ifndef __SWWDT_H__
#define __SWWDT_H__

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
 ** \defgroup SwwdtGroup SoftWare WatchDog Timer(Swwdt)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
******************************************************************************/
/**
 *******************************************************************************
 ** \brief Software watchdog timing window settings
 ******************************************************************************/
typedef enum en_swwdt_timing_window
{
    en_swwdt_timing_window_100 = 0x00,  ///< Reload can be executed at less than or equal to WDOGLOAD.
    en_swwdt_timing_window_75  = 0x01,  ///< Reload can be executed at less than or equal to 75% of WDOGLOAD.
    en_swwdt_timing_window_50  = 0x02,  ///< Reload can be executed at less than or equal to 50% of WDOGLOAD
    en_swwdt_timing_window_25  = 0x03   ///< Reload can be executed at less than or equal to 25% of WDOGLOAD
} en_swwdt_timing_window_t;

/**
 *******************************************************************************
 ** \brief Software and Hardware Watchdog configuration
 ******************************************************************************/
typedef struct stc_swwdt_config
{
    uint32_t   u32LoadValue;                    ///< Timer interval
    boolean_t  bResetEnable;                    ///< TRUE:  Enables SW watchdog reset
    boolean_t  bWinWdtEnable;                   ///< TRUE:  Enables Window watchdog mode
    boolean_t  bWinWdtResetEnable;              ///< TRUE:  Enables reset when reload without timing window was occured.
                                                ///< FALSE: Enables interrupt when reload without timing window was occured.
    en_swwdt_timing_window_t enTimingWindow;    ///< Timing window setting, see description of #en_swwdt_timing_window_t
    func_ptr_t pfnSwwdtIrqCb;                   ///< Pointer to interrupt callback funciton
} stc_swwdt_config_t;

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
extern en_result_t Swwdt_Init(stc_swwdt_config_t* pstcConfig);
extern void Swwdt_DeInit(boolean_t bTouchNvic);

/* Start/Stop */
extern en_result_t Swwdt_Start(void);
extern void Swwdt_Stop(void);

/* Write/Read for counter */
extern void Swwdt_WritewdtLoad(uint32_t u32LoadValue);
extern uint32_t Swwdt_ReadWdtValue(void);

/* Feed watchdog */
extern void Swwdt_Feed(void) ;

/* Setting Debug Break Watchdog Timer Control */
extern void Swwdt_EnableDbgBrkWdtCtl(void);
extern void Swwdt_DisableDbgBrkWdtCtl(void);

//@} // SwwdtGroup

#ifdef __cplusplus
}
#endif

#endif /* __SWWDT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
