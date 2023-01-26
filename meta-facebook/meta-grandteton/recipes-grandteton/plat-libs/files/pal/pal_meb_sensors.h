#ifndef __PAL_MEB_SENSORS_H__
#define __PAL_MEB_SENSORS_H__

#include "pal_sensors.h"
#include <libpldm/base.h>
#include <libpldm/platform.h>

#define JCN_CONFIG_STR_CXL "cxl"
#define JCN_CONFIG_STR_E1S "e1s"
#define JCN_CONFIG_STR_NIC "nic"

// MEB sensor table
enum {
  MEB_SENSOR_INLET_TEMP = 1,
  MEB_SENSOR_OUTLET_TEMP,
  MEB_SENSOR_PU4_TEMP,
  MEB_SENSOR_P12V_AUX_VOL,
  MEB_SENSOR_P3V3_AUX_VOL,
  MEB_SENSOR_P1V2_AUX_VOL,
  MEB_SENSOR_P3V3_VOL,
  MEB_SENSOR_P12V_AUX_CARD1_VOL,
  MEB_SENSOR_P12V_AUX_CARD2_VOL,
  MEB_SENSOR_P12V_AUX_CARD3_VOL,
  MEB_SENSOR_P12V_AUX_CARD4_VOL,
  MEB_SENSOR_P12V_AUX_CARD5_VOL,
  MEB_SENSOR_P12V_AUX_CARD6_VOL,
  MEB_SENSOR_P12V_AUX_CARD7_VOL,
  MEB_SENSOR_P12V_AUX_CARD8_VOL,
  MEB_SENSOR_P12V_AUX_CARD9_VOL,
  MEB_SENSOR_P12V_AUX_CARD10_VOL,
  MEB_SENSOR_P12V_AUX_CARD11_VOL,
  MEB_SENSOR_P12V_AUX_CARD12_VOL,
  MEB_SENSOR_P12V_AUX_CARD13_VOL,
  MEB_SENSOR_P12V_AUX_CARD14_VOL,
  MEB_SENSOR_P12V_AUX_CUR,
  MEB_SENSOR_P12V_AUX_CARD1_CUR,
  MEB_SENSOR_P12V_AUX_CARD2_CUR,
  MEB_SENSOR_P12V_AUX_CARD3_CUR,
  MEB_SENSOR_P12V_AUX_CARD4_CUR,
  MEB_SENSOR_P12V_AUX_CARD5_CUR,
  MEB_SENSOR_P12V_AUX_CARD6_CUR,
  MEB_SENSOR_P12V_AUX_CARD7_CUR,
  MEB_SENSOR_P12V_AUX_CARD8_CUR,
  MEB_SENSOR_P12V_AUX_CARD9_CUR,
  MEB_SENSOR_P12V_AUX_CARD10_CUR,
  MEB_SENSOR_P12V_AUX_CARD11_CUR,
  MEB_SENSOR_P12V_AUX_CARD12_CUR,
  MEB_SENSOR_P12V_AUX_CARD13_CUR,
  MEB_SENSOR_P12V_AUX_CARD14_CUR,
  MEB_SENSOR_P12V_AUX_PWR,
  MEB_SENSOR_P12V_AUX_CARD1_PWR,
  MEB_SENSOR_P12V_AUX_CARD2_PWR,
  MEB_SENSOR_P12V_AUX_CARD3_PWR,
  MEB_SENSOR_P12V_AUX_CARD4_PWR,
  MEB_SENSOR_P12V_AUX_CARD5_PWR,
  MEB_SENSOR_P12V_AUX_CARD6_PWR,
  MEB_SENSOR_P12V_AUX_CARD7_PWR,
  MEB_SENSOR_P12V_AUX_CARD8_PWR,
  MEB_SENSOR_P12V_AUX_CARD9_PWR,
  MEB_SENSOR_P12V_AUX_CARD10_PWR,
  MEB_SENSOR_P12V_AUX_CARD11_PWR,
  MEB_SENSOR_P12V_AUX_CARD12_PWR,
  MEB_SENSOR_P12V_AUX_CARD13_PWR,
  MEB_SENSOR_P12V_AUX_CARD14_PWR,
  MEB_SENSOR_E1S_0_TEMP,
  MEB_SENSOR_E1S_1_TEMP,
  MEB_SENSOR_E1S_2_TEMP,
  MEB_SENSOR_E1S_3_TEMP,
};

enum {
  CXL_SENSOR_Inlet_TEMP = 1,
  CXL_SENSOR_CTRL_TEMP,
  CXL_SENSOR_P12V_STBY_4CP_VOL,
  CXL_SENSOR_P3V3_STBY_4CP_VOL,
  CXL_SENSOR_P5V_STBY_VOL,
  CXL_SENSOR_P1V8_ASIC_VOL,
  CXL_SENSOR_P12V_STBY_VOL,
  CXL_SENSOR_P3V3_STBY_VOL,
  CXL_SENSOR_PVPP_AB_VOL,
  CXL_SENSOR_PVTT_AB_VOL,
  CXL_SENSOR_PVPP_CD_VOL,
  CXL_SENSOR_PVTT_CD_VOL,
  CXL_SENSOR_P0V8_ASICA_VOL,
  CXL_SENSOR_P0V9_ASICA_VOL,
  CXL_SENSOR_P0V8_ASICD_VOL,
  CXL_SENSOR_PVDDQ_AB_VOL,
  CXL_SENSOR_PVDDQ_CD_VOL,
  CXL_SENSOR_P12V_STBY_4CP_CUR,
  CXL_SENSOR_P3V3_STBY_4CP_CUR,
  CXL_SENSOR_P0V8_ASICA_CUR,
  CXL_SENSOR_P0V9_ASICA_CUR,
  CXL_SENSOR_P0V8_ASICD_CUR,
  CXL_SENSOR_PVDDQ_AB_CUR,
  CXL_SENSOR_PVDDQ_CD_CUR,
  CXL_SENSOR_P12V_STBY_4CP_PWR,
  CXL_SENSOR_P3V3_STBY_4CP_PWR,
  CXL_SENSOR_P0V8_ASICA_PWR,
  CXL_SENSOR_P0V9_ASICA_PWR,
  CXL_SENSOR_P0V8_ASICD_PWR,
  CXL_SENSOR_PVDDQ_AB_PWR,
  CXL_SENSOR_PVDDQ_CD_PWR,
};

enum {
  E1S_SENSOR_E1S_0_TEMP =1,
  E1S_SENSOR_E1S_1_TEMP,
};

typedef struct {
  uint8_t status;
  uint8_t integer_l;
  uint8_t integer_h;
  uint8_t fraction_l;
  uint8_t fraction_h;
} oem_1s_sensor_reading_resp;

#endif
