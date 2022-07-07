/* Copyright 2020-present Facebook. All Rights Reserved.
 *
 * This file contains code to support IPMI2.0 Specificaton available @
 * http://www.intel.com/content/www/us/en/servers/ipmi/ipmi-specifications.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <syslog.h>
#include <facebook/fby35_common.h>
#include "fby35_gpio.h"


//Crater lake BIC GPIO
const char *cl_gpio_pin_name[] = {
  "FM_BMC_PCH_SCI_LPC_R_N",         //0
  "FM_BIOS_POST_CMPLT_BMC_N",
  "FM_SLPS3_PLD_N",
  "IRQ_BMC_PCH_SMI_LPC_R_N",
  "IRQ_UV_DETECT_N",
  "FM_UV_ADR_TRIGGER_EN_R",
  "IRQ_SMI_ACTIVE_BMC_N",
  "HSC_SET_EN_R",
  "FM_BIC_RST_RTCRST_R",
  "RST_USB_HUB_N_R",
  "A_P3V_BAT_SCALED_EN_R",          //10
  "FM_SPI_PCH_MASTER_SEL_R",
  "FM_PCHHOT_N",
  "FM_SLPS4_PLD_N",
  "FM_S3M_CPU0_CD_INIT_ERROR",
  "PWRGD_SYS_PWROK",
  "FM_HSC_TIMER",
  "IRQ_SMB_IO_LVC3_STBY_ALRT_N",
  "IRQ_CPU0_VRHOT_N",
  "DBP_CPU_PREQ_BIC_N",
  "FM_CPU_THERMTRIP_LATCH_LVT3_N",  //20
  "FM_CPU_SKTOCC_LVT3_PLD_N",
  "H_CPU_MEMHOT_OUT_LVC3_N",
  "RST_PLTRST_PLD_N",
  "PWRBTN_N",
  "RST_BMC_R_N",
  "H_BMC_PRDY_BUF_N",
  "BMC_READY",
  "BIC_READY",
  "FM_RMCA_LVT3_N",
  "HSC_MUX_SWITCH_R",               //30
  "FM_FORCE_ADR_N_R",
  "PWRGD_CPU_LVC3",
  "FM_PCH_BMC_THERMTRIP_N",
  "FM_THROTTLE_R_N",
  "IRQ_HSC_ALERT2_N",
  "SMB_SENSOR_LVC3_ALERT_N",
  "FM_CATERR_LVT3_N",
  "SYS_PWRBTN_N",
  "RST_PLTRST_BUF_N",
  "IRQ_BMC_PCH_NMI_R",              //40
  "IRQ_SML1_PMBUS_ALERT_N",
  "IRQ_PCH_CPU_NMI_EVENT_N",
  "FM_BMC_DEBUG_ENABLE_N",
  "FM_DBP_PRESENT_N",
  "FM_FAST_PROCHOT_EN_N_R",
  "FM_SPI_MUX_OE_CTL_PLD_N",
  "FBRK_N_R",
  "FM_PEHPCPU_INT",
  "FM_BIOS_MRC_DEBUG_MSG_DIS_R",
  "FAST_PROCHOT_N",                 //50
  "FM_JTAG_TCK_MUX_SEL_R",
  "BMC_JTAG_SEL_R",
  "H_CPU_ERR0_LVC3_N",
  "H_CPU_ERR1_LVC3_N",
  "H_CPU_ERR2_LVC3_N",
  "RST_RSMRST_BMC_N",
  "FM_MP_PS_FAIL_N",
  "H_CPU_MEMTRIP_LVC3_N",
  "FM_CPU_BIC_PROCHOT_LVT3_N",
  "BOARD_ID2",                      //60
  "IRQ_PVCCD_CPU0_VRHOT_LVC3_N",
  "FM_PVCCIN_CPU0_PWR_IN_ALERT_N",
  "BOARD_ID0",
  "BOARD_ID1",
  "BOARD_ID3",
  "FM_THROTTLE_IN_N",
  "AUTH_COMPLETE",
  "AUTH_PRSNT_N",
  "SGPIO_BMC_CLK_R",
  "SGPIO_BMC_LD_R_N",               //70
  "SGPIO_BMC_DOUT_R",
  "SGPIO_BMC_DIN",
};

//Halfdome BIC GPIO
const char *hd_gpio_pin_name[] = {
  "HD_FM_BIOS_POST_CMPLT_BIC_N",    //0
  "HD_FM_CPU_BIC_SLP_S3_N",
  "HD_APML_CPU_ALERT_BIC_N",
  "HD_IRQ_UV_DETECT_N",
  "HD_PVDDCR_CPU0_BIC_OCP_N",
  "HD_HSC_OCP_GPIO1_R",
  "HD_PVDDCR_CPU1_BIC_OCP_N",
  "HD_RST_USB_HUB_R_N",
  "HD_P3V_BAT_SCALED_EN_R",
  "HD_HDT_BIC_TRST_R_N",
  "HD_FM_CPU_BIC_SLP_S5_N",      //10
  "HD_PVDD11_S3_BIC_OCP_N",
  "HD_FM_HSC_TIMER",
  "HD_IRQ_SMB_IO_LVC3_STBY_ALRT_N",
  "HD_PVDDCR_CPU1_PMALERT_N",
  "HD_FM_CPU_BIC_THERMTRIP_N",
  "HD_FM_PRSNT_CPU_BIC_N",
  "HD_AUTH_PRSNT_BIC_N",
  "HD_RST_CPU_RESET_BIC_N",
  "HD_PWRBTN_R1_N",
  "HD_RST_BMC_R_N",                    //20
  "HD_HDT_BIC_DBREQ_R_N",
  "HD_BMC_READY",
  "HD_BIC_READY",
  "HD_FM_SOL_UART_CH_SEL_R",
  "HD_PWRGD_CPU_LVC3",
  "HD_CPU_ERROR_BIC_LVC3_R_N",
  "HD_PVDD11_S3_PMALERT_N",
  "HD_IRQ_HSC_ALERT1_N",
  "HD_SMB_SENSOR_LVC3_ALERT_N",
  "HD_SYS_PWRBTN_BIC_N",        //30
  "HD_RST_PLTRST_BIC_N",
  "HD_CPU_SMERR_BIC_N",
  "HD_IRQ_HSC_ALERT2_N",
  "HD_FM_BMC_DEBUG_ENABLE_N",
  "HD_FM_DBP_PRESENT_N",
  "HD_FM_FAST_PROCHOT_EN_R_N",
  "HD_FM_BIOS_MRC_DEBUG_MSG_DIS",
  "HD_FAST_PROCHOT_N",
  "HD_BIC_JTAG_SEL_R",
  "HD_HSC_OCP_GPIO2_R",        //40
  "HD_HSC_OCP_GPIO3_R",
  "HD_RST_RSMRST_BMC_N",
  "HD_FM_CPU_BIC_PROCHOT_LVT3_N",
  "HD_BIC_JTAG_MUX_SEL",
  "HD_BOARD_ID2",
  "HD_PVDDCR_CPU0_PMALERT_N",
  "HD_BOARD_ID0",
  "HD_BOARD_ID1",
  "HD_BOARD_ID3",
  "HD_BOARD_ID5",                    //50
  "HD_BOARD_ID4",
  "HD_HSC_TYPE_0",
  "HD_HSC_TYPE_1",
};

/* GPIO Expander gpio table */
gpio_cfg gpio_expander_gpio_table[] = {
  /* shadow_name, pin_name, direction, value */
  // LAST
  {NULL, NULL, GPIO_DIRECTION_INVALID, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2}
};

/* BMC gpio table */
gpio_cfg bmc_gpio_table[] = {
  /* shadow_name, pin_name, direction, value */

  [GPIO_PWRGD_NIC_BMC] =  {"PWRGD_NIC_BMC", "GPIOB0", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_OCP_DEBUG_BMC_PRSNT_N] = {"OCP_DEBUG_BMC_PRSNT_N", "GPIOB1", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_PWROK_STBY_BMC_SLOT1_R] = {"PWROK_STBY_BMC_SLOT1_R", "GPIOB2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_PWROK_STBY_BMC_SLOT2] = {"PWROK_STBY_BMC_SLOT2", "GPIOB3", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_PWROK_STBY_BMC_SLOT3_R] = {"PWROK_STBY_BMC_SLOT3_R", "GPIOB4", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_PWROK_STBY_BMC_SLOT4] = {"PWROK_STBY_BMC_SLOT4", "GPIOB5", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},

  [GPIO_OCP_NIC_PRSNT_BMC_N] = {"OCP_NIC_PRSNT_BMC_N", "GPIOC0", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_FM_NIC_WAKE_BMC_N] = {"FM_NIC_WAKE_BMC_N", "GPIOC1", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_FM_PWRBRK_PRIMARY_R] = {"FM_PWRBRK_PRIMARY_R", "GPIOC2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_SMB_RST_PRIMARY_BMC_N_R] = {"SMB_RST_PRIMARY_BMC_N_R", "GPIOC3", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_SMB_RST_SECONDARY_BMC_N_R] = {"SMB_RST_SECONDARY_BMC_N_R", "GPIOC4", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_USB_MUX_CB_R] = {"USB_MUX_CB_R", "GPIOC5", GPIO_DIRECTION_OUT, GPIO_VALUE_LOW, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},

  [GPIO_FM_RESBTN_SLOT1_BMC_N] = {"FM_RESBTN_SLOT1_BMC_N", "GPIOF0", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_FM_RESBTN_SLOT2_N] = {"FM_RESBTN_SLOT2_N", "GPIOF1", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_FM_RESBTN_SLOT3_BMC_N] = {"FM_RESBTN_SLOT3_BMC_N", "GPIOF2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_FM_RESBTN_SLOT4_N] = {"FM_RESBTN_SLOT4_N", "GPIOF3", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SMB_TEMP_ALERT_BMC_N_R] = {"SMB_TEMP_ALERT_BMC_N_R", "GPIOF4", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_FM_DEBUG_UART_MUX_BMC_R] = {"FM_DEBUG_UART_MUX_BMC_R", "GPIOF5", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SMB_HOTSWAP_BMC_ALERT_N_R] = {"SMB_HOTSWAP_BMC_ALERT_N_R", "GPIOF6", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_EMMC_PRESENT_N] = {"EMMC_PRESENT_N", "GPIOF7", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
   
  [GPIO_SMB_BMC_SLOT1_ALT_R_N] = {"SMB_BMC_SLOT1_ALT_R_N", "GPIOH0", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_SMB_BMC_SLOT2_ALT_R_N] = {"SMB_BMC_SLOT2_ALT_R_N", "GPIOH1", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SMB_BMC_SLOT3_ALT_R_N] = {"SMB_BMC_SLOT3_ALT_R_N", "GPIOH2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SMB_BMC_SLOT4_ALT_R_N] = {"SMB_BMC_SLOT4_ALT_R_N", "GPIOH3", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_PRSNT_MB_BMC_SLOT1_BB_N] = {"PRSNT_MB_BMC_SLOT1_BB_N", "GPIOH4", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_PRSNT_MB_SLOT2_BB_N] = {"PRSNT_MB_SLOT2_BB_N", "GPIOH5", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_PRSNT_MB_BMC_SLOT3_BB_N] = {"PRSNT_MB_BMC_SLOT3_BB_N", "GPIOH6", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_PRSNT_MB_SLOT4_BB_N] = {"PRSNT_MB_SLOT4_BB_N", "GPIOH7", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  
  [GPIO_FM_SPI_WP_DISABLE_STATUS_R_N] = {"FM_SPI_WP_DISABLE_STATUS_R_N", "GPIOI5", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_BB_BUTTON_BMC_CO_N_R] = {"BB_BUTTON_BMC_CO_N_R", "GPIOI6", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_FM_PWRBRK_SECONDARY_R] = {"FM_PWRBRK_SECONDARY_R", "GPIOI7", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
   
  [GPIO_P3V3_NIC_FAULT_N] = {"P3V3_NIC_FAULT_N", "GPIOM0", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_NIC_POWER_BMC_EN_R] = {"NIC_POWER_BMC_EN_R", "GPIOM1", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_P12V_NIC_FAULT_N] = {"P12V_NIC_FAULT_N", "GPIOM2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_USB_BMC_EN_R] = {"USB_BMC_EN_R", "GPIOM3", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1},
  [GPIO_FAST_PROCHOT_BMC_N_R] = {"FAST_PROCHOT_BMC_N_R", "GPIOM4", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_RST_BMC_USB_HUB_N_R] = {"RST_BMC_USB_HUB_N_R", "GPIOM5", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1},
   
  [GPIO_HSC_FAULT_BMC_SLOT1_N_R] = {"HSC_FAULT_BMC_SLOT1_N_R", "GPION0", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_HSC_FAULT_SLOT2_N] = {"HSC_FAULT_SLOT2_N", "GPION1", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_HSC_FAULT_BMC_SLOT3_N_R] = {"HSC_FAULT_BMC_SLOT3_N_R", "GPION2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_HSC_FAULT_SLOT4_N] = {"HSC_FAULT_SLOT4_N", "GPION3", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_BMC_READY_R] = {"BMC_READY_R", "GPION4", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_FM_HSC_BMC_FAULT_N_R] = {"FM_HSC_BMC_FAULT_N_R", "GPION5", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
   
  [GPIO_DUAL_FAN0_DETECT_BMC_N_R] = {"DUAL_FAN0_DETECT_BMC_N_R", "GPIOO4", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_DUAL_FAN1_DETECT_BMC_N_R] = {"DUAL_FAN1_DETECT_BMC_N_R", "GPIOO5", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_FAN0_BMC_CPLD_EN_R] = {"FAN0_BMC_CPLD_EN_R", "GPIOO6", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1},
  [GPIO_FAN1_BMC_CPLD_EN_R] = {"FAN1_BMC_CPLD_EN_R", "GPIOO7", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1},
   
  [GPIO_FAN2_BMC_CPLD_EN_R] = {"FAN2_BMC_CPLD_EN_R", "GPIOP0", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1},
  [GPIO_FAN3_BMC_CPLD_EN_R] = {"FAN3_BMC_CPLD_EN_R", "GPIOP1", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1},
  [GPIO_RST_PCIE_RESET_SLOT1_N] = {"RST_PCIE_RESET_SLOT1_N", "GPIOP2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_RST_PCIE_RESET_SLOT2_N] = {"RST_PCIE_RESET_SLOT2_N", "GPIOP3", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_RST_PCIE_RESET_SLOT3_N] = {"RST_PCIE_RESET_SLOT3_N", "GPIOP4", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_RST_PCIE_RESET_SLOT4_N] = {"RST_PCIE_RESET_SLOT4_N", "GPIOP5", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_USB_MUX_EN_BMC_N_R] = {"USB_MUX_EN_BMC_N_R", "GPIOP6", GPIO_DIRECTION_OUT, GPIO_VALUE_LOW, SYS_CFG_CLASS1},
   
  [GPIO_SLOT1_ID1_DETECT_BMC_N] = {"SLOT1_ID1_DETECT_BMC_N", "GPIOR0", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SLOT1_ID0_DETECT_BMC_N] = {"SLOT1_ID0_DETECT_BMC_N", "GPIOR1", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SLOT2_ID1_DETECT_BMC_N] = {"SLOT2_ID1_DETECT_BMC_N", "GPIOR2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SLOT2_ID0_DETECT_BMC_N] = {"SLOT2_ID0_DETECT_BMC_N", "GPIOR3", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SLOT3_ID1_DETECT_BMC_N] = {"SLOT3_ID1_DETECT_BMC_N", "GPIOR4", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SLOT3_ID0_DETECT_BMC_N] = {"SLOT3_ID0_DETECT_BMC_N", "GPIOR5", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SLOT4_ID1_DETECT_BMC_N] = {"SLOT4_ID1_DETECT_BMC_N", "GPIOR6", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_SLOT4_ID0_DETECT_BMC_N] = {"SLOT4_ID0_DETECT_BMC_N", "GPIOR7", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
   
  [GPIO_P5V_USB_PG_BMC] = {"P5V_USB_PG_BMC", "GPIOS2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_FM_BMC_TPM_PRSNT_N] = {"FM_BMC_TPM_PRSNT_N", "GPIOS3", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_FM_BMC_SLOT1_ISOLATED_EN_R] = {"FM_BMC_SLOT1_ISOLATED_EN_R", "GPIOS4", GPIO_DIRECTION_OUT, GPIO_VALUE_LOW, SYS_CFG_CLASS1},
  [GPIO_FM_SMB_ISOLATED_EN_R] = {"FM_SMB_ISOLATED_EN_R", "GPIOS4", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS2},
  [GPIO_FM_BMC_SLOT2_ISOLATED_EN_R] = {"FM_BMC_SLOT2_ISOLATED_EN_R", "GPIOS5", GPIO_DIRECTION_OUT, GPIO_VALUE_LOW, SYS_CFG_CLASS1},
  [GPIO_FM_BMC_SLOT3_ISOLATED_EN_R] = {"FM_BMC_SLOT3_ISOLATED_EN_R", "GPIOS6", GPIO_DIRECTION_OUT, GPIO_VALUE_LOW, SYS_CFG_CLASS1},
  [GPIO_FM_BMC_SLOT4_ISOLATED_EN_R] = {"FM_BMC_SLOT4_ISOLATED_EN_R", "GPIOS7", GPIO_DIRECTION_OUT, GPIO_VALUE_LOW, SYS_CFG_CLASS1},

  [GPIO_P12V_EFUSE_DETECT_N] = {"P12V_EFUSE_DETECT_N", "GPIOU1", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
   
  [GPIO_AC_ON_OFF_BTN_BMC_SLOT1_N_R] = {"AC_ON_OFF_BTN_BMC_SLOT1_N_R", "GPIOV0", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_AC_ON_OFF_BTN_SLOT2_N] = {"AC_ON_OFF_BTN_SLOT2_N", "GPIOV1", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_AC_ON_OFF_BTN_BMC_SLOT3_N_R] = {"AC_ON_OFF_BTN_BMC_SLOT3_N_R", "GPIOV2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_AC_ON_OFF_BTN_SLOT4_N] = {"AC_ON_OFF_BTN_SLOT4_N", "GPIOV3", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},
  [GPIO_BOARD_ID0] = {"BOARD_ID0", "GPIOV4", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_BOARD_ID1] = {"BOARD_ID1", "GPIOV5", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_BOARD_ID2] = {"BOARD_ID2", "GPIOV6", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_BOARD_ID3] = {"BOARD_ID3", "GPIOV7", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},

  [GPIO_HSC_BB_BMC_DETECT0] = {"HSC_BB_BMC_DETECT0", "GPIOX2", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},

  // [GPIO_RST_BMC_WDRST1_R] = {"RST_BMC_WDRST1_R", "GPIOY0", GPIO_DIRECTION_OUT, GPIO_VALUE_LOW, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_RST_BMC_WDRST2_R] = {"RST_BMC_WDRST2_R", "GPIOY1", GPIO_DIRECTION_OUT, GPIO_VALUE_LOW, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_SPI_LOCK_REQ_BMC_N] = {"SPI_LOCK_REQ_BMC_N", "GPIOY2", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  [GPIO_EMMC_RST_N_R] = {"EMMC_RST_N_R", "GPIOY3", GPIO_DIRECTION_OUT, GPIO_VALUE_HIGH, SYS_CFG_CLASS1 | SYS_CFG_CLASS2},
  
  [GPIO_HSC_BB_BMC_DETECT1] = {"HSC_BB_BMC_DETECT1", "GPIOZ0", GPIO_DIRECTION_IN, GPIO_VALUE_INVALID, SYS_CFG_CLASS1},  
  
  // LAST
  {NULL, NULL, GPIO_DIRECTION_INVALID, GPIO_VALUE_INVALID, SYS_CFG_CLASS1 | SYS_CFG_CLASS2}
};

const uint8_t cl_gpio_pin_size = sizeof(cl_gpio_pin_name)/sizeof(cl_gpio_pin_name[0]);
const uint8_t hd_gpio_pin_size = sizeof(hd_gpio_pin_name)/sizeof(hd_gpio_pin_name[0]);

uint8_t
y35_get_gpio_list_size(uint8_t fru) {
  
  if (fby35_common_get_slot_type(fru) == SERVER_TYPE_HD) {
    return  hd_gpio_pin_size;
  }
  return cl_gpio_pin_size;
}

int
y35_get_gpio_name(uint8_t fru, uint8_t gpio, char *name) {
  uint8_t gpio_pin_size = cl_gpio_pin_size;
  const char **gpio_pin_name = cl_gpio_pin_name;

  //TODO: Add support for BMC GPIO pins
  if (fru < 1 || fru > 4) {
#ifdef DEBUG
    syslog(LOG_WARNING, "y35_get_gpio_name: Wrong fru %u", fru);
#endif
    return -1;
  }

  if (fby35_common_get_slot_type(fru) == SERVER_TYPE_HD) {
    gpio_pin_size = hd_gpio_pin_size;
    gpio_pin_name = hd_gpio_pin_name;
  }

  if (gpio < 0 || gpio > gpio_pin_size) {
#ifdef DEBUG
    syslog(LOG_WARNING, "y35_get_gpio_name: Wrong gpio pin %u", gpio);
#endif
    return -1;
  }

  sprintf(name, "%s", gpio_pin_name[gpio]);
  return 0;
}
