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

