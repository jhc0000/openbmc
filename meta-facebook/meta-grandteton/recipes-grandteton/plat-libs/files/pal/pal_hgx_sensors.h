#ifndef __PAL_HGX_SENSORS_H__
#define __PAL_HGX_SENSORS_H__

enum {
  NONE =0,
  EVT = 1,
  DVT = 2
};

// HGX sensor table
enum {
  //Baseboard
  HGX_SNR_PWR_GB_HSC0 = 0x01,
  HGX_SNR_PWR_GB_HSC1,
  HGX_SNR_PWR_GB_HSC2,
  HGX_SNR_PWR_GB_HSC3,
  HGX_SNR_PWR_GB_HSC4,
  HGX_SNR_PWR_GB_HSC5,
  HGX_SNR_PWR_GB_HSC6,
  HGX_SNR_PWR_GB_HSC7,
  HGX_SNR_PWR_GB_HSC8,
  HGX_SNR_PWR_GB_HSC9,
  HGX_SNR_PWR_GB_HSC10,
  Total_Power,
  Total_GPU_Power,
  Altitude_Pressure0,

  TEMP_GB_FPGA = 0x10,
  TEMP_GB_HSC0,
  TEMP_GB_HSC1,
  TEMP_GB_HSC2,
  TEMP_GB_HSC3,
  TEMP_GB_HSC4,
  TEMP_GB_HSC5,
  TEMP_GB_HSC6,
  TEMP_GB_HSC7,
  TEMP_GB_HSC8,
  TEMP_GB_HSC9,
  TEMP_GB_HSC10,
  TEMP_GB_INLET0,
  TEMP_GB_INLET1,

  TEMP_GB_PCB0 = 0x20,
  TEMP_GB_PCB1,
  TEMP_GB_PCB2,
  TEMP_GB_PCB3,
  TEMP_GB_PCIERETIMER0,
  TEMP_GB_PCIERETIMER1,
  TEMP_GB_PCIERETIMER2,
  TEMP_GB_PCIERETIMER3,
  TEMP_GB_PCIERETIMER4,
  TEMP_GB_PCIERETIMER5,
  TEMP_GB_PCIERETIMER6,
  TEMP_GB_PCIERETIMER7,
  TEMP_GB_PCIESWITCH0,
 
  //GPU1 and GPU2
  GPU1_ENG = 0x30,
  GPU1_PWR,
  GPU1_VOL,
  GPU1_TEMP_0,
  GPU1_TEMP_1,
  GPU1_DRAM_PWR,
  GPU1_DRAM_TEMP,

  GPU2_ENG = 0x38,
  GPU2_PWR,
  GPU2_VOL,
  GPU2_TEMP_0,
  GPU2_TEMP_1,
  GPU2_DRAM_PWR,
  GPU2_DRAM_TEMP,

  //GPU3 and GPU4
  GPU3_ENG = 0x40,
  GPU3_PWR,
  GPU3_VOL,
  GPU3_TEMP_0,
  GPU3_TEMP_1,
  GPU3_DRAM_PWR,
  GPU3_DRAM_TEMP,

  GPU4_ENG = 0x48,
  GPU4_PWR,
  GPU4_VOL,
  GPU4_TEMP_0,
  GPU4_TEMP_1,
  GPU4_DRAM_PWR,
  GPU4_DRAM_TEMP,

  //GPU5 and GPU6
  GPU5_ENG = 0x50,
  GPU5_PWR,
  GPU5_VOL,
  GPU5_TEMP_0,
  GPU5_TEMP_1,
  GPU5_DRAM_PWR,
  GPU5_DRAM_TEMP,

  GPU6_ENG = 0x58,
  GPU6_PWR,
  GPU6_VOL,
  GPU6_TEMP_0,
  GPU6_TEMP_1,
  GPU6_DRAM_PWR,
  GPU6_DRAM_TEMP,

  //GPU7 and GPU8
  GPU7_ENG = 0x60,
  GPU7_PWR,
  GPU7_VOL,
  GPU7_TEMP_0,
  GPU7_TEMP_1,
  GPU7_DRAM_PWR,
  GPU7_DRAM_TEMP,

  GPU8_ENG = 0x68,
  GPU8_PWR,
  GPU8_VOL,
  GPU8_TEMP_0,
  GPU8_TEMP_1,
  GPU8_DRAM_PWR,
  GPU8_DRAM_TEMP,

  //NV Switch
  TEMP_GB_NVS0 = 0x70,
  TEMP_GB_NVS1,
  TEMP_GB_NVS2,
  TEMP_GB_NVS3,
  HGX_SNR_CNT = 0x7f,
};

#endif
