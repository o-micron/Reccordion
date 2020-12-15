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
# - RECCORDION WRAPPER CLASS
# --------------------------------------------------------------------------------------------------------

import ctypes
import os

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))

class Reccordion():
    """
    A class that acts as a wrapper for the Reccordion C Api
    """
    
    @staticmethod
    def loadSystemLibraries(release: bool):
        DLL_DIR = os.path.join(SCRIPT_DIR, "..", "..", "..", "build", "bin", "Release" if release else "Debug")
        DLL_FILEPATH = os.path.join(DLL_DIR, "Reccordion.dll")
        Reccordion.ReccordionDll = ctypes.cdll.LoadLibrary(DLL_FILEPATH)

    @staticmethod
    def initialize():
        return Reccordion.ReccordionDll.ReccordionInitialize()

    @staticmethod
    def deinitialize():
        Reccordion.ReccordionDll.ReccordionDeinitialize()

    @staticmethod
    def enableLogRedirection():
        Reccordion.ReccordionDll.ReccordionEnableLogRedirection()

    @staticmethod
    def disableLogRedirection():
        Reccordion.ReccordionDll.ReccordionDisableLogRedirection()

    @staticmethod
    def getScreenWidth():
        return Reccordion.ReccordionDll.ReccordionGetScreenWidth()

    @staticmethod
    def getScreenHeight():
        return Reccordion.ReccordionDll.ReccordionGetScreenHeight()

    @staticmethod
    def startRecording(directory, name, x, y, width, height):
        Reccordion.ReccordionDll.ReccordionStartRecording(
            directory, name, x, y, width, height)

    @staticmethod
    def stopRecording():
        Reccordion.ReccordionDll.ReccordionStopRecording()
