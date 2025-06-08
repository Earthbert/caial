# Writed by Wu Wanquan

# need to have RISCVTESTS enviroment variable
# set the name of the config you want to use for simulation
# 	you must compile the simulator by yourself first
ROCKET_CHIP ?= ${ROCKETCHIP}
ifndef LLVM_PASS
    $(error LLVM_PASS environment variable must be set)
endif
LLVM_PASS ?= ${LLVM_PASS}
SIMCONFIG ?= DefaultConfigPosit
RV64_TOOLCHAIN_ROOT ?= ${RV64_TOOLCHAIN_ROOT}

# enable process substitution
SHELL=/bin/bash


RV_LIBC_PATH ?= $(shell cat $(RV64_TOOLCHAIN_ROOT)/nix-support/orig-libc)
RV_GCC_PATH ?= $(shell cat $(RV64_TOOLCHAIN_ROOT)/nix-support/orig-cc)

CC = riscv64-none-elf-gcc
CLANG = clang-19
INC_RD = ${RISCV_TESTS_ROOT}/source
INCS = -I $(INC_RD)/env -I $(INC_RD)/benchmarks/common -I .
SYSCALLSRC = $(INC_RD)/benchmarks/common/syscalls.c
CRTSRC = $(INC_RD)/benchmarks/common/crt.S
CLANG_FLAGS = -target riscv64-unknown-elf -march=rv64gc \
        -nostdinc -fno-stack-protector \
        -mcmodel=medany \
        -include common/custom_undef.h \
        -isystem $(RV_LIBC_PATH)/riscv64-none-elf/include \
        -isystem $(RV_GCC_PATH)/lib/gcc/riscv64-none-elf/12.3.0/include \
        -isystem $(RV_GCC_PATH)/lib/gcc/riscv64-none-elf/12.3.0/include-fixed
LLVM_PASS_PATH = $(LLVM_PASS)/src/build/libNRSConversionPass.so
NRSSL_JARS = $(LLVM_PASS)/src/jars
PASS_FLAGS = -fpass-plugin=$(LLVM_PASS_PATH)

CFLAGS = -mcmodel=medany -std=gnu99 -fno-common -fno-builtin-printf -g
LFLAGS = -nostdlib -nostartfiles -lm -lgcc -T $(INC_RD)/benchmarks/common/test.ld
RISCV_OBJDUMP = riscv64-none-elf-objdump --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data
SIM_PREFIX = $(ROCKET_CHIP)/out/emulator/freechips.rocketchip.system.TestHarness/freechips.rocketchip.system.
SIM = $(SIM_PREFIX)$(SIMCONFIG)/verilator/elf.dest/emulator
VERBOSE_TO_FILE = > $@.stdout 2> >(spike-dasm > $@.log)
RISCV_DEBUG = ${RISCV_TESTS_ROOT}/debug
