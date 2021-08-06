# segyConvert

A tool with C++ to read 3D seismic data in SEG-Y format 
and convert 3D seismic data in binary format to SEG-Y format.

segy format <------> binary format

About SEGY-Y format: [SEG-Y rev 1](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev1.pdf), [SEG-Y rev 2](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev2_0-mar2017.pdf) and [A comparison of all version](https://wiki.seg.org/images/4/42/SEG-Y_bytestream_all_revisions.pdf).

Some code references Google's respository: [segy-stack](https://github.com/google/segy-stack).


## Usage

### Compile

```shell
# readSegy
g++ -o printTextHeader segy.cpp printTextHeader.cpp
g++ -o scan scan.cpp segy.cpp
g++ -o convertToDat convertToDat.cpp segy.cpp

# addSegy
g++ -o convrtToSegy addSegy.cpp convrtToSegy.cpp
```

The three files are also avaliable in [**Releases**](https://github.com/JintaoLee-Roger/segyConvert/releases)

### Run

The usage of `printTextHeader`, `scan` and `convertToDat` see [**readSegy**](https://github.com/JintaoLee-Roger/segyConvert/blob/master/readSegy/README.md) 

The usage of `convertToSegy` see [**addSegy**](https://github.com/JintaoLee-Roger/segyConvert/blob/master/addSegy/README.md)


### TODO List

- [x] 4-byte IBM floating-point to 4-byte IEEE floating-point
- [ ] 4-byte IEEE floating-point to 4-byte IBM floating-point
