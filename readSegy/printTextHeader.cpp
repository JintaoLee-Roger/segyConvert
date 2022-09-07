/*
 * Copyright (c) 2022 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @update: 2022-06-21 add cmdline support
 *
 * @file: printTextHeader.cpp
 * @brief: Print the 3200 bytes text header.
 */

#include <iostream>

#include "cmdline.h"
#include "segy.h"

int main(int argc, char** argv) {
  cmdline::parser args;
  args.add<std::string>("infile", 'i', "input segy file name", true);
  args.add("help", 'h', "print the 3200 bytes text header");

  std::string brif = "Print the 3200 bytes text header of a segy file.\n";

  args.parse(argc, argv);
  if (argc == 1 || args.exist("help")) {
    std::cerr << brif << std::endl;
    std::cerr << args.usage();
    return 0;
  }
  args.parse_check(argc, argv);

  SegyFile f(args.get<std::string>("infile"));
  f.printTextHeader();
}