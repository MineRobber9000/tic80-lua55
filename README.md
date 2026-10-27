# TIC-80 Lua 5.5

A Lua 5.5.1-based core for TIC-80's dynamic script runtime system.

## How to build

Clone this repository next to TIC-80, and use cmake to build it. You should get a `lua55.dll` (or `.dylib` or `.so`, depending on platform).

You will, unfortunately, have to build TIC-80 yourself, since modern TIC-80 builds the language runtimes statically by default, but if you do, you can toss the shared library into the same directory as the TIC-80 binary and create a lua cart with `-- script: lua55`. Now you can use the new features in Lua that have come since 5.3.6.
