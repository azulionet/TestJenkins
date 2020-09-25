import os
import sys
import shutil

os.system("echo move_folder_unity_version")

if len(sys.argv) != 3:
    quit()

# unity plugin root
root = sys.argv[1]

# version
versionFolder = root + "BUILD_" + sys.argv[2]
dest = versionFolder + "\\Assets\\"

if os.path.exists(versionFolder):
    shutil.rmtree(versionFolder)

src = root + "\\Assets\\"

# python version 3.8+
shutil.copytree(src, dest)

os.system("echo move_folder_unity_version_end")
