# readSegy

<table>
  <tr>
    <td><a href="./README.md">English</a></td>
    <td><b>中文</b></td>
  </tr>
</table>

一个使用 C++ 写的读取segy格式三维地震数据，并转化为二进制格式的工具，其会先去掉segy文件的道头，然后补全空道，形成一个规则立方体的数据体，并保存为没有道头信息的二进制文件。

关于 SEGY-Y 格式: [SEG-Y rev 1](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev1.pdf), [SEG-Y rev 2](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev2_0-mar2017.pdf) 和 [A comparison of all version](https://wiki.seg.org/images/4/42/SEG-Y_bytestream_all_revisions.pdf).

一些代码参考了Google的 [segy-stack](https://github.com/google/segy-stack).


## 使用

### 编译

```shell
g++ -o printTextHeader segy.cpp printTextHeader.cpp
g++ -o scan scan.cpp segy.cpp
g++ -o convertToDat convertToDat.cpp segy.cpp
```

这三个文件可以在 [**Releases**](https://github.com/JintaoLee-Roger/segyConvert/releases) 中直接下载。

### 运行

输出 3200 bytes 的文本格式道头
```shell
# 查看帮助文档
./printTextHeader
# Help: print the 3200 bytes text header of a segy format file 
# Usage: printTextHeader infile 
# param: 
#        infile: input segy format file name, e.g. RMS.segy.

./printTextHeader /home/user/Documents/test.segy
```

扫描一个segy文件, 获取 in-line 和 cross-line 的范围
```shell
# 查看帮助文档
./scan
# Help: Scan a segy file, obtain the range of in-line and cross-line numbers. 
# Usage: scan infile [iloc xloc] 
# param: 
#        infile: input segy format file name, e.g. RMS.segy 
#        iloc (opt): location for inline number in trace header, e.g. 5
#        xloc (opt): location for crossline number in trace header, e.g. 21

# 不知道 in-line 和 cross-line 在 trace header 中的存储位置, 
# 会先猜测 in-line 和 cross-line 的存储位置
./scan /home/user/Documents/test.segy

# 知道存储位置, 比如 in-line: 5, cross-line: 21
./scan /home/user/Documents/test.segy 5 21
```

将segy格式的三维地震数据转化为没有道头信息的二进制格式数据. 
如果segy格式的数据缺失了某些地震道，`convertToDat` 将会用 0 填充。
```shell
# 查看帮助文档
./convertToDat
# Help: Convert segy format file to binary file.
# Usage: convertToDat infile outfile [iloc xloc [inmin inmax xmin xmax]]
# Param: 
#        infile: input segy file name, e.g. RMS.segy 
#        outfile: output binary file name, e.g. out.dat 
#        iloc (opt): location for inline number in trace header, e.g. 5
#        xloc (opt): location for crossline number in trace header, e.g. 21
#        inmin (opt): the minimum number of inline, e.g. 1000 
#        inmax (opt): the maximum number of inline, e.g. 1800 
#        xmin (opt): the minimum number of crossline, e.g. 50 
#        xmax (opt): the maximum number of crossline, e.g. 580 
# The more parameters entered, the less time used

./convertToDat /home/user/Documents/test.segy out.dat

# 知道存储位置, 比如 in-line: 5, cross-line: 21
./convertToDat /home/user/Documents/test.segy out.dat 5 21

# 知道存储位置, 比如 in-line: 5, cross-line: 21
# 并知道 in-line 和 cross-line 的范围, 
# 比如 in-line range: 1000-1569, cross-line range: 1-1835  
./convertToDat /home/user/Documents/test.segy out.dat 5 21 1000 1569 1 1835
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

- [ ] 增加 python 的调用接口