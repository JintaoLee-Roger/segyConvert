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
 * @file: convertToDat.cpp
 * @brief: Convert a segy file to a binary file without any header. 
 *         If there are some missing traces, 
 *         `convertToDat` will pad them with 0.
 */

#include "segy.h"
#include <iostream>

int main(int argc, char** argv){
    std::string help = 
"Help: Convert segy format file to binary file.\n" \
"Usage: convertToDat infile outfile [iloc xloc [inmin inmax xmin xmax]]\n" \
"Param: \n" \
"       infile: input segy file name, e.g. RMS.segy \n" \
"       outfile: output binary file name, e.g. out.dat \n" \
"       iloc (opt): location for inline number in trace header, e.g. 5\n" \
"       xloc (opt): location for crossline number in trace header, e.g. 21\n" \
"       inmin (opt): the minimum number of inline, e.g. 1000 \n" \
"       inmax (opt): the maximum number of inline, e.g. 1800 \n" \
"       xmin (opt): the minimum number of crossline, e.g. 50 \n" \
"       xmax (opt): the maximum number of crossline, e.g. 580 \n" \
"The more parameters entered, the less time used\n";

    if (argc == 1){
        std::cout << help;
        exit(1);
    }
    else if (!(3 == argc || 5 == argc || 9 == argc)){
        std::cout << "Input error! You can enter 2/4/8 parameters." << std::endl
                  << help;
        exit(1);
    }

    SegyFile segy(argv[1]);

    if (3 < argc){
        size_t inLoc = std::stoi(argv[3]);
        size_t xLoc = std::stoi(argv[4]);
        if (5 == argc){
            segy.setParameters(inLoc, xLoc);
        }
        else if(9 == argc){
            size_t inmin = std::stoi(argv[5]);
            size_t inmax = std::stoi(argv[6]);
            size_t xmin = std::stoi(argv[7]);
            size_t xmax = std::stoi(argv[8]);
            segy.setParameters(inLoc, xLoc, inmin, inmax, xmin, xmax);
        }
    }
    segy.toDat(argv[2]);
}
