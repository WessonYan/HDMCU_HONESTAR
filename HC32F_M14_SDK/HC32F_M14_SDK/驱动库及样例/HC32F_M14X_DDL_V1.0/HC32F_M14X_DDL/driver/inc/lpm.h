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
/** \file lpm.h
 **
 ** A detailed description is available at
 ** @link LpmGroup Lpm description @endlink
 **
 **   - 2017-08-10  1.0  HeChun First version for Device Driver Library of lpm.
 **
 **
 ******************************************************************************/

#ifndef __LPM_H__
#define __LPM_H__

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
 ** \defgroup LpmGroup Low Power Mode(LPM)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief low power mode type definition
 **
 ** \note set power mode type
 **
 ******************************************************************************/
typedef enum en_lpm_mode
{
    IdleMode        = 0u,   ///< Idle mode
    StbStopMode     = 1u,   ///< standby stop mode
    StbTimerMode    = 2u    ///< standby timer mode
} en_lpm_mode_t;

/**
 *******************************************************************************
 ** \brief backup register struct definition
 **
 ******************************************************************************/
typedef struct stc_backupreg
{
    uint8_t u8BREG01;
    uint8_t u8BREG02;
    uint8_t u8BREG03;
    uint8_t u8BREG04;
    uint8_t u8BREG05;
    uint8_t u8BREG06;
    uint8_t u8BREG07;
    uint8_t u8BREG08;
    uint8_t u8BREG09;
    uint8_t u8BREG10;
    uint8_t u8BREG11;
    uint8_t u8BREG12;
    uint8_t u8BREG13;
    uint8_t u8BREG14;
    uint8_t u8BREG15;
    uint8_t u8BREG16;
} stc_backupreg_t;

/**
 *******************************************************************************
 ** \brief backup register index type definition
 **
 ******************************************************************************/
typedef enum en_bakup_reg
{
    BackupReg1 = 0u,      ///< index of backup register 1
    BackupReg2 = 1u,      ///< index of backup register 2
    BackupReg3 = 2u,      ///< index of backup register 3
    BackupReg4 = 3u,      ///< index of backup register 4
    BackupReg5 = 4u,      ///< index of backup register 5
    BackupReg6 = 5u,      ///< index of backup register 6
    BackupReg7 = 6u,      ///< index of backup register 7
    BackupReg8 = 7u,      ///< index of backup register 8
    BackupReg9 = 8u,      ///< index of backup register 9
    BackupReg10 = 9u,     ///< index of backup register 10
    BackupReg11 = 10u,    ///< index of backup register 11
    BackupReg12 = 11u,    ///< index of backup register 12
    BackupReg13 = 12u,    ///< index of backup register 13
    BackupReg14 = 13u,    ///< index of backup register 14
    BackupReg15 = 14u,    ///< index of backup register 15
    BackupReg16 = 15u     ///< index of backup register 15

} en_bakup_reg_t;


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* enter lpm mode function */
void Lpm_GoToStandByMode(en_lpm_mode_t enMode, boolean_t bIoRemain);

/* backup register read and write function */
en_result_t Lpm_WriteBackupReg(en_bakup_reg_t enRegIndex, uint8_t u8Data);
en_result_t Lpm_ReadBackupRegs(stc_backupreg_t* stcBackUpReg);



//@} // LpmGroup

#ifdef __cplusplus
}
#endif

#endif /* __LPM_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
