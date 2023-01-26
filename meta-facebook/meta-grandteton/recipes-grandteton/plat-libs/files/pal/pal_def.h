#ifndef __PAL_DEF_COMMON_H__
#define __PAL_DEF_COMMON_H__
#include <openbmc/misc-utils.h>
#include "pal_def_cover.h"

#define MAX_TXBUF_SIZE         (255)
#define MAX_RXBUF_SIZE         (255)

//GPIO EVENT Common
#define FM_UARTSW_LSB_N          "FM_UARTSW_LSB_N"
#define FM_UARTSW_MSB_N          "FM_UARTSW_MSB_N"
#define FM_POST_CARD_PRES_N      "FM_DEBUG_PORT_PRSNT_N_IN"
#define FM_CPU_CATERR_N          "H_CPU_CATERR_LVC2_R2_N"

//GPIO Power Control Common
#define FP_RST_BTN_IN_N          "ID_RST_BTN_BMC_IN"
#define FP_PWR_BTN_IN_N          "SYS_BMC_PWRBTN_IN"
#define FP_PWR_BTN_OUT_N         "SYS_BMC_PWRBTN_OUT"
#define FP_RST_BTN_OUT_N         "RST_BMC_RSTBTN_OUT_R_N"

//GPIO SWB
#define BIC_READY                "FM_SWB_BIC_READY_ISO_R_N"

//GPIO HGX
#define HMC_PRESENCE             "GPU_HMC_PRSNT_ISO_R_N"
#define GPU_FPGA_RST_N           "GPU_FPGA_RST_N"

// GPIO FAN_BP PRSNT
#define FAN_BP1_PRSNT      "FAN_BP1_PRSNT_N"
#define FAN_BP2_PRSNT      "FAN_BP2_PRSNT_N"

//GPIO FAN PRSNT
#define  FAN0_PRSNT        "FAN0_PRESENT"
#define  FAN1_PRSNT        "FAN1_PRESENT"
#define  FAN2_PRSNT        "FAN2_PRESENT"
#define  FAN3_PRSNT        "FAN3_PRESENT"
#define  FAN4_PRSNT        "FAN4_PRESENT"
#define  FAN5_PRSNT        "FAN5_PRESENT"
#define  FAN6_PRSNT        "FAN6_PRESENT"
#define  FAN7_PRSNT        "FAN7_PRESENT"
#define  FAN8_PRSNT        "FAN8_PRESENT"
#define  FAN9_PRSNT        "FAN9_PRESENT"
#define  FAN10_PRSNT       "FAN10_PRESENT"
#define  FAN11_PRSNT       "FAN11_PRESENT"
#define  FAN12_PRSNT       "FAN12_PRESENT"
#define  FAN13_PRSNT       "FAN13_PRESENT"
#define  FAN14_PRSNT       "FAN14_PRESENT"
#define  FAN15_PRSNT       "FAN15_PRESENT"

//GPIO SWB CABLE PRESENT
#define CABLE_PRSNT_G     "SWB_CABLE_PRSNT_G_N"
#define CABLE_PRSNT_F     "SWB_CABLE_PRSNT_F_N"
#define CABLE_PRSNT_C     "SWB_CABLE_PRSNT_C_N"
#define CABLE_PRSNT_B     "SWB_CABLE_PRSNT_B_N"

//GPIO GPU CABLE PRESENT
#define CABLE_PRSNT_A     "GPU_CABLE_PRSNT_A_N"
#define CABLE_PRSNT_D     "GPU_CABLE_PRSNT_D_N"
#define CABLE_PRSNT_E     "GPU_CABLE_PRSNT_E_N"
#define CABLE_PRSNT_H     "GPU_CABLE_PRSNT_H_N"

//GPIO PDB BUSBAR PRESENT
#define FM_HS1_EN_BUSBAR_BUF  "FM_HS1_EN_BUSBAR_BUF"
#define FM_HS2_EN_BUSBAR_BUF  "FM_HS2_EN_BUSBAR_BUF"

//AC POWER BUTTON
#define FP_AC_PWR_BMC_BTN  "AC_PWR_BMC_BTN_N"


#define BMC_DEF_SLAVE_ADDR (0x10)

#define PWR_OPTION_LIST "status, graceful-shutdown, off, on, reset, cycle"

#define FRU_EEPROM "/sys/class/i2c-dev/i2c-%d/device/%d-00%x/eeprom"
#define MAX_NUM_FRUS    (FRU_CNT-1)
#define MAX_NODES       (2)

#define LARGEST_DEVICE_NAME (120)
#define UNIT_DIV            (1000)
#define ERR_NOT_READY       (-2)

#define PAGE_SIZE  0x1000
#define PREPARE_FW_UPDATE_CLEAR (0x00)
#define PREPARE_FW_UPDATE_SET   (0x01)

#define READING_SKIP    (1)
#define READING_NA      (-2)

//NM Device Info
#define NM_IPMB_BUS_ID             (I2C_BUS_6)
#define NM_SLAVE_ADDR              (0x2C)

//SWB Device Info
#define SWB_BIC_EID    (0x0A)
#define SWB_BUS_ID     (3)
#define SWB_UART_ID    (5)

#define MAX_CPU_CNT    (2)

/* Artemis */
// ACB Device Info
#define ACB_BIC_BUS 3
#define ACB_BIC_ADDR 0x20
#define ACB_BIC_EID 0x0A

// MEB Device Info
#define MEB_BIC_BUS 9
#define MEB_BIC_ADDR 0x20
#define MEB_BIC_EID 0x0A

#define ACB_VR_BUS_ID   (1)
#define CXL_VR_BUS_ID   (2)

#define CXL_VR_NUM      (3)

// JCN1,2,3,4,9,10,11,12 can plug CXL or E1.S
// JCN5,6,7,8 plugs E1.S
#define MAX_CXL_NUM 8

typedef struct {
  uint8_t fru_prsnt;
  uint8_t fru_type;
} fru_status;
/* Artemis End */

#define GPION_COMMON_SOURCE_OFFSET   (0xE0)
#define AST_GPIO_BASE                (0x1e780000)

enum {
  DIMM_ID0 = 0,
  DIMM_ID1,
  DIMM_ID2,
  DIMM_ID3,
  DIMM_ID4,
  DIMM_ID5,
  DIMM_ID6,
  DIMM_ID7,
  DIMM_ID8,
  DIMM_ID9,
  DIMM_ID10,
  DIMM_ID11,
  DIMM_ID12,
  DIMM_ID13,
  DIMM_ID14,
  DIMM_ID15,
  DIMM_ID16,
  DIMM_ID17,
  DIMM_ID18,
  DIMM_ID19,
  DIMM_ID20,
  DIMM_ID21,
  DIMM_ID22,
  DIMM_ID23,
  DIMM_ID24,
  DIMM_ID25,
  DIMM_ID26,
  DIMM_ID27,
  DIMM_ID28,
  DIMM_ID29,
  DIMM_ID30,
  DIMM_ID31,
  MAX_DIMM_NUM,
};

enum {
  DIMM_CPU0_CRPA = 0,
  DIMM_CPU0_CRPB,
  DIMM_CPU0_CRPC,
  DIMM_CPU0_CRPD,
  DIMM_CPU0_CRPE,
  DIMM_CPU0_CRPF,
  DIMM_CPU0_CRPG,
  DIMM_CPU0_CRPH,
  DIMM_CPU1_CRPA,
  DIMM_CPU1_CRPB,
  DIMM_CPU1_CRPC,
  DIMM_CPU1_CRPD,
  DIMM_CPU1_CRPE,
  DIMM_CPU1_CRPF,
  DIMM_CPU1_CRPG,
  DIMM_CPU1_CRPH,
  MAX_DIMM_CHANNEL,
};

#define PER_CPU_DIMM_NUMBER_MAX         (MAX_DIMM_NUM/MAX_CPU_CNT)
#define PER_CPU_DIMM_CHANNEL_MAX        (MAX_DIMM_CHANNEL/MAX_CPU_CNT)


enum {
  VR0_COMP,
  VR1_COMP,
  BIC_COMP,
  PEX0_COMP,
  PEX1_COMP,
  PEX2_COMP,
  PEX3_COMP,
  CPLD_COMP,
  NIC0_COMP,
  NIC1_COMP,
  NIC2_COMP,
  NIC3_COMP,
  NIC4_COMP,
  NIC5_COMP,
  NIC6_COMP,
  NIC7_COMP,
};

enum {
  FRU_ALL = 0,
  FRU_MB,
  FRU_SWB,
  FRU_HGX,
  FRU_NIC0,
  FRU_NIC1,
  FRU_OCPDBG,
  FRU_BMC,
  FRU_SCM,
  FRU_VPDB,
  FRU_HPDB,
  FRU_FAN_BP1,
  FRU_FAN_BP2,
  FRU_FIO,
  FRU_HSC,
  FRU_SHSC,
  // Artemis FRU ID
  FRU_ACB,
  FRU_MEB,
  FRU_ACB_ACCL1,
  FRU_ACB_ACCL2,
  FRU_ACB_ACCL3,
  FRU_ACB_ACCL4,
  FRU_ACB_ACCL5,
  FRU_ACB_ACCL6,
  FRU_ACB_ACCL7,
  FRU_ACB_ACCL8,
  FRU_ACB_ACCL9,
  FRU_ACB_ACCL10,
  FRU_ACB_ACCL11,
  FRU_ACB_ACCL12,
  // Artemis MEB CXL/NIC/E1.S board
  FRU_MEB_JCN1,
  FRU_MEB_JCN2,
  FRU_MEB_JCN3,
  FRU_MEB_JCN4,
  FRU_MEB_JCN5,
  FRU_MEB_JCN6,
  FRU_MEB_JCN7,
  FRU_MEB_JCN8,
  FRU_MEB_JCN9,
  FRU_MEB_JCN10,
  FRU_MEB_JCN11,
  FRU_MEB_JCN12,
  FRU_MEB_JCN13,
  FRU_MEB_JCN14,
  FRU_CNT,
};

enum {
  FRU_MEB_JCN_CNT = 14,
};

enum {
  REV_PO = 0,
  REV_EVT,
  REV_DVT,
  REV_PVT,
  REV_MP,
};

// Sensors Under Side Plane
enum {
  MB_SENSOR_TBD,
};

enum{
  MEZZ_SENSOR_TBD,
};

enum {
  BOOT_DEVICE_USB      = 0x0,
  BOOT_DEVICE_IPV4     = 0x1,
  BOOT_DEVICE_HDD      = 0x2,
  BOOT_DEVICE_CDROM    = 0x3,
  BOOT_DEVICE_OTHERS   = 0x4,
  BOOT_DEVICE_IPV6     = 0x9,
  BOOT_DEVICE_RESERVED = 0xff,
};

enum {
  IPMI_CHANNEL_0 = 0,
  IPMI_CHANNEL_1,
  IPMI_CHANNEL_2,
  IPMI_CHANNEL_3,
  IPMI_CHANNEL_4,
  IPMI_CHANNEL_5,
  IPMI_CHANNEL_6,
  IPMI_CHANNEL_7,
  IPMI_CHANNEL_8,
  IPMI_CHANNEL_9,
  IPMI_CHANNEL_A,
  IPMI_CHANNEL_B,
  IPMI_CHANNEL_C,
  IPMI_CHANNEL_D,
  IPMI_CHANNEL_E,
  IPMI_CHANNEL_F,
};

enum {
  I2C_BUS_0 = 0,
  I2C_BUS_1,
  I2C_BUS_2,
  I2C_BUS_3,
  I2C_BUS_4,
  I2C_BUS_5,
  I2C_BUS_6,
  I2C_BUS_7,
  I2C_BUS_8,
  I2C_BUS_9,
  I2C_BUS_10,
  I2C_BUS_11,
  I2C_BUS_12,
  I2C_BUS_13,
  I2C_BUS_14,
  I2C_BUS_15,
  I2C_BUS_16,
  I2C_BUS_17,
  I2C_BUS_18,
  I2C_BUS_19,
  I2C_BUS_20,
  I2C_BUS_21,
  I2C_BUS_22,
  I2C_BUS_23,
  I2C_BUS_24,
  I2C_BUS_25,
  I2C_BUS_26,
  I2C_BUS_27,
  I2C_BUS_28,
  I2C_BUS_29,
  I2C_BUS_30,
  I2C_BUS_31,
  I2C_BUS_32,
  I2C_BUS_33,
  I2C_BUS_34,
  I2C_BUS_35,
  I2C_BUS_36,
  I2C_BUS_37,
  I2C_BUS_38,
  I2C_BUS_39,
  I2C_BUS_40,
  I2C_BUS_41,
  I2C_BUS_42,
  I2C_BUS_43,
  I2C_BUS_44,
  I2C_BUS_45,
  I2C_BUS_46,
  I2C_BUS_47,
  I2C_BUS_48,
  I2C_BUS_49,
  I2C_BUS_50,
  I2C_BUS_51,
  I2C_BUS_52,
  I2C_BUS_53,
  I2C_BUS_54,
  I2C_BUS_55,
  I2C_BUS_56,
  I2C_BUS_57,
  I2C_BUS_58,
  I2C_BUS_59,
  I2C_BUS_60,
};

enum {
  POSTCODE_BY_BMC,
  POSTCODE_BY_HOST,
};

enum {
  CC_OEM_DEVICE_NOT_PRESENT = 0x30,
  CC_OEM_DEVICE_INFO_ERR = 0x31,
  CC_OEM_DEVICE_DESTINATION_ERR = 0x32,
  CC_OEM_DEVICE_SEND_SLAVE_RESTORE_POWER_POLICY_FAIL =0x33,
  CC_OEM_GET_SELF_ADDRESS_ERR = 0x34,
  CC_OEM_ONLY_SUPPORT_MASTER = 0x35,
};

enum PLDM_FRU_ID {
  PLDM_FRU_NOT_SUPPORT = -1,
  PLDM_FRU_SWB,
  PLDM_FRU_MEB,
  PLDM_FRU_FIO,
  PLDM_FRU_SHSC,
};

enum FRU_PRESENT {
  FRU_NOT_PRSNT,
  FRU_PRSNT,
};

enum FRU_PATH_TYPE {
  FRU_PATH_NONE,
  FRU_PATH_EEPROM,
  FRU_PATH_PLDM,
};

/* Artemis */
enum ARTEMIS_OEM_COMMAND {
  CMD_OEM_1S_GET_ASIC_CARD_STATUS = 0x76,
  CMD_OEM_1S_GET_SENSOR_READING = 0x77,
};

// JCN can plug dual or single devices
enum MEB_JCN_LOCATION {
  JCN_0_1 = 0, // dual device on location 0 and 1
  JCN_0, // single device on location 0
  JCN_1, // single device on location 1
};

enum MEB_JCN_TYPE {
  E1S_CARD,
  E1S_0_CARD,
  E1S_1_CARD,
  E1S_0_1_CARD,
  NIC_CARD,
  CXL_CARD,
  UNKNOWN_CARD = 0xff,
};

/* Artemis End */

#endif
