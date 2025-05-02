# Writed by Wu Wanquan
# setting about this program
APP ?= KNN
# name of this program
FILES = ./$(APP)/$(APP).c    # files that need to compile (they will not be compile separately)
OPTFLAGS = -O2

# --------------------------------------------
include ./Makefile.inc		# include Makefile.inc here
all: $(APP).riscv

clean:
	rm $(APP).riscv || true
	rm $(APP)_opted.riscv || true
	rm $(APP).riscv.dump || true
	rm $(APP)_opted.riscv.dump || true
	rm simv.log || true
	rm simv.stdout || true

$(APP).riscv: $(FILES)
	$(CC) $(INCS) $(CFLAGS) -O0 -o $@ $(FILES) $(OSRCS) $(LFLAGS)
	$(RISCV_OBJDUMP) $@ > $@.dump

$(APP)_opted.riscv: $(FILES)
	$(CC) $(INCS) $(CFLAGS) $(OPTFLAGS) -o $@ $(FILES) $(OSRCS) $(LFLAGS)
	$(RISCV_OBJDUMP) $@ > $@.dump

# start simulation without cycle-by-cycle log
# output of program will be directly write to console
sim: $(APP).riscv
	$(SIM) +verbose $<

sim_pk: $(APP).riscv
	$(SIM) $(which pk) $<

# start simulation
# output of program > simv.stdout
# cycle-by-cycle log > simv.log
simv: $(APP).riscv
	$(SIM) +verbose $< $(VERBOSE_TO_FILE)
	
simopt: $(APP)_opted.riscv
	$(SIM) $<

# written to simoptv.stdout, simoptv.log instead
simoptv: $(APP)_opted.riscv
	$(SIM) +verbose $< $(VERBOSE_TO_FILE)

# start simulation with jtag, keep in mind that cpu config should have jtag
simjtag: $(APP).riscv
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
