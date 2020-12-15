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
/// - RECCORDION RUST EXAMPLE
/// --------------------------------------------------------------------------------------------------------

mod reccordion;

use reccordion::Reccordion;

fn main() {
    let equal = std::iter::repeat("=").take(60).collect::<String>();
    println!("{}", format!("\n\n{0}\nSTARTING RUST VIDEO RECORDING EXAMPLE\n{0}\n", &equal));
    if Reccordion::initialize() {
        Reccordion::enable_log_redirection();
        let directory = String::from("output/");
        let name = String::from("rust_video");
        Reccordion::start_recording(directory, name, 0, 0, Reccordion::get_screen_width(), Reccordion::get_screen_height());
        print!("Recording ..");
        std::thread::sleep(std::time::Duration::from_millis(10000));
        println!(" done");
        Reccordion::stop_recording();
        Reccordion::deinitialize();
    }
    println!("{}", format!("\n{0}\nENDING RUST VIDEO RECORDING EXAMPLE.\n{0}\n\n", &equal));
}
