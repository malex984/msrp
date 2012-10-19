#ifndef __RENAMECOMMAND_H
#define __RENAMECOMMAND_H


class RenameCommand {
  public:
  /// returns non-zero value in case of an error
  virtual int rename(const char *oldpath, const char *newpath, bool preserve_mode) {
    return 1; // cannot rename!
  };
};

class PlainRename : public RenameCommand {
  typedef RenameCommand base;
public:
  virtual int rename(const char *oldpath, const char *newpath, bool preserve_mode);
};

class HgRename : public PlainRename {
  typedef PlainRename base;
public:
  virtual int rename(const char *oldpath, const char *newpath, bool preserve_mode);
};

class SvnRename : public PlainRename {
  typedef PlainRename base;
public:
  virtual int rename(const char *oldpath, const char *newpath, bool preserve_mode);
};

class GitRename : public PlainRename {
  typedef PlainRename base;
public:
  virtual int rename(const char *oldpath, const char *newpath, bool preserve_mode);
};

class BzrRename : public PlainRename {
  typedef PlainRename base;
public:
  virtual int rename(const char *oldpath, const char *newpath, bool preserve_mode);
};

#endif // __RENAMECOMMAND_H
