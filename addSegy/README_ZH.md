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
g++ -o convertToSegy addSegy.cpp convertToSegy.cpp
```

文件 `convertTosegy` 可以在 [**Releases**](https://github.com/JintaoLee-Roger/segyConvert/releases) 中直接下载。

### Run

将二进制文件转化为segy格式
```shell
# 帮助文档
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

# e.g. 
./convertToSegy -i yourfile.dat --nt 100 --nx 1835 --ni 540

# 指定输出名字
./convertToSegy -i yourfile.dat --nt 100 --nx 1835 --ni 540 -o outfile

# 指定时间间隔
./convertToSegy -i yourfile.dat --nt 100 --nx 1835 --ni 540 -o outfile --dt 4000

# 指定开始的 inline 和 crossline number
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

- [x] 将 4-byte 的 IBM 浮点数转为 4-byte 的 IEEE 浮点数
- [x] 将 4-byte 的 IEEE 浮点数转为 4-byte 的 IBM 浮点数
- [ ] 增加 python 的调用接口

