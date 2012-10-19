#ifndef __RENAMECOMMAND_H
#define __RENAMECOMMAND_H

/// Pure interface
class VCSCommand 
{
  public:
    /// returns false path is not tracked
    virtual bool is_tracked(const char *path) = 0;

    /// returns non-zero value in case of an error
    virtual int rename(const char *oldpath, const char *newpath, bool preserve_mode) = 0;
    //  { return 1; }; // cannot rename!

    /// should be able to delete any derived class with the pointer to this interface
    virtual ~VCSCommand() {};
  protected:  
    /// disable default constructor
    VCSCommand(){}; 
  private: 
    /// disable default copy constructor
    VCSCommand& operator=(const VCSCommand&);
};

/// Represents commands that deal with the usual FileSystem
class PlainCommand : public VCSCommand {
  typedef VCSCommand base;
public:
  PlainCommand(){}
    virtual bool is_tracked(const char *path) { return true; };
    virtual int rename(const char *oldpath, const char *newpath, bool preserve_mode);
};

/* Dear Open Source Community:
 *
 * Your help is requested here to finish MSRP VCS-support.
 * Any feedback is welcome!
 */


class HgCommand : public PlainCommand {
  typedef PlainCommand base;
public:
  HgCommand(){}
  int rename(const char *oldpath, const char *newpath, bool preserve_mode);
  bool is_tracked(const char *path);
};

class SvnCommand : public PlainCommand {
  typedef PlainCommand base;
public:
  SvnCommand(){}
  int rename(const char *oldpath, const char *newpath, bool preserve_mode);
  bool is_tracked(const char *path);
};

class GitCommand : public PlainCommand {
  typedef PlainCommand base;
public:
  GitCommand(){}
  int rename(const char *oldpath, const char *newpath, bool preserve_mode);
  bool is_tracked(const char *path);
};

class BzrCommand : public PlainCommand {
  typedef PlainCommand base;
public:
  BzrCommand(){}
  int rename(const char *oldpath, const char *newpath, bool preserve_mode);
  bool is_tracked(const char *path);
};

#endif // __RENAMECOMMAND_H
