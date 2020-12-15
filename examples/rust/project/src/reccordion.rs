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

extern crate libc;
use libc::{c_int, c_char};

#[link(name = "Reccordion", kind = "dylib")]
extern "C" {
    #[allow(dead_code)]
    fn ReccordionInitialize() -> bool;

    #[allow(dead_code)]
    fn ReccordionDeinitialize();

    #[allow(dead_code)]
    fn ReccordionEnableLogRedirection();

    #[allow(dead_code)]
    fn ReccordionDisableLogRedirection();

    #[allow(dead_code)]
    fn ReccordionGetScreenWidth() -> c_int;

    #[allow(dead_code)]
    fn ReccordionGetScreenHeight() -> c_int;

    #[allow(dead_code)]
    fn ReccordionStartRecording(directory: *const c_char, name: *const c_char, x: c_int, y: c_int, width: c_int, height: c_int);

    #[allow(dead_code)]
    fn ReccordionStopRecording();
}

#[allow(dead_code)]
pub struct Reccordion {}

impl Reccordion {

    #[allow(dead_code)]
    pub fn initialize() -> bool {
        unsafe { ReccordionInitialize() }
    }

    #[allow(dead_code)]
    pub fn deinitialize() {
        unsafe { ReccordionDeinitialize() }
    }

    #[allow(dead_code)]
    pub fn enable_log_redirection() {
        unsafe { ReccordionEnableLogRedirection() }
    }

    #[allow(dead_code)]
    pub fn disable_log_redirection() {
        unsafe { ReccordionDisableLogRedirection() }
    }

    #[allow(dead_code)]
    pub fn get_screen_width() -> i32 {
        unsafe { ReccordionGetScreenWidth() }
    }

    #[allow(dead_code)]
    pub fn get_screen_height() -> i32 {
        unsafe { ReccordionGetScreenHeight() }
    }

    #[allow(dead_code)]
    pub fn start_recording(directory: String, name: String, x: i32, y: i32, width: i32, height: i32) {
        let mut cdirectory: Vec<c_char> = directory.bytes().map(|b| b as c_char).collect();
        cdirectory.push('\0' as c_char);
        let mut cname: Vec<c_char> = name.bytes().map(|b| b as c_char).collect();
        cname.push('\0' as c_char);
        unsafe { ReccordionStartRecording(cdirectory.as_ptr() as *const c_char, cname.as_ptr() as *const c_char, x, y, width, height) }
    }

    #[allow(dead_code)]
    pub fn stop_recording() {
        unsafe { ReccordionStopRecording() }
    }

}