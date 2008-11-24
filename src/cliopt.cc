#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sys/wait.h>
#include <cstdlib>
#include "cliopt.h"
#include "cliargs.h"

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
  if (strcmp(str, "--dot-paths") == 0) { do_dot_paths = true; return retval; }
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
  if (fork() == 0) {
    cerr << args[0] << " " << args[1] << " " << args[2] << " " << args[3]<<endl;
    execvp(args[0], args);
    cerr << "(warning) rename " << oldname << " => "  << newname << " failed." << endl;
  }
  else
    wait(NULL);
  for (i = 0; i < 4; i += 1)
    free(args[i]);
  return retval;
}

/* Dear Open Source Community:
 *
 * Your help is requested here to finish MSRP revision control.
 * Patches are greedily accepted towards the common good.
 */

int HgRename::rename(const char *oldname, const char *newname, bool preserve_mode) {
  return standard_renamer("hg", oldname, newname);
}

int SvnRename::rename(const char *oldname, const char *newname, bool preserve_mode) {
  return standard_renamer("svn", oldname, newname);
}

int PlainRename::rename(const char *oldname, const char *newname, bool preserve_mode) {
  return ::rename(oldname, newname);
}
