#! /bin/sh

echo "TESTING --first option: "

cp -f "$DIR/first.in" tmp.first.out

$MSRP --first -f -d "Ab" "!!" tmp.first.out

cmp --quiet tmp.first.out "$DIR/first.out" || exit 1

echo "PASSED!"
rm tmp.first.out



