/// --------------------------------------------------------------------------------------------------------
/// MIT License
/// --------------------------------------------------------------------------------------------------------
/// Copyright (c) 2020 Omar Sherif Fathy
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
/// --------------------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------------------
/// SECTIONS OF THIS FILE
/// --------------------------------------------------------------------------------------------------------
/// - RECCORDION WRAPPER CLASS
/// --------------------------------------------------------------------------------------------------------

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class Reccordion {
    private static native boolean ReccordionInitialize();

    private static native void ReccordionDeinitialize();

    private static native void ReccordionEnableLogRedirection();

    private static native void ReccordionDisableLogRedirection();

    private static native int ReccordionGetScreenWidth();

    private static native int ReccordionGetScreenHeight();

    private static native void ReccordionStartRecording(String directory, String name, int x, int y, int width, int height);

    private static native void ReccordionStopRecording();

    public static void loadSystemLibraries(boolean release) {
        String LibrariesDirectory = "build/bin/";
        String[] libraries;
        if (release) {
            LibrariesDirectory += "Release/";
            libraries = new String[] {
                "jpeg62",
                "lzma",
                "zlib1",
                "libpng16",
                "tiff",
                "webp",
                "opencv_core",
                "opencv_imgproc",
                "opencv_imgcodecs",
                "opencv_videoio",
                "Reccordion",
                "ReccordionJava"
            };
        } else {
            LibrariesDirectory += "Debug/";
            libraries = new String[] {
                "jpeg62",
                "lzmad",
                "zlibd1",
                "libpng16d",
                "tiffd",
                "webpd",
                "opencv_cored",
                "opencv_imgprocd",
                "opencv_imgcodecsd",
                "opencv_videoiod",
                "Reccordion",
                "ReccordionJava"
            };   
        }
        for (int i = 0; i < libraries.length; ++i) {
            System.load(new File(LibrariesDirectory + System.mapLibraryName(libraries[i])).getAbsolutePath());
        }
    }

    public static boolean initialize() {
        return ReccordionInitialize();
    }

    public static void deinitialize() {
        ReccordionDeinitialize();
    }

    public static void enableLogRedirection() {
        ReccordionEnableLogRedirection();
    }

    public static void disableLogRedirection() {
        ReccordionDisableLogRedirection();
    }

    public static int getScreenWidth() {
        return ReccordionGetScreenWidth();
    }

    public static int getScreenHeight() {
        return ReccordionGetScreenHeight();
    }

    public static void startRecording(String directory, String name, int x, int y, int width, int height) {
        ReccordionStartRecording(directory, name, x, y, width, height);
    }

    public static void stopRecording() {
        ReccordionStopRecording();
    }
}
