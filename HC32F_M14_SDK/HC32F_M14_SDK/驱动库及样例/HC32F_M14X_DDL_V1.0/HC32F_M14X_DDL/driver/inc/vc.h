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
/** \file vc.h
 **
 ** A detailed description is available at
 ** @link VCGroup vc description @endlink
 **
 **   - 2017-08-10  1.0  WangM First version for Device Driver Library of vc.
 **
 ******************************************************************************/

#ifndef __VC_H__
#define __VC_H__

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
 ** \defgroup VCGroup Voltage Compare (VC)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief VC convert function select
 ******************************************************************************/
typedef enum en_vc_func_sel
{
    VcFuncACvt = 0u,                    ///< VC active convert function 
    VcFuncFilter,                       ///< VC compare filter function
}en_vc_func_sel_t;

/**
 *******************************************************************************
 ** \brief VC convert analog source select
 ******************************************************************************/
typedef enum en_vc_analog_src
{
    VcExtDirect = 0u,                   ///< exit input voltage as reference
    VcAvcc,                             ///< AVCC voltage as reference
}en_vc_analog_src_t;

/**
 *******************************************************************************
 ** \brief VC filter time 
 ******************************************************************************/
typedef enum en_vc_resp_filter
{
    VcFilter4cycle = 0u,
    VcFilter8cycle,
    VcFilter16cycle,
    VcFilter32cycle,
    VcFilter64cycle,
    VcFilter128cycle,
    VcFilter256cycle,
}en_vc_resp_filter_t;

/**
 *******************************************************************************
 ** \brief VC input volatege of AVCC set
 ******************************************************************************/
typedef enum en_vc_cmpsel_n
{
    AvccDiv6 = 0u,
    AvccDiv4,
    AvccDiv3,
    AvccDiv2,
}en_vc_cmpsel_n_t;

/**
 *******************************************************************************
 ** \brief VC interrupt mode(trigger edge)
 ******************************************************************************/
typedef enum en_vc_irq_sel
{
    VcIrqCmpIrqRise = 0u,               ///< rising edge
    VcIrqCmpIrqFall,                    ///< falling edge
    VcIrqCmpIrqEdge,                    ///< rising and falling edge
    VcIrqCmpIrqHigh,                    ///< high voltage
}en_vc_irq_sel_t;

/**
 *******************************************************************************
 ** \brief VC compare result status
 ******************************************************************************/
typedef enum en_vc_stat
{
    VcCmpResult = 0u,                   ///< VC compare result, VC_OUT flag
    VcCmpIrqResult,                     ///< VC interrupt flag status
}en_vc_stat_t;

/**
 *******************************************************************************
 ** \brief VC init configuration
 ******************************************************************************/
typedef struct stc_vc_config
{
    en_vc_analog_src_t      enVcAnalogInputN;   ///< analog input nagative
    en_vc_cmpsel_n_t        enCompSelN;         ///< negative standard voltage select(Avcc)
    boolean_t               bVcCmpDly;          ///< if enable compare delay
    boolean_t               bACmpOutputReverse; ///< if reserve output 
    en_vc_resp_filter_t     enVcFilterTime;     ///< filter time
    boolean_t               bVcFilterEn;        ///< filter enable
    boolean_t               bTimer2Rev;         ///< reseve
    boolean_t               bTimer0Rev;         ///< reseve
    en_vc_irq_sel_t         enIrqEdgeSel;       ///< irq edge select configure
    func_ptr_t              pfnAnalogCmpCb;     ///< irq copyback function
}stc_vc_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* get status */
extern boolean_t   Vc_GetStatus(en_vc_stat_t enStat);
/* clear stat */
extern en_result_t Vc_ClearStat(en_vc_stat_t enStat);

/* irq enable/disable */
extern en_result_t Vc_EnableIrq(void);
extern en_result_t Vc_DisableIrq( void);

/* init /deinit */
extern en_result_t Vc_Init(stc_vc_config_t *pstcConfig);
extern en_result_t Vc_DeInit(void);

/* enable /disable function */
extern en_result_t Vc_EnableFunc(en_vc_func_sel_t enFunc);
extern en_result_t Vc_DisableFunc(en_vc_func_sel_t enFunc);

//@} // VCGroup

#ifdef __cplusplus
}
#endif

#endif /* __VC_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
