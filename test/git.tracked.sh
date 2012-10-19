#! /bin/sh

#expected to be run using a git clone of msrp repo!?
echo "TESTING --git --tracked-only option on file content: "

echo "aa" > file.aa.file || exit 1
echo "aa" > file.aa.file.git || exit 1

git add file.aa.file.git || exit 1

echo "testing: --git --tracked-only on file-contents: "
$MSRP --git --tracked-only -d "aa" "bb" file.aa.file.git file.aa.file || (git rm -f file.aa.file.git; rm file.aa.file; exit 1)

# processed tracked file?
test -e file.bb.file.git || (git rm -f file.aa.file.git; rm file.aa.file; exit 1)

# processed untracked file?
test -e file.bb.file && (git rm -f file.bb.file.git; rm -f file.bb.file file.aa.file; exit 1)

# didn't move untracked file...
test -e file.aa.file || (git rm -f file.bb.file.git; rm -f file.bb.file file.aa.file; exit 1)


echo "bb" > file.bb.file || exit 1

cmp file.bb.file file.aa.file && exit 1

cmp file.bb.file file.bb.file.git || exit 1

rm file.aa.file file.bb.file || exit 1
git rm -f file.bb.file.git || exit 1


echo "PASSED!"
