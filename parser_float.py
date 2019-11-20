# Writed by Wu Wanquan
import collections
import numpy
import argparse
from functools import reduce

# the object help to decode cycle info
class CycleInfo(object):
    def __init__(self, log):
        self.regid = int(log[1])
        self.cycle = int(log[3:15])
        self.valid = int(log[16]) == 1
        self.pc    = int(log[23:39], 16)
        self.wreg  = int(log[44:46])
        self.wval  = int(log[47:63], 16)
        #self.wvalid= int(log[65]) == 1
        self.sreg1 = int(log[68:70])
        self.src1  = int(log[71:87], 16)
        self.sreg2 = int(log[92:94])
        self.src2  = int(log[95:111], 16)
        self.binary= int(log[119:127], 16)
        self._disam = log[129:]
        self.instr = self._disam.split()[0]
        self.ops = map(lambda x : x.rstrip(','), self._disam.split()[1:])

if __name__ == '__main__':
    # set up arg parser
    parser = argparse.ArgumentParser(
        description='Preprocess the cycle-by-cyle log file outputed\
            by rocket-chip simulator.')
    parser.add_argument('f', metavar='logfile', help='log file')
    parser.add_argument('o', metavar='outputfilename', help='the file name of output file, \
        \'.csv\' will be appended automatically.')

    args = vars(parser.parse_args())
    input_filename = args['f']
    output_filename = args['o'] + '.csv'

    instr_freq_dict = {}
    instr_cycles_dict = {}
    pc_cycle_dict = {}
    instr_stack = []
    instr_has_retired = False

    with open(input_filename) as f:
        for line in f:
            # if it's not a cycle log line
            if (not ('C' in line and ':' in line)):
                continue

            cycleinfo = CycleInfo(line)
            instr = cycleinfo.instr
            pc = cycleinfo.pc
            binary = cycleinfo.binary


            # initialize corresponding entry in two dict
            if (not instr in instr_cycles_dict):
                instr_freq_dict[instr] = 0
                instr_cycles_dict[instr] = []
            
            # initialize pc entry in pc_cycle_dict
            if (not pc in pc_cycle_dict):
                pc_cycle_dict[pc] = []

            # if instr has retired and current instr is new or valid one
            # cycle counting of last instr finished, add it to dict, clear stack
            if instr_has_retired and (instr_stack[len(instr_stack) - 1][1] != instr or cycleinfo.valid):
                instr_has_retired = False
                last_pc, last_instr, cycles = instr_stack.pop()
                instr_cycles_dict[last_instr].append(cycles)
                pc_cycle_dict[last_pc].append(cycles)
                instr_stack = []

            # if a instr retire, find corresponding item in stack and add cycles up to it
            if cycleinfo.valid: # or instr == 'ebreak' or instr == 'c.addi4spn':
                instr_freq_dict[instr] += 1
                
                # deal with ebreak
                if (len(instr_stack) != 0 and instr_stack[len(instr_stack) - 1][1] == 'ebreak'):
                    last_pc, last_instr, cycles = instr_stack.pop()
                    instr_cycles_dict['ebreak'].append(cycles)
                    pc_cycle_dict[last_pc].append(cycles)
                    instr_stack = []
                    instr_freq_dict['ebreak'] += 1

                idx = next((i for i in range(len(instr_stack)) if instr_stack[i][1] == instr), -1) 
                instr_stack.append((pc, instr, sum(map(lambda x: x[2], instr_stack)) if idx != -1 else 0))
                instr_has_retired = True

            # if the stack is empty or instr is not the same as top, initialize its entry
            if len(instr_stack) == 0 or (instr_stack[len(instr_stack) - 1][1] != instr):
                instr_stack.append((pc, instr, 0))

            # update cycle counting
            last_pc, prev_instr, cycles = instr_stack.pop()
            instr_stack.append((last_pc, prev_instr, cycles+1))
            
    # sort by instr
    ordered_instr_cycles_dict = collections.OrderedDict(sorted(instr_cycles_dict.items()))
    ordered_pc_cycles_dict = collections.OrderedDict(sorted(pc_cycle_dict.items()))
    with open(output_filename, "w+") as f:
        total_freq = 0
        total_cycle = 0
        f.write('instr,freq,cycle,mean,stddev,min,max\n')
        for instr, exec_list in ordered_instr_cycles_dict.items():
            # ignore instr that has not been actually executed
            if len(exec_list) == 0:
                continue
            instr_exec_cycles_arr = numpy.array(exec_list)
            cycles = numpy.sum(instr_exec_cycles_arr)
            total_freq += instr_freq_dict[instr]            
            total_cycle += cycles
            f.write(instr + ','
             + str(instr_freq_dict[instr]) + ','
             + str(cycles) + ','
             + str(numpy.mean(instr_exec_cycles_arr)) + ','
             + str(numpy.std(instr_exec_cycles_arr)) + ','
             + str(numpy.min(instr_exec_cycles_arr)) + ','
             + str(numpy.max(instr_exec_cycles_arr))
             + '\n')
        print('total freq: %d, cycle: %d'%(total_freq, total_cycle))
        
        # f.write('pc,freq,cycle,mean,stddev,min,max\n')
        # for pc, exec_list in ordered_pc_cycles_dict.items():
        #     # ignore pc that has not been actually executed
        #     if len(exec_list) == 0:
        #         continue
        #     pc_exec_cycles_arr = numpy.array(exec_list)
        #     f.write('%x'%(pc) + ','
        #      + str(numpy.size(pc_exec_cycles_arr)) + ','
        #      + str(numpy.sum(pc_exec_cycles_arr)) + ','
        #      + str(numpy.mean(pc_exec_cycles_arr)) + ','
        #      + str(numpy.std(pc_exec_cycles_arr)) + ','
        #      + str(numpy.min(pc_exec_cycles_arr)) + ','
        #      + str(numpy.max(pc_exec_cycles_arr))
        #      + '\n')
