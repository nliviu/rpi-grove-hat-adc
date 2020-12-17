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

struct i2c;

struct i2c *i2c_open(const char *i2cbus);

bool i2c_close(struct i2c **i2c);

bool i2c_read_byte(const struct i2c *i2c, const uint8_t addr, const uint8_t reg,
                   uint8_t *value);
bool i2c_write_byte(const struct i2c *i2c, const uint8_t addr,
                    const uint8_t reg, const uint8_t value);

bool i2c_read_word(const struct i2c *i2c, const uint8_t addr, const uint8_t reg,
                   uint16_t *value);
bool i2c_write_word(const struct i2c *i2c, const uint8_t addr,
                    const uint8_t reg, const uint16_t value);

bool i2c_read_bytes(const struct i2c *i2c, const uint8_t addr,
                    const uint8_t reg, uint8_t *values, const uint8_t count);
bool i2c_write_bytes(const struct i2c *i2c, const uint8_t addr,
                     const uint8_t reg, const uint8_t *values,
                     const uint8_t count);

#ifdef __cplusplus
}
#endif
