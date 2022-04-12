# addSegy

<table>
  <tr>
    <td><b>English</b></td>
    <td><a href="./README_ZH.md">中文</a></td>
  </tr>
</table>

A tool with C++ to convert 3D seismic binary data to SEG-Y format.

About SEGY-Y format: [SEG-Y rev 1](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev1.pdf), [SEG-Y rev 2](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev2_0-mar2017.pdf) and [A comparison of all version](https://wiki.seg.org/images/4/42/SEG-Y_bytestream_all_revisions.pdf).

## Usage

### Compile

```shell
g++ -o convertToSegy addSegy.cpp convertToSegy.cpp
```

The file `convertTosegy` is also avaliable in [**Releases**](https://github.com/JintaoLee-Roger/segyConvert/releases)

### Run

Convert a binary file to a segy format file.
```shell
# get help
./convertToSegy
# Usage: convertToSegy infile n1 n2 n3 [outfile dt sxline sinline]
# param:
#        infile: binary file path, e.g. mybin.dat
#        n1:     ns, i.e. number of samples per data trace
#        n2:     crossline number, i.e. number of traces per inline
#        n3:     inline number, i.e. number of traces per crossline
#        outfile (optional): output file name, e.g. mytest, default: OUT_AddSegy
#        dt (optional): Sample interval, default: 4000 microsecond
#        sxline (optional): The first crossline number, default: 1
#        sinline (optional): The first inline number, default: 1

# 4 parameters
./convertToSegy yourfile.dat 100 1835 540

# 5 parameters
./convertToSegy yourfile.dat 100 1835 540 outfile

# 6 parameters
./convertToSegy yourfile.dat 100 1835 540 outfile 4000

# 8 parameters
./convertToSegy yourfile.dat 100 1835 540 outfile 4000 20 50
```

### API
Some API
```c++
AddSegy(const std::string infile, 
            const int ns, const int nxline, const int ninline);

void setSampleInterval(int t); // default 4000 microsecond
void setLoc(int iloc, int xloc); // default 5, 17
void setStart(int is, int xs); // default 1, 1
void setOutName(const std::string outfile);
void convert();
```

### TODO List

- [x] stored in 4-byte IEEE floating-point format
- [x] stored in 4-byte IBM floating-point format
- [ ] add python API

