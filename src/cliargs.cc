#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "autover.h"
#include "cliargs.h"
#include "VCSCommand.h"

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
  if( options.renamer == NULL )
  {
    if( options.process_tracked_only ) 
    {
      cerr << "Warning: --tracked-only only makes sence if VCS is specified as well!"; 
      options.process_tracked_only = false;
    }
    options.renamer = new PlainCommand(); 
  }
    
  searchpat = args.front(); args.pop_front();
  repstr = args.front(); args.pop_front();
}

bool does_file_exist_quiet(const char *path)
{
  struct stat s;
  if (stat(path, &s)) {
    return false;
  }
  return S_ISREG(s.st_mode);
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
  while ((cur = readdir(d)) != 0) {
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

bool directorySortCriterion(const string& s1, const string& s2) {
  int l1 = s1.length();
  int l2 = s2.length();
  if (l2 < l1)
    return true;
  if (l2 > l1)
    return false;
  return (s2 > s1);
}

TargetList CliArgs::make_target_list(void)
{
  TargetList tl;
  deque<string>::iterator i;
  for (i = args.begin(); i != args.end(); i++) {
    tl.handle_path(options, (*i).c_str(), options.subdir_recursion);
  }
  sort(tl.dirs.begin(), tl.dirs.end(), directorySortCriterion);
  return tl;
}

void CliArgs::printHelp(void) const
{
  cerr << endl;
  cerr << MSRP_PACKAGE_NAME << " version " << MSRP_PACKAGE_VERSION << endl;
  cerr << MSRP_PACKAGE_NAME << " is a meta search and replace utility." << endl;
  cerr << endl;
  cerr << "It was originally written by Rudi Cilibrasi but is currently" << endl;
  cerr << "maintained by Oleksandr Motsak and Aurélien Gâteau." << endl;
  cerr << endl;
  cerr << "Homepage: " << MSRP_PACKAGE_URL << endl; 
  cerr << "Please send your feedback to " << MSRP_PACKAGE_BUGREPORT << endl;
  cerr << endl;
  cerr << "msrp is a utility to perform regular expression substitution"<< endl;
  cerr << "It reads one or more files as input and modifies both their "<< endl;
  cerr << "contents and filenames (by renaming files if appropriate)." << endl;
  cerr << endl;
  cerr << "Usage: msrp searchpat repstr [-cdfipqsw] [file1] [dir2/]..." << endl;
  cerr << endl;
  cerr << "files or directories may be listed.  Directories are " << endl;
  cerr << "traversed recursively." << endl;
  cerr << endl;
  cerr << "Options:" << endl;
  cerr << "  -c   disable file contents transformation" << endl;
  cerr << "  -d   disable directory renaming transformation" << endl;
  cerr << "  -f   disable file renaming transformation" << endl;
  cerr << "  -i   case insensitive match" << endl;
  cerr << "  -p   preserve original file mode (default is overwrite)" << endl;
  cerr << "  -q   quiet mode" << endl;
  cerr << "  -s   disable subdirectory recursion" << endl;
  cerr << "  -w   enable word boundary constraint" << endl;
  cerr << "  --first       change the first match only" << endl;
  cerr << "  --dot-paths   enable hidden (.) directory recursion" << endl;
  cerr << endl;
  cerr << "  Integrated *optional* renaming options:" << endl;
  cerr << "  --svn  or --subversion : Subversion RCS rename support" << endl;
  cerr << "  --preserve             : same as -p (preserve .orig files)"<< endl;
  cerr << "  --hg  or --mercurial   : Mercurial    RCS rename support" << endl;
  cerr << "  --git                  : Git          RCS rename support" << endl;
  cerr << "  --bzr or --bazaar      : Bazaar       RCS rename support" << endl;
  cerr << "  --mv  or --plain       : (default) plain Unix rename" << endl << endl;
  cerr << "  --tracked-only         : process only tracked files" << endl;
  
  cerr << endl;
}

