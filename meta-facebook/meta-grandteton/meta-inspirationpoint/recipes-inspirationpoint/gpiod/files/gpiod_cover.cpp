/*
 * sensord
 *
 * Copyright 2022-present Facebook. All Rights Reserved.
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
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/file.h>
#include <pthread.h>
#include <openbmc/pal.h>
#include <openbmc/kv.h>
#include <openbmc/obmc-i2c.h>
#include <openbmc/pal_common.h>
#include <openbmc/pal_def.h>
#include <openbmc/aries_api.h>
#include "gpiod.h"

static void
pwr_err_event_handler(gpiopoll_pin_t *desc, gpio_value_t last, gpio_value_t curr) {
  const struct gpiopoll_config *cfg = gpio_poll_get_config(desc);
  char fn[32] = "/dev/i2c-7";
  int fd, ret;
  uint8_t tlen, rlen;
  uint8_t addr = 0x46;
  uint8_t tbuf[16] = {0};
  uint8_t rbuf[16] = {0};
  uint8_t cmds[1] = {0x27};

  if (!sgpio_valid_check())
    return;
  fd = open(fn, O_RDWR);
  if (fd < 0) {
    syslog(LOG_ERR, "can not open i2c device\n");
    return;
  }

  tlen = 1;
  rlen = 3;

  tbuf[0] = cmds[0];
  if ((ret = i2c_rdwr_msg_transfer(fd, addr, tbuf, tlen, rbuf, rlen))) {
    syslog(LOG_ERR, "i2c transfer fail\n");
  }

  if (fd > 0) {
    close(fd);
  }

  if (curr) {
    syslog(LOG_CRIT, "FRU: %d ASSERT: %s - %s power status:%02X%02X%02X\n", FRU_MB, cfg->description, cfg->shadow, rbuf[0], rbuf[1], rbuf[2]);
  } else {
    syslog(LOG_CRIT, "FRU: %d DEASSERT: %s - %s\n", FRU_MB, cfg->description, cfg->shadow);
  }
}

static void
device_prsnt_event_handler(gpiopoll_pin_t *desc, gpio_value_t last, gpio_value_t curr) {
  const struct gpiopoll_config *cfg = gpio_poll_get_config(desc);

  if (curr) {
    syslog(LOG_CRIT, "FRU: %d %s LOST- %s\n", FRU_MB, cfg->description, cfg->shadow);
  } else {
    syslog(LOG_CRIT, "FRU: %d %s INSERT- %s\n", FRU_MB, cfg->description, cfg->shadow);
  }
}

static void
rst_perst_event_handler(gpiopoll_pin_t *desc, gpio_value_t last, gpio_value_t curr) {
  if (system("sh /etc/init.d/rebind-rt-mux.sh &") != 0) {
    syslog(LOG_CRIT, "Failed to run: %s", __FUNCTION__);
  }
}

static void
rst_perst_init_handler(gpiopoll_pin_t *desc, gpio_value_t curr) {
  if (curr) {
    rst_perst_event_handler(desc, curr, curr);
  }
}

// GPIO table to be monitored
static struct gpiopoll_config gpios_plat_list[] = {
  // shadow, description, edge, handler, oneshot
  {IRQ_UV_DETECT_N,             "SGPIO188",  GPIO_EDGE_BOTH,    uv_detect_handler,          NULL},
  {IRQ_OC_DETECT_N,             "SGPIO178",  GPIO_EDGE_BOTH,    oc_detect_handler,          NULL},
  {IRQ_HSC_FAULT_N,             "SGPIO36",   GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {IRQ_HSC_ALERT_N,             "SGPIO2",    GPIO_EDGE_BOTH,    sml1_pmbus_alert_handler,   NULL},
  {FM_CPU0_PROCHOT_N,           "SGPIO202",  GPIO_EDGE_BOTH,    cpu_prochot_handler,        NULL},
  {FM_CPU1_PROCHOT_N,           "SGPIO186",  GPIO_EDGE_BOTH,    cpu_prochot_handler,        NULL},
  {FM_CPU0_THERMTRIP_N,         "SGPIO136",  GPIO_EDGE_BOTH,    cpu_thermtrip_handler,      NULL},
  {FM_CPU1_THERMTRIP_N,         "SGPIO118",  GPIO_EDGE_BOTH,    cpu_thermtrip_handler,      NULL},
  {FM_CPU_ERR0_N,               "SGPIO142",  GPIO_EDGE_BOTH,    cpu_error_handler,          NULL},
  {FM_CPU_ERR1_N,               "SGPIO144",  GPIO_EDGE_BOTH,    cpu_error_handler,          NULL},
  {FM_SYS_THROTTLE,             "SGPIO20" ,  GPIO_EDGE_BOTH,    gpio_event_pson_handler,    NULL},
  {RST_PLTRST_N,                "SGPIO200",  GPIO_EDGE_BOTH,    platform_reset_handle,      platform_reset_init},
  {FM_LAST_PWRGD,               "SGPIO240",  GPIO_EDGE_BOTH,    pwr_good_handler,           pwr_good_init},
  {FM_CPU0_SKTOCC,              "SGPIO112",  GPIO_EDGE_BOTH,    sgpio_event_handler,        cpu_skt_init},
  {FM_CPU1_SKTOCC,              "SGPIO114",  GPIO_EDGE_BOTH,    sgpio_event_handler,        cpu_skt_init},
  {FM_CPU0_PWR_FAIL,            "SGPIO174",  GPIO_EDGE_BOTH,    pwr_err_event_handler,      NULL},
  {FM_CPU1_PWR_FAIL,            "SGPIO176",  GPIO_EDGE_BOTH,    pwr_err_event_handler,      NULL},
  {FM_UV_ADR_TRIGGER,           "SGPIO26",   GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {FM_PVDDCR_CPU0_P0_PMALERT,   "SGPIO154",  GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {FM_PVDDCR_CPU0_P1_PMALERT,   "SGPIO156",  GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {FM_PVDDCR_CPU1_P0_SMB_ALERT, "SGPIO158",  GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {FM_PVDDCR_CPU1_P1_SMB_ALERT, "SGPIO160",  GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {RST_PERST_N,                 "SGPIO230",  GPIO_EDGE_RISING,  rst_perst_event_handler,    rst_perst_init_handler},
  {FM_CPU0_PRSNT,               "CPU0",      GPIO_EDGE_BOTH,    device_prsnt_event_handler, NULL},
  {FM_CPU1_PRSNT,               "CPU1",      GPIO_EDGE_BOTH,    device_prsnt_event_handler, NULL},
  {FM_OCP0_PRSNT,               "NIC0",      GPIO_EDGE_BOTH,    device_prsnt_event_handler, NULL},
  {FM_OCP1_PRSNT,               "NIC1",      GPIO_EDGE_BOTH,    device_prsnt_event_handler, NULL},
  {FM_E1S0_PRSNT,               "E1.S",      GPIO_EDGE_BOTH,    device_prsnt_event_handler, NULL},
  {FM_PVDD11_S3_P0_OCP,         "SGPIO14",   GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {FM_PVDD11_S3_P1_OCP,         "SGPIO16",   GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {PVDD11_S3_P0_PMALERT,        "SGPIO182",  GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {PVDD11_S3_P1_PMALERT,        "SGPIO184",  GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
  {FM_BIOS_POST_CMPLT,          "SGPIO146",  GPIO_EDGE_BOTH,    sgpio_event_handler,        NULL},
};

int get_gpios_plat_list(struct gpiopoll_config** list) {

  uint8_t cnt = sizeof(gpios_plat_list)/sizeof(gpios_plat_list[0]);
  *list = gpios_plat_list;

  return cnt;
}


pthread_t tid_gpio_timer;
pthread_t tid_iox_gpio_handle;

int gpiod_plat_thread_create(void) {

  //Create thread for platform reset event filter check
  if (pthread_create(&tid_gpio_timer, NULL, gpio_timer, NULL) < 0) {
    syslog(LOG_WARNING, "pthread_create for platform_reset_filter_handler\n");
    return -1;
  }

  //Create thread for platform reset event filter check
  if (pthread_create(&tid_iox_gpio_handle, NULL, iox_gpio_handle, NULL) < 0) {
    syslog(LOG_WARNING, "pthread_create for iox_gpio_handler\n");
    return -1;
  }

  return 0;
}

void gpiod_plat_thread_finish(void) {
  pthread_join(tid_gpio_timer, NULL);
  pthread_join(tid_iox_gpio_handle, NULL);
}
