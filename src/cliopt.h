#ifndef __CLIOPT_H
#define __CLIOPT_H

class CliOpt {
  public:
  CliOpt(void) {

    case_insensitive = false;
    word_boundary = false;

    do_filenames = true;
    do_directorynames = true;
    do_contents = true;

    is_quiet = false;

    subdir_recursion = true;

  }
  bool case_insensitive, word_boundary;
  bool do_filenames, do_directorynames, do_contents;
  bool is_quiet;

  bool subdir_recursion;
  /* returns true iff it was an option */
  bool handle_arg(const char *str);
};

#endif
