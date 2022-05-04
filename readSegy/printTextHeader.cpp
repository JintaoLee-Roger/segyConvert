/*
 * Copyright (c) 2021 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @version: v0.1
 * @date: 2021-05-29
 * @update: 2021-08-06 Optimized error output
 *
 * @file: printTextHeader.cpp
 * @brief: Print the 3200 bytes text header.
 */

#include <iostream>

#include "segy.h"

int main(int argc, char** argv) {
    std::string help =
        "Help: print the 3200 bytes text header of a segy format file \n"
        "Usage: printTextHeader infile \n"
        "param: \n"
        "       infile: input segy format file name, e.g. RMS.segy.\n";

    if (argc == 1) {
        std::cout << help;
        exit(1);
    } else if (2 != argc) {
        std::cout << "Input error! Only one input parameter (segy file's name) "
                     "is necessary."
                  << std::endl
                  << help;
        exit(1);
    }

    SegyFile f(argv[1]);
    f.printTextHeader();
}