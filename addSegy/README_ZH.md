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

- [x] 将 4-byte 的 IBM 浮点数转为 4-byte 的 IEEE 浮点数
- [ ] 将 4-byte 的 IEEE 浮点数转为 4-byte 的 IBM 浮点数

