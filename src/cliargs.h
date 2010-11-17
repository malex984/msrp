#ifndef __CLIARGS_H
#define __CLIARGS_H

#include <deque>
#include "cliopt.h"

int get_path_mode(const char *path);
bool is_path_file(const char *path);
bool is_path_dir(const char *path);
bool does_file_exist_quiet(const char *path);
bool is_path_filtered(CliOpt options, const char *path);
std::deque<std::string> expand_dir(CliOpt options, const char *path);


class TargetList {
  public:
  std::deque<std::string> files, dirs;
  void handle_path(CliOpt options, const char *path, bool recurse);
};

class CliArgs {
  public:
  CliOpt options;
  std::deque<std::string> args;
  std::string searchpat, repstr;
  void printHelp(void) const;
  CliArgs(const char **argv);
  TargetList make_target_list(void);
};

#endif
