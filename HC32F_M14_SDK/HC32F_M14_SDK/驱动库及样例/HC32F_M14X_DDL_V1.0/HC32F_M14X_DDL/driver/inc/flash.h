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
/** \file flash.h
 **
 ** A detailed description is available at
 ** @link FLashGroup Flash description @endlink
 **
 **   - 2017-5-31  1.0  Zhangxl First version for Device Driver Library of flash
 **
 ******************************************************************************/

#ifndef __FLASH_H__
#define __FLASH_H__

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
 ** \defgroup FlashGroup Flash
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief FLASH FUDR
 **
 ** \note To select the FLASH User Data Register
 ******************************************************************************/
typedef enum en_flash_fudr
{
    FUDR0 = 0u,                         ///<Flash user data register0
} en_flash_fudr_t;

/**
 *******************************************************************************
 ** \brief FLASH FOCR
 **
 ** \note To select the operation of FLASH control
 ******************************************************************************/
typedef enum en_flash_focr
{
    Prgm = 0x1ACC0001u,                 ///< program mode
    Pers = 0x1ACC0002u,                 ///< page erase mode
    Cers = 0x1ACC0004u,                 ///< chip erase mode
    Dpstb = 0x1ACC0008u,                ///< deep sleep mode
    Prefetch = 0x1ACC0020u              ///< pre-fetch
} en_flash_focr_t;

/**
 *******************************************************************************
 ** \brief FLASH FLC
 **
 ** \note To select flash page lock
 ******************************************************************************/
typedef enum en_flash_lock
{
    LockDisable = 0u,                   ///< Flash page lock2K enable
    LockEnable = 1u                     ///< Flash page lock2K disable
} en_flash_lock_t;

/**
 *******************************************************************************
 ** \brief FLASH Programing Timer
 **
 ** \note To define flash programing timer register (4MHz)
 ******************************************************************************/
typedef enum en_flash_prg_timer
{
    Tnvs        = 38u,
    Tprog       = 27u,
    Tpgs        = 23u,
    Trcv_pgm    = 32u,
    Trcv_pers   = 40u,
    Trcv_cers   = 10u,
    Tpers       = 18u,
    Tcers       = 34u
} en_flash_prg_timer_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* Flash User Data Register write and read */
en_result_t Flash_UdrWrite(en_flash_fudr_t enFudr, uint32_t u32Data);
en_result_t Flash_UdrRead(en_flash_fudr_t enFudr, uint32_t* pu32Data);

/* Flash write */
en_result_t Flash_WriteByte(uint32_t u32Addr, uint8_t u8Data);
en_result_t Flash_WriteHalfWord(uint32_t u32Addr, uint16_t u16Data);
en_result_t Flash_WriteWord(uint32_t u32Addr, uint32_t u32Data);

/* Flash erase */
en_result_t Flash_PageErase(uint32_t u32PageAddr);
en_result_t Flash_ChipErase(void);

/* Flash timer set */
en_result_t Flash_PgmTimerConfig(uint8_t u8FreqCfg);

/* Enable flash pre-fetch function */
en_result_t Flash_PreFetchEnable(void);

/* Disable flash pre-fetch function */
en_result_t Flash_PreFetchEnable(void);

//@} // FlashGroup

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H_ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
