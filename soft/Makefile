# set the following variables 
# path to Arduino IDE
# PATH_TO_ARDUINO_IDE = /path/to/your/arduinoIDE
# Arduino IDE version
# ARDUINO_IDE_VER = 1.6.X
# local github mirror, if applicable, https://github.com is used per default
# GITHUB_MIRROR = /path/to/your/github mirror

ARDUINO_DIR = $(PATH_TO_ARDUINO_IDE)/arduino-$(ARDUINO_IDE_VER)

ifneq ("$(wildcard $(GITHUB_MIRROR))","")
GITHUB = $(GITHUB_MIRROR)
else
GITHUB = https://github.com
endif

.PHONY: default git_setup clean_all clean_builds fuse_read fuse_write

#board info
ISP_PROG = usbasp
MCU=atmega328p
F_CPU=20000000UL 
CORE=arduino
VARIANT=standard
HEX_MAXIMUM_SIZE=32256

BOARD_TAG    = PowerSeq

#default target
default: git_setup
######################################################################################
# setup section
#local library directories
ARDMK_DIR := ./local-libs/Arduino-Makefile

USER_LIB_PATH := ./local_libs
ARDUINO_LIBS = EEPROM


LOCAL_GIT_LIBRARIES = $(ARDMK_DIR)

# local library includes
# Arduino Makefile environment
ifneq ("$(wildcard $(ARDMK_DIR))","")
include $(ARDMK_DIR)/Arduino.mk
endif

#setup part
git_setup: $(LOCAL_GIT_LIBRARIES)
	@echo Do the following:
	@echo make ispload

#local library target/rules
$(ARDMK_DIR): 
	@rm -rf $@
	@git clone $(GITHUB)/sudar/Arduino-Makefile $@

######################################################################################
#cleanup section
clean_all: clean_builds 
	@echo "Cleaning local libs ..."
	@rm -rf ./local-libs
	@rm -rf ./build-*

clean_builds: 
	@echo "Cleaning builds ..."
	@rm -rf ./build-*

#other PHONY stuff
fuse_read:
	avrdude -p $(MCU) $(AVRDUDE_ISP_OPTS) -v
#20 MHz crystal
fuse_write: 
	avrdude -p $(MCU) $(AVRDUDE_ISP_OPTS) -v -U lfuse:w:0xF7:m -U hfuse:w:0xD2:m -U efuse:w:0x05:m


