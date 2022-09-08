# readSegy

<table>
  <tr>
    <td><b>English</b></td>
    <td><a href="./README_ZH.md">中文</a></td>
  </tr>
</table>

A tool with C++ to read 3D seismic data in SEG-Y format.

About SEGY-Y format: [SEG-Y rev 1](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev1.pdf), [SEG-Y rev 2](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev2_0-mar2017.pdf) and [A comparison of all version](https://wiki.seg.org/images/4/42/SEG-Y_bytestream_all_revisions.pdf).

Some code references Google's respository: [segy-stack](https://github.com/google/segy-stack).


## Usage

### Compile

```shell
g++ -o printTextHeader segy.cpp printTextHeader.cpp
g++ -o scan scan.cpp segy.cpp
g++ -o convertToDat convertToDat.cpp segy.cpp
```

The three files are also avaliable in [**Releases**](https://github.com/JintaoLee-Roger/segyConvert/releases)

### Run

Print the 3200 bytes text header.
```shell
# get help
./printTextHeader
# Print the 3200 bytes text header of a segy file.

# usage: printTextHeader --infile=string [options] ...
# options:
#   -i, --infile    input segy file name (string)
#   -h, --help      print the 3200 bytes text header
#
# Examples: 
#   ./printTextHeader f3.segy

./printTextHeader -i /home/user/Documents/test.segy
```

Scan a segy file, obtain the range of in-line and cross-line numbers.
```shell
# get help
./scan
# Scan a segy file, obtain the range of in-line and cross-line numbers.

# usage: scan --infile=string [options] ...
# options:
#   -i, --infile    input segy format file name (string)
#       --iloc      location for inline number in trace header (int [=-1])
#       --xloc      location for inline number in trace header (int [=-1])
#   -h, --help      Scan a segy file
#
# Examples: 
#   ./scan -i f3.segy
#   ./scan -i f3.segy --iloc 5 --ixloc 17

# don't know the location of in-line and cross-line.
./scan -i /home/user/Documents/test.segy

# know the location, such as in-line in 5, cross-line in 21
./scan -i /home/user/Documents/test.segy --iloc 5 --xloc 21
```

Convert a segy file to a binary file without any header. If there are some 
missing traces, `convertToDat` will pad them with `fill`.
```shell
# get help
./convertToDat
# Convert a segy file to a binary file without any header.
# If there are some missing traces, this program will pad them with a fill number (default 0).

# usage: convertToDat --infile=string [options] ...
# options:
#   -i, --infile     input Segy file name (string)
#   -o, --outfile    output binary file name (string [=out.dat])
#       --iloc       location for inline number in trace header (int [=-1])
#       --xloc       location for crossline number in trace header (int [=-1])
#       --inmin      the mininum number of inline (int [=-1])
#       --inmax      the maxinum number of inline (int [=-1])
#       --xmin       the mininum number of crossline (int [=-1])
#       --xmax       the maxinum number of crossline (int [=-1])
#   -f, --fill       the number to fill the miss trace, can be any float or nan, or NAN (string [=0.0])
#   -h, --help       Convert a segy file to a binary file
#
# Examples: 
#   ./convertToDat -i f3.segy
#   ./convertToDat -i f3.segy -o f3.dat
#   ./convertToDat -i f3.segy -o f3.dat --nt 100 --nx 105 --ni 200
#   ./convertToDat -i f3.segy -o f3.dat -f nan
#   ./convertToDat -i f3.segy -o out.dat --iloc 5 --xloc 21 --inmin 1000 --inmax 1569 --xmin 1 --xmax 1835

# The more parameters entered, the less time used

./convertToDat -i /home/user/Documents/test.segy -o out.dat

# know the location, such as in-line in 5, cross-line in 21
./convertToDat -i /home/user/Documents/test.segy -o out.dat --iloc 5 --xloc 21

# fill the missing traces with any float or nan or NAN
./convertToDat -i /home/user/Documents/test.segy -o out.dat -f nan


# know the location and range, such as in-line in 5, cross-line in 21,
# in-line range: 1000-1569, cross-line range: 1-1835  
./convertToDat -i /home/user/Documents/test.segy -o out.dat --iloc 5 --xloc 21 --inmin 1000 --inmax 1569 --xmin 1 --xmax 1835
```

### API
Some API
```c++
// class construct
SegyFile segy(const std::string segyname); 

// 3200 bytes text header 
void SegyFile::printTextHeader();

// 400 bytes binary header (not all of them)
void SegyFile::getBinaryHeader();

// set some parameters: in-line storage location, cross-line storage location
void SegyFile::setParameters(size_t iloc, size_t xloc);
// override, add more parameters: in-line range and cross-line range
void SegyFile::setParameters(size_t iloc, size_t xloc, 
                   size_t imin, size_t imax, 
                   size_t xmin, size_t xmax);

// if you don't know in-line and cross-line location, use this.
void SegyFile::guessLoc();

// scan the range of in-line and cross-line number
void SegyFile::scan();

// convert to a binary file without any header, and padding
void SegyFile::toDat(const std::string outfile="out.dat");

// two header parameters
binaryHeader SegyFile::binaryHeader_;
traceHeader SegyFile::th;

// binaryHeader and traceHeader's definition not in SegyFile class

// 400 bytes binary header, stored in 3200-3600 bytes
struct binaryHeader{
    short dt;   // Sample interval, 17-18 (17-1 in cpp)
    short ns;   // Number of samples per data trace, 21-22
    short dformat;  //Data sample format code, 25-26
    short numExHeader;  // Number of 3200-byte, Extended Textual File Header, 305-306
    int numTrailer;     // Number of 3200-byte data trailer stanza records, 329-332
    size_t totalTrace; // Total traces in segy file. need to calculate
};

// 240 bytes trace header
struct traceHeader{
    int inlLoc;   // in-line number, 5-8 or 9-12 or 189-192
    int xlLoc;    // cross-line number, 17-20 or 21-24 or 193-196
    int inmax;    // maximum of in-line number, need to scan or set
    int xlmax;    // minimum of in-line number, ...
    int inmin;    // maximum of cross-line number, ...
    int xlmin;    // minimum of cross-line number, ...
};
```

### TODO List

- [ ] Determine whether it is three-dimensional or two-dimensional data
- [ ] Add python API
