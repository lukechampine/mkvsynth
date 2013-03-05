# parser for the delbrot scripting language
# translates delbrot into C, then compiles with gcc and executes
# v0.1 authored on 03/05/13 by Luke Champine

use feature "switch";

# open files for reading/writing
open(INFILE, 'script.txt');
open(OUTFILE, '>output.c');

# add necessary headers and other structural elements 
print OUTFILE "#include <stdio.h>\n#include <string.h>\nint main() {\n";

# main parsing loop
while(<INFILE>) {
    $linenumber++;
    
    # read in a line and remove newline character
    $inputline = $_;
    chomp($inputline);

    # remove leading/trailing whitespace
    $inputline =~ s/^[ \t]+|[ \t]+$//;

    # get the first word of the line (i.e. the command type) and the variable name
    ($command, $varname) = split(/[ \t]+/, $inputline, 3);
    
    # chop off the declarative part of the line for easier parsing
        # this regex matches everything up to the = character, then any whitespace that comes afterwards
    $inputline =~ s/[^=]*=\s*//;

    # remaining parsing varies by command type
        # for a description of what each command does and what parameters it takes, see the documentation or consult the delbrot man page
    given ($command) {
        when (/^string/) {  # argument is a string
            print OUTFILE "\tchar $varname [1024] = $inputline;\n";
            $varcoms{$varname} = $command;
        }
        when (/^filter/) { # argument is a filter with parameters
            # get filter type
            ($filter) = split('\(', $inputline, 2);
            
            # chop off leading edge, leaving arguments
            $inputline =~ s/^[^(]+\(//;

            # remaining parsing varies by filter type
                # for a description of what each filter does and what parameters it takes, see the documentation or consult the delbrot man page
            given ($filter) {
                # ensure that variable exists
                if (!exists $varcoms{$varname}) {
                    print STDERR "Error: could not find variable \"$varname\" to filter on line $linenumber\n";
                    exit();
                }
                
                when (/^append/) { # argument is a string
                    # remove trailing paren
                    $inputline =~ s/\)$//;
                    print OUTFILE "\tstrcpy ($varname + strlen($varname),$inputline);\n";                   
                }
                default {
                    print STDERR "Error: unrecognized filter \"$filter\" on line $linenumber\n";
                    exit();
                }
            }
        }
        when (/^output/) { # no argument
            # ensure that variable exists
            if (!exists $varcoms{$varname}) {
                print STDERR "Could not find variable \"$varname\" to output on line $linenumber\n";
                exit();
            }

            print OUTFILE "\tprintf(\"%s\",$varname);\n"
        }
        default {
            print STDERR "Error: unrecognized command \"$command\" on line $linenumber\n";
            exit();
        }
    }

    # map the variable name to its location (i.e. line number) in the generated C file
}

# add closing statements/braces
print OUTFILE "\treturn 0;\n}\n";

# close files
close(OUTFILE);
close(INFILE);

# compile and execute finished program
system("gcc output.c -o compiledscript");
system("./compiledscript");
