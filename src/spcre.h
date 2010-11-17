#ifndef __SPCRE_H
#define __SPCRE_H

#include "cliopt.h"

#include <string>
#include <vector>
#include <pcre.h>


class SPCREResult {
  public:
  std::string backsub(std::string inpstr);
  bool didmatch, didfinish;
  int start_offset, pastend_offset;
  int matchlen(void) const { return pastend_offset - start_offset; }
  std::vector<std::string> cap;
};

/* A Search pattern for PCRE */
class SPCRE {
  pcre *get_pcre(void);
  public:
  SPCRE(std::string pattern, CliOpt co);
  CliOpt options;
  std::string pat;
  pcre *prex;
  pcre_extra *prex_study;
  SPCREResult match(std::string inpstr, int offset);
  std::string gsub(std::string origstr, std::string repstr);
};

#endif

