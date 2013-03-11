# parser for the delbrot scripting language
# translates delbrot into C, then compiles with gcc and executes
# v0.1 authored on 03/05/13 by Luke Champine

use Text::ParseWords;
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

    # if line is a comment, ignore it
    if (substr($inputline, 0, 1) eq "#") {
        next;
    }

    # otherwise, parse line
    parse($inputline);
}

# add closing statements/braces
print OUTFILE "\treturn 0;\n}\n";

# close files
close(OUTFILE);
close(INFILE);

# compile and execute finished program
system("gcc output.c -o compiledscript");
system("./compiledscript");

### END MAIN BODY ###


# main parsing logic
sub parse {
    # get argument
    $inputline = @_[0];

    # get the first word of the line (i.e. the command type) and the variable name
    ($command, $varname) = split(/[ \t]+/, $inputline, 3);
    
    # chop off the declarative part of the line for easier parsing
    $inputline =~ s/[^=]*=\s*//;

    # remaining parsing varies by command type
    given ($command) {
        when (/^string/) {  # argument is a string
            # check that argument is surrounded by quotes


            print OUTFILE "\tchar $varname [1024] = $inputline;\n";

            # register variable as a string in the hash
            $varcoms{$varname} = $command;
        }
        when (/^filter/) { # argument is one or more filters, each with their own parameters
            # ensure that variable to be filtered exists
            if (!exists $varcoms{$varname}) {
                error("could not find variable \"$varname\" to filter");
            }
            
            # pass line to special filter parser
            parsefilter($inputline);
        }
        when (/^output/) { # no argument
            # ensure that variable exists
            if (!exists $varcoms{$varname}) {
                error("could not find variable \"$varname\" to output");
            }
            print OUTFILE "\tprintf(\"%s\",$varname);\n"
        }
        default {
            error("unrecognized command \"$command\"");
        }
    }
}

# filter-specific parsing
sub parsefilter {
    # get argument
    $filterline = @_[0];

    # split line into an array of individual filter commands, delimited by '->'
    @filters = quotewords('\s*->\s*', 0, $inputline);

    # get filter type
    ($filter) = split('\(', $inputline, 2);
            
    # chop off leading edge, leaving arguments
    $inputline =~ s/^[^(]+\(//;

    # remaining parsing varies by filter type
    given ($filter) {
        when (/^append/) { # argument is a string
            # remove trailing paren
            $inputline =~ s/\)$//;
            print OUTFILE "\tstrcpy ($varname + strlen($varname),$inputline);\n";                   
        }
        default {
            error("unrecognized filter \"$filter\"");
        }
    }
}

# error handler
sub error {
    # get argument
    $errormsg = @_[0];
    
    # print error message and line number to STDERR
    print STDERR "error (line $linenumber): $errormsg\n";
    
    # close files and delete output.c
    close(OUTFILE);
    close(INFILE);
    unlink "output.c";

    exit();
}
