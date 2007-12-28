#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "cliopt.h"
#include "cliargs.h"

using namespace std;

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
  if (strcmp(str, "--hg") == 0 || strcmp(str, "--mercurial") == 0)
    { renamer = new HgRename(); return retval; }
  if (strcmp(str, "--svn") == 0 || strcmp(str, "--subversion") == 0)
    { renamer = new SvnRename(); return retval; }
  if (strcmp(str, "--mv") == 0 || strcmp(str, "--plain") == 0)
    { renamer = new PlainRename(); return retval; }
  return false;
}

static int standard_renamer(const char *reposcommand, const char *oldname, const char *newname) {
  char *args[5];
  int retval = 0;
  int i;
  args[0] = strdup(reposcommand);
  args[1] = strdup("rename");
  args[2] = strdup(oldname);
  args[3] = strdup(newname);
  args[4] = NULL;
  if (fork() == 0)
    execv(args[0], args);
  if (is_path_file(oldname) || is_path_dir(oldname)) {
    cerr << "(warning) rename " << oldname << " => "  << newname << " failed." << endl;
    retval = 1;
  }
  for (i = 0; i < 4; i += 1)
    free(args[i]);
  return retval;
}

int HgRename::rename(const char *oldname, const char *newname) {
  return standard_renamer("hg", oldname, newname);
}

int SvnRename::rename(const char *oldname, const char *newname) {
  return standard_renamer("svn", oldname, newname);
}

int PlainRename::rename(const char *oldname, const char *newname) {
  return ::rename(oldname, newname);
}
