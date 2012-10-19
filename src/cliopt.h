#ifndef __CLIOPT_H
#define __CLIOPT_H

#include "RenameCommand.h"

class CliOpt {
  public:
  CliOpt(void) {

    case_insensitive = false;
    word_boundary = false;

    do_filenames = true;
    do_directorynames = true;
    do_contents = true;

    is_quiet = false;
    do_dot_paths = false;
    do_preserve = false;

    subdir_recursion = true;

    renamer = new PlainRename();
  }

  RenameCommand *renamer;

  bool case_insensitive, word_boundary;
  bool do_filenames, do_directorynames, do_contents;
  bool is_quiet, do_dot_paths;
  bool do_preserve;

  bool subdir_recursion;
  /* returns true iff it was an option */
  bool handle_arg(const char *str);
};

#endif
