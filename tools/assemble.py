import subprocess
import argparse
import os

argparser = argparse.ArgumentParser()
argparser.add_argument("--file", type=str, help="file to run")

args = argparser.parse_args()

filename = args.file

# remove extension
hex_filename = os.path.splitext(filename)[0]
hex_filename = f'{hex_filename}.x'

print(hex_filename)

cmd = f'spim -asm -db -dl -nmio -nt -ne -file {filename} -assemble -dump'
subprocess.call(cmd, shell=True)

print(cmd)

output_filename = f'{filename}.out'

with open(output_filename, 'r') as f:
    # second line
    line = f.readlines()[1]
    
    insts = line.removeprefix(".word").strip().split(", ")
    
    insts = list(map(lambda x: int(x, base=16), insts))
    
    with open(hex_filename, "w") as f:
        # fill zero to be 32bits instructions
        for inst in insts[0x100000:]:
            f.write(f'{inst:08x}\n')
