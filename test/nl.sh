#! /bin/sh

echo "TESTING \n option: "

printf "a1b2c3" > newline.test.in
printf "a\r1\nb\r2\nc\r3\n" > newline.test.out


echo "forward test:"
cp newline.test.in newline.test
../src/msrp -f -d "([123])" "\1\n" newline.test
../src/msrp -f -d "([abc])" "\1\r" newline.test

cmp newline.test newline.test.out || exit 1

echo "PASSED!"

echo "reverse test:"
../src/msrp -f -d "\n" "" newline.test
../src/msrp -f -d "\r" "" newline.test

cmp newline.test newline.test.in || exit 1

echo "PASSED!"

rm newline.test.out newline.test newline.test.in


