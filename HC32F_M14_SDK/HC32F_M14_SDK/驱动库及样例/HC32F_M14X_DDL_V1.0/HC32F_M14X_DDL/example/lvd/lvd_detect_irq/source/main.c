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
 ** \brief The example for LVD interrupt function demonstration
 **
 **   - 2017-08-10  1.0  WangM First version for LVD module.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "lvd.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define TEST_PORT1       (0)
#define TEST_PIN1        (0)

#define TEST_PORT2       (0)
#define TEST_PIN2        (1)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t vol1 = 0u,vol2 = 0u;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief LVD channel 1 interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void Lvd1IrqCallback(void)
{
    vol1 = !vol1;
    Gpio_SetIO(TEST_PORT1,TEST_PIN1,vol1);
}

/**
 *******************************************************************************
 ** \brief LVD channel 2 interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void Lvd2IrqCallback(void)
{
    vol2 = !vol2;
    Gpio_SetIO(TEST_PORT2,TEST_PIN2,vol2);
}

/**
 *******************************************************************************
 ** \brief  main function for lvd interrupt function
 **
 ** \param [in]  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_lvd_config_t   stcCfg;

    /* Initialize the test pin */
    Gpio_InitIO(TEST_PORT1, TEST_PIN1, GpioDirOut, FALSE, FALSE);
    Gpio_InitIO(TEST_PORT2, TEST_PIN2, GpioDirOut, FALSE, FALSE);

    /* Clear the config structural */
    DDL_ZERO_STRUCT(stcCfg);

    /* Initialize the lvd interrupt function of channel 1 */
    stcCfg.enIrqType = LvdTypeIrq;
    stcCfg.enThresholdR = Lvd_svhr_4_20V;
    stcCfg.enThresholdD = Lvd_svhd_3_00V;
    stcCfg.pfnIrqCbk = Lvd1IrqCallback;
    Lvd_Init(LvdChanel1, &stcCfg);

    /* Enable detect funtion of lvd channel 1 */
    Lvd_Enable(LvdChanel1);

    /* Clear the config structural */
    DDL_ZERO_STRUCT(stcCfg);

    /* Initialize the lvd interrupt function of channel 2 */
    stcCfg.enIrqType = LvdTypeIrq;
    stcCfg.enThresholdR = Lvd_svhr_3_70V;
    stcCfg.enThresholdD = Lvd_svhd_3_00V;
    stcCfg.pfnIrqCbk = Lvd2IrqCallback;
    Lvd_Init(LvdChanel2, &stcCfg);

    /* Enable detect funtion of lvd channel 2 */
    Lvd_Enable(LvdChanel2);

    while (1)
    {
    }

}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
