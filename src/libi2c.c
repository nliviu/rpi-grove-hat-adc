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

#include "libi2c.h"

#include <byteswap.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>
#include <linux/i2c.h>

static inline int32_t i2c_smbus_exec(int file, char read_write, uint8_t command,
                                     int size, union i2c_smbus_data *data) {
  struct i2c_smbus_ioctl_data args;

  args.read_write = read_write;
  args.command = command;
  args.size = size;
  args.data = data;
  return ioctl(file, I2C_SMBUS, &args);
}

struct i2c {
  int i2c;
};

struct i2c *i2c_open(const char *i2cbus) {
  struct i2c *i2c = NULL;
  int handle = open(i2cbus, O_RDWR);
  if (handle < 0) {
    return i2c;
  }
  i2c = (struct i2c *) calloc(1, sizeof(*i2c));
  i2c->i2c = handle;
  return i2c;
}

bool i2c_close(struct i2c **i2c) {
  if (*i2c == NULL) {
    return false;
  }
  int handle = (*i2c)->i2c;
  if (handle >= 0) {
    close(handle);
  }

  free(*i2c);
  *i2c = NULL;
  return true;
}

bool i2c_read_byte(const struct i2c *i2c, const uint8_t addr, const uint8_t reg,
                   uint8_t *value) {
  bool ret = false;
  if (value == NULL) {
    return ret;
  }
  *value = 0;
  int32_t err = ioctl(i2c->i2c, I2C_SLAVE, addr);
  if (err == 0) {
    union i2c_smbus_data data;
    err = i2c_smbus_exec(i2c->i2c, I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA,
                         &data);
    ret = err != -1;
    if (ret) {
      *value = data.byte;
    }
  }

  return ret;
}

bool i2c_write_byte(const struct i2c *i2c, const uint8_t addr,
                    const uint8_t reg, const uint8_t value) {
  bool ret = false;
  int32_t err = ioctl(i2c->i2c, I2C_SLAVE, addr);
  if (err == 0) {
    union i2c_smbus_data data;
    data.byte = value;
    ret = i2c_smbus_exec(i2c->i2c, I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA,
                         &data) == 0;
  }

  return ret;
}

bool i2c_read_word(const struct i2c *i2c, const uint8_t addr, const uint8_t reg,
                   uint16_t *value) {
  bool ret = false;
  if (value == NULL) {
    return ret;
  }
  *value = 0;
  int32_t err = ioctl(i2c->i2c, I2C_SLAVE, addr);
  if (err == 0) {
    union i2c_smbus_data data;
    err = i2c_smbus_exec(i2c->i2c, I2C_SMBUS_READ, reg, I2C_SMBUS_WORD_DATA,
                         &data);
    ret = err != -1;
    if (ret) {
      *value = __bswap_16(data.word);
    }
  }

  return ret;
}

bool i2c_write_word(const struct i2c *i2c, const uint8_t addr,
                    const uint8_t reg, const uint16_t value) {
  bool ret = false;
  int32_t err = ioctl(i2c->i2c, I2C_SLAVE, addr);
  if (err == 0) {
    union i2c_smbus_data data;
    data.word = __bswap_16(value);
    ret = i2c_smbus_exec(i2c->i2c, I2C_SMBUS_WRITE, reg, I2C_SMBUS_WORD_DATA,
                         &data) == 0;
  }

  return ret;
}

bool i2c_read_bytes(const struct i2c *i2c, const uint8_t addr,
                    const uint8_t reg, uint8_t *values, const uint8_t count) {
  bool ret = false;
  if ((values == NULL) && (count > I2C_SMBUS_BLOCK_MAX)) {
    return ret;
  }

  int32_t err = ioctl(i2c->i2c, I2C_SLAVE, addr);
  if (err == 0) {
    union i2c_smbus_data data;
    data.block[0] = count;
    ret = i2c_smbus_exec(i2c->i2c, I2C_SMBUS_READ, reg,
                         I2C_SMBUS_I2C_BLOCK_DATA, &data) == 0;
    if (ret) {
      // copy the data
      for (uint8_t i = 0; i < count; ++i) {
        values[i] = data.block[i + 1];
      }
    }
  }

  return ret;
}

bool i2c_write_bytes(const struct i2c *i2c, const uint8_t addr,
                     const uint8_t reg, const uint8_t *values,
                     const uint8_t count) {
  bool ret = false;
  if ((values == NULL) && (count > I2C_SMBUS_BLOCK_MAX)) {
    return ret;
  }

  int32_t err = ioctl(i2c->i2c, I2C_SLAVE, addr);
  if (err == 0) {
    union i2c_smbus_data data;
    data.block[0] = count;
    // copy the data
    for (uint8_t i = 0; i < count; ++i) {
      data.block[i + 1] = values[i];
    }
    ret = i2c_smbus_exec(i2c->i2c, I2C_SMBUS_WRITE, reg,
                         I2C_SMBUS_I2C_BLOCK_DATA, &data) == 0;
  }

  return ret;
}
