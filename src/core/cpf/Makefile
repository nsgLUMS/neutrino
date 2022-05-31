ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif

# Default target, can be overriden by command line or environment
RTE_TARGET ?= x86_64-native-linuxapp-gcc

include $(RTE_SDK)/mk/rte.vars.mk

# binary name
APP = cpf

# Use g++ to compile the application
CC = g++

# all source are stored in SRCS-y
SRCS-y := main.c

# Include Common libs
SRCS-y += $(ROOT)Common/request/request.cpp
SRCS-y += $(ROOT)Common/time/time.cpp
SRCS-y += $(ROOT)Common/logger/Logger.cpp
SRCS-y += $(ROOT)Common/custom_header/CustomHeaderParser.cpp

# Include CPF
SRCS-y += $(ROOT)src/cpf.cpp
SRCS-y += $(ROOT)src/tools/state.cpp
SRCS-y += $(ROOT)src/tools/serializers/asn1_state.cpp
SRCS-y += $(ROOT)src/tools/serializers/flatbuffers_state.cpp
SRCS-y += $(ROOT)src/workers/workers.cpp
SRCS-y += $(ROOT)src/asn1/libasn1.so
SRCS-y += $(ROOT)src/flatbuffer/src/decoder.cpp
SRCS-y += $(ROOT)src/flatbuffer/src/encoder.cpp
SRCS-y += $(ROOT)src/flatbuffer/src/utility.cpp
SRCS-y += $(ROOT)src/flatbuffer/lib/libflatccrt.a

CFLAGS += -w
CFLAGS += -g
CFLAGS += -lpthread
CFLAGS += -O0
CFLAGS += -std=c++11
CFLAGS += -fpermissive

CFLAGS += -I$(ROOT)src/asn1/compiled
CFLAGS += -I$(ROOT)src/flatbuffer/generated
CFLAGS += -I$(ROOT)src/flatbuffer/include
CFLAGS += -W

LDFLAGS += -lconfig++

include $(RTE_SDK)/mk/rte.extapp.mk
