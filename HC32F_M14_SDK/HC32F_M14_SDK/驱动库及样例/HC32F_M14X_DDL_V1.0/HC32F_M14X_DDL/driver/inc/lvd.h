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
/** \file lvd.h
 **
 ** A detailed description is available at
 ** @link LvdGroup lvd description @endlink
 **
 **   - 2017-08-09  1.0  WangM First version for Device Driver Library of lvd
 **
 ******************************************************************************/

#ifndef __LVD_H__
#define __LVD_H__

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
 ** \defgroup LVD Group Low Voltage Dectect (LVD)
 ** 
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief LVD channel enumeration
 ** 
 ** \note
 **
 ******************************************************************************/
typedef enum en_lvd_chanel
{
    LvdChanel1 = 0u,
    LvdChanel2,
    LvdChanelMax,
}en_lvd_chanel_t;

/**
 *******************************************************************************
 ** \brief LVD event type enumeration
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_type
{
    LvdTypeIrq = 0u,
    LvdTypeRst,
    LvdTypeMax,
}en_lvd_type_t;


/**
 *******************************************************************************
 ** \brief LVD threshold of falling edge value enumeration
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_threshold_down
{
    Lvd_svhd_2_70V = 0u,
    Lvd_svhd_2_80V,
    Lvd_svhd_3_00V,
    Lvd_svhd_3_20V,
    Lvd_svhd_3_60V,
    Lvd_svhd_3_70V,
    Lvd_svhd_4_00V,
    Lvd_svhd_4_10V,
    Lvd_svhd_4_20V,
    Lvd_svhd_Max,
}en_lvd_threshold_down_t;

/**
 *******************************************************************************
 ** \brief LVD threshold of rising edge value enumeration
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_threshold_rise
{
    Lvd_svhr_2_80V = 0u,
    Lvd_svhr_3_00V,
    Lvd_svhr_3_20V,
    Lvd_svhr_3_60V,
    Lvd_svhr_3_70V,
    Lvd_svhr_4_00V,
    Lvd_svhr_4_10V,
    Lvd_svhr_4_20V,
    Lvd_svhr_4_30V,
    Lvd_svhr_Max,
}en_lvd_threshold_rise_t;

/**
 *******************************************************************************
 ** \brief LVD initialize configuration 
 ** \note
 ******************************************************************************/
typedef struct stc_lvd_config
{
    en_lvd_threshold_down_t    enThresholdD; ///< falling edge threshold voltage
    en_lvd_threshold_rise_t    enThresholdR; ///< rising edge threshold voltage
    en_lvd_type_t              enIrqType;    ///< interrupt type
    func_ptr_t                 pfnIrqCbk;    ///< interrupt callback function
}stc_lvd_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)                        
 ******************************************************************************/
/* Clear LVD interrupt flag */
extern en_result_t Lvd_ClearIrq(en_lvd_chanel_t enChanel);
/* Get LVD interrupt flag */
extern boolean_t Lvd_GetIrqStat(en_lvd_chanel_t enChanel);
/* Set LVD event, interrupt or reset */
extern en_result_t LvdEventTypeSet(en_lvd_chanel_t enChanel, en_lvd_type_t enType);

/* Initialize LVD function */
extern en_result_t Lvd_Init(en_lvd_chanel_t enChanel,stc_lvd_config_t *pstcConfig);
/* De-Initialize the LVD module */
extern en_result_t Lvd_DeInit(en_lvd_chanel_t enChanel,boolean_t bNvic);

/* LVD detect enable */
extern en_result_t Lvd_Enable(en_lvd_chanel_t enChanel);
/* LVD detect disable */
extern en_result_t Lvd_Disable(en_lvd_chanel_t enChanel);
//@} // LvdGroup

#ifdef __cplusplus
}
#endif

#endif /* __LVD_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
