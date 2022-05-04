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
 * @file: scan.cpp
 * @brief: Scan a segy file, obtain the range of in-line and cross-line numbers.
 */

#include <iostream>

#include "segy.h"

int main(int argc, char** argv) {
    std::string help =
        "Help: Scan a segy file, obtain the range of in-line and cross-line numbers. \n"
        "Usage: scan infile [iloc xloc] \n"
        "param: \n"
        "       infile: input segy format file name, e.g. RMS.segy \n"
        "       iloc (opt): location for inline number in trace header, e.g. 5\n"
        "       xloc (opt): location for crossline number in trace header, e.g. 21\n";

    if (argc == 1) {
        std::cout << help;
        exit(1);
    } else if (!(2 == argc || 4 == argc)) {
        std::cout << "Input error! You can enter 1/3 parameters." << std::endl
                  << help;
        exit(1);
    }

    SegyFile segy(argv[1]);
    if (4 == argc) {
        size_t inLoc = std::stoi(argv[2]);
        size_t xLoc = std::stoi(argv[3]);
        segy.setParameters(inLoc, xLoc);
    }
    segy.scan();
}