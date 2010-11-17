There is a nice tool: msrp

(just take a look at:
http://agateau.wordpress.com/2010/03/10/msrp-a-useful-addition-to-your-toolbo/)

it relies on PCRE to do regexp-matching and is limited to \\0..\\1 in back-regexp-ing

i wanted to be able to substitute some tag by file' name while
changing the file. 

Example: imagine a file 'file.cc' containing:
//// \\file $ID$ 

i want it to look as follows:
//// \\file file.cc 

after doing msrp -f -d "[$]ID[$]" "\\f" file.cc


ps: yes, i know about 'sed', but for doing search-and-replace in multiple files i prefere this tool 
and the missing feature was bugging me a bit.

ps2: yes, this is not my tool. Unfortunately (AFAIK) it is unmaintained :(
with this repo (https://github.com/malex984/msrp) i just want to publish my improvements.

Yours, 
 malex984


