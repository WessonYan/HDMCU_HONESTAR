/******************************************************************************
* Copyright (C) 2019, Huada Semiconductor Co.,Ltd All rights reserved.
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
 **   - 2019-03-01  1.0  Lux First version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "sysctrl.h"
#include "dac.h"
#include "flash.h"
#include "gpio.h"
#include "math.h"
#include "dmac.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint16_t SendBuf[128];
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                             
 ******************************************************************************/
#define PI 3.14159265358979           //圆周率
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void App_SysClkInit(void);               ///< 系统时钟初始化
static void App_GpioInit(void);                 ///< GPIO初始化
static void App_DACInit(void);                  ///< DAC初始化
static void App_DMAInit(void);                  ///< DMA初始化

/**
******************************************************************************
    ** \brief  主函数
    ** 
    ** @param  无
    ** \retval 无
    **
******************************************************************************/
int32_t main(void)
{    
    App_SysClkInit();               ///< 系统时钟初始化
    App_GpioInit();                 ///< GPIO初始化
    App_DACInit();                  ///< DAC初始化
    App_DMAInit();                  ///< DMA初始化
    
    while(1)
    {
        Dac_SoftwareTriggerCmd();   ///< 软件触发
        delay10us(12);
    }
}    

/**
 ******************************************************************************
 ** \brief  初始化外部系统时钟
 **
 ** \return 无
 ******************************************************************************/
static void App_SysClkInit(void)
{
    stc_sysctrl_clk_cfg_t stcCfg;
    stc_sysctrl_pll_cfg_t stcPLLCfg;
    
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralFlash, TRUE);    ///< 使能FLASH模块的外设时钟
    Flash_WaitCycle(FlashWaitCycle1);
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);             ///< PLL使用RCH作为时钟源，因此需要先设置RCH    
    
    stcPLLCfg.enInFreq    = SysctrlPllInFreq4_6MHz;     ///< RCH 4MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  ///< PLL 输出48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllRch;              ///< 输入时钟源选择RCH
    stcPLLCfg.enPllMul    = SysctrlPllMul12;            ///< 4MHz x 12 = 48MHz
    Sysctrl_SetPLLFreq(&stcPLLCfg);
    
    ///< 选择PLL作为HCLK时钟源;
    stcCfg.enClkSrc  = SysctrlClkPLL;
    ///< HCLK SYSCLK/2
    stcCfg.enHClkDiv = SysctrlHclkDiv1;
    ///< PCLK 为HCLK/8
    stcCfg.enPClkDiv = SysctrlPclkDiv1;
    ///< 系统时钟初始化
    Sysctrl_ClkInit(&stcCfg);
}



/**
 ******************************************************************************
 ** \brief  初始化外部GPIO引脚
 **
 ** \return 无
 ******************************************************************************/
static void App_GpioInit(void)
{    
    ///< 开启GPIO外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    // 使能GPIO模块的外设时钟
    
    Gpio_SetAnalogMode(GpioPortA, GpioPin4);       //PA04作为DAC的模拟输出
}

/**
 ******************************************************************************
 ** \brief  产生正弦波的采样点
 ** \param  buf:用于存放正弦波采样点的缓存
 ** \return 无
 ******************************************************************************/
static void _BufProduce(uint16_t* buf)
{
    uint8_t tmp;
    double tmp1;
    tmp1=PI/64;
    for(tmp=0; tmp<128; tmp++)
    {
        buf[tmp] =(uint16_t)(((sin(tmp1*tmp))*2047)+2048);
    }        
}

/**
******************************************************************************
    ** \brief  配置DAC
    ** 
    ** @param  无
    ** \retval 无
    **
******************************************************************************/ 
static void App_DACInit(void)
{
    stc_dac_cfg_t  dac_initstruct;
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralDac, TRUE);     ///< 使能DAC模块的时钟
    
    dac_initstruct.boff_t = DacBoffDisable;
    dac_initstruct.ten_t  = DacTenEnable;
    dac_initstruct.sref_t = DacVoltageAvcc;
    dac_initstruct.mamp_t = DacMenp4095;
    dac_initstruct.tsel_t = DacSwTriger;      ///< 软件触发方式
    dac_initstruct.align  = DacRightAlign;    ///< 右对齐
    Dac_Init(&dac_initstruct);
    Dac_Cmd(TRUE);
    Dac_DmaCmd(TRUE);                       ///< DAC通道DMA使能
    
    _BufProduce(SendBuf);                   ///< 产生正弦波的采样点
    
}

/**
******************************************************************************
    ** \brief  配置DMA
    ** 
    ** @param  无
    ** \retval 无
    **
******************************************************************************/ 
static void App_DMAInit(void)
{
    stc_dma_cfg_t DmaInitStruct;
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralDma, TRUE);          ///< 使能DMA模块的外设时钟
    
    DmaInitStruct.enMode =  DmaMskBlock;                            ///< 选择块传输
    DmaInitStruct.u16BlockSize = 1;                                 ///< 块传输个数
    DmaInitStruct.u16TransferCnt = 128;                             ///< 块传输次数，一次传输数据大小为 块传输个数*BUFFER_SIZE
    DmaInitStruct.enTransferWidth = DmaMsk16Bit;                    ///< 传输数据的宽度，此处选择字(16Bit)宽度
    DmaInitStruct.enSrcAddrMode = DmaMskSrcAddrInc;                 ///< 源地址自增
    DmaInitStruct.enDstAddrMode = DmaMskDstAddrFix;                 ///< 目的地址自增
    DmaInitStruct.enDestAddrReloadCtl = DmaMskDstAddrReloadDisable; ///< 禁止重新加载传输目的地址
    DmaInitStruct.enSrcAddrReloadCtl = DmaMskSrcAddrReloadEnable;   ///< 使能重新加载传输源地址
    DmaInitStruct.enSrcBcTcReloadCtl = DmaMskBcTcReloadEnable;      ///< 使能重新加载BC/TC值
    DmaInitStruct.u32SrcAddress = (uint32_t)SendBuf;                ///< 源地址
    DmaInitStruct.u32DstAddress = 0x40002508;                       ///< 目标地址：DAC_DHR12R0 
    DmaInitStruct.enRequestNum = DmaDAC0Trig;                       ///< 设置DAC0触发
    DmaInitStruct.enTransferMode = DmaMskContinuousTransfer;        ///< 连续传输
    DmaInitStruct.enPriority = DmaMskPriorityFix;                   ///< 各通道固定优先级，CH0优先级 > CH1优先级    
    Dma_InitChannel(DmaCh0,&DmaInitStruct);                         ///< 初始化dma通道0
    
    Dma_Enable();                                                   ///< 使能DMA
    Dma_EnableChannel(DmaCh0);                                      ///< 使能DMA通道0
    Dma_ClrStat(DmaCh0);                                            ///< 清零：STAT[2:0]           
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


