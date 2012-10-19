#! /bin/sh
set -e

echo "MSRP=$MSRP, DIR: $DIR"

"$DIR/nl.sh"
"$DIR/file.cc.sh"
"$DIR/git.mv.sh"
"$DIR/git.tracked.sh"
