import os
import sys

os.system("echo GPAINSTALLER_START")

ar = sys.argv[2].split(".")

# make nsis version format ( e.g. x.x.x.x // GPA version format x.x.x )
if len(ar) == 3:
    sys.argv[2] += ".0"

# name, version, debug
if len(sys.argv) == 4:
    os.system("makensis /V1 /DGPA_GAMENAME=" + sys.argv[1] + " /DGPA_VERSION=" + sys.argv[2] + " /DGPA_DEBUG=" + sys.argv[3] + " MakeGPA.nis")
else:
    os.system("makensis /V1 MakeGPA.nis")

os.system("echo GPAINSTALLER_END")
