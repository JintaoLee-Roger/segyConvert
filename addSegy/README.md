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
# usage: convertToSegy --infile=string --nt=int --nx=int --ni=int [options] ...
# options:
#   -i, --infile     input binary file path (string)
#       --nt         ns or nt, i.e. number of samples per tarce (int)
#       --nx         crossline number, i.e. number of traces per inline (int)
#       --ni         inline number, i.e. number of traces per crossline (int)
#   -o, --outfile    output file name, e.g. mytest (string [=OUT_AddSegy])
#       --dt         Sample interval in microseconds (int [=4000])
#       --sxline     The first crossline number (int [=1])
#       --sinline    The first inline number (int [=1])
#   -h, --help       Convert binary file to Segy format file.
# Example:
# convertToSegy -i test.dat --nt 200 --nx 250 --ni 100 -o test
# convertToSegy -i test.dat --nt 200 --nx 250 --ni 100 -o test --dt 2000

# e.g. 1
./convertToSegy -i yourfile.dat --nt 100 --nx 1835 --ni 540

# specialize out name
./convertToSegy -i yourfile.dat --nt 100 --nx 1835 --ni 540 -o outfile

# add sample interval
./convertToSegy -i yourfile.dat --nt 100 --nx 1835 --ni 540 -o outfile --dt 4000

# specializa the first crossline/inline number
./convertToSegy -i yourfile.dat --nt 100 --nx 1835 --ni 540 -o outfile --dt 4000 --sxline 20 --sinline 50
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

