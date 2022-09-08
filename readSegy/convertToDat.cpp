/*
 * Copyright (c) 2021-2022 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @update: 2022-09-08 add cmdline support
 *
 * @file: convertToDat.cpp
 * @brief: Convert a segy file to a binary file without any header.
 *         If there are some missing traces,
 *         `convertToDat` will pad them with a fill number (default 0).
 */

#include <algorithm>
#include <cmath>
#include <iostream>

#include "cmdline.h"
#include "segy.h"

int main(int argc, char **argv) {
  cmdline::parser args;
  args.add<std::string>("infile", 'i', "input Segy file name", true);
  args.add<std::string>("outfile", 'o', "output binary file name", false,
                        "out.dat");
  args.add<int>("iloc", '\0', "location for inline number in trace header",
                false, -1);
  args.add<int>("xloc", '\0', "location for crossline number in trace header",
                false, -1);
  args.add<int>("range", 'r', 4,
                "[inmin, inmax, xmin. xmax], the range of in/x-line", false,
                cmdline::vectors<int>(-1, -1, -1, -1));
  args.add<std::string>(
      "fill", 'f',
      "the number to fill the miss trace, can be any float or nan, or NAN",
      false, "0.0");
  args.add("help", 'h', "Convert a segy file to a binary file");

  std::string brief =
      "Convert a segy file to a binary file without any header. \nIf there are "
      "some missing traces, this program will pad them with a fill number "
      "(default 0).";

  args.add_overview(brief);
  args.add_examples("./convertToDat -i f3.segy");
  args.add_examples("./convertToDat -i f3.segy -o f3.dat");
  args.add_examples(
      "./convertToDat -i f3.segy -o f3.dat --nt 100 --nx 105 --ni 200");
  args.add_examples("./convertToDat -i f3.segy -o f3.dat -f nan");
  args.add_examples(
      "./convertToDat -i f3.segy -o out.dat --iloc 5 --xloc 21 -r 100 500 20 "
      "800");

  args.parse(argc, argv);
  if (argc == 1 || args.exist("help")) {
    std::cerr << args.usage();
    return 0;
  }

  args.parse_check(argc, argv);

  if (args.exist("iloc") ^ args.exist("xloc")) {
    std::cerr << "--iloc and --xloc must be (or not be) specialized together"
              << std::endl;
    return 0;
  }

  SegyFile segy(args.get<std::string>("infile"));

  std::vector<int> rg = args.get<int>("range", 0);

  if (rg[1] < rg[0] || rg[3] < rg[2]) {
    std::cerr << "Invalid range: " << rg[0] << " " << rg[1] << " " << rg[2]
              << " " << rg[3] << std::endl;
  }

  if (args.get<int>("iloc") >= 0 && args.get<int>("xloc") >= 0) {
    if (rg[0] >= 0 && rg[1] >= 0 && rg[2] >= 0 && rg[3] >= 0) {
      segy.setParameters(args.get<int>("iloc"), args.get<int>("xloc"), rg[0],
                         rg[1], rg[2], rg[3]);
    } else {
      segy.setParameters(args.get<int>("iloc"), args.get<int>("xloc"));
    }
  }

  float fills;
  if (args.get<std::string>("fill") == "nan" ||
      args.get<std::string>("fill") == "NAN") {
    fills = NAN;
  } else {
    try {
      fills = std::stof(args.get<std::string>("fill"));
    } catch (std::invalid_argument) {
      std::cerr << "Invalid number args --fill or -f: "
                << args.get<std::string>("fill") << std::endl
                << "Please input a number (e.g. 0, 1.2, -100) or nan or NAN"
                << std::endl;
      std::cerr << args.usage();
      return 0;
    }
  }
  segy.toDat(args.get<std::string>("outfile"), fills);
}
