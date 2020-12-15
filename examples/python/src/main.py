# --------------------------------------------------------------------------------------------------------
# MIT License
# --------------------------------------------------------------------------------------------------------
# Copyright (c) 2020 Omar Sherif Fathy
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# --------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------
# SECTIONS OF THIS FILE
# --------------------------------------------------------------------------------------------------------
# - RECCORDION PYTHON EXAMPLE
# --------------------------------------------------------------------------------------------------------

import time
from reccordion import Reccordion

print("\n\n{}\nSTARTING PYTHON VIDEO RECORDING EXAMPLE\n{}\n".format('=' * 60, '=' * 60))
Reccordion.loadSystemLibraries(True)
if Reccordion.initialize():
    Reccordion.enableLogRedirection()
    Reccordion.startRecording(b'output/', b'python_video', 0, 0, 0, 0)
    print("Recording ..", end='')
    time.sleep(10)
    print(" done")
    Reccordion.stopRecording()
    Reccordion.deinitialize()
print("\n{}\nENDING PYTHON VIDEO RECORDING EXAMPLE.\n{}\n\n".format('=' * 60, '=' * 60))
