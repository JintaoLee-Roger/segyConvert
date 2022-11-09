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
g++ -std=c++11 -O3 -o convertToSegy addSegy.cpp convertToSegy.cpp
```

The file `convertTosegy` is also avaliable in [**Releases**](https://github.com/JintaoLee-Roger/segyConvert/releases)

### Run

Convert a binary file to a segy format file.
```shell
# get help
./convertToSegy
# Convert a binary file to a segy format file

# usage: convertToSegy --infile=string --dims=int [options] ... 
# options:
#   -i, --infile     input binary file path (string)
#   -n, --dims       dimentions, the order: nt nx ni (int)
#   -o, --outfile    output file name, e.g. mytest.segy (string [=OUT_AddSegy.segy])
#   -d, --dt         Sample interval in microseconds (int [=4000])
#   -c, --sxline     The first crossline number (int [=1])
#   -s, --sinline    The first inline number (int [=1])
#   -x, --dx         The interval of x (float [=25])
#   -y, --dy         The interval of y (float [=25])
#   -t, --dtype      Data type(4 bytes float): 1 for IBM, 5 for IEEE (short [=5])
#   -h, --help       Convert binary file to Segy format file.

# Examples: 
#   ./convertToSegy -i test.dat -n 200 250 100 -o test.segy
#   ./convertToSegy -i test.dat -n 200 250 100 -o test.segy -d 2000
#   ./convertToSegy -i test.dat -n 200 250 100 -o test.segy -s 20 -x 10
#   ./convertToSegy -i test.dat -n 200 250 100 -o test.segy -t 1
#   ./convertToSegy -i test.dat -n 200 250 100 -o test.segy -x 12.5
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

