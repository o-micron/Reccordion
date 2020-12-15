use std::env;
use std::path::Path;

fn main() {
    let dir = env::var("CARGO_MANIFEST_DIR").unwrap();

    #[cfg(debug_assertions)]
    {
        #[cfg(target_os = "windows")]
        {
            println!("\n✔︎ DEBUG MODE\n✔︎ WINDOWS\n\n");
            let p = Path::new(&dir).join("..").join("..").join("..").join("build").join("bin").join("Debug");
            println!("cargo:rustc-link-lib=dylib=Reccordion");
            println!("cargo:rustc-link-search={}", p.display());
        }
    }

    #[cfg(not(debug_assertions))]
    {
        #[cfg(target_os = "windows")]
        {
            println!("\n✔︎ RELEASE MODE\n✔︎ WINDOWS\n\n");
            let p = Path::new(&dir).join("..").join("..").join("..").join("build").join("bin").join("Release");
            println!("cargo:rustc-link-lib=dylib=Reccordion");
            println!("cargo:rustc-link-search={}", p.display());
        }
    }
}
