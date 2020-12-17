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

#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct grove_hat_adc;

struct grove_hat_adc *grove_hat_adc_create(const int i2c_bus_number,
                                           const uint8_t hat_i2c_addr,
                                           const uint8_t max_channels);

bool grove_hat_adc_close(struct grove_hat_adc **adc);

bool grove_hat_adc_read_raw(const struct grove_hat_adc *adc,
                            const uint8_t channel, uint16_t *value);

bool grove_hat_adc_read_voltage(const struct grove_hat_adc *adc,
                                const uint8_t channel, uint16_t *value);

bool grove_hat_adc_read_ratio(const struct grove_hat_adc *adc,
                              const uint8_t channel, uint16_t *value);

#ifdef __cplusplus
}
#endif
