#! /bin/sh

echo "TESTING \n option: "

echo -n -e "a1b2c3" > tmp.test

echo "forward test:"
../src/msrp -f -d "([123])" "\1\n" tmp.test
../src/msrp -f -d "([abc])" "\1\r" tmp.test

echo -n -e "a\r1\nb\r2\nc\r3\n" > tmp.test.out

cmp tmp.test tmp.test.out || exit 1

echo "PASSED!"


echo "reverse test:"
../src/msrp -f -d "\n" "" tmp.test
../src/msrp -f -d "\r" "" tmp.test

echo -n -e "a1b2c3" > tmp.test.out

cmp tmp.test tmp.test.out || exit 1

echo "PASSED!"

rm tmp.test.out tmp.test



