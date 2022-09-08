/*
 * Copyright (c) 2021-2022 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @update: 2022-09-08 add cmdline support
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

  args.add_overview("Print the 3200 bytes text header of a segy file.");
  args.add_examples("./printTextHeader f3.segy");

  args.parse(argc, argv);
  if (argc == 1 || args.exist("help")) {
    std::cerr << args.usage();
    return 0;
  }
  args.parse_check(argc, argv);

  SegyFile f(args.get<std::string>("infile"));
  f.printTextHeader();
}