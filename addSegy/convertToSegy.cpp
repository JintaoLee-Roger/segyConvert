/*
 * Copyright (c) 2021 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @date: 2022-09-08 add cmdline support
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
  args.add<int>("dims", 'n', 3, "dimentions, the order: nt nx ni", true,
                cmdline::vectors(0, 0, 0));
  args.add<std::string>("outfile", 'o', "output file name, e.g. mytest.segy",
                        false, "OUT_AddSegy.segy");
  args.add<int>("dt", 'd', "Sample interval in microseconds", false, 4000);
  args.add<int>("sxline", 'c', "The first crossline number", false, 1);
  args.add<int>("sinline", 's', "The first inline number", false, 1);
  args.add<float>("dx", 'x', "The interval of x", false, 25);
  args.add<float>("dy", 'y', "The interval of y", false, 25);
  args.add<short>("dtype", 't',
                  "Data type(4 bytes float): 1 for IBM, 5 for IEEE", false, 5,
                  cmdline::oneof(1, 5));
  args.add("help", 'h', "Convert binary file to Segy format file.");

  args.add_overview("Convert a binary file to a segy format file");
  args.add_examples("./convertToSegy -i test.dat -n 200 250 100 -o test.segy");
  args.add_examples(
      "./convertToSegy -i test.dat -n 200 250 100 -o test.segy -d 2000");
  args.add_examples(
      "./convertToSegy -i test.dat -n 200 250 100 -o test.segy -s 20 -x 10");
  args.add_examples("./convertToSegy -i test.dat -n 200 250 100 -o test.segy -t 1");
  args.add_examples("./convertToSegy -i test.dat -n 200 250 100 -o test.segy -x 12.5");


  args.parse(argc, argv);
  if (argc == 1 || args.exist("help")) {
    std::cerr << args.usage();
    return 0;
  }

  args.parse_check(argc, argv);

  std::vector<int> dims = args.get<int>("dims", 0);

  size_t nt = dims[0];
  size_t nx = dims[1];
  size_t ni = dims[2];
  size_t dt = args.get<int>("dt");
  float dx = args.get<float>("dx");
  float dy = args.get<float>("dy");
  size_t sx = args.get<int>("sxline");
  size_t si = args.get<int>("sinline");
  short dtype = args.get<short>("dtype");
  std::string infile = args.get<std::string>("infile");
  std::string outname = args.get<std::string>("outfile");

  AddSegy segy(infile, nt, nx, ni);
  segy.setSampleInterval(dt);
  segy.setXInterval(dx);
  segy.setYInterval(dy);
  segy.setInStart(si);
  segy.setXStart(sx);
  segy.setOutName(outname);
  segy.setDtype(dtype);

  segy.convert();
}