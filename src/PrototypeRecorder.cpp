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
/// - DESIGN (NO DESIGN ACTUALLY, BASIC LOOP ... )
/// - STRUCTURES
/// - STATIC VARIABLES
/// - FREE INTERNAL FUNCTIONS
/// - EXPORTS
/// --------------------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------------------
/// HELPFUL LINKS (DOCS FOR THE CORE)
/// --------------------------------------------------------------------------------------------------------
/// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchblt
/// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getdibits
/// --------------------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------------------
/// LIMITATIONS
/// --------------------------------------------------------------------------------------------------------
/// - COULD CRASH WHEN CHANGING SCREEN RESOLUTION MIDST RECORDING (VERY UNLICKLY BUT POSSIBLE)
/// - THE VIDEO SIZE IS HUGE COMPARED TO THE QUALITY (NO COMPRESSION, I REALLY JUST TRANSFER BUFFERS)
/// --------------------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------------------
/// TODO
/// --------------------------------------------------------------------------------------------------------
/// - [ ] TRY COMPRESS THE FRAMES SEQUENCE WHEN POSSIBLE IN REALTIME
/// - [ ] TRY TO COMPRESS THE WHOLE VIDEO AT THE VERY END OF THE CAPTURE (SHOULD BE A LOT EASIER)
/// --------------------------------------------------------------------------------------------------------

#include "../include/Reccordion/Reccordion.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <opencv2/opencv.hpp>

#define WIN32_LEAN_AND_MEAN
#include <ShellScalingApi.h>
#include <windows.h>

/// --------------------------------------------------------------------------------------------------------
/// CONSTANTS
/// --------------------------------------------------------------------------------------------------------
#define RECCORDION_FPS                      30.0
#define RECCORDION_ASSET(A)                 std::string(MainAssetsDirectory).append(A)
#define RECCORDION_CERR_FILESTREAM_LOG_FILE RECCORDION_ASSET("LoggedError.txt")
#define RECCORDION_CIN_FILESTREAM_LOG_FILE  RECCORDION_ASSET("LoggedInput.txt")
#define RECCORDION_COUT_FILESTREAM_LOG_FILE RECCORDION_ASSET("LoggedOutput.txt")
/// --------------------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------------------
/// DESIGN (NO DESIGN ACTUALLY, BASIC LOOP ... )
/// --------------------------------------------------------------------------------------------------------
///
///  --> CAPTURE --> ?? (add more blocks here, post processing for example ...) --> STREAM --> |
///  |                                                                                         |
///  <-- <-- <-- XX <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <--
///                |
///                |
///                |    --------------
///                --- / STOP BUTTON /
///                    --------------
/// --------------------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------------------
/// STRUCTURES
/// --------------------------------------------------------------------------------------------------------
template<typename T>
struct vec2
{
    union
    {
        struct
        {
            T _0;
            T _1;
        };
        struct
        {
            T x;
            T y;
        };
        struct
        {
            T data[2];
        };
    };
};

template<typename T>
struct vec3
{
    union
    {
        struct
        {
            T _0;
            T _1;
            T _2;
        };
        struct
        {
            T x;
            T y;
            T z;
        };
        struct
        {
            T data[3];
        };
        struct
        {
            vec2<T> xy;
            T       _z;
        };
        struct
        {
            T       _x;
            vec2<T> yz;
        };
    };
};

template<typename T>
struct vec4
{
    union
    {
        struct
        {
            T _0;
            T _1;
            T _2;
            T _3;
        };
        struct
        {
            T x;
            T y;
            T z;
            T w;
        };
        struct
        {
            T data[4];
        };
        struct
        {
            vec3<T> xyz;
            T       __w;
        };
        struct
        {
            T       __x;
            vec3<T> yzw;
        };
        struct
        {
            vec2<T> xy;
            vec2<T> zw;
        };
    };
};

enum ReccordionPipelineState_
{
    ReccordionPipelineState_Idle = 0,
    ReccordionPipelineState_Running,
    ReccordionPipelineState_Paused
};

enum ReccordionPipelineStage_
{
    ReccordionPipelineStage_Capture = 0,

    // insert extra stages here ..

    // ----------------------------------------------------------------------
    // MARK: INTERMEDIATE STAGE EXAMPLE
    // ----------------------------------------------------------------------
    // ReccordionPipelineStage_Flip,
    // ----------------------------------------------------------------------

    ReccordionPipelineStage_Stream,
    ReccordionPipelineStage_Count
};

typedef std::function<void()> ReccordionPipelineCommand;

struct ReccordionPipeline
{
    ReccordionPipeline() {}
    ~ReccordionPipeline() { stop(); }
    void clearStageCommand(ReccordionPipelineStage_ stage) noexcept
    {
        {
            std::unique_lock<std::mutex> lock(_eventMutex);
            _commands[stage].clear();
        }
        _eventVar.notify_one();
    }
    void pushStageCommand(const ReccordionPipelineCommand command, ReccordionPipelineStage_ stage) noexcept
    {
        {
            std::unique_lock<std::mutex> lock(_eventMutex);
            _commands[stage].push_back(std::move(command));
        }
        _eventVar.notify_one();
    }
    bool isLeaking()
    {
        for (size_t i = 0; i < ReccordionPipelineStage_Count; ++i) {
            if (_commands[i].empty()) { return true; }
        }
        return false;
    }
    void reset()
    {
        {
            std::unique_lock<std::mutex> lock(_eventMutex);
            for (size_t stage = 0; stage < ReccordionPipelineStage_Count; ++stage) { _commands[stage].clear(); }
            _stopped.store(false);
            _capturedCount.store(0);
            _processedCount.store(0);
        }
    }

    void run()
    {
        if (isLeaking()) { return; }

        _threads.emplace_back(std::thread([&]() {
            using std::literals::chrono_literals::operator""s;

            constexpr std::chrono::duration<double> DELTA_TIME = 1.0s / RECCORDION_FPS;
            auto                                    nextStep   = std::chrono::system_clock::now() + DELTA_TIME;
            {
                std::unique_lock<std::mutex> lock(_eventMutex);
                _eventVar.wait(lock, [=]() { return !_commands[ReccordionPipelineStage_Capture].empty(); });
            }
            while (!_stopped.load()) {
                _commands[ReccordionPipelineStage_Capture][0]();
                ++_capturedCount;

                std::this_thread::sleep_until(nextStep);
                nextStep += DELTA_TIME;
            }
        }));

        _threads.emplace_back(std::thread([&]() {
            {
                std::unique_lock<std::mutex> lock(_eventMutex);
                _eventVar.wait(lock, [=]() { return !_commands[ReccordionPipelineStage_Capture].empty(); });
            }
            while (_capturedCount.load() > _processedCount.load() || !_stopped.load()) {
                while (_capturedCount.load() > _processedCount.load()) {
                    uint64_t difference = _capturedCount.load() - _processedCount.load();
                    for (uint64_t d = 0; d < difference; ++d) {
                        for (size_t i = ReccordionPipelineStage_Capture + 1; i < ReccordionPipelineStage_Count; ++i) {
                            _commands[i][0]();
                        }
                    }
                    _processedCount.store(_processedCount.load() + difference);
                }
            }
        }));
    }
    void stop()
    {
        {
            std::unique_lock<std::mutex> lock(_eventMutex);
            _stopped.store(true);
        }
        _eventVar.notify_all();
        for (std::thread& thread : _threads) {
            if (thread.joinable()) { thread.join(); }
        }
    }

    std::atomic_bool                                                                  _stopped;
    std::atomic_uint64_t                                                              _capturedCount;
    std::atomic_uint64_t                                                              _processedCount;
    std::condition_variable                                                           _eventVar;
    std::mutex                                                                        _eventMutex;
    std::vector<std::thread>                                                          _threads;
    std::array<std::vector<ReccordionPipelineCommand>, ReccordionPipelineStage_Count> _commands;
};
/// --------------------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------------------
/// STATIC VARIABLES
/// --------------------------------------------------------------------------------------------------------
static ReccordionPipelineState_ MainRecordingState;            // Saves the recording state
static std::queue<cv::Mat>      MainCapturedImages;            // Holds the captured images until they're written to stream
static ReccordionPipeline       MainPipeline;                  // Thread pool to capture and stream frames asynchronously
static cv::VideoWriter          MainVideoWriter;               // Main Video writer
static HWND                     MainDesktopHwnd;               // Desktop Window Handle
static HDC                      MainWindowHandleDeviceContext; // Window handle device context
static HDC                      MainWindowHandleCompatibleDeviceContext; // Compatible Device context
static HBITMAP                  MainWindowHandleCompatibleBitmap;        // The bitmap
static BITMAPINFOHEADER         MainBitmapInfoHeader;                    // The info header of the bitmap
static vec4<int32_t>            MainVideoCoordinates; // The desired coordinates (Left-Top corner) and (Width-Height)
static cv::Mat                  MainFrame;            // Holds the pixels data, or the frame buffer data
static std::streambuf*          MainCerrBufferRef;    // Reference to the default standard error stream buffer
static std::ifstream            MainErrorFileStream;  // Error file stream
static std::streambuf*          MainCinBufferRef;     // Reference to the default standard input stream buffer
static std::ifstream            MainInputFileStream;  // Input file stream
static std::streambuf*          MainCoutBufferRef;    // Reference to the default standard output stream buffer
static std::ifstream            MainOutputFileStream; // Output file stream
static std::string              MainAssetsDirectory;  // Main assets directory
static bool                     MainRedirectsStream;  // (Cin, Cerr, Cout) rediction switch
/// --------------------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------------------
/// INTERNAL FUNCTIONS
/// --------------------------------------------------------------------------------------------------------
int
ReccordionErrorHandler(int status, const char* func_name, const char* err_msg, const char* file_name, int line, void*)
{
    // handle errors here if you want ...
    return 0;
}

void
ReccordionSwitchStdStreams()
{
    {
        MainErrorFileStream = std::ifstream(RECCORDION_CERR_FILESTREAM_LOG_FILE);
        if (!MainErrorFileStream.is_open()) {
            std::ofstream errfile(RECCORDION_CERR_FILESTREAM_LOG_FILE);
            errfile.close();
            MainErrorFileStream = std::ifstream(RECCORDION_CERR_FILESTREAM_LOG_FILE);
        }
        MainCerrBufferRef = std::cerr.rdbuf();
        std::cerr.rdbuf(MainErrorFileStream.rdbuf());
    }
    {
        MainInputFileStream = std::ifstream(RECCORDION_CIN_FILESTREAM_LOG_FILE);
        if (!MainInputFileStream.is_open()) {
            std::ofstream infile(RECCORDION_CIN_FILESTREAM_LOG_FILE);
            infile.close();
            MainInputFileStream = std::ifstream(RECCORDION_CIN_FILESTREAM_LOG_FILE);
        }
        MainCinBufferRef = std::cin.rdbuf();
        std::cin.rdbuf(MainInputFileStream.rdbuf());
    }
    {
        MainOutputFileStream = std::ifstream(RECCORDION_COUT_FILESTREAM_LOG_FILE);
        if (!MainOutputFileStream.is_open()) {
            std::ofstream outfile(RECCORDION_COUT_FILESTREAM_LOG_FILE);
            outfile.close();
            MainOutputFileStream = std::ifstream(RECCORDION_COUT_FILESTREAM_LOG_FILE);
        }
        MainCoutBufferRef = std::cout.rdbuf();
        std::cout.rdbuf(MainOutputFileStream.rdbuf());
    }
}

void
ReccordionRevertStdStreams()
{
    {
        MainErrorFileStream.close();
        std::cerr.rdbuf(MainCerrBufferRef);
    }
    {
        MainInputFileStream.close();
        std::cin.rdbuf(MainCinBufferRef);
    }
    {
        MainOutputFileStream.close();
        std::cout.rdbuf(MainCoutBufferRef);
    }
}
/// --------------------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------------------
/// EXPORTS
/// --------------------------------------------------------------------------------------------------------
RECCORDION_EXTERN RECCORDION_API bool
ReccordionInitialize()
{
    MainRecordingState  = ReccordionPipelineState_Idle;
    MainRedirectsStream = true;
    return true;
}

RECCORDION_EXTERN RECCORDION_API void
ReccordionDeinitialize()
{
    ReccordionStopRecording();
    MainRecordingState  = ReccordionPipelineState_Idle;
    MainRedirectsStream = true;
}

RECCORDION_EXTERN RECCORDION_API void
ReccordionEnableLogRedirection()
{
    if (MainRecordingState != ReccordionPipelineState_Idle) { return; }

    MainRedirectsStream = true;
}

RECCORDION_EXTERN RECCORDION_API void
ReccordionDisableLogRedirection()
{
    if (MainRecordingState != ReccordionPipelineState_Idle) { return; }

    MainRedirectsStream = false;
}

RECCORDION_EXTERN RECCORDION_API int32_t
ReccordionGetScreenWidth()
{
    return GetSystemMetricsForDpi(SM_CXVIRTUALSCREEN, GetDpiForSystem());
}

RECCORDION_EXTERN RECCORDION_API int32_t
ReccordionGetScreenHeight()
{
    return GetSystemMetricsForDpi(SM_CYVIRTUALSCREEN, GetDpiForSystem());
}

RECCORDION_EXTERN RECCORDION_API void
ReccordionStartRecording(const char* directory, const char* name, int32_t x, int32_t y, int32_t width, int32_t height)
{
    if (MainRecordingState != ReccordionPipelineState_Idle) { return; }

    MainAssetsDirectory = std::string(directory);

    if (MainRedirectsStream) {
        ReccordionSwitchStdStreams();
        cv::setBreakOnError(false);
        cv::redirectError(ReccordionErrorHandler);
    }

    // Get window handle
    MainDesktopHwnd = GetDesktopWindow();

    // get handles to a device context (DC)
    MainWindowHandleDeviceContext           = GetDC(MainDesktopHwnd);
    MainWindowHandleCompatibleDeviceContext = CreateCompatibleDC(MainWindowHandleDeviceContext);
    SetStretchBltMode(MainWindowHandleCompatibleDeviceContext, COLORONCOLOR);

    // ----------------------------------------------------------------
    if (x <= 0) {
        MainVideoCoordinates.x = 0;
    } else {
        MainVideoCoordinates.x = x;
    }
    if (y <= 0) {
        MainVideoCoordinates.y = 0;
    } else {
        MainVideoCoordinates.y = y;
    }
    if (width <= 0) {
        MainVideoCoordinates.z = GetSystemMetricsForDpi(SM_CXVIRTUALSCREEN, GetDpiForSystem());
    } else {
        MainVideoCoordinates.z = width;
    }
    if (height <= 0) {
        MainVideoCoordinates.w = GetSystemMetricsForDpi(SM_CYVIRTUALSCREEN, GetDpiForSystem());
    } else {
        MainVideoCoordinates.w = height;
    }
    // ----------------------------------------------------------------

    MainWindowHandleCompatibleBitmap =
      CreateCompatibleBitmap(MainWindowHandleDeviceContext, MainVideoCoordinates.z, MainVideoCoordinates.w);
    MainBitmapInfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
    MainBitmapInfoHeader.biWidth         = MainVideoCoordinates.z;
    MainBitmapInfoHeader.biHeight        = -MainVideoCoordinates.w;
    MainBitmapInfoHeader.biPlanes        = 1;
    MainBitmapInfoHeader.biBitCount      = 24;
    MainBitmapInfoHeader.biCompression   = BI_RGB;
    MainBitmapInfoHeader.biSizeImage     = 0;
    MainBitmapInfoHeader.biXPelsPerMeter = 0;
    MainBitmapInfoHeader.biYPelsPerMeter = 0;
    MainBitmapInfoHeader.biClrUsed       = 0;
    MainBitmapInfoHeader.biClrImportant  = 0;

    SelectObject(MainWindowHandleCompatibleDeviceContext, MainWindowHandleCompatibleBitmap);
    try {
        std::filesystem::create_directories(std::filesystem::path(directory));
        std::string fullpath = std::string(directory).append(name).append(".mp4");
        MainVideoWriter      = cv::VideoWriter();
        MainVideoWriter.open(fullpath,
                             cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
                             RECCORDION_FPS,
                             cv::Size(MainVideoCoordinates.z, MainVideoCoordinates.w),
                             true);
    } catch (...) {}

    MainPipeline.reset();

    MainPipeline.clearStageCommand(ReccordionPipelineStage_Capture);
    MainPipeline.pushStageCommand(
      [&]() {
          cv::Mat frame(MainVideoCoordinates.w, MainVideoCoordinates.z, CV_8UC3);
          // copy from the window device context to the bitmap device context
          // https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchblt
          StretchBlt(MainWindowHandleCompatibleDeviceContext, // Handle to the destination device context
                     0,                                       // xDest
                     0,                                       // yDest
                     MainVideoCoordinates.z,                  // widthDest
                     MainVideoCoordinates.w,                  // heightDest
                     MainWindowHandleDeviceContext,           // Handle to the source device context
                     MainVideoCoordinates.x,                  // xSrc
                     MainVideoCoordinates.y,                  // ySrc
                     MainVideoCoordinates.z,                  // widthSrc
                     MainVideoCoordinates.w,                  // heightSrc
                     SRCCOPY);                                // The raster operation to be performed
          // copy from hwindowCompatibleDC to hbwindow
          // https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getdibits
          GetDIBits(MainWindowHandleCompatibleDeviceContext, // Handle to the device context
                    MainWindowHandleCompatibleBitmap,        // Handle to the bitmap
                    0,                                       // The first scan line to retrieve
                    MainVideoCoordinates.w,                  // The number of scan lines to retrieve
                    frame.data,                              // Pointer to the pixel buffer
                    (BITMAPINFO*)&MainBitmapInfoHeader,      // A pointer to a BITMAPINFO structure
                    DIB_RGB_COLORS);                         // The format of the bmiColors
          MainCapturedImages.push(std::move(frame));
      },
      ReccordionPipelineStage_Capture);

    // ----------------------------------------------------------------------
    // MARK: INTERMEDIATE STAGE EXAMPLE
    // ----------------------------------------------------------------------
    // MainPipeline.clearStageCommand(ReccordionPipelineStage_Flip);
    // MainPipeline.pushStageCommand(
    //   [&]() {
    //       if (MainCapturedImages.empty()) { return; }
    //       static cv::Mat flipped;
    //       cv::flip(MainCapturedImages.front(), flipped, 1);
    //       cv::swap(flipped, MainCapturedImages.front());
    //   },
    //   ReccordionPipelineStage_Flip);
    // ----------------------------------------------------------------------

    MainPipeline.clearStageCommand(ReccordionPipelineStage_Stream);
    MainPipeline.pushStageCommand(
      [&]() {
          if (MainCapturedImages.empty()) { return; }
          MainVideoWriter.operator<<(MainCapturedImages.front());
          MainCapturedImages.pop();
      },
      ReccordionPipelineStage_Stream);

    MainPipeline.run();

    MainRecordingState = ReccordionPipelineState_Running;
}

RECCORDION_EXTERN RECCORDION_API void
ReccordionStopRecording()
{
    if (MainRecordingState != ReccordionPipelineState_Running) { return; }

    MainPipeline.stop();
    try {
        MainVideoWriter.release();
    } catch (...) {}

    if (MainRedirectsStream) {
        ReccordionRevertStdStreams();
        cv::redirectError(NULL);
    }

    // avoid memory leaks, or chaos !
    DeleteObject(MainWindowHandleCompatibleBitmap);
    DeleteDC(MainWindowHandleCompatibleDeviceContext);
    ReleaseDC(MainDesktopHwnd, MainWindowHandleDeviceContext);

    MainRecordingState = ReccordionPipelineState_Idle;
}
/// --------------------------------------------------------------------------------------------------------