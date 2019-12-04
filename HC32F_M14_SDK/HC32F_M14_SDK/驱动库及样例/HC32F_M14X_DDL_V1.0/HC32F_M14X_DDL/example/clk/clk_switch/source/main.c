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
 ** \brief This example shows how to switch the clock as mater clock.
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     clock.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define FAST_IO_INIT( )   FGpio_InitIO(0, 0, GpioDirOut)
#define FAST_IO_PORT_PIN  bM0P_FASTIO_FSET0_P00

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
 ** \brief  Main function of project
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_clk_config_t stcCfg;
    en_clk_source_t enSource = ClkLSXT;
    en_result_t enRet = Ok;

    /* zero sturcture */
    DDL_ZERO_STRUCT(stcCfg);

    /* Initialize I/O */
    FAST_IO_INIT();

    /* Initialize clock configuration structure */
    stcCfg.u8PllMulti = 2;
    stcCfg.enPllSrc = MAIN2PLL;
    stcCfg.enClkSrc = ClkHICR;
    stcCfg.enBaseClkDiv = ClkBaseDiv1;
    stcCfg.enAPB0Div = ClkApb0Div1;
    stcCfg.enAPB1Div = ClkApb1Div1;
    stcCfg.bAPB1En = TRUE;
    stcCfg.enClkWaitMain = ClkCswtMain524ms;
    stcCfg.enClkWaitSub = ClkCswtSub512ms;
    stcCfg.enClkWaitPll = ClkPswtPll680us;
    stcCfg.bHicrEn = TRUE;
    stcCfg.bMainEn = TRUE;
    stcCfg.bSubEn  = TRUE;
    stcCfg.bPllEn  = TRUE;
    enRet = Clk_Init(&stcCfg); /* Initialize Clock */
    if(Ok != enRet)
    {
        while(1)
        {
            /* nop */
        }
    }

    while (1)
    {
        if(ClkHICR == enSource)
        {
            enSource = ClkHSXT;
        }
        else if(ClkHSXT == enSource)
        {
            enSource = ClkMPLL;
        }
        else if(ClkMPLL == enSource)
        {
            enSource = ClkLICR;
        }
        else if(ClkLICR == enSource)
        {
            enSource = ClkLSXT;
        }
        else if(ClkLSXT == enSource)
        {
            enSource = ClkHICR;
        }
        else
        {
            /* nop */
        }

        /* Switch clock */
        Clk_SwitchTo(enSource);

        /* Toggle Fast I/O*/
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
        FAST_IO_PORT_PIN = 0xFF;
        FAST_IO_PORT_PIN = 0;
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
