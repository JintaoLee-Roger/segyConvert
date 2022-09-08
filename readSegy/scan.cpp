/*
 * Copyright (c) 2021-2022 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @update: 2022-09-08 add cmdline support
 *
 * @file: scan.cpp
 * @brief: Scan a segy file, obtain the range of in-line and cross-line numbers.
 */

#include <iostream>

#include "cmdline.h"
#include "segy.h"

int main(int argc, char** argv) {
  cmdline::parser args;
  args.add<std::string>("infile", 'i', "input segy format file name", true);
  args.add<int>("iloc", 0, "location for inline number in trace header", false,
                -1);
  args.add<int>("xloc", 0, "location for inline number in trace header", false,
                -1);
  args.add("help", 'h', "Scan a segy file");

  std::string brif =
      "Scan a segy file, obtain the range of in-line and cross-line numbers.";
  args.add_overview(brif);
  args.add_examples("./scan -i f3.segy");
  args.add_examples("./scan -i f3.segy --iloc 5 --ixloc 17");

  args.parse(argc, argv);
  if (argc == 1 || args.exist("help")) {
    std::cerr << args.usage();
    return 0;
  }
  args.parse_check(argc, argv);

  if (args.exist("iloc") ^ args.exist("xloc")) {
    std::cerr << "--iloc and --xloc must be (or not be) specialized together."
              << std::endl;
    return 0;
  }

  SegyFile segy(args.get<std::string>("infile"));

  if (args.get<int>("iloc") >= 0 && args.get<int>("xloc") >= 0) {
    segy.setParameters(args.get<int>("iloc"), args.get<int>("xloc"));
  }

  segy.scan();
}