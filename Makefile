# Project Name
TARGET = miku

# Sources
CPP_SOURCES = miku.cpp

# Library Locations
LIBDAISY_DIR = lib/libDaisy
DAISYSP_DIR = lib/DaisySP

LDFLAGS = -u _printf_float

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

.PHONY: udev-rules
udev-rules:
	@sudo cp 70-daisy-seed.udev.rules /etc/udev/rules.d
	@sudo systemctl reload udev
	@sudo udevadm trigger

.PHONY: dfu-id
dfu-id:
	# TODO combo this with udev-rules
	@dfu-util -l | \
		grep --max-count 1 "Found DFU" | \
		sed -n 's/.*\[\([0-9a-fA-F]\+\):\([0-9a-fA-F]\+\)\].*/ATTRS{idVendor}=="\1", ATTRS{idProduct}=="\2", MODE="660", GROUP="plugdev", TAG+="uaccess"/p'		