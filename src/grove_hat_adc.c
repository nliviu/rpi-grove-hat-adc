/*
 * Copyright 2020 Liviu Nicolescu <nliviu@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "grove_hat_adc.h"

#include "libi2c.h"

#include <stdio.h>
#include <stdlib.h>

struct grove_hat_adc {
  struct i2c *i2c;
  uint8_t hat_i2c_addr;
  uint8_t max_channels;
};

enum adc_registers_start {
  REG_RAW_START = 0x10,
  REG_VOLTAGE_START = 0x20,
  REG_RATIO_START = 0x30,
};

/*
 * From https://github.com/Seeed-Studio/grove.py/blob/master/grove/adc.py
 * Read the ADC Core (through I2C) registers
 * Grove Base Hat for RPI I2C Registers
 *  - 0x00 ~ 0x01:
 *  - 0x10 ~ 0x17: ADC raw data
 *  - 0x20 ~ 0x27: input voltage
 * - 0x29: output voltage (Grove power supply voltage)
 * - 0x30 ~ 0x37: input voltage / output voltage
 */
static bool adc_read_register(const struct grove_hat_adc *adc,
                              const uint8_t reg, uint16_t *value) {
  // Assume adc and value are not NULL. The check has been already made.
  return i2c_read_word(adc->i2c, adc->hat_i2c_addr, reg, value);
}

struct grove_hat_adc *grove_hat_adc_create(const int i2c_bus_number,
                                           const uint8_t hat_i2c_addr,
                                           const uint8_t max_channels) {
  struct grove_hat_adc *adc = NULL;
  char buf[16];
  snprintf(buf, sizeof(buf), "/dev/i2c-%d", i2c_bus_number);
  struct i2c *i2c = i2c_open(buf);
  if (i2c == NULL) {
    fprintf(stderr, "Failed to open i2c device %s\n", buf);
    return adc;
  }
  adc = (struct grove_hat_adc *) calloc(1, sizeof(*adc));
  if (adc == NULL) {
    fprintf(stderr, "Failed to allocate struct grove_hat_adc\n");
    i2c_close(&i2c);
    return adc;
  }
  adc->i2c = i2c;
  adc->hat_i2c_addr = hat_i2c_addr;
  adc->max_channels = max_channels;

  return adc;
}

bool grove_hat_adc_close(struct grove_hat_adc **adc) {
  if (*adc == NULL) {
    return false;
  }
  i2c_close(&(*adc)->i2c);
  free(*adc);
  *adc = NULL;
  return true;
}

bool grove_hat_adc_read_raw(const struct grove_hat_adc *adc,
                            const uint8_t channel, uint16_t *value) {
  if ((adc == NULL) || (value == NULL) || (channel > adc->max_channels)) {
    return false;
  }

  return adc_read_register(adc, REG_RAW_START + channel, value);
}

bool grove_hat_adc_read_voltage(const struct grove_hat_adc *adc,
                                const uint8_t channel, uint16_t *value) {
  if ((adc == NULL) || (value == NULL) || (channel > adc->max_channels)) {
    return false;
  }

  return adc_read_register(adc, REG_VOLTAGE_START + channel, value);
}

bool grove_hat_adc_read_ratio(const struct grove_hat_adc *adc,
                              const uint8_t channel, uint16_t *value) {
  if ((adc == NULL) || (value == NULL) || (channel > adc->max_channels)) {
    return false;
  }

  return adc_read_register(adc, REG_RATIO_START + channel, value);
}