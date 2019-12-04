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
/** \file main.c
 **
 ** \brief The example for VC function demonstration
 **
 **   - 2017-08-10  1.0  WangM First version for sample of VC module.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "vc.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define TEST_PIN            (1u)
#define TEST_PORT           (0u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint32_t gu32Cnt = 0u;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief VC callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
void VcIrqCallback(void)
{
    if(1 == Vc_GetStatus(VcCmpResult))
    {
        Gpio_SetIO(TEST_PORT,TEST_PIN,TRUE);
    }
    else
    {
        Gpio_SetIO(TEST_PORT,TEST_PIN,FALSE);
    }

    Vc_ClearStat(VcCmpIrqResult);
    gu32Cnt++;
}

/**
 *******************************************************************************
 ** \brief  main function for VC function demonstrate
 **
 ** \param [in]  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_vc_config_t stcCfg;

    Gpio_SetFunc_CAOUT(1u);
    Gpio_InitIO(TEST_PORT, TEST_PIN, GpioDirOut,FALSE,FALSE);
    DDL_ZERO_STRUCT(stcCfg);

    stcCfg.enVcAnalogInputN = VcAvcc;    /* VcExtDirect or VcAvcc */
    stcCfg.bVcCmpDly = FALSE;

    stcCfg.enVcFilterTime = VcFilter256cycle;
    stcCfg.bVcFilterEn = TRUE;

    stcCfg.enCompSelN = AvccDiv4;
    stcCfg.enIrqEdgeSel = VcIrqCmpIrqEdge;
    stcCfg.pfnAnalogCmpCb = VcIrqCallback;

    stcCfg.bACmpOutputReverse = FALSE;

    Vc_Init(&stcCfg);

    Vc_EnableFunc(VcFuncACvt);
    Vc_EnableIrq();

    delay1ms(200u);                       /* wait vc ready */

    while(1)
    {
    }

}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
