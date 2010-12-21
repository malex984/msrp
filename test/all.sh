#! /bin/sh
set -e

echo "MSRP=$MSRP"
./file.cc.sh
./git.mv.sh
./nl.sh
