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
/** \file timer4_cnt.h
 **
 ** A detailed description is available at
 ** @link Timer4CntGroup Timer4Cnt description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Cnt
 **
 ******************************************************************************/

#ifndef __TIMER4_CNT_H__
#define __TIMER4_CNT_H__

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
 ** \defgroup Timer4CntGroup Timer4 Counter(Timer4Cnt)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief CNT soucre clock
 ******************************************************************************/
typedef enum en_cnt_clock
{
    CntClockPclk = 0u,                  ///<  Uses the internal clock (PCLK) as CNT's count clock.
    CntClockExck = 1u,                  ///<  Uses an external input clock (EXCK) as CNT's count clock.
}en_cnt_clock_t;

/**
 *******************************************************************************
 ** \brief CNT Clock Setting
 ******************************************************************************/
typedef enum en_cnt_clock_div
{
    CntPclkDiv1   = 0u,                 ///< Cnt clock: PCLK
    CntPclkDiv2   = 1u,                 ///< Cnt clock: PCLK/2
    CntPclkDiv4   = 2u,                 ///< Cnt clock: PCLK/4
    CntPclkDiv8   = 3u,                 ///< Cnt clock: PCLK/8
    CntPclkDiv16  = 4u,                 ///< Cnt clock: PCLK/16
    CntPclkDiv32  = 5u,                 ///< Cnt clock: PCLK/32
    CntPclkDiv64  = 6u,                 ///< Cnt clock: PCLK/64
    CntPclkDiv128 = 7u,                 ///< Cnt clock: PCLK/128
    CntPclkDiv256 = 8u,                 ///< Cnt clock: PCLK/256
    CntPclkDiv512 = 9u,                 ///< Cnt clock: PCLK/512
    CntPclkDiv1024 = 10u,               ///< Cnt clock: PCLK/1024
} en_cnt_clock_div_t;

/**
 *******************************************************************************
 ** \brief CNT count mode
 ******************************************************************************/
typedef enum en_cnt_mode
{
    UpCnt      = 0u,                    ///< up-count mode
    UpDownCnt  = 1u,                    ///< up-/down-count mode
} en_cnt_mode_t;

/**
 *******************************************************************************
 ** \brief Enumeration of CNT interrupt selection
 ******************************************************************************/
typedef struct stc_cnt_irq_en
{
    boolean_t bCntZeroMatchIrq;         ///<  zero match interrupt enable
    boolean_t bCntPeakMatchIrq;         ///<  peak match interrupt enable
} stc_cnt_irq_en_t;

/**
 *******************************************************************************
 ** \brief Enumeration of CNT interrupt index
 ******************************************************************************/
typedef enum en_cnt_irq
{
    CntZeroMatchIrq = 0u,               ///<  zero match interrupt index
    CntPeakMatchIrq = 1u,               ///<  peak match interrupt index
} en_cnt_irq_t;

/**
 *******************************************************************************
 ** \brief CNT interrupt callback function
 ******************************************************************************/
typedef struct stc_cnt_irq_cb
{
    func_ptr_t  pfnCntZeroIrqCb;        ///< match zero interrupt callback function
    func_ptr_t  pfnCntPeakIrqCb;        ///< match peak interrupt callback function
} stc_cnt_irq_cb_t;

/**
 *******************************************************************************
 ** \brief CNT start mode
 ******************************************************************************/
typedef enum en_timer4_cnt_start_mode
{
    CntStartFromZeroValue = 0u,         ///<  CNT starts counting from the CISR value.
    CntStartFromCurValue  = 1u,         ///<  CISR value is cleared to start counting from CISR =0x0000.
}en_cnt_start_mode_t;

/**
 *******************************************************************************
 ** \brief CNT stop mode
 ******************************************************************************/
typedef enum en_timer4_cnt_stop_mode
{
    CntStopClearCISR = 0u,              ///<  CNT starts counting from the CISR value.
    CntStopKeepCISR  = 1u,              ///<  CISR value is cleared to start counting from CISR =0x0000.
}en_cnt_stop_mode_t;

/**
 *******************************************************************************
 ** \brief CNT configure
 ******************************************************************************/
typedef struct stc_timer4_cnt_config
{
    en_cnt_mode_t      enCntMode;       ///< CNT count mode
    en_cnt_clock_t     enCntClock;      ///< Select count clock
    en_cnt_clock_div_t enCntClockDiv;   ///< CNT clock divide

    uint8_t            u8ZeroIntMsk;    ///< CNT zero interrupt mask times
    uint8_t            u8PeakIntMsk;    ///< CNT peak interrupt mask times
    boolean_t          bEnBuffer;       ///< enable buffer
    stc_cnt_irq_en_t*  pstcIrqEn;       ///< Pointer to CNT interrupt enable structure
    stc_cnt_irq_cb_t*  pstcIrqCb;       ///< Pointer to CNT interrupt callback functions structure
    boolean_t          bTouchNvic;      ///< TRUE: Enable NVIC, FALSE: don't enable NVIC
}stc_timer4_cnt_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/


/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* 1. Timer4-CNT configuration */
extern en_result_t Timer4_Cnt_Init(stc_timer4_cnt_config_t *pstcCntConfig);
extern void Timer4_Cnt_DeInit(boolean_t bTouchNvic);

/* 2 Clock set */
extern void Timer4_Cnt_SetClock(en_cnt_clock_t enCntClock);
extern void Timer4_Cnt_SetClockDiv(en_cnt_clock_div_t enClockDiv);

/* 3 Mask zero/peak write/read */
extern void Timer4_Cnt_SetMaskZeroTimes(uint8_t u8MaskTimes);
extern uint8_t Timer4_Cnt_GetCurMaskZeroTimes(void);
extern void Timer4_Cnt_SetMaskPeakTimes(uint8_t u8MaskTimes);
extern uint8_t Timer4_Cnt_GetCurMaskPeakTimes(void);

/* 4 Func/Int enable/disable */
extern void Timer4_Cnt_Start(en_cnt_start_mode_t enStartMode);
extern void Timer4_Cnt_Stop(en_cnt_stop_mode_t enStopMode);
extern void Timer4_Cnt_EnableIrq(       en_cnt_irq_t enIrqType);
extern void Timer4_Cnt_DisableIrq(       en_cnt_irq_t enIrqType);

/* 5 Status get/clear */
extern en_irq_flag_t Timer4_Cnt_GetIrqFlag(       en_cnt_irq_t enIrqType);
extern void Timer4_Cnt_ClrIrqFlag(      en_cnt_irq_t enIrqType);

/* 6 Count write/read */
extern void Timer4_Cnt_SetCycleVal(uint16_t u16Cycle);
extern uint16_t Timer4_Cnt_GetCycleVal(void);
extern void Timer4_Cnt_SetCountVal(uint16_t u16Count);
extern uint16_t Timer4_Cnt_GetCountVal(void);

//@} // Timer4CntGroup

#ifdef __cplusplus
}
#endif

#endif /* __TIMER4_CNT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
