#
# libperif
# Copyright (C) 2015 David Jolly
# ----------------------
#
# libperif is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libperif is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# device configuration
DEV=atmega32u4
DEV_SRT=m32u4
F_CPU=16000000

# source files
SRC_LIB_HC595=hc595
SRC_LIB_I2C=i2c
SRC_LIB_LS165=ls165
SRC_LIB_SPI=spi
SRC_LIB_USART=usart
SRC_MAIN=main

CC=avr-gcc
CC_FLG=-Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(DEV)
DIR_BIN=./bin/
DIR_BUILD=./build/
DIR_LOG=./log/
DIR_LIB_INC=./src/lib/include/
DIR_LIB_SRC=./src/lib/src/
DIR_TOOL=./src/tool/
IMG=perif
STAT=stat

all: clean init image stat

clean:
	rm -rf $(DIR_BIN)
	rm -rf $(DIR_BUILD)
	#rm -rf $(DIR_LOG)

init:
	mkdir $(DIR_BIN)
	mkdir $(DIR_BUILD)
	#mkdir $(DIR_LOG)

image:
	@echo ""
	@echo "============================================"
	@echo "BUILDING IMAGE"
	@echo "============================================"
	# compile objects
	$(CC) $(CC_FLG) -c $(DIR_LIB_SRC)$(SRC_LIB_HC595).c -o $(DIR_BUILD)$(SRC_LIB_HC595).o -Wa,-ahl=$(DIR_BUILD)$(SRC_LIB_HC595).s
	$(CC) $(CC_FLG) -c $(DIR_LIB_SRC)$(SRC_LIB_I2C).c -o $(DIR_BUILD)$(SRC_LIB_I2C).o -Wa,-ahl=$(DIR_BUILD)$(SRC_LIB_I2C).s
	$(CC) $(CC_FLG) -c $(DIR_LIB_SRC)$(SRC_LIB_LS165).c -o $(DIR_BUILD)$(SRC_LIB_LS165).o -Wa,-ahl=$(DIR_BUILD)$(SRC_LIB_LS165).s
	$(CC) $(CC_FLG) -c $(DIR_LIB_SRC)$(SRC_LIB_SPI).c -o $(DIR_BUILD)$(SRC_LIB_SPI).o -Wa,-ahl=$(DIR_BUILD)$(SRC_LIB_SPI).s
	$(CC) $(CC_FLG) -c $(DIR_LIB_SRC)$(SRC_LIB_USART).c -o $(DIR_BUILD)$(SRC_LIB_USART).o -Wa,-ahl=$(DIR_BUILD)$(SRC_LIB_USART).s
	$(CC) $(CC_FLG) -c $(DIR_TOOL)$(SRC_MAIN).c -o $(DIR_BUILD)$(IMG).o -Wa,-ahl=$(DIR_BUILD)$(IMG).s
	# link objects
	$(CC) $(CC_FLG) -o $(DIR_BUILD)$(IMG).elf $(DIR_BUILD)$(IMG).o $(DIR_BUILD)$(SRC_LIB_HC595).o $(DIR_BUILD)$(SRC_LIB_I2C).o $(DIR_BUILD)$(SRC_LIB_LS165).o $(DIR_BUILD)$(SRC_LIB_SPI).o $(DIR_BUILD)$(SRC_LIB_USART).o
	# build image
	avr-objcopy -j .text -j .data -O ihex $(DIR_BUILD)$(IMG).elf $(DIR_BIN)$(IMG).hex

flash:
	@echo ""
	@echo "============================================"
	@echo "FLASHING IMAGE"
	@echo "============================================"
	avrdude -p $(DEV_SRT) -P usb -c usbtiny -U flash:w:$(DIR_BIN)$(IMG).hex

stat:
	avr-size --format=avr --mcu=$(DEV) $(DIR_BUILD)$(IMG).elf #> $(DIR_LOG)$(STAT).log
