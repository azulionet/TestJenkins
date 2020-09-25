import os
import sys
import shutil

print("move_files")

if len(sys.argv) != 4:
    quit()

# root path
root = sys.argv[1]

# mode : debug or release
mode = sys.argv[2]

# dest
dest = sys.argv[3]

outFrom = root + "\\GPA\\build_jenkins\\outputs\\" + mode
outDest = root + dest

if os.path.exists(outDest):
    shutil.rmtree(outDest)

# python version 3.8+
shutil.copytree(outFrom, outDest)

# delete CTS folder
if os.path.exists(outDest + "\\CTS"):
    shutil.rmtree(outDest + "\\CTS")

print("move_files end")
