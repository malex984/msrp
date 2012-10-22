#! /bin/sh

echo "TESTING \n option: "

printf "a1b2c3" > newline.test.in
printf "a\r1\nb\r2\nc\r3\n" > newline.test.out


echo "forward test:"
cp newline.test.in newline.test
$MSRP -f -d "([123])" "\1\n" newline.test
$MSRP -f -d "([abc])" "\1\r" newline.test

cmp --quiet newline.test newline.test.out || exit 1

echo "PASSED!"

echo "reverse test:"
$MSRP -f -d "\n" "" newline.test
$MSRP -f -d "\r" "" newline.test

cmp --quiet newline.test newline.test.in || exit 1


rm newline.test.out newline.test newline.test.in || exit 1


echo "PASSED!"
