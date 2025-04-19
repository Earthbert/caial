# Writed by Wu Wanquan

# need to have RISCVTESTS enviroment variable
# set the name of the config you want to use for simulation
# 	you must compile the simulator by yourself first
ROCKET_CHIP ?= ${ROCKETCHIP}
SIMCONFIG ?= DefaultConfig

# enable process substitution
SHELL=/bin/bash

CC = riscv64-none-elf-gcc
INC_RD = ${RISCV_TESTS_ROOT}/source
INCS = -I $(INC_RD)/env -I $(INC_RD)/benchmarks/common -I .
OSRCS = $(INC_RD)/benchmarks/common/syscalls.c $(INC_RD)/benchmarks/common/crt.S		# other src files that need to compile with program srcs
CFLAGS = -mcmodel=medany -std=gnu99 -fno-common -fno-builtin-printf -g
LFLAGS = -nostdlib -nostartfiles -lm -lgcc -T $(INC_RD)/benchmarks/common/test.ld
RISCV_OBJDUMP = riscv64-none-elf-objdump --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data
SIM_PREFIX = $(ROCKET_CHIP)/out/emulator/freechips.rocketchip.system.TestHarness/freechips.rocketchip.system.
SIM = $(SIM_PREFIX)$(SIMCONFIG)/verilator/elf.dest/emulator
VERBOSE_TO_FILE = > $@.stdout 2> >(spike-dasm > $@.log)
RISCV_DEBUG = ${RISCV_TESTS_ROOT}/debug
