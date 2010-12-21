#! /bin/sh

echo "TESTING --git option on untracked files: "

echo "a" > file.aa.file || exit 1
echo "g" > file.aa.file.git || exit 1

git add file.aa.file.git || exit 1

echo "testing: mv --git / tracked: "
$MSRP --git -c -d "aa" "bb" file.aa.file.git || (git rm -f file.aa.file.git; rm file.aa.file; exit 1)

test -e file.bb.file.git || (git rm -f file.aa.file.git; rm file.aa.file; exit 1)

echo "PASSED!"

echo "testing: mv --git / untracked: "
$MSRP --git -c -d "aa" "bb" file.aa.file || (git rm -f file.bb.file.git; rm file.aa.file; exit 1)

test -e file.bb.file || (git rm -f file.bb.file.git; rm file.aa.file; exit 1)

echo "PASSED!"

rm file.bb.file || exit 1
git rm -f file.bb.file.git || exit 1





