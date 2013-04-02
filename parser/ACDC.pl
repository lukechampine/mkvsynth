# ACDC: the "Actually C" Delbrot Compiler
# scans, lexes, parses, compiles, and executes a delbrot script

print "ACDC: scanning....";
system("perl scanner.pl");
print "done!\nACDC: lexing......";
system("perl lexer.pl");
if ($?) { unlink ("scannedsource", "lexedsource"); exit(); }
print "done!\nACDC: parsing.....";
system("perl parser.pl");
if ($?) { unlink ("scannedsource", "lexedsource", "parsedsource"); exit(); }
print "done!\nACDC: compiling...";
system("gcc output.c -o compiledscript");
if ($?) { unlink ("scannedsource", "lexedsource", "parsedsource"); exit(); }
print "done!\n\n";
system("./compiledscript");
