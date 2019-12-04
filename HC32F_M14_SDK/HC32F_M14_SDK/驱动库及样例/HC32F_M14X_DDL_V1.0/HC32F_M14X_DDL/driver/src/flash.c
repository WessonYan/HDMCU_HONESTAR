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
/** \file flash.c
 **
 ** A detailed description is available at
 ** @link FlashGroup Flash description @endlink
 **
 **   - 2017-5-31  1.0  Zhangxl First version for Device Driver Library of Flash
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "flash.h"

/**
 *******************************************************************************
 ** \addtogroup FlashGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define FRWPW               (0x1ACC0000u)       ///< Key of flash operation
#define FLASH_END_ADDR      (0x0001FFFFu)       ///< End address of flash memory
#define USERDATA0           (0x00100004u)       ///< Address of user data
#define SECURITY            (0x00100000u)       ///< Address of flash security

/*! Parameter valid check for user data input */
#define IS_VALID_FUDR(x) (FUDR0 == (x))

/*! Parameter valid check for flash operation address */
#define IS_VALID_ADDR(x)                \
(   FLASH_END_ADDR >= (x)   ||          \
    USERDATA0 == (x)        ||          \
    SECURITY == (x))

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
 ** \brief FLASH UDR write
 **
 ** This function set flash user data register.
 **
 ** \param [in] enFudr            User data register number
 ** \param [in] u32Data           User data
 **
 ** \retval Ok                    Process successfully done.
 **
 ******************************************************************************/
en_result_t Flash_UdrWrite(en_flash_fudr_t enFudr, uint32_t u32Data)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_FUDR(enFudr));

    (&(M0P_FLASH_IF->FUDR0))[enFudr] = u32Data;

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief FLASH UDR read
 **
 ** This function read flash user data register.
 **
 ** \param [in]  enFudr             User data register number
 ** \param [out] pu32Data           The pointer of user data
 **
 ** \retval Ok                      Process successfully done.
 **
 ******************************************************************************/
en_result_t Flash_UdrRead(en_flash_fudr_t enFudr, uint32_t* pu32Data)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_FUDR(enFudr));

    *pu32Data = (&(M0P_FLASH_IF->FUDR0))[enFudr];

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief FLASH write byte
 **
 ** This function write 1 byte data to flash.
 **
 ** \param [in]  u32Addr            Flash address
 ** \param [in]  u8Data             The data be written
 **
 ** \retval Ok                      Process successfully done.
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                                     - u32Addr is invalid
 ******************************************************************************/
en_result_t Flash_WriteByte(uint32_t u32Addr, uint8_t u8Data)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_ADDR(u32Addr));

    /* unlock flash */
    M0P_FLASH_IF->FLCR0 = 0;
    M0P_FLASH_IF->FLCR1 = 0;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* set PRGM */
    M0P_FLASH_IF->FOCR = Prgm;

    /* write data */
    *((volatile uint8_t*)u32Addr) = u8Data;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* clear PRGM */
    M0P_FLASH_IF->FOCR = FRWPW;

    /* lock flash */
    M0P_FLASH_IF->FLCR0 = 0xFFFFFFFF;
    M0P_FLASH_IF->FLCR1 = 0xFFFFFFFF;

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief FLASH write half-word
 **
 ** This function write 1 halfword(2 bytes) data to flash.
 **
 ** \param [in]  u32Addr            Flash address
 ** \param [in]  u16Data            The data be written
 **
 ** \retval Ok                      Process successfully done.
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                                     - u32Addr is invalid
 ******************************************************************************/
en_result_t Flash_WriteHalfWord(uint32_t u32Addr, uint16_t u16Data)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_ADDR(u32Addr));
    if (u32Addr % 2)
    {
        enResult = ErrorInvalidParameter;
        return (enResult);
    }

    /* unlock flash */
    M0P_FLASH_IF->FLCR0 = 0;
    M0P_FLASH_IF->FLCR1 = 0;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* set PRGM */
    M0P_FLASH_IF->FOCR = Prgm;

    /* write data */
    *((volatile uint16_t*)u32Addr) = u16Data;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* clear PRGM */
    M0P_FLASH_IF->FOCR = FRWPW;

    /* lock flash */
    M0P_FLASH_IF->FLCR0 = 0xFFFFFFFF;
    M0P_FLASH_IF->FLCR1 = 0xFFFFFFFF;

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief FLASH write word
 **
 ** This function write 1 word(4 bytes) data to flash.
 **
 ** \param [in]  u32Addr            Flash address
 ** \param [in]  u32Data            The data be written
 **
 ** \retval Ok                      Process successfully done.
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                                     - u32Addr is invalid
 ******************************************************************************/
en_result_t Flash_WriteWord(uint32_t u32Addr, uint32_t u32Data)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_ADDR(u32Addr));
    if (u32Addr % 4)
    {
        enResult = ErrorInvalidParameter;
        return (enResult);
    }

    /* unlock flash */
    M0P_FLASH_IF->FLCR0 = 0;
    M0P_FLASH_IF->FLCR1 = 0;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* set PRGM */
    M0P_FLASH_IF->FOCR = Prgm;

    /* write data */
    *((volatile uint32_t*)u32Addr) = u32Data;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* clear PRGM */
    M0P_FLASH_IF->FOCR = FRWPW;

    /* lock flash */
    M0P_FLASH_IF->FLCR0 = 0xFFFFFFFF;
    M0P_FLASH_IF->FLCR1 = 0xFFFFFFFF;

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief FLASH page erase
 **
 ** This function erase flash page.
 **
 ** \param [in]  u32PageAddr        Flash page address
 **
 ** \retval Ok                      Process successfully done.
 **
 ******************************************************************************/
en_result_t Flash_PageErase(uint32_t u32PageAddr)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_ADDR(u32PageAddr));

    /* unlock flash */
    M0P_FLASH_IF->FLCR0 = 0;
    M0P_FLASH_IF->FLCR1 = 0;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* set PERS */
    M0P_FLASH_IF->FOCR = Pers;

    /* write invalid data */
    *((volatile uint32_t*)(u32PageAddr & 0xFFFFFFFC)) = 0;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* lock flash */
    M0P_FLASH_IF->FLCR0 = 0xFFFFFFFF;
    M0P_FLASH_IF->FLCR1 = 0xFFFFFFFF;

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief FLASH page erase
 **
 ** This function erase whole chip flash.
 **
 ** \retval Ok                    Process successfully done.
 **
 ******************************************************************************/
en_result_t Flash_ChipErase(void)
{
    en_result_t enResult = Ok;
    uint32_t u32PageAddr = 0;

    /* unlock flash */
    M0P_FLASH_IF->FLCR0 = 0;
    M0P_FLASH_IF->FLCR1 = 0;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* set CERS */
    M0P_FLASH_IF->FOCR = Cers;

    /* write invalid data */
    *((volatile uint32_t*)(u32PageAddr & 0xFFFFFFFC)) = 0;

    /* busy? */
    while (TRUE == M0P_FLASH_IF->FSTR_f.BUSY)
    {
    }

    /* lock flash */
    M0P_FLASH_IF->FLCR0 = 0xFFFFFFFF;
    M0P_FLASH_IF->FLCR1 = 0xFFFFFFFF;

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief FLASH programing timer config
 **
 ** This function is config FLASH programing timer register.
 **
 ** \param [in]  u8FreqCfg        The frequency of Fchip
 **                               u8FreqCfg - Fchip
 **                                  0      - [ 0, 4]MHz;
 **                                  1      - [ 4, 8]MHz;
 **                                  2      - [ 8,16]MHz;
 **                                  3      - [16,32]MHz;
 **                                  4      - [32,48]MHz
 **                                 other   -  Invalid
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                 - u8FreqCfg is invalid
 ******************************************************************************/
en_result_t Flash_PgmTimerConfig(uint8_t u8FreqCfg)
{
  uint8_t au8FlashOpTm[] = {1, 2, 4, 8, 12};

    if (u8FreqCfg > 4)
    {
        return (ErrorInvalidParameter);
    }

    /* Flash timer register config */
    M0P_FLASH_IF->TNVS = Tnvs * au8FlashOpTm[u8FreqCfg];
    M0P_FLASH_IF->TPROG = Tprog * au8FlashOpTm[u8FreqCfg];
    M0P_FLASH_IF->TPGS = Tpgs * au8FlashOpTm[u8FreqCfg];
    M0P_FLASH_IF->TRCVPGM = Trcv_pgm * au8FlashOpTm[u8FreqCfg];
    M0P_FLASH_IF->TRCVPERS = Trcv_pers * au8FlashOpTm[u8FreqCfg];
    M0P_FLASH_IF->TRCVCERS = Trcv_cers * au8FlashOpTm[u8FreqCfg];
    M0P_FLASH_IF->TPERS = Tpers * au8FlashOpTm[u8FreqCfg];
    M0P_FLASH_IF->TCERS = Tcers * au8FlashOpTm[u8FreqCfg];

    return (Ok);
}

/**
 *******************************************************************************
 ** \brief FLASH pre-fetch function enable
 **
 ** This function enables the flash memory pre-fetch function.
 **
 ** \retval Ok                    Process successfully done.
 **
 ******************************************************************************/
en_result_t Flash_PreFetchEnable(void)
{
    M0P_FLASH_IF->FOCR = Prefetch;
    return (Ok);
}

/**
 *******************************************************************************
 ** \brief FLASH pre-fetch function disable
 **
 ** This function disables the flash memory pre-fetch function.
 **
 ** \retval Ok                    Process successfully done.
 **
 ******************************************************************************/
en_result_t Flash_PreFetchDisable(void)
{
    M0P_FLASH_IF->FOCR = FRWPW;
    return (Ok);
}

//@} // FlashGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
