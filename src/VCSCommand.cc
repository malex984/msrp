#include "VCSCommand.h"

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sys/wait.h>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>

using namespace std;

namespace 
{

static int standard_exec(const char* const szArgs[])
{
  pid_t pid; 
   
  errno = 0;  
  
  if( (pid = fork()) == -1 )
  {
    cerr << "Error: sorry could not fork!" << endl << flush;
    return 1;    
  }
   

//  int fds[2]; if( pipe(fds) < 0 ) cerr << "Error: sorry could not pipe!" << endl << flush;
  
  
  if (pid == 0)  // child!?
  { 
    int i = 0;
//    close(fds[0]); close(fds[1]);
/*    
    cerr << "Trying Command Execution:["; // what about quiet mode?
    while( szArgs[i] != NULL )
    {
      cerr << " " << szArgs[i];
      i++;
    }
    cerr << " ]..."  << flush;
*/
     
    // redirect cout and cerr to /dev/null for executed command...
//    dup2(open("/dev/null", 0), STDOUT_FILENO);  // should not do redirect STDOUT :(((
    dup2(open("/dev/null", 0), STDERR_FILENO);
       
    // no return on success!!!    
    i = execvp(szArgs[0], const_cast<char* const*>(szArgs));
     
    cerr << endl << "Error: command execution(execvp) failed." << endl << flush;
    
    if ( (errno != 0) || (i == -1))
      _exit(-1);
    _exit(0);
  }
  else // fork > 0 => parent
  {
//    close(fds[1]);    close(fds[0]);
    int i;
    if( wait(&i) ==-1 ) 
    {
      cerr << "Error: sorry could not wait!" << endl << flush;
      return 1;    
    }
     
//    cerr << "Execution Status: " << i << flush; 
//    if( errno != 0 ) cerr << "Error:" << flush; 
       
    // did the child terminate normally?
    if(WIFEXITED(i))
    {   
//      cerr << ", Exitcode: " << WEXITSTATUS(i) << endl << flush; 
      return ((WEXITSTATUS(i) != 0) || (errno != 0));
    }
     
      
    // was the child terminated by a signal?
    if (WIFSIGNALED(i))
    {   
//      cerr << ", Signal: " << WTERMSIG(i) << endl << flush;	  
      return WTERMSIG(i);
    }
     
    
//    cerr << "Exit Status: " << i << ", errno: " << errno << endl;
    return ((i != 0) || (errno != 0));
  }
}


static int standard_command(const char *reposcommand, const char *repossubcommand, const char *name) 
{
  const char* const a[] = {reposcommand, repossubcommand, name, NULL};
  return standard_exec(a);
}


static int standard_renamer(const char *reposcommand, const char *repossubcommand, const char *oldname, const char *newname) {
  const char* const a[] = {reposcommand, repossubcommand, oldname, newname, NULL};
  return standard_exec(a);
}


}; // namespace


int PlainCommand::rename(const char *oldname, const char *newname, bool preserve_mode) {
  errno = 0;
//  cout << "trying: plain rename" << endl << flush;
  int retval = ::rename(oldname, newname);
  return ((retval != 0) || (errno != 0));
}


int SvnCommand::rename(const char *oldname, const char *newname, bool preserve_mode) {
  int ret = standard_renamer("svn", "rename", oldname, newname);
  return ret != 0 ? base::rename(oldname, newname, preserve_mode) : 0;

// TODO: svn check?
  if( is_tracked(oldname) )
    return standard_renamer("svn", "rename", oldname, newname);
  else
    return base::rename(oldname, newname, preserve_mode);    
}

int HgCommand::rename(const char *oldname, const char *newname, bool preserve_mode) {
  int ret = standard_renamer("hg", "rename", oldname, newname);
  return ret != 0 ? base::rename(oldname, newname, preserve_mode) : 0;

// TODO: hg check?
  if( is_tracked(oldname) )
    return standard_renamer("hg", "rename", oldname, newname);
  else
    return base::rename(oldname, newname, preserve_mode);
}


int GitCommand::rename(const char *oldname, const char *newname, bool preserve_mode) {
//  if( is_tracked(oldname) )
//  {
//  } 
  const char* const a[] = {"git", "mv", "-k", oldname, newname, NULL};
  if( standard_exec(a) != 0) 
    return base::rename(oldname, newname, preserve_mode); // fallback to plain rename
  return 0;
}

int BzrCommand::rename(const char *oldname, const char *newname, bool preserve_mode) {
  int ret = standard_renamer("bzr", "mv", oldname, newname);
  return ret != 0 ? base::rename(oldname, newname, preserve_mode) : 0;
// TODO: bzr check?
  if( is_tracked(oldname) )
    return standard_renamer("bzr", "mv", oldname, newname) == 0;
  else
    return base::rename(oldname, newname, preserve_mode);    
}


bool SvnCommand::is_tracked(const char *path) {
  return standard_command("svn", "status", path) == 0; // TODO: command?
}
bool HgCommand::is_tracked(const char *path) {
  return standard_command("hg", "status", path) == 0; // TODO: command?
}
bool BzrCommand::is_tracked(const char *path) {
  return standard_command("bzr", "status", path) == 0; // TODO: command?
}
bool GitCommand::is_tracked(const char *path) {
  const char* const a[] = {"git", "ls-files", "--error-unmatch", "--", path, NULL};
  return  standard_exec(a) == 0;
//  standard_command("git", "status", path);
}
