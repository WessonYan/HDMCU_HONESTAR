/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
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
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
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
 ** \brief can tx rx Irq mode sample
 **
 **   - 2018-12-22  1.0  Lux First version
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SysClkConfig(void)
{
    stc_clk_sysclk_cfg_t    stcSysClkCfg;
    stc_clk_xtal_cfg_t      stcXtalCfg;
    stc_clk_mpll_cfg_t      stcMpllCfg;

    MEM_ZERO_STRUCT(stcSysClkCfg);
    MEM_ZERO_STRUCT(stcXtalCfg);
    MEM_ZERO_STRUCT(stcMpllCfg);

    /* Set bus clk div. */
    stcSysClkCfg.enHclkDiv  = ClkSysclkDiv1;  // Max 168MHz
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;  // Max 84MHz
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;  // Max 168MHz
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;  // Max 84MHz
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;  // Max 60MHz
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;  // Max 42MHz
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;  // Max 84MHz
    CLK_SysClkConfig(&stcSysClkCfg);

    /* Switch system clock source to MPLL. */
    /* Use Xtal as MPLL source. */
    stcXtalCfg.enMode        = ClkXtalModeOsc;
    stcXtalCfg.enDrv         = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);

    /* MPLL config. */
    stcMpllCfg.pllmDiv = 1;
    stcMpllCfg.plln =50;
    stcMpllCfg.PllpDiv = 4;
    stcMpllCfg.PllqDiv = 4;
    stcMpllCfg.PllrDiv = 4;
    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_MpllConfig(&stcMpllCfg);

    /* flash read wait cycle setting */
    EFM_Unlock();
    EFM_SetLatency(5);
    EFM_Lock();

    /* Enable MPLL. */
    CLK_MpllCmd(Enable);

    /* Wait MPLL ready. */
    while(Set != CLK_GetFlagStatus(ClkFlagMPLLRdy));

    /* Switch system clock source to MPLL. */
    CLK_SetSysClkSource(CLKSysSrcMPLL);

}
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
stc_can_rxframe_t       stcRxFrame;
uint8_t                 u8RxFlag = false;
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
void CAN_RxIrqCallBack(void)
{
    if(true == CAN_IrqFlgGet(CanRxIrqFlg))
    {
        CAN_IrqFlgClr(CanRxIrqFlg);
        CAN_IrqCmd(CanRxIrqEn, Disable);

        CAN_Receive(&stcRxFrame);

        u8RxFlag = true;
    }

}



/**
 *******************************************************************************
 ** \brief  Main function of can tx rx Irq mode project
 **
 ** \param  None
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{

    stc_pwc_ram_cfg_t       stcRamCfg;
    stc_can_init_config_t   stcCanInitCfg;
    stc_can_filter_t        stcFilter;
    stc_can_txframe_t       stcTxFrame;

    stc_irq_regi_conf_t     stcIrqRegiConf;

    uint8_t u8Idx = 0;

    MEM_ZERO_STRUCT(stcRamCfg);
    MEM_ZERO_STRUCT(stcCanInitCfg);
    MEM_ZERO_STRUCT(stcFilter);
    MEM_ZERO_STRUCT(stcTxFrame);
    MEM_ZERO_STRUCT(stcRxFrame);

    //<<System clk initial
    SysClkConfig();

    //<<Enable can peripheral clock and buffer(ram)
    stcRamCfg.enRamOpMd = HighSpeedMd;
    stcRamCfg.enCan = DynamicCtl;
    PWC_RamCfg(&stcRamCfg);
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_CAN, Enable);

    //<<CAN GPIO config
    PORT_SetFunc(PortB, Pin06, Func_Can1_Rx, Disable);
    PORT_SetFunc(PortB, Pin07, Func_Can1_Tx, Disable);
    PORT_ResetBits(PortD, Pin15);
    PORT_OE(PortD, Pin15, Enable);

    //<<Can bit time config
    stcCanInitCfg.stcCanBt.PRESC = 1-1;
    stcCanInitCfg.stcCanBt.SEG_1 = 5-2;
    stcCanInitCfg.stcCanBt.SEG_2 = 3-1;
    stcCanInitCfg.stcCanBt.SJW   = 3-1;

    stcCanInitCfg.stcWarningLimit.CanErrorWarningLimitVal = 10;
    stcCanInitCfg.stcWarningLimit.CanWarningLimitVal = 16-1;

    stcCanInitCfg.enCanRxBufAll  = CanRxNormal;
    stcCanInitCfg.enCanRxBufMode = CanRxBufNotStored;
    stcCanInitCfg.enCanSAck      = CanSelfAckEnable;
    stcCanInitCfg.enCanSTBMode   = CanSTBFifoMode;

    CAN_Init(&stcCanInitCfg);

    //<<Can filter config
    stcFilter.enAcfFormat = CanAllFrames;
    stcFilter.enFilterSel = CanFilterSel1;
    stcFilter.u32CODE     = 0x00000000;
    stcFilter.u32MASK     = 0x1FFFFFFF;
    CAN_FilterConfig(&stcFilter, Enable);


    //<<Can Irq Enable
    CAN_IrqCmd(CanRxIrqEn, Enable);

    stcIrqRegiConf.enIRQn = Int000_IRQn;
    stcIrqRegiConf.enIntSrc = INT_CAN_INT;
    stcIrqRegiConf.pfnCallback = CAN_RxIrqCallBack;
    enIrqRegistration(&stcIrqRegiConf);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);


    while(1)
    {
        if(true == u8RxFlag)
        {
            u8RxFlag = false;

            if(1 == stcRxFrame.Cst.Control_f.RTR)
            {
                continue;
            }

            //<<Can Tx
            stcTxFrame.StdID         = 0x123;
            stcTxFrame.Control_f.DLC = stcRxFrame.Cst.Control_f.DLC;
            stcTxFrame.Control_f.IDE = stcRxFrame.Cst.Control_f.IDE;

            for(u8Idx=0; u8Idx<stcRxFrame.Cst.Control_f.DLC; u8Idx++)
            {
                stcTxFrame.Data[u8Idx] = stcRxFrame.Data[u8Idx];
            }

            CAN_SetFrame(&stcTxFrame);
            CAN_TransmitCmd(CanPTBTxCmd);

            CAN_IrqCmd(CanRxIrqEn, Enable);
        }

    }

}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/