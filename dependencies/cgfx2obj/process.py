############################################################################
#   Project Name: strip3ds
#   Description:  Tool for extracting resources from 3DS ROMs.
#
#   Copyright (C) 2024 Martin Louvel
#
#   This file is part of strip3ds.
#
#   Permission is hereby granted, free of charge, to any person obtaining 
#   a copy of this software and associated documentation files (the 
#   "Software"), to deal in the Software without restriction, including 
#   without limitation other than credits to the original author
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom
#   the Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be 
#   included in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
#   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
#   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
#   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
#   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
#   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
#   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
############################################################################

import os
import subprocess
from tqdm import tqdm

# Paths (replace these with your actual paths)
script_directory = os.path.dirname(os.path.realpath(__file__))
if not script_directory.endswith("/"):
    script_directory += "/"
dependencies = os.path.join(script_directory, "../")
python_executable = os.path.join(dependencies, "python2/python.exe")
folder = os.path.join(dependencies, "../")

# File extensions to search for
extensions = [".bcmdl", ".bcres", ".cgfx"]

# Function to find files with the given extensions
def find_files(directory, extensions):
    for root, _, files in os.walk(directory):
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                yield os.path.join(root, file)

# Find files
files = list(find_files(folder, extensions))
total_files = len(files)

# Progress bar format
bar_format = "[{bar}] {percentage:3.0f}% | File: {n_fmt}/{total_fmt} || {elapsed} | {remaining}"

# Process each file with a progress bar
with tqdm(total=total_files, unit="file", ncols=100, bar_format=bar_format) as progress_bar:
    for file_index, file_path in enumerate(files, 1):
        # Message before processing
        tqdm.write(f"    - Processing: {file_path}")
        
        output_file = f"{file_path}.obj"
        with open(output_file, "w") as output:
            command = [
                python_executable, 
                os.path.join(dependencies, "cgfx2obj/cgfx2obj.py"), 
                file_path
            ]
            subprocess.run(command, stdout=output, stderr=subprocess.STDOUT)
        
        # Message after processing
        tqdm.write(f"    - Output saved to {output_file}.")
        tqdm.write(f"    - Done.\n")
        
        # Update the progress bar
        progress_bar.update(1)
