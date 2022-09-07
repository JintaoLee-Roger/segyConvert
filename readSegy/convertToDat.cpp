/*
 * Copyright (c) 2022 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @update: 2022-06-21 add cmdline support
 *
 * @file: convertToDat.cpp
 * @brief: Convert a segy file to a binary file without any header.
 *         If there are some missing traces,
 *         `convertToDat` will pad them with a fill number (default 0).
 */

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
  args.add<int>("inmin", '\0', "the mininum number of inline", false, -1);
  args.add<int>("inmax", '\0', "the maxinum number of inline", false, -1);
  args.add<int>("xmin", '\0', "the mininum number of crossline", false, -1);
  args.add<int>("xmax", '\0', "the maxinum number of crossline", false, -1);
  args.add<std::string>(
      "fill", 'f',
      "the number to fill the miss trace, can be any float or nan, or NAN",
      false, "0.0");
  args.add("help", 'h', "Convert a segy file to a binary file");

  std::string brief =
      "Convert a segy file to a binary file without any header. \nIf there are "
      "some missing traces, this program will pad them with a fill number "
      "(default 0). \n";

  args.parse(argc, argv);
  if (argc == 1 || args.exist("help")) {
    std::cerr << brief << std::endl;
    std::cerr << args.usage();
    return 0;
  }

  args.parse_check(argc, argv);

  SegyFile segy(args.get<std::string>("infile"));

  if (args.get<int>("iloc") >= 0 && args.get<int>("xloc") >= 0) {
    if (args.get<int>("inmin") >= 0 && args.get<int>("inmax") >= 0 &&
        args.get<int>("xmin") >= 0 && args.get<int>("xmax") >= 0) {
      segy.setParameters(args.get<int>("iloc"), args.get<int>("xloc"),
                         args.get<int>("inmin"), args.get<int>("inmax"),
                         args.get<int>("xmin"), args.get<int>("xmax"));
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
