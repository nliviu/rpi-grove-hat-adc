# A ADC test application for SeedStudio's Grove Base Hat for Raspberry Pi

## How to compile
```bash
git clone https://github.com/nliviu/rpi-grove-hat-adc
cd rpi-grove-hat-adc
gcc -Wall -Werror -Wextra -fsanitize=address -fno-omit-frame-pointer src/libi2c.c src/grove_hat_adc.c src/test.c -static-libasan -o test
./test
```
