
# 注意: 此仓库已经被废弃，不再更新。请转移到新的更友好的仓库: [cigsegy](https://github.com/JintaoLee-Roger/cigsegy)

# segyConvert

<table>
  <tr>
    <td><a href="https://github.com/JintaoLee-Roger/segyConvert">English</a></td>
    <td><b>中文</b></td>
  </tr>
</table>

一个使用 C++ 写的将**三维**地震数据在**segy格式**和**二进制格式**之间互相转换的工具，可以将三维地震数据从segy格式去道头、补全空道，最后转化为二进制格式的数据，也可以将二进制格式的数据添加道头信息，转化为segy格式的数据。

segy format <------> binary format

关于 segy 格式: [SEG-Y rev 1](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev1.pdf), [SEG-Y rev 2](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev2_0-mar2017.pdf) 和 [A comparison of all version](https://wiki.seg.org/images/4/42/SEG-Y_bytestream_all_revisions.pdf).

一些代码参考了Google的 [segy-stack](https://github.com/google/segy-stack).

`cmdline.h` 来自于 Tanaka 的 [cmdline](https://github.com/tanakh/cmdline).

## 用法

### 编译

```shell
# readSegy
g++ -std=c++11 -O3 -o printTextHeader segy.cpp printTextHeader.cpp
g++ -std=c++11 -O3 -o scan scan.cpp segy.cpp
g++ -std=c++11 -O3 -o convertToDat convertToDat.cpp segy.cpp

# addSegy
g++ -std=c++11 -O3 -o convertToSegy addSegy.cpp convertToSegy.cpp
```

这四个文件也可以直接在 [**Releases**](https://github.com/JintaoLee-Roger/segyConvert/releases) 中下载。

### 运行

`printTextHeader`, `scan` 和 `convertToDat` 的详细使用方法见 [**readSegy**](readSegy/README_ZH.md) 

`convertToSegy` 的详细使用方法见 [**addSegy**](addSegy/README_ZH.md)


### TODO List

- [x] 将 4-byte 的 IBM 浮点数转为 4-byte 的 IEEE 浮点数
- [x] 将 4-byte 的 IEEE 浮点数转为 4-byte 的 IBM 浮点数
- [ ] 判断是3维数据还是2维数据 (目前无法处理二维数据且不会报错, 会得到错误的结果)
- [ ] 增加 python 的调用接口
