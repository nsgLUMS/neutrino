ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# make log directory
logs := logs

# binary name
APP = pktgen

# all source are stored in SRCS-y
SRCS-y := main.c
SRCS-y += $(ROOT)helper/pkt_helper.c
SRCS-y += $(ROOT)helper/init.c
SRCS-y += $(ROOT)header/header.c
SRCS-y += $(ROOT)state_machine/state_machine.c

# =========== Including ASN1 ==================
SRCS-y += $(ROOT)asn1/lib/asn1.a
# =========== Including Flatbuffers ============
SRCS-y += $(ROOT)flatbuffers/lib/fb.a
SRCS-y += $(ROOT)flatbuffers/lib/libflatcc.a


ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif

# Default target, can be overridden by command line or environment
RTE_TARGET ?= x86_64-native-linuxapp-gcc

include $(RTE_SDK)/mk/rte.vars.mk

CFLAGS += -O3
CFLAGS += -I$(ROOT)state_machine
CFLAGS += -I$(ROOT)asn1/includes
CFLAGS += -I$(ROOT)flatbuffers/includes
CFLAGS += -I$(ROOT)header
# Disable annoying errors
# CFLAGS += $(WERROR_FLAGS)

include $(RTE_SDK)/mk/rte.extapp.mk
