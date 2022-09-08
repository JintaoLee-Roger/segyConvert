/*
 * Copyright (c) 2021-2022 Jintao Li. All rights reserved.
 * University of Science and Technology of China (USTC),
 * Computational and Interpretation Group (CIG).
 *
 * @author: Jintao Li
 * @date: 2022-09-08 support fill any float or nan NAN
 *
 * @file: segy.h
 * @brief: statement of SegyFile class, which is used to convert
 *         a segy file to a binary file without any header information
 *         and padded with zeros if necessary.
 */

#ifndef READSEGY_SEGY_H_
#define READSEGY_SEGY_H_

#include <climits>
#include <fstream>
#include <map>
#include <vector>

// 400 bytes binary header, stored in 3200-3600 bytes
struct binaryHeader {
  short dt;           // Sample interval, 17-18 (17-1 in cpp)
  short ns;           // Number of samples per data trace, 21-22
  short dformat;      // Data sample format code, 25-26
  short numExHeader;  // Number of 3200-byte, Extended Textual File Header,
                      // 305-306
  int numTrailer;  // Number of 3200-byte data trailer stanza records, 329-332
  size_t totalTrace;  // Total traces in segy file. need to calculate
};

// 240 bytes trace header
struct traceHeader {
  int inlLoc;  // in-line number, 5-8 or 9-12 or 189-192
  int xlLoc;   // cross-line number, 17-20 or 21-24 or 193-196
  int inmax;   // maximum of in-line number, need to scan or set
  int xlmax;   // minimum of in-line number, ...
  int inmin;   // maximum of cross-line number, ...
  int xlmin;   // minimum of cross-line number, ...
               // int x;          // 73-76
               // int y;          // 77-80
};

// A key map that convert EBCDIC to ASCII format
const std::map<unsigned char, char> kEBCDICtoASCIImap = {
    {64, ' '},   {75, '.'},  {76, '<'},   {77, '('},  {78, '+'},  {79, '|'},
    {80, '&'},   {90, '!'},  {91, '$'},   {92, '*'},  {93, ')'},  {94, ';'},
    {96, '-'},   {97, '/'},  {106, '|'},  {107, ','}, {108, '%'}, {109, '_'},
    {110, '>'},  {111, '?'}, {121, '`'},  {122, ':'}, {123, '#'}, {124, '@'},
    {125, '\''}, {126, '='}, {127, '"'},  {129, 'a'}, {130, 'b'}, {131, 'c'},
    {132, 'd'},  {133, 'e'}, {134, 'f'},  {135, 'g'}, {136, 'h'}, {137, 'i'},
    {145, 'j'},  {146, 'k'}, {147, 'l'},  {148, 'm'}, {149, 'n'}, {150, 'o'},
    {151, 'p'},  {152, 'q'}, {153, 'r'},  {161, '~'}, {162, 's'}, {163, 't'},
    {164, 'u'},  {165, 'v'}, {166, 'w'},  {167, 'x'}, {168, 'y'}, {169, 'z'},
    {192, '{'},  {193, 'A'}, {194, 'B'},  {195, 'C'}, {196, 'D'}, {197, 'E'},
    {198, 'F'},  {199, 'G'}, {200, 'H'},  {201, 'I'}, {208, '}'}, {209, 'J'},
    {210, 'K'},  {211, 'L'}, {212, 'M'},  {213, 'N'}, {214, 'O'}, {215, 'P'},
    {216, 'Q'},  {217, 'R'}, {224, '\\'}, {226, 'S'}, {227, 'T'}, {228, 'U'},
    {229, 'V'},  {230, 'W'}, {231, 'X'},  {232, 'Y'}, {233, 'Z'}, {240, '0'},
    {241, '1'},  {242, '2'}, {243, '3'},  {244, '4'}, {245, '5'}, {246, '6'},
    {247, '7'},  {248, '8'}, {249, '9'}};

// @brief: a class to read a segy file and convert it to a binary file
//         without any header information, pading with zeros if there are
//         some missing traces.
// @param:
//      const std::string infile: input segy file name
class SegyFile {
 public:
  char textheader[3200];  // 3200 bytes text header, EBCDIC or ASCII format
  std::fstream in_;
  std::fstream out_;
  binaryHeader binaryHeader_;
  traceHeader th;
  std::string outfile;

  SegyFile(const std::string infile);
  ~SegyFile();

  void printTextHeader();
  void getBinaryHeader();
  // set some parameters: storage location of in-line and cross-line number
  void setParameters(size_t iloc, size_t xloc);
  // override, add more parameters: in-line range and cross-line number range
  void setParameters(size_t iloc, size_t xloc, size_t imin, size_t imax,
                     size_t xmin, size_t xmax);
  // convert a segy file to a binary file
  void toDat(const std::string outfile = "out.dat", float fills = 0.0);
  // guess the storage location of in-line number and cross-line number
  void guessLoc();
  // scan the whole segy file, obtain the range of in-line and cross-line
  // number
  void scan();

 private:
  // determine if the text header is EBCDIC format
  bool isTextInEBCDICFormat(const char* text);
  // convert a EBCDIC format char to ASCII format
  char getASCIIFromEBCDIC(char c);
  // wite a trace to the binary file
  void writeTrace(const std::vector<float>& trace);
  // read a trace from specific location
  void readTrace(std::vector<float>& trace, int64_t loc);
  // swap a number between big endian and little endian
  template <typename T>
  T swap_endian(T u);
  // convert a IBM 4-points float number to IEEE 4-points float format.
  float ibm_to_ieee(float value, bool is_big_endian_input);
  // read a specific format number from a specific loaction, used in binary or
  // trace header
  template <typename T>
  T readData(int64_t loc);
};

template <typename T>
T SegyFile::swap_endian(T u) {
  static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

  union {
    T u;
    unsigned char u8[sizeof(T)];
  } source, dest;

  source.u = u;

  for (size_t k = 0; k < sizeof(T); k++)
    dest.u8[k] = source.u8[sizeof(T) - k - 1];

  return dest.u;
}

template <typename T>
T SegyFile::readData(int64_t loc) {
  in_.seekg(loc, std::ios::beg);
  T temp = 0;
  in_.read((char*)&temp, sizeof(T));
  return temp;
}

#endif
