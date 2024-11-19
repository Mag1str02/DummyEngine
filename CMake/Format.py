import os
import subprocess
for root, dirs, files in os.walk("."):
    for file in files:
        if file.endswith(".cpp") or file.endswith(".hpp") or file.endswith(".c") or file.endswith(".h"):
             subprocess.run(["clang-format", "-i", "-style=file", os.path.join(root, file)])