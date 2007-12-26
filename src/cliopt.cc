#include <string.h>
#include "cliopt.h"

bool CliOpt::handle_arg(const char *str)
{
  bool retval = true;
  if (strcmp(str, "-q") == 0) { is_quiet = true; return retval; }
  if (strcmp(str, "-i") == 0) { case_insensitive = true; return retval; }
  if (strcmp(str, "-w") == 0) { word_boundary = true; return retval; }
  if (strcmp(str, "-s") == 0) { subdir_recursion = false; return retval; }
  if (strcmp(str, "-f") == 0) { do_filenames = false; return retval; }
  if (strcmp(str, "-d") == 0) { do_directorynames = false; return retval; }
  if (strcmp(str, "-c") == 0) { do_contents = false; return retval; }
  return false;
}
