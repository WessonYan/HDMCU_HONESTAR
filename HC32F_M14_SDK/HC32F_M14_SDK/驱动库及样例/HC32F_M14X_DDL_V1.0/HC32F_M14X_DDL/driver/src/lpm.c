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
/** \file lpm.c
 **
 ** A detailed description is available at
 ** @link LpmGroup lpm @endlink
 **
 **   - 2017-08-07  1.0  HeChun First version for Device Driver Library of lpm.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "lpm.h"

/**
 *******************************************************************************
 ** \addtogroup LpmGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define CM_CORE_SCR                 *((volatile unsigned int*)(0xE000ED10UL))

#define SLEEPDEEP_BIT                (uint32_t)0x01<<2

#define LPM_CTL_KEY                  (uint32_t)0x1ACC<<16

#define SPLV_RETAIN                  (uint32_t)0x00
#define SPLV_HIGH_Z                  (uint32_t)0x10

#define STBM_TIMER                   (uint32_t)0x00
#define STBM_STOP                    (uint32_t)0x02

#define IS_VALID_WORKMODE(x)            \
(   (IdleMode <= (x)) &&                \
    (StbTimerMode >= (x)))

#define IS_VALID_BACKUPREG_INDEX(x)     \
(   (BackupReg1 <= (x)) &&              \
    (BackupReg16 >= (x)))

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
 ** \brief go to standby mode
 **
 ** \param enMode the type of standby mode.
 **        This parameter can be one of the following values:
 ** \arg   IdleMode       Sleep mode
 ** \arg   StbStopMode    Stop mode
 ** \arg   StbTimerMode   Timer mode

 ** \param bIoRemain Set io status when enter standby mode.
 **        This parameter can be one of the following values:
 ** \arg   FALSE Set IO to Hi-z
 ** \arg   TRUE  Remain IO status
 **
 ******************************************************************************/
void Lpm_GoToStandByMode(en_lpm_mode_t enMode, boolean_t bIoRemain)
{
    ASSERT(IS_VALID_WORKMODE(enMode));
    switch (enMode)
    {
        case IdleMode:
            /* SLEEPDEEP=0 */
            CM_CORE_SCR &= ~(SLEEPDEEP_BIT);
            __WFI();
            break;
        case StbStopMode:
            if( TRUE == bIoRemain )
            {
                /* Retains status of each pin */
                M0P_CLOCK->LPM_CTL = LPM_CTL_KEY | STBM_STOP | SPLV_RETAIN;
            }
            else
            {
                /* Sets the status of each pin to high impedance */
                M0P_CLOCK->LPM_CTL = LPM_CTL_KEY | STBM_STOP | SPLV_HIGH_Z;
            }
            /* SLEEPDEEP=1 */
            CM_CORE_SCR |= SLEEPDEEP_BIT;
            __WFI();
            break;
        case StbTimerMode:
            if( TRUE == bIoRemain )
            {
                /* Retains status of each pin */
                M0P_CLOCK->LPM_CTL = LPM_CTL_KEY | STBM_TIMER | SPLV_RETAIN;
            }
            else
            {
                /* Sets the status of each pin to high impedance */
                M0P_CLOCK->LPM_CTL = LPM_CTL_KEY | STBM_TIMER | SPLV_HIGH_Z;
            }
            /* SLEEPDEEP=1 */
            CM_CORE_SCR |= SLEEPDEEP_BIT;
            __WFI();
            break;
        default:
            break;
    }
}

/**
 *******************************************************************************
 ** \brief Write the backup register
 **
 ** \param enRegIndex register index
 ** \arg BackupReg1     Back up register 1 index
 ** \arg BackupReg2     Back up register 2 index
 ** \arg BackupReg3     Back up register 3 index
 ** \arg BackupReg4     Back up register 4 index
 ** \arg BackupReg5     Back up register 5 index
 ** \arg BackupReg6     Back up register 6 index
 ** \arg BackupReg7     Back up register 7 index
 ** \arg BackupReg8     Back up register 8 index
 ** \arg BackupReg9     Back up register 9 index
 ** \arg BackupReg10    Back up register 10 index
 ** \arg BackupReg11    Back up register 11 index
 ** \arg BackupReg12    Back up register 12 index
 ** \arg BackupReg13    Back up register 13 index
 ** \arg BackupReg14    Back up register 14 index
 ** \arg BackupReg15    Back up register 15 index
 ** \arg BackupReg16    Back up register 16 index
 ** \param u8Data the data written into backup register
 **
 ** \retval Ok The backup register written normally
 ** \retval ErrorInvalidParameter enRegIndex > BackupReg15
 **
 ******************************************************************************/
en_result_t Lpm_WriteBackupReg(en_bakup_reg_t enRegIndex, uint8_t u8Data)
{
    volatile uint8_t* pBufReg = (volatile uint8_t*)&(M0P_WSUNIT->BUR01);
    ASSERT(IS_VALID_BACKUPREG_INDEX(enRegIndex));

    if(enRegIndex > BackupReg16)
    {
        return ErrorInvalidParameter;
    }
    pBufReg += (uint8_t)enRegIndex;
    *pBufReg = u8Data;
    return Ok;
}


/**
 *******************************************************************************
 ** \brief Read Backup Registers
 **
 ** \param [out]  stcBackUpReg     double pointer to user backup register
 **                                structure
 **
 ** \retval Ok                     Successfully read
 ******************************************************************************/
en_result_t Lpm_ReadBackupRegs(stc_backupreg_t* pstcBackUpReg)
{
    uint8_t  u8Counter;
    volatile uint8_t* pBufReg = NULL;
    uint8_t* pBufRead = NULL;
    ASSERT(NULL != pstcBackUpReg);

    pBufReg = (volatile uint8_t*)&(M0P_WSUNIT->BUR01);
    pBufRead = (uint8_t*)&pstcBackUpReg->u8BREG01;

    for(u8Counter = BackupReg1; u8Counter <= BackupReg16; u8Counter ++)
    {
        *pBufRead++ = *pBufReg++;
    }

    return Ok;
}

//@} // LpmGroup


/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
