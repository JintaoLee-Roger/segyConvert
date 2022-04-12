/*
 * Copyright (c) 2021 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 * 
 * @author: Jintao Li
 * @version: v0.1
 * @date: 2021-08-06
 * 
 * @file: addSegy.h
 * @brief: A class for adding segy format header information to binary file,
 *          e.g. convert a binary file to segy format file.
 */

#ifndef ADDSEGY_H_

#include <fstream>
#include <map>
#include <vector>
#include <climits>

struct keys{
    std::string outName;
    size_t ns; // ns
    size_t nxline; // xline
    size_t ninline; // inline
    size_t dt;
    size_t sxline;
    size_t sinline;
    bool big_endian;
    short dtype;
    size_t iloc;
    size_t xloc;
};


const std::map<char, unsigned char> kASCIItoEBCDICmap = {
    {'.', 75},  {'<', 76},   {'(', 77},  {'+', 78},  {'|', 79},  {'&', 80},
    {'!', 90},  {'$', 91},   {'*', 92},  {')', 93},  {';', 94},  {'-', 96},
    {'/', 97},  {'|', 106},  {',', 107}, {'%', 108}, {'_', 109}, {'>', 110},
    {'?', 111}, {'`', 121},  {':', 122}, {'#', 123}, {'@', 124}, {'\'', 125},
    {'=', 126}, {'"', 127},  {'a', 129}, {'b', 130}, {'c', 131}, {'d', 132},
    {'e', 133}, {'f', 134},  {'g', 135}, {'h', 136}, {'i', 137}, {'j', 145},
    {'k', 146}, {'l', 147},  {'m', 148}, {'n', 149}, {'o', 150}, {'p', 151},
    {'q', 152}, {'r', 153},  {'~', 161}, {'s', 162}, {'t', 163}, {'u', 164},
    {'v', 165}, {'w', 166},  {'x', 167}, {'y', 168}, {'z', 169}, {'{', 192},
    {'A', 193}, {'B', 194},  {'C', 195}, {'D', 196}, {'E', 197}, {'F', 198},
    {'G', 199}, {'H', 200},  {'I', 201}, {'}', 208}, {'J', 209}, {'K', 210},
    {'L', 211}, {'M', 212},  {'N', 213}, {'O', 214}, {'P', 215}, {'Q', 216},
    {'R', 217}, {'\\', 224}, {'S', 226}, {'T', 227}, {'U', 228}, {'V', 229},
    {'W', 230}, {'X', 231},  {'Y', 232}, {'Z', 233}, {'0', 240}, {'1', 241},
    {'2', 242}, {'3', 243},  {'4', 244}, {'5', 245}, {'6', 246}, {'7', 247},
    {'8', 248}, {'9', 249}
};

class AddSegy{
public:
    char textheader[3200] = 
"C01 SEGY OUTPUT FROM AddSegy by Jintao Li (CIG, USTC, 2021)                     "\
"C02 github: https://github.com/JintaoLee-Roger/segyConvert                      "\
"C03                                                                             "\
"C04 Name: OUT_AddSegy                                                           "\
"C05 Type: 3D seismic  Created Time: 2021/08/04T15:02                            "\
"C06                                                                             "\
"C07 First inline: 1000   Last inline: 1569                                      "\
"C08 First xline:  1      Last xline:  1835                                      "\
"C09                                                                             "\
"C10 Sample interval: 4000   microsecond                                         "\
"C11 Number of samples per data trace: 1260                                      "\
"C12 Byte endian: BIG_ENDIAN                                                     "\
"C13 Data sample format: 4-byte IEEE floating-point                              "\
"C14                                                                             "\
"C15                                                                             "\
"C16                                                                             "\
"C17                                                                             "\
"C18 Binary header locations:                                                    "\
"C19 Sample interval             : bytes 17-18                                   "\
"C20 Number of samples per trace : bytes 21-22                                   "\
"C21 Data sample format code     : bytes 25-26                                   "\
"C22                                                                             "\
"C23 Trace header locations:                                                     "\
"C24 Inline number               : bytes 5-8                                     "\
"C25 Xline number                : bytes 21-24                                   "\
"C26 X coordinate                : bytes 73-76                                   "\
"C27 Y coordinate                : bytes 77-80                                   "\
"C28 Trace start time/depth      : bytes 109-110                                 "\
"C29 Number of samples per trace : bytes 115-116                                 "\
"C30 Sample interval             : bytes 117-118                                 "\
"C31                                                                             "\
"C32                                                                             "\
"C33                                                                             "\
"C34                                                                             "\
"C35                                                                             "\
"C36                                                                             "\
"C37                                                                             "\
"C38                                                                             "\
"C39                                                                             "\
"C40 END EBCDIC                                                                 ";

    char binaryheader[400] = {0};
    char traceheader[240] = {0};
    keys _key{"OUT_AddSegy", 0, 0, 0, 4000, 1, 1, true, 5, 5, 21};

    std::fstream in_;
    std::fstream out_;

    AddSegy(const std::string infile, 
            const int ns, const int nxline, const int ninline);
    ~AddSegy();

    void setSampleInterval(int t); // default 4000 microsecond
    void setLoc(int iloc, int xloc); // default 5, 17
    void setStart(int is, int xs); // default 1, 1
    void setOutName(const std::string outfile);
    void convert();

private:
    void writeTextHeader();
    void updateTextHeader();
    void updateBinaryHeader();
    void initialTraceHeader();
    void updateTraceHeader(int32_t inum, int32_t xnum, int32_t x, int32_t y);
    void replaceStr(char *t, const std::string s, size_t start, size_t len);
    template <typename T> void replaceStr (char *t, T u, size_t start);
    char getEBCIDFromASCII(char c);
    void readTrace(std::vector<float>& trace, int64_t loc);
    void writeTrace();
    template <typename T> T swap_endian(T u);
    // TODO: complete 4-byte ieee floating-point to 4-byte ibm floating-point 
    float ieee_to_ibm(float value, bool is_litte_endian_input);

};

template <typename T> void AddSegy::replaceStr (char *t, T u, size_t start){
    if (_key.big_endian)
        u = swap_endian(u);
    char *p = reinterpret_cast<char *>(&u);
    for (int i = 0; i < sizeof(T); ++i){
        t[start + i] = *(p+i);
    }
}

template <typename T> T AddSegy::swap_endian (T u){
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union{
        T u;
        unsigned char u8[sizeof(T)];    
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

#endif
