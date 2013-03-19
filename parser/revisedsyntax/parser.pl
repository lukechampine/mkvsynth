# parser for the delbrot scripting language
# translates delbrot into C, then compiles with gcc and executes
# v0.1 authored on 03/05/13 by Luke Champine

use Text::ParseWords;
use feature "switch";

# open files for reading/writing
open(INFILE, 'script.del');
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

    # get the variable name and chop it off
    ($varname) = split(/\./, $inputline, 2);
    $inputline =~ s/[^\.]*\.//;

    # get the method name and chop it off
    ($method) = split(/\(/, $inputline, 2);
    $inputline =~ s/[^\(]*\(//;
    
    # remove trailing paren and whitespace
    $inputline =~ s/\)[\s]*$//;

    # parse by command type
    given ($method) {
        when (/^init/) {  # arguments: type, value
            # pass to initializer function
            init($inputline)
        }
        when (/^append/) { # argument is a string
            # ensure that variable to be modifed exists and is of the right type
            if (!exists $varhash{$varname}) {
                error("could not find variable \"$varname\" to filter");
            }
            if ($varhash{$varname} ne "string") {
                error("method \"$method\" is not defined for type \"$varhash{$varname}\"");
            }
            
            print OUTFILE "\tstrcpy ($varname + strlen($varname), $inputline);\n"
        }
        when (/^output/) { # no argument
            # ensure that variable exists
            if (!exists $varhash{$varname}) {
                error("could not find variable \"$varname\" to output");
            }
            # add printing code
            print OUTFILE "\tprintf(\"%s\",$varname);\n"
        }
        default {
            error("unrecognized command \"$method\"");
        }
    }
}

sub init {
    # get argument
    ($type, $rest) = split(/[\s]*,[\s]*/, @_[0], 3);
    
    # parse by command type
    given ($type) {
        when (/^string/) {
            print OUTFILE "\tchar $varname [1024] = $rest;\n";
        }
        default {
            error("unrecognized type \"$type\"");
        }
    }

    # register variable in hash
    $varhash{$varname} = $type;
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
