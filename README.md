# PGM Compressor

Command line utility for de/compressing PGM image files using Huffman compression algorithm. Compression outputs 2 files; the encoded file (.enc) and the frequency tree (.frq).

## Build
Make sure you have any C++ compiler then build  */src/main.cpp*.

Example using *g++*.
```
$ g++ src/main.cpp -o huff-pgm
```
## Usage
```sh
# Compression
$ huff-pgm image.pgm

# Decompression
$ huff-pgm image.enc -t image.frq
```
There's a *data.zip* folder containing sample PGM image files for testing.