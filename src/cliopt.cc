#include "cliopt.h"
#include "cliargs.h"
#include "VCSCommand.h"

#include <string.h>

using namespace std;

bool CliOpt::handle_arg(const char *str)
{
  bool retval = true;
  if (strcmp(str, "-p") == 0) { do_preserve = true; return retval; }
  if (strcmp(str, "-q") == 0) { is_quiet = true; return retval; }
  if (strcmp(str, "-i") == 0) { case_insensitive = true; return retval; }
  if (strcmp(str, "-w") == 0) { word_boundary = true; return retval; }
  if (strcmp(str, "-s") == 0) { subdir_recursion = false; return retval; }
  if (strcmp(str, "-f") == 0) { do_filenames = false; return retval; }
  if (strcmp(str, "-d") == 0) { do_directorynames = false; return retval; }
  if (strcmp(str, "-c") == 0) { do_contents = false; return retval; }
  if (strcmp(str, "--preserve") == 0) { do_preserve = true; return retval; }
  if (strcmp(str, "--dot-paths") == 0) { do_dot_paths = true; return retval; }
  if (strcmp(str, "--hg") == 0 || strcmp(str, "--mercurial") == 0)
    { renamer = new HgCommand(); return retval; }
  if (strcmp(str, "--svn") == 0 || strcmp(str, "--subversion") == 0)
    { renamer = new SvnCommand(); return retval; }
  if (strcmp(str, "--git") == 0)
    { renamer = new GitCommand(); return retval; }
  if (strcmp(str, "--bzr") == 0 || strcmp(str, "--bazaar") == 0)
    { renamer = new BzrCommand(); return retval; }
  if (strcmp(str, "--mv") == 0 || strcmp(str, "--plain") == 0)
    { renamer = new PlainCommand(); return retval; }
  if (strcmp(str, "--tracked-only") == 0)
    { process_tracked_only = true; return retval; }
    
  return false;
}
