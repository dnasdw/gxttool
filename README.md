# gxttool

A tool for exporting/importing gxt file.

## History

- v0.1.0 @ 2018.04.22 - First pre release

## Platforms

- Windows
- Linux
- macOS

## Building

### Dependencies

- cmake
- zlib
- libpng

### Compiling

- make 64-bit version
~~~
mkdir build
cd build
cmake -DUSE_DEP=OFF ..
make
~~~

- make 32-bit version
~~~
mkdir build
cd build
cmake -DBUILD64=OFF -DUSE_DEP=OFF ..
make
~~~

### Installing

~~~
make install
~~~

## Usage

### Windows

~~~
gxttool [option...] [option]...
~~~

### Other

~~~
gxttool.sh [option...] [option]...
~~~

## Options

See `gxttool --help` messages.
