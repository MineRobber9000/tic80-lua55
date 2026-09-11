# TIC-80 Example Core

An example of building a script core outside of the TIC-80 codebase.

## How to build

Clone TIC-80 next to this repository (you don't need to do a recursive checkout; this is just so that this repository has access to the TIC-80 headers).

Run `tool/generate_demo_cart.py` to generate `src/democart.dat`, and then use cmake to build the project. You should get an `examplecore.dll` (or `.dylib` or `.so`, depending on platform).

Drop this dynamic library into a build of TIC-80 1.2 or newer (use the [nightly](https://nightly.link/nesbox/TIC-80/workflows/build/main) if you're unsure) and run TIC-80; you should be able to run `new examplecore` and then run the resulting cart, which should work.
