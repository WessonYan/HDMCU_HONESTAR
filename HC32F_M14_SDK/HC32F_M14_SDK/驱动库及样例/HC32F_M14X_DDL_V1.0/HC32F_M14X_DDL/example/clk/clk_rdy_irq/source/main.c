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
 ** \brief This example shows how to use the ready interrupt function of the clock.
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
/* LED I/O */
#define Led_Init() Gpio_InitIO(2, 4, GpioDirOut, FALSE, FALSE)
#define Led_On()   Gpio_SetIO(2, 4, FALSE)
#define Led_Off()  Gpio_SetIO(2, 4, TRUE)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void PLLRdyIrqCb(void);
static void HSXTRdyIrqCb(void);
static void HSXTRdyIrqCb(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static boolean_t m_bPLLRdy = FALSE;
static boolean_t m_bHSXTRdy = FALSE;
static boolean_t m_bLSXTRdy = FALSE;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief PLL ready interrupt callback function
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void PLLRdyIrqCb(void)
{
    m_bPLLRdy = TRUE;
}

/**
 *******************************************************************************
 ** \brief HSXT ready interrupt callback function
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void HSXTRdyIrqCb(void)
{
    m_bHSXTRdy = TRUE;
}

/**
 *******************************************************************************
 ** \brief LSXT ready interrupt callback function
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void LSXTRdyIrqCb(void)
{
    m_bLSXTRdy = TRUE;
}

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_clk_config_t stcCfg;
    en_result_t enRet = Ok;

    /* Zero structure */
    DDL_ZERO_STRUCT(stcCfg);

    /* Disable PLL/HSXT/LSXT */
    Clk_Enable(ClkMPLL, FALSE);
    Clk_Enable(ClkHSXT, FALSE);
    Clk_Enable(ClkLSXT, FALSE);

    /* Initialize I/O */
    Led_Init();

    /* Initialize clock configuration structure */
    stcCfg.enClkSrc = ClkHICR;
    stcCfg.enBaseClkDiv = ClkBaseDiv1;
    stcCfg.enAPB0Div = ClkApb0Div1;
    stcCfg.enAPB1Div = ClkApb1Div1;
    stcCfg.bAPB1En = TRUE;
    stcCfg.enClkWaitMain = ClkCswtMain524ms;
    stcCfg.enClkWaitSub = ClkCswtSub256ms;
    stcCfg.enClkWaitPll = ClkPswtPll680us;
    stcCfg.u8PllMulti = 2;
    stcCfg.enPllSrc = MAIN2PLL;
    stcCfg.bHicrEn = TRUE;
    stcCfg.bMainEn = TRUE;
    stcCfg.bSubEn  = TRUE;
    stcCfg.bPllEn  = TRUE;
    stcCfg.bPLLIrq = TRUE;
    stcCfg.bHSXTIrq = TRUE;
    stcCfg.bLSXTIrq = TRUE;
    stcCfg.pfnPLLRdyCb = PLLRdyIrqCb;
    stcCfg.pfnHSXTRdyCb = HSXTRdyIrqCb;
    stcCfg.pfnLSXTRdyCb = LSXTRdyIrqCb;
    enRet = Clk_Init(&stcCfg); /* Initialize clock */
    if(Ok != enRet)
    {
        while(1)
        {
            /* nop */
        }
    }
    else
    {
        /* nop */
    }

    while(1)
    {
        if((TRUE == m_bPLLRdy)  &&
           (TRUE == m_bLSXTRdy) &&
           (TRUE == m_bHSXTRdy))
        {
            Led_On();   /* Led on */
            delay1ms(1000u);

            Led_Off();  /* Led off */
            delay1ms(1000u);
        }
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
