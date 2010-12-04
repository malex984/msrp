#! /bin/sh

cp -f file.cc.in file.cc

echo "forward test:"
../src/msrp -f -d "[$]ID[$]" "\\f" file.cc

cmp file.cc file.cc.out || exit 1

echo "PASSED!"

echo "backward test:"
../src/msrp -f -d "\f" "\$ID\$" file.cc

cmp file.cc file.cc.in || exit 1

echo "PASSED!"

rm file.cc



