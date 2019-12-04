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
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2016-02-16  1.0  XYZ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "gpio.h"
#include "opa.h"
#include "bgr.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
 
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void App_GpioInit(void);
static void App_OpaInit(void);
/**
 ******************************************************************************
 ** \brief  主函数
 **
 ** \return 无
 ******************************************************************************/
int32_t main(void)
{   
    ///< GPIO 初始化
    App_GpioInit();

    ///< OPA 初始化
    App_OpaInit();
    
    while (1)
    { 
        ;
    }
}

static void App_GpioInit(void)
{
    stc_gpio_cfg_t GpioInitStruct;
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);   // 使能GPIO模块时钟
    
    // 引脚配置
    GpioInitStruct.enDir = GpioDirIn;
    Gpio_Init(GpioPortB, GpioPin0, &GpioInitStruct);     // OPA_INP引脚：PB00
}

static void App_OpaInit(void)
{
    stc_opa_zcfg_t OpaInitStruct;
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralOpa, TRUE);    // 使能OPA模块时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); // 使能ADC模块时钟
    Bgr_BgrEnable();                                          // 使能BGR模块，使用OPA功能，必须使能ADC模块时钟及BGR模块
    
    Opa_Cmd(TRUE);                                       // 使能OPA通道
    
    OpaInitStruct.bAzen = TRUE;                          // 自动校零使能
    OpaInitStruct.bClk_sw_set = TRUE;                    // 软件校准使能
    OpaInitStruct.bAz_pulse = TRUE;                      // 软件校准
    Opa_SetZero(&OpaInitStruct);                         // 启动OPA软件校零
    
    delay10us(20);                                       // 延时20us，等待教零完成
    
    Opa_ZeroBitCtrl(enAzPulse, FALSE);                   // 校零结束，清除
}
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


