#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include "cliargs.h"

using namespace std;

CliArgs::CliArgs(const char **argv) {
  int i;
  for (i = 1; argv[i]; i += 1) {
    if (!options.handle_arg(argv[i]))
      args.push_back(string(argv[i]));
  }
  if (args.size() < 3) {
    cerr << "You must supply at least one file or directory." << endl;
    printHelp();
    exit(1);
  }
  searchpat = args.front(); args.pop_front();
  repstr = args.front(); args.pop_front();
}

int get_path_mode(const char *path)
{
  struct stat s;
  if (stat(path, &s)) {
    cerr << "Cannot stat " << path << endl;
    return false;
  }
  return s.st_mode;
}

bool is_path_file(const char *path)
{
  return S_ISREG(get_path_mode(path));
}

bool is_path_dir(const char *path)
{
  return S_ISDIR(get_path_mode(path));
}

bool is_path_filtered(CliOpt options, const char *path)
{
  if (options.do_dot_paths)
    return (strcmp(path, ".") == 0 || strcmp(path, "..") == 0);
  else
    return (path[0] == 0 || path[0] == '.');
}

deque<string> expand_dir(CliOpt options, const char *path)
{
  deque<string> res;
  DIR *d;
  d = opendir(path);
  if (d == 0) {
    cerr << "Cannot open directory " << path << endl;
    return res;
  }
  string spath = string(path) + string("/");
  struct dirent *cur;
  while (cur = readdir(d)) {
    if (is_path_filtered(options, cur->d_name))
      continue;
    string cpath = spath + string(cur->d_name);
    const char *cst = cpath.c_str();
    if (is_path_file(cst) || is_path_dir(cst))
      res.push_back(cpath);
  }
  closedir(d);
  return res;
}

void TargetList::handle_path(CliOpt options, const char *path, bool recurse)
{
  if (is_path_file(path)) {
    files.push_front(string(path));
    return;
  }
  if (is_path_dir(path)) {
    dirs.push_front(string(path));
    if (recurse) {
      deque<string> ds = expand_dir(options, path);
      deque<string>::iterator i;
      for (i = ds.begin(); i != ds.end(); i++) {
        handle_path(options, (*i).c_str(), recurse);
      }
    }
    return;
  }
  cerr << "Skipping irregular " << path << endl;
}

TargetList CliArgs::make_target_list(void)
{
  TargetList tl;
  deque<string>::iterator i;
  for (i = args.begin(); i != args.end(); i++) {
    tl.handle_path(options, (*i).c_str(), options.subdir_recursion);
  }
  return tl;
}

void CliArgs::printHelp(void) const
{
  cerr << endl;
  cerr << "msrp is a meta search and replace utility." << endl;
  cerr << endl;
  cerr << "msrp was written by Rudi Cilibrasi (cilibrar@cilibrar.com)." << endl;
  cerr << endl;
  cerr << "msrp is a utility to perform regular expression substitution"<< endl;
  cerr << "It reads one or more files as input and modifies both their "<< endl;
  cerr << "contents and filenames (by renaming files if appropriate)." << endl;
  cerr << endl;
  cerr << "Usage: msrp searchpat repstr [-cdfiqsw] [file1] [dir2/]..." << endl;
  cerr << endl;
  cerr << "files or directories may be listed.  Directories are " << endl;
  cerr << "traversed recursively." << endl;
  cerr << endl;
  cerr << "Options:" << endl;
  cerr << "  -c   disable file contents transformation" << endl;
  cerr << "  -d   disable directory renaming transformation" << endl;
  cerr << "  -f   disable file renaming transformation" << endl;
  cerr << "  -i   case insensitive match" << endl;
  cerr << "  -q   quiet mode" << endl;
  cerr << "  -s   disable subdirectory recursion" << endl;
  cerr << "  -w   enable word boundary constraint" << endl;
  cerr << "  --dot-paths   enable hidden (.) directory recursion" << endl;
  cerr << endl;
  cerr << "  Integrated renaming options:" << endl;

  cerr << "  --svn  or --subversion : Subversion RCS rename support" << endl;
  cerr << "  --hg  or --mercurial : Mercurial    RCS rename support" << endl;
  cerr << "  --mv  or --plain     : (default) plain Unix rename" << endl;
  cerr << endl;
}

