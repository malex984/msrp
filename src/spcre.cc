#include <iostream>
#include <string>
#include <pcre.h>
#include "spcre.h"

using namespace std;

string SPCREResult::backsub(string origstr)
{
  int curoffset = 0;
  int len = origstr.size();
  string res;
  const char *cs = origstr.c_str();
  while (curoffset < len) {
    int slashind = origstr.find('\\', curoffset);
    if (slashind != string::npos) {
      res.append(cs + curoffset, slashind - curoffset);
      if (slashind + 1 < len) {
        char c = cs[slashind+1];
        if (c >= '0' && c <= '9') {
          int whichnum = c - '0';
          if (whichnum <= cap.size())
            res.append(cap[whichnum]);
        }
      }
      curoffset = slashind + 2;
    }
    else {
      res.append(cs + curoffset, len - curoffset);
      break;
    }
  }
  return res;
}

string SPCRE::gsub(string origstr, string repstr)
{
  int curoffset = 0;
  int len = origstr.size();
  string res;
  const char *cs = origstr.c_str();
  while (curoffset < len) {
    SPCREResult r = match(origstr, curoffset);
    if (r.didmatch) {
      res.append(cs + curoffset, r.start_offset - curoffset);
      res.append(r.backsub(repstr));
      curoffset = r.pastend_offset;
    }
    else {
      res.append(cs+curoffset, len - curoffset);
      break;
    }
  }
  return res;
}

SPCREResult SPCRE::match(string inpstr, int offset)
{
  int rc, ovector[30];
  SPCREResult res;
  res.didmatch = false;
  res.didfinish = false;
  const char *cs = inpstr.c_str();
  rc = pcre_exec(prex, prex_study, cs, inpstr.size(), offset, 0,
                 ovector, sizeof(ovector) / sizeof(ovector[0]));
  if (rc > 0) {
    res.didmatch = true;
  } else {
    res.didfinish = true;
  }
  if (res.didmatch) {
    int i;
    res.start_offset = ovector[0];
    res.pastend_offset = ovector[1];
    if (res.pastend_offset >= inpstr.size())
      res.didfinish = true;
    for (i = 0; i < rc; i += 1) {
      int olen = ovector[2*i+1] - ovector[2*i];
      if (olen < 0) {
        cerr << "Error on offset " << i << " : " << ovector[2*i] << ","<<ovector[2*i+1] << endl;
        exit(1);
      }
      res.cap.push_back(string(cs+ovector[2*i], olen));
    }
  }
  return res;
}

SPCRE::SPCRE(string pattern, CliOpt co)
{
  const char *study_error;
  options = co;
  pat = pattern;
  prex = get_pcre();
  prex_study = pcre_study(prex, 0, &study_error);
  if (study_error != 0) {
    cerr << "Error studying pattern " << pat << endl;
    exit(1);
  }
}

pcre *SPCRE::get_pcre(void)
{
  pcre *result;
  int optval = PCRE_MULTILINE;
  const char *errptr;
  int erroffset;
  string curpat = pat;
  if (options.word_boundary)
    curpat = string("\\b") + curpat + string("\\b");
  if (options.case_insensitive)
    optval |= PCRE_CASELESS;
  result = pcre_compile(curpat.c_str(), optval, &errptr, &erroffset, NULL);
  if (result == 0) {
    int i;
    cerr << "Error in pattern, offset " << erroffset << ": " << errptr << endl;
    cerr << curpat << endl;
    for (i = 0; i < erroffset; i += 1)
      cerr << ' ';
    cerr << "^" << endl;
    exit(1);
  }
  return result;
}
