# Writed by Wu Wanquan
# setting about this program
APP ?= KNN
# name of this program
FILES = ./$(APP)/$(APP).c    # files that need to compile (they will not be compile separately)
OPTFLAGS = -O2

SYSCALL = syscalls
CRT = crt

BUILD_DIR = out

# --------------------------------------------
include ./Makefile.inc		# include Makefile.inc here
all: $(BUILD_DIR)/$(APP).riscv

clean:
	rm -rf $(BUILD_DIR) || true
	rm simv.log || true
	rm simv.stdout || true

$(BUILD_DIR)/$(SYSCALL).o: $(SYSCALLSRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCS) $(CFLAGS) -O0 -o $@ -c $^

$(BUILD_DIR)/$(CRT).o: $(CRTSRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCS) $(CFLAGS) -O0 -o $@ -c $^

$(BUILD_DIR)/$(APP).o: $(APP)/$(APP).c
	@mkdir -p $(BUILD_DIR)
ifdef NRS
	LD_PRELOAD=${JAVA_HOME}/lib/server/libjvm.so NRSSL_JARS=$(NRSSL_JARS) $(CLANG) $(INCS) $(CLANG_FLAGS) $(PASS_FLAGS) -O0 -o $@ -c $<
else
	$(CLANG) $(INCS) $(CLANG_FLAGS) $(PASS_FLAGS) -O0 -o $@ -c $<
endif

$(BUILD_DIR)/$(APP)_opted.o: $(APP)/$(APP).c
	@mkdir -p $(BUILD_DIR)
ifdef NRS
	LD_PRELOAD=${JAVA_HOME}/lib/server/libjvm.so NRSSL_JARS=$(NRSSL_JARS) $(CLANG) $(INCS) $(CLANG_FLAGS) $(PASS_FLAGS) $(OPTFLAGS) -o $@ -c $<
else
	$(CLANG) $(INCS) $(CLANG_FLAGS) $(PASS_FLAGS) $(OPTFLAGS) -o $@ -c $<
endif

$(BUILD_DIR)/$(APP).riscv: $(BUILD_DIR)/$(APP).o $(BUILD_DIR)/$(SYSCALL).o $(BUILD_DIR)/$(CRT).o
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCS) $(CFLAGS) -O0 -o $@ $(LFLAGS) $^
	$(RISCV_OBJDUMP) $@ > $@.dump

$(BUILD_DIR)/$(APP)_opted.riscv: $(BUILD_DIR)/$(APP)_opted.o $(BUILD_DIR)/$(SYSCALL).o $(BUILD_DIR)/$(CRT).o
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCS) $(CFLAGS) $(OPTFLAGS) -o $@ $(LFLAGS) $^
	$(RISCV_OBJDUMP) $@ > $@.dump

# start simulation without cycle-by-cycle log
# output of program will be directly write to console
sim: $(BUILD_DIR)/$(APP).riscv
	$(SIM) +verbose $<

sim_pk: $(BUILD_DIR)/$(APP).riscv
	$(SIM) $(which pk) $<

# start simulation
# output of program > simv.stdout
# cycle-by-cycle log > simv.log
simv: $(BUILD_DIR)/$(APP).riscv
	$(SIM) +verbose $< $(VERBOSE_TO_FILE)
	
simopt: $(BUILD_DIR)/$(APP)_opted.riscv
	$(SIM) $<

# written to simoptv.stdout, simoptv.log instead
simoptv: $(BUILD_DIR)/$(APP)_opted.riscv
	$(SIM) +verbose $< $(VERBOSE_TO_FILE)

# start simulation with jtag, keep in mind that cpu config should have jtag
simjtag: $(BUILD_DIR)/$(APP).riscv
	$(SIM) +jtag_rbb_enable=1 \
	--rbb-port=3332 \
	$< \

# start openocd
openocd:
	openocd -f openocd.cfg \

# ---------------------------------------------------

# this is used to extract cycle records not in Debug Mode (those pc not in the objdump file) 
#	and Infinite Loop (the "c.j pc + 0" for waiting till simulation be terminated)
# 	thank Stefan for writing this command for me 
strip: $(APP).riscv
	awk 'NF>2 && NR>5 { split($$1, subfield, ":"); print "00000000"subfield[1] }' $(APP).riscv.dump > pc_list.tmp
	awk -F "[][]" 'NR == FNR { a[$$0]; next } {if(($$4 in a) && ($$14!="0000a001")) print $$0}' pc_list.tmp simv.log > strip_simv.log
	spike-dasm < strip_simv.log > strip_simv_2.log
	python parser.py strip_simv_2.log output
	rm pc_list.tmp
