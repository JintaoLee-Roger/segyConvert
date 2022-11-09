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
g++ -std=c++11 -O3 -o printTextHeader segy.cpp printTextHeader.cpp
g++ -std=c++11 -O3 -o scan scan.cpp segy.cpp
g++ -std=c++11 -O3 -o convertToDat convertToDat.cpp segy.cpp
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
#   -r, --range      [inmin, inmax, xmin. xmax], the range of in/x-line (int [= -1 -1 -1 -1 ])
#   -f, --fill       the number to fill the miss trace, can be any float or nan, or NAN (string [=0.0])
#   -h, --help       Convert a segy file to a binary file

# Examples: 
#   ./convertToDat -i f3.segy
#   ./convertToDat -i f3.segy -o f3.dat
#   ./convertToDat -i f3.segy -o f3.dat --nt 100 --nx 105 --ni 200
#   ./convertToDat -i f3.segy -o f3.dat -f nan
#   ./convertToDat -i f3.segy -o out.dat --iloc 5 --xloc 21 -r 100 500 20 800
```

### TODO List

- [ ] Determine whether it is three-dimensional or two-dimensional data
- [ ] Add python API
