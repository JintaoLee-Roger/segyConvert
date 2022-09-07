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

#include "../readSegy/cmdline.h"
#include "addSegy.h"

int main(int argc, char** argv) {
  cmdline::parser args;
  args.add<std::string>("infile", 'i', "input binary file path", true);
  args.add<int>("nt", '\0', "ns or nt, i.e. number of samples per tarce", true);
  args.add<int>("nx", '\0',
                "crossline number, i.e. number of traces per inline", true);
  args.add<int>("ni", '\0',
                "inline number, i.e. number of traces per crossline", true);
  args.add<std::string>("outfile", 'o', "output file name, e.g. mytest", false,
                        "OUT_AddSegy");
  args.add<int>("dt", '\0', "Sample interval in microseconds", false, 4000);
  args.add<int>("sxline", '\0', "The first crossline number", false, 1);
  args.add<int>("sinline", '\0', "The first inline number", false, 1);
  args.add("help", 'h', "Convert binary file to Segy format file.");
  std::string exp =
      "Example: \n"
      "convertToSegy -i test.dat --nt 200 --nx 250 --ni 100 -o test \n"
      "convertToSegy -i test.dat --nt 200 --nx 250 --ni 100 -o test --dt 2000";

  args.parse(argc, argv);
  if (argc == 1 || args.exist("help")) {
    std::cerr << args.usage();
    std::cerr << exp << std::endl;
    return 0;
  }

  args.parse_check(argc, argv);

  size_t nt = args.get<int>("nt");
  size_t nx = args.get<int>("nx");
  size_t ni = args.get<int>("ni");
  size_t dt = args.get<int>("dt");
  size_t sx = args.get<int>("sxline");
  size_t si = args.get<int>("sinline");
  std::string infile = args.get<std::string>("infile");
  std::string outname = args.get<std::string>("outfile");

  AddSegy segy(infile, nt, nx, ni);
  segy.setSampleInterval(dt);
  segy.setStart(si, sx);
  segy.setOutName(outname);
  segy.convert();
}