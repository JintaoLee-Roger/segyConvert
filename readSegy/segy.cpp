/*
 * Copyright (c) 2021-2022 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @date: 2022-09-08 support fill any float or nan NAN
 *
 * @file: segy.cpp
 * @brief: definition of SegyFile class
 */

#include "segy.h"

#include <iostream>
#include <limits>
#include <vector>

#define SIGN_BIT_MASK 0x80000000
#define EXPONENT_BIT_MASK 0x7f000000
#define FRACTION_BIT_MASK 0x00ffffff
#define FRACTION_IMPLICIT_BIT_MASK 0x00800000

SegyFile::SegyFile(const std::string infile) {
  this->in_.open(infile, std::ios::in | std::ios::binary);
  if (!this->in_) {
    std::cout << "Open file: " << infile << " failure" << std::endl;
    exit(1);
  }
  this->in_.seekg(0, std::ios::beg);
  this->in_.read(&textheader[0], 3200);
  this->getBinaryHeader();
  this->th.inlLoc = -1;
  this->th.xlLoc = -1;
  this->th.inmin = -1;
  this->th.inmax = -1;
  this->th.xlmin = -1;
  this->th.xlmax = -1;
}

SegyFile::~SegyFile() {
  in_.close();
  if (out_) {
    out_.close();
  }
}

void SegyFile::printTextHeader() {
  char line[81] = {0};  // initial and line[80] = '\0' in c++'s string
  bool isEBCDIC = isTextInEBCDICFormat(&textheader[0]);
  for (int i = 0, j = 0; i < 3200; ++i, ++j) {
    j = j % 80;
    if (isEBCDIC) {
      line[j] = getASCIIFromEBCDIC(textheader[i]);
    } else {
      line[j] = textheader[i];
    }

    if ((i + 1) % 80 == 0) {
      std::cout << line << std::endl;
    }
  }
}

void SegyFile::getBinaryHeader() {
  binaryHeader_.dt = swap_endian<short>(readData<short>(3200 + 16));
  binaryHeader_.ns = swap_endian<short>(readData<short>(3200 + 20));
  binaryHeader_.dformat = swap_endian<short>(readData<short>(3200 + 24));
  binaryHeader_.numExHeader = swap_endian<short>(readData<short>(3200 + 304));
  binaryHeader_.numTrailer =
      swap_endian<int32_t>(readData<int32_t>(3200 + 328));
  in_.seekg(0, std::ios::end);
  uintmax_t flength = in_.tellg();
  binaryHeader_.totalTrace =
      (flength - (3600 + binaryHeader_.numExHeader * 3200)) /
      (240 + binaryHeader_.ns * 4);
}

void SegyFile::guessLoc() {
  // size_t inline_loc[3] = {188, 8, 4};
  // size_t xline_loc[3] = {192, 20, 16};
  size_t inline_loc[3] = {4, 8, 188};
  size_t xline_loc[3] = {16, 20, 192};
  uintmax_t start1 = 3600 + 3200 * binaryHeader_.numExHeader;
  // uintmax_t start2 = start1 + 240 + binaryHeader_.ns*4;
  uintmax_t start3 =
      start1 + (240 + binaryHeader_.ns * 4) * (binaryHeader_.totalTrace - 1);

  for (size_t i = 0; i < 3; ++i) {
    int inl1 = swap_endian<int32_t>(readData<int32_t>(start1 + inline_loc[i]));
    int inl2 = swap_endian<int32_t>(readData<int32_t>(start3 + inline_loc[i]));
    // printf("inl1: %d, inl2: %d\n", inl1, inl2);
    if (inl1 != inl2 && (inl2 - inl1) < 10000) {
      th.inlLoc = inline_loc[i];
      break;
    }
  }
  for (size_t i = 0; i < 3; ++i) {
    int xl1 = swap_endian<int32_t>(readData<int32_t>(start1 + xline_loc[i]));
    int xl2 = swap_endian<int32_t>(readData<int32_t>(start3 + xline_loc[i]));
    // printf("xl1: %d, xl2: %d\n", xl1, xl2);
    if (xl1 != xl2 && (xl2 - xl1) < 10000) {
      th.xlLoc = xline_loc[i];
      break;
    }
  }
  std::cout << "Guess Location: Inline: " << th.inlLoc + 1
            << "  Crossline: " << th.xlLoc + 1 << std::endl
            << std::endl;
}

void SegyFile::setParameters(size_t iloc, size_t xloc) {
  th.inlLoc = iloc - 1;
  th.xlLoc = xloc - 1;
}

void SegyFile::setParameters(size_t iloc, size_t xloc, size_t imin, size_t imax,
                             size_t xmin, size_t xmax) {
  th.inlLoc = iloc - 1;
  th.xlLoc = xloc - 1;
  th.inmin = imin;
  th.inmax = imax;
  th.xlmin = xmin;
  th.xlmax = xmax;
}

void SegyFile::toDat(const std::string outfile, float fills) {
  if (-1 == th.inlLoc || -1 == th.xlLoc) {
    guessLoc();
  }
  if (-1 == th.inmin || -1 == th.inmax || -1 == th.xlmin || -1 == th.xlmax) {
    scan();
  }

  out_.open(outfile, std::ios::out | std::ios::binary);
  if (!out_) {
    std::cout << "Open file: " << outfile << " failure" << std::endl;
    exit(1);
  }

  size_t totalT = (th.inmax - th.inmin + 1) * (th.xlmax - th.xlmin + 1);
  std::cout << "Total traces: " << totalT << "    Segy file only contains "
            << binaryHeader_.totalTrace << " traces    need "
            << (totalT - binaryHeader_.totalTrace) << " zero traces to pad it"
            << std::endl;

  std::vector<float> trace(binaryHeader_.ns, 0);
  size_t idx1 = 0;
  size_t idx2 = 0;

  uintmax_t start = 3600 + binaryHeader_.numExHeader * 3200;
  size_t iline = swap_endian<int32_t>(readData<int32_t>(start + th.inlLoc));
  size_t xline = swap_endian<int32_t>(readData<int32_t>(start + th.xlLoc));
  size_t process = 0;
  for (size_t i = th.inmin; i <= th.inmax; ++i) {
    for (size_t j = th.xlmin; j <= th.xlmax; ++j) {
      if (iline == i && xline == j) {
        readTrace(trace, start + 240);
        writeTrace(trace);
        idx1++;
        start += (240 + 4 * binaryHeader_.ns);
        iline = swap_endian<int32_t>(readData<int32_t>(start + th.inlLoc));
        xline = swap_endian<int32_t>(readData<int32_t>(start + th.xlLoc));
      } else {
        std::fill(trace.begin(), trace.end(), fills);
        writeTrace(trace);
        idx2++;
      }
      if (0 == (idx1 + idx2) % (totalT / 10)) {
        process++;
        std::cout << "Process: [ " << (idx1 + idx2) << " / " << totalT
                  << " ] ( " << process << " / 10 )" << std::endl;
      }
    }
  }
  if (idx1 != binaryHeader_.totalTrace) {
    std::cout << "Error! There are " << binaryHeader_.totalTrace
              << " traces in Segy file, but read " << idx1 << " traces."
              << std::endl;
    exit(1);
  } else if ((idx1 + idx2) != totalT) {
    std::cout << "Error!  " << totalT << " traces need to be written, but "
              << (idx1 + idx2) << " traces have been written." << std::endl;
    exit(1);
  } else {
    std::cout << idx1
              << " traces have been read from segy file and written to the "
                 "binary file."
              << std::endl
              << idx2 << " traces with zero have been padded." << std::endl
              << binaryHeader_.totalTrace << " traces in segy file."
              << std::endl
              << "Totally, " << totalT
              << " traces have been written to the binary file." << std::endl;
  }

  std::cout << std::endl
            << "Out file information: \n\tdt: " << binaryHeader_.dt
            << "\n\tn1(ns): " << binaryHeader_.ns
            << "\n\tn2(xline): " << th.xlmax - th.xlmin + 1
            << "\n\tn3(inline): " << th.inmax - th.inmin + 1 << std::endl;
}

void SegyFile::readTrace(std::vector<float>& trace, int64_t loc) {
  in_.seekg(loc, std::ios::beg);
  in_.read((char*)&trace[0], binaryHeader_.ns * sizeof(float));

  for (auto& i : trace) {
    if (1 == binaryHeader_.dformat) {
      i = ibm_to_ieee(i, true);
    } else {
      i = swap_endian(i);
    }
  }
}

void SegyFile::writeTrace(const std::vector<float>& trace) {
  out_.write((char*)&trace[0], binaryHeader_.ns * sizeof(float));
}

void SegyFile::scan() {
  if ((-1 == th.inlLoc) || (-1 == th.xlLoc)) guessLoc();

  int temp;
  uintmax_t start = 3600 + binaryHeader_.numExHeader * 3200;
  th.inmin = swap_endian<int32_t>(readData<int32_t>(start + th.inlLoc));
  temp = swap_endian<int32_t>(readData<int32_t>(start + th.xlLoc));
  start = start + (240 + binaryHeader_.ns * 4) * (binaryHeader_.totalTrace - 1);
  th.inmax = swap_endian<int32_t>(readData<int32_t>(start + th.inlLoc));

  th.xlmax = temp;
  th.xlmin = temp;
  start = 3600 + binaryHeader_.numExHeader * 3200;
  for (uintmax_t i = 1; i < binaryHeader_.totalTrace; ++i) {
    start += (240 + binaryHeader_.ns * 4);
    temp = swap_endian<int32_t>(readData<int32_t>(start + th.xlLoc));
    if (temp > th.xlmax) th.xlmax = temp;
    if (temp < th.xlmin) th.xlmin = temp;
  }
  std::cout << "inline range: " << th.inmin << " - " << th.inmax << std::endl
            << "xline range:  " << th.xlmin << " - " << th.xlmax << std::endl;

  std::cout << "n1(ns), n2, n3 = " << binaryHeader_.ns << ", "
            << th.xlmax - th.xlmin + 1 << ", " << th.inmax - th.inmin + 1
            << std::endl
            << std::endl;
}

bool SegyFile::isTextInEBCDICFormat(const char* text) {
  int alnumASCII = 0;
  for (size_t i = 0; i < 3200; i++) {
    if (std::isalnum(text[i])) alnumASCII++;
  }

  int alnumEBCDIC = 0;
  for (size_t i = 0; i < 3200; i++) {
    if (std::isalnum(getASCIIFromEBCDIC(text[i]))) alnumEBCDIC++;
  }

  if (alnumASCII > alnumEBCDIC) return false;
  return true;
}

char SegyFile::getASCIIFromEBCDIC(char c) {
  if (kEBCDICtoASCIImap.find(c) != kEBCDICtoASCIImap.end())
    return kEBCDICtoASCIImap.at(c);
  return ' ';
}

float SegyFile::ibm_to_ieee(float value, bool is_big_endian_input) {
  if (is_big_endian_input) {
    value = swap_endian<float>(value);
  }

  int32_t* int_addr = reinterpret_cast<int32_t*>(&value);
  int32_t int_val = *int_addr;

  int32_t sign = int_val >> 31;
  int32_t fraction = int_val & FRACTION_BIT_MASK;

  if (fraction == 0) {
    return sign ? -0.0f : 0.0f;
  }

  // Convert exponent to be of base 2 and remove IBM exponent bias.
  int32_t exponent = ((int_val & EXPONENT_BIT_MASK) >> 22) - 256;

  // Drop the last bit since we can store only 23 bits in IEEE.
  fraction >>= 1;

  // Normalize such that the implicit leading bit of the fraction is 1.
  while (fraction && (fraction & FRACTION_IMPLICIT_BIT_MASK) == 0) {
    fraction <<= 1;
    --exponent;
  }

  // Drop the implicit leading bit.
  fraction &= 0x007fffff;

  // Add IEEE bias to the exponent.
  exponent += 127;

  // Handle overflow.
  if (exponent >= 255) {
    return (sign ? -std::numeric_limits<float>::max()
                 : std::numeric_limits<float>::max());
  }

  int32_t ieee_value;

  // Handle underflow.
  if (exponent <= 0)
    ieee_value = (sign << 31) | fraction;
  else
    ieee_value = (sign << 31) | (exponent << 23) | fraction;

  float* float_addr = reinterpret_cast<float*>(&ieee_value);
  return *float_addr;
}
