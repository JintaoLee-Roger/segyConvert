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
g++ -std=c++11 -O3 -o printTextHeader segy.cpp printTextHeader.cpp
g++ -std=c++11 -O3 -o scan scan.cpp segy.cpp
g++ -std=c++11 -O3 -o convertToDat convertToDat.cpp segy.cpp
```

这三个文件可以在 [**Releases**](https://github.com/JintaoLee-Roger/segyConvert/releases) 中直接下载。

### 运行

输出 3200 bytes 的文本格式道头
```shell
# 查看帮助文档
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

扫描一个segy文件, 获取 in-line 和 cross-line 的范围
```shell
# 查看帮助文档
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

# 不知道 in-line 和 cross-line 在 trace header 中的存储位置, 
# 会先猜测 in-line 和 cross-line 的存储位置
./scan -i /home/user/Documents/test.segy

# 知道存储位置, 比如 in-line: 5, cross-line: 21
./scan -i /home/user/Documents/test.segy --iloc 5 --xloc 21
```

将segy格式的三维地震数据转化为没有道头信息的二进制格式数据. 
如果segy格式的数据缺失了某些地震道，`convertToDat` 将会用 `fill` 填充。
```shell
# 查看帮助文档
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

- [ ] 判断是3维数据还是2维数据 (目前无法处理二维数据且不会报错, 会得到错误的结果)
- [ ] 增加 python 的调用接口