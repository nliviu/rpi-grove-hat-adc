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

#include <stdio.h>

int main(void) {
  /*
   * Raspberry Pi hat: hat_i2c_addr=0x04, max_channels=6
   * Raspberry Pi Zero hat: hat_i2c_addr=0x05, max_channels=8
   */
  const uint8_t hat_i2c_addr = 0x04;
  const uint8_t max_channels = 6;
  struct grove_hat_adc *adc =
      grove_hat_adc_create(1, hat_i2c_addr, max_channels);
  if (adc == NULL) {
    fprintf(stderr, "Failed to create struct grove_hat_adc\n");
    return 1;
  }
  for (uint8_t i = 0; i < max_channels; ++i) {
    uint16_t raw_value = 0;
    bool ok = grove_hat_adc_read_raw(adc, i, &raw_value);
    printf("channel %hhu - ok: %d, raw_value: %hu\n", i, ok, raw_value);
    uint16_t voltage = 0;
    ok = grove_hat_adc_read_voltage(adc, i, &voltage);
    printf("channel %hhu - ok: %d, voltage (mV): %hu\n", i, ok, voltage);
    uint16_t ratio = 0;
    ok = grove_hat_adc_read_ratio(adc, i, &ratio);
    printf("channel %hhu - ok: %d, ratio: %hu\n", i, ok, ratio);
  }

  grove_hat_adc_close(&adc);
  return 0;
}
