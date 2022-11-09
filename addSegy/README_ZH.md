# addSegy

<table>
  <tr>
    <td><a href="./README.md">English</a></td>
    <td><b>中文</b></td>
  </tr>
</table>

一个 C++ 写的将没有道头信息的二进制格式三维地震数据添加道头信息，转化为segy格式的三维地震数据。

关于 SEGY-Y 格式: [SEG-Y rev 1](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev1.pdf), [SEG-Y rev 2](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev2_0-mar2017.pdf) 和 [A comparison of all version](https://wiki.seg.org/images/4/42/SEG-Y_bytestream_all_revisions.pdf).

## 使用

### 编译

```shell
g++ -std=c++11 -O3 -o convertToSegy addSegy.cpp convertToSegy.cpp
```

文件 `convertTosegy` 可以在 [**Releases**](https://github.com/JintaoLee-Roger/segyConvert/releases) 中直接下载。

### Run

将二进制文件转化为segy格式
```shell
# 帮助文档
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

- [x] 将 4-byte 的 IBM 浮点数转为 4-byte 的 IEEE 浮点数
- [x] 将 4-byte 的 IEEE 浮点数转为 4-byte 的 IBM 浮点数
- [ ] 增加 python 的调用接口

