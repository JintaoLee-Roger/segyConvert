/*
 * Copyright (c) 2021 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @version: v0.1
 * @date: 2021-08-06
 *
 * @file: convertToSegy.cpp
 * @brief: Convert a binary file to a segy format file.
 */

#include <iostream>

#include "addSegy.h"

int main(int argc, char** argv) {
    std::string p =
        "Help: Convert binary file to Segy format file. \n"
        "Usage: convertToSegy infile n1 n2 n3 [outfile dt sxline sinline]\n"
        "param: \n"
        "       infile: binary file path, e.g. mybin.dat\n"
        "       n1:     ns, i.e. number of samples per data trace\n"
        "       n2:     crossline number, i.e. number of traces per inline\n"
        "       n3:     inline number, i.e. number of traces per crossline\n"
        "       outfile (optional): output file name, e.g. mytest, default: OUT_AddSegy\n"
        "       dt (optional): Sample interval, default: 4000 microsecond\n"
        "       sxline (optional): The first crossline number, default: 1\n"
        "       sinline (optional): The first inline number, default: 1\n";

    if (argc == 1) {
        std::cout << p;
        exit(1);
    } else if (argc < 5) {
        std::cout << "Input error! You can enter 4/5/6/8 parameters."
                  << std::endl;
        std::cout << p;
        exit(1);
    }

    size_t ns = std::stoi(argv[2]);
    size_t nxline = std::stoi(argv[3]);
    size_t ninline = std::stoi(argv[4]);

    AddSegy segy(argv[1], ns, nxline, ninline);

    if (argc > 5) {
        segy.setOutName(argv[5]);
        if (argc > 6) {
            segy.setSampleInterval(std::stoi(argv[6]));
            if (argc > 7) {
                if (argc == 9) {
                    segy.setStart(std::stoi(argv[8]), std::stoi(argv[7]));
                } else {
                    std::cout
                        << "Input error! You can enter 4/5/6/8 parameters."
                        << std::endl;
                    std::cout << p;
                    exit(1);
                }
            }
        }
    }

    segy.convert();
}