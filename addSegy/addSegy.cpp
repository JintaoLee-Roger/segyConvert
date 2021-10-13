/*
 * Copyright (c) 2021 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 * 
 * @author: Jintao Li
 * @version: v0.1
 * @date: 2021-08-06
 * 
 * @file: addSegy.cpp
 * @brief: definition of SegyFile class
 */

#include "addSegy.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>

AddSegy::AddSegy(const std::string infile, 
            const int ns, const int nxline, const int ninline){
    this->in_.open(infile, std::ios::in | std::ios::binary);
    if (!this->in_){
        std::cout << "Open file: " << infile << " failure" << std::endl;
        exit(1);
    }

    in_.seekg(0, std::ios::end);
    uintmax_t flength = in_.tellg();
    if (ns * nxline * ninline * sizeof(float) != flength){
        std::cout << "Error dimension, " << ns << "*" << ninline 
        << "*" << nxline << "*sizeof(float) is " 
        << ns * nxline * ninline * sizeof(float) 
        << ", which is not equal to the file size: " << flength << std::endl;
        exit(1);
    }
    in_.seekg(0, std::ios::beg);

    this->_key.ns = ns;
    this->_key.nxline = nxline;
    this->_key.ninline = ninline;
    updateTextHeader();
    updateBinaryHeader();
    initialTraceHeader();
}

AddSegy::~AddSegy(){
    in_.close();
    if (out_){
        out_.close();
    }
}

void AddSegy::setSampleInterval(int t){
    _key.dt = t;
}

void AddSegy::setLoc(int iloc, int xloc){
    _key.iloc = iloc;
    _key.xloc = xloc;
}

void AddSegy::setStart(int is, int xs){
    _key.sinline = is;
    _key.sxline = xs;
}

void AddSegy::setOutName(const std::string outfile){
    _key.outName = outfile;
}

void AddSegy::convert(){
    updateTextHeader();
    updateBinaryHeader();
    initialTraceHeader();

    out_.open(_key.outName+".segy", std::ios::out | std::ios::binary);

    writeTextHeader();
    out_.write(&binaryheader[0], 400);

    std::vector<float> trace(_key.ns, 0);
    int64_t loc = 0;
    for (int i = _key.sinline; i < (_key.sinline+_key.ninline); ++i){
        for (int j = _key.sxline; j < (_key.sxline+_key.nxline); ++j){
            // x and y interval ~ 25 40 12.5
            updateTraceHeader(i, j, 10000+i*2500, 10000+j*2500); 
            out_.write(&traceheader[0], 240);
            readTrace(trace, loc);
            out_.write((char *)&trace[0], _key.ns*sizeof(float));
            loc += (_key.ns*sizeof(float));
        }
    }
}

char AddSegy::getEBCIDFromASCII(char c){
    if (kASCIItoEBCDICmap.find(c) != kASCIItoEBCDICmap.end()){
        return kASCIItoEBCDICmap.at(c);
    }
    return ' ';
}

void AddSegy::writeTextHeader(){
    char eb_textheader[3200];
    for (int i = 0; i < 3200; ++i){
        eb_textheader[i] = getEBCIDFromASCII(textheader[i]);
    }
    out_.write(&eb_textheader[0], 3200);
}

void AddSegy::updateTextHeader(){
    replaceStr(textheader, _key.outName, 80*3+10, 70);

    std::stringstream s;
    time_t now = time(0);
    s << std::put_time(std::localtime(&now), "%Y/%m/%dT%H:%M");
    replaceStr(textheader, s.str(), 80*4+36, 20);

    replaceStr(textheader, std::to_string(_key.sinline), 80*6+18, 6);
    replaceStr(textheader, std::to_string(_key.sinline + _key.ninline - 1), 80*6+38, 6);
    replaceStr(textheader, std::to_string(_key.sxline), 80*7+18, 6);
    replaceStr(textheader, std::to_string(_key.sxline + _key.nxline - 1), 80*7+38, 6);
    replaceStr(textheader, std::to_string(_key.dt), 80*9+21, 6);
    replaceStr(textheader, std::to_string(_key.ns), 80*10+38, 6);
    if (!_key.big_endian)
        replaceStr(textheader, "LITTLE_ENDIAN", 80*11+17, 50);
    if (_key.dtype == 1) // 1 or 5
        replaceStr(textheader, "4-byte IBM floating-point", 80*12+24, 50);

    if (_key.iloc == 5){
        replaceStr(textheader, "5-8", 80*23+40, 20);
    }
    else if (_key.iloc == 9){
        replaceStr(textheader, "9-12", 80*23+40, 20);
    }
    else if (_key.iloc == 189){
        replaceStr(textheader, "189-192", 80*23+40, 20);
    }
    else {
        std::cout << "please don't save inline number in " << _key.iloc << std::endl;
    }

    if (_key.xloc == 17){
        replaceStr(textheader, "17-20", 80*24+40, 20);
    }
    else if (_key.xloc == 21){
        replaceStr(textheader, "21-24", 80*24+40, 20);
    }
    else if (_key.xloc == 193){
        replaceStr(textheader, "193-196", 80*24+40, 20);
    }
    else {
        std::cout << "please don't save xline number in " << _key.xloc << std::endl;
    }
    
}

void AddSegy::updateBinaryHeader(){
    replaceStr(binaryheader, (int32_t)1, 0);
    replaceStr(binaryheader, (int32_t)1, 8);
    replaceStr(binaryheader, (int16_t)1, 12);
    replaceStr(binaryheader, (int16_t)_key.dt, 16);
    replaceStr(binaryheader, (int16_t)_key.ns, 20);
    replaceStr(binaryheader, (int16_t)_key.dtype, 24);
    replaceStr(binaryheader, (int16_t)1, 26);
    replaceStr(binaryheader, (int16_t)4, 28);
    replaceStr(binaryheader, (int16_t)1, 30);
    replaceStr(binaryheader, (int16_t)1, 54);
}

void AddSegy::initialTraceHeader(){
    replaceStr(traceheader, (int32_t)1, 0);
    replaceStr(traceheader, (int32_t)_key.sinline, 4);
    replaceStr(traceheader, (int32_t)_key.sinline, 8);
    replaceStr(traceheader, (int32_t)1, 12);
    replaceStr(traceheader, (int32_t)_key.sxline, 16);
    replaceStr(traceheader, (int32_t)_key.sxline, 20);
    replaceStr(traceheader, (int32_t)1, 24);
    replaceStr(traceheader, (int32_t)1, 28);
    replaceStr(traceheader, (int16_t)1, 34);
    replaceStr(traceheader, (int16_t)1, 68);
    replaceStr(traceheader, (int16_t)-100, 70); // scale 
    replaceStr(traceheader, (int16_t)1, 88);
    replaceStr(traceheader, (int16_t)_key.ns, 114);
    replaceStr(traceheader, (int16_t)_key.dt, 116);
    replaceStr(traceheader, (int16_t)1, 124);
    replaceStr(traceheader, (int16_t)1, 132);
    replaceStr(traceheader, (int16_t)1, 138);
    replaceStr(traceheader, (int32_t)_key.sinline, 188);
    replaceStr(traceheader, (int32_t)_key.sxline, 192);
}

void AddSegy::updateTraceHeader(int32_t inum, int32_t xnum, int32_t x, int32_t y){
    replaceStr(traceheader, (int32_t)inum, 4);
    replaceStr(traceheader, (int32_t)xnum, 8);
    replaceStr(traceheader, (int32_t)inum, 16);
    replaceStr(traceheader, (int32_t)xnum, 20);
    replaceStr(traceheader, (int32_t)inum, 188);
    replaceStr(traceheader, (int32_t)xnum, 192);
    replaceStr(traceheader, (int32_t)x, 72);
    replaceStr(traceheader, (int32_t)y, 76);
    replaceStr(traceheader, (int32_t)x, 180);
    replaceStr(traceheader, (int32_t)y, 184);
}

void AddSegy::replaceStr(char* t, const std::string s, size_t start, size_t len){
    if (s.length() > len){
        std::cout << "Warning! input number is out of range" << std::endl;
    }
    for (int i = 0; i < len; ++i){
        t[start + i] = (i < s.length()) ? s[i] : ' ';
    }
}

void AddSegy::readTrace(std::vector<float>& trace, int64_t loc){
    in_.seekg(loc, std::ios::beg);
    in_.read((char *)&trace[0], _key.ns * sizeof(float));

    // TODO: complete ieee format to ibm format
    for(auto &i : trace){
        // if (1 == _key.dtype){
        //     i = ieee_to_ibm(i, true);
        // }
        // else {
            i = swap_endian(i);
        // }
    }   
}
