# parser for the delbrot scripting language
# checks lexed source for errors, then translates into C and executes

use feature "switch";

# open files for reading/writing
open(INFILE, 'lexedsource');
open(OUTFILE, '>output.c');

# read lexedsource into an array for easier parsing
@source = <INFILE>;

# add necessary headers and other structural elements 
print OUTFILE "#include <stdio.h>\n#include <string.h>\nint main() {\n";

# initial linenumber value, used for error messages
$linenumber = 1;

# main parsing loop
while (@source) {

    # quit if end of file encountered
    if (@source[0] eq "END") {
        last;
    }

    # ignore comments
    if (type(@source[0]) eq "COMMENT") {
        shift @source; shift @source;
    }

    # read in a full statement
    my @statement;
    while (1) {
        my $nextline = shift(@source);
        chomp($nextline);

        # statements ends when terminator is encountered
        last if ($nextline =~ /^TERMINATOR/ or $nextline =~ /^END/);

        push(@statement, $nextline);
    }

    # parse statement
    parse(@statement);

    $linenumber++;
}

# add closing statements/braces
print OUTFILE "\treturn 0;\n}\n";

# close files
close(OUTFILE);
close(INFILE);

### END MAIN BODY ###

# error handler
sub error {
    # get argument
    $errormsg = @_[0];
    
    # print error message and line number to STDERR
    print STDERR "failed!\nACDC: error (line $linenumber): $errormsg\n";
    
    exit(-1);
}

sub checkvar {
    # get arguments
    my $varname = shift(@_);
    my $type = shift(@_);

    # check that variable has been initialized
    if (!exists $varhash{$varname}) {
        error("could not find variable \"$varname\" to filter");
    }

    # check that variable is the right type
    if ($varhash{$varname} ne $type) {
        error("method \"$method\" is not defined for type \"$varhash{$varname}\"");
    }
}
# simple functions for getting the type and name of a line
sub type { return (split(' ', pop))[0]; }
sub name { return substr(@_[0], length(type(@_[0])) + 1); }

# expect: determine what statements are allowed to follow the current one
sub expect {
    # get arguments
    my $type = type(@_[0]);
    my $name = name(@_[0]);
    my $next = type(@_[1]);

    # determine what types can follow the current type
    given ($type) {
        when (/^IDENTIFIER/) {
            if ($next ne "SYMBOL") {
                return "identifier \"$name\" must be followed by a symbol";
            }
        }
        when (/^SYMBOL/) {
            if ($next eq "TERMINATOR") {
                return "bare symbol \"$name\"";
            }
        }
        when (/^STRING/) {
            if ($next ne "SYMBOL") {
                return "string \"$name\" must be followed by a symbol";
            }
        }
        when (/^TERMINATOR/) {
            if ($next ne "IDENTIFIER" || $next ne "TERMINATOR") {
                $linenumber++;
                return "statements must begin with an identifier";
            }
            $linenumber++;
        }
        default {
            error("unrecognized token type \"$type\"");
        }
    }
}

# main parsing logic
sub parse {
    # get lines to be parsed
    my @statement = @_;

    # check for type errors
    for (my $i = 0; $i < $#statement; $i++) {
        expect(@statement[$i],@statement[$i + 1]);
    }

    # get variable name
    if (type(@statement[0]) ne "IDENTIFIER") {
        error("lines must begin with an identifier");
    }
    # store in a global variable for easy access
    $varname = name(shift(@statement));

    # skip '.' symbol
    shift(@statement);
    # store method as a global variable for easy access
    $method = name(shift(@statement));

    # get arguments
    shift(@statement);
    my @arguments;
    while (1) {
        $argument = name(shift(@statement));
        if ($argument eq ")") { last; }
        if ($argument eq ",") { next; }
        if (!$argument) { error("invalid method (missing parenthesis?)"); }
        push(@arguments, $argument);
    }

    # process arguments based on method
    given ($method) {
        when (/^init/) {  # arguments: type, value
            # pass to initializer function
            init(@arguments);
            
            # register variable in hash
            $varhash{$varname} = shift(@arguments);
        }
        when (/^append/) { # arguments: string
            # ensure that variable to be modifed exists and is of the right type
            checkvar($varname, "string");
            
            print OUTFILE "\tstrcpy($varname + strlen($varname), @arguments);\n"
        }
        when (/^output/) { # arguments: none
            # ensure that variable exists
            if (!exists $varhash{$varname}) {
                error("could not find variable \"$varname\" to output");
            }

            print OUTFILE "\tprintf(\"%s\",$varname);\n"
        }
        default {
            error("unrecognized command \"$method\"");
        }
    }
}

sub init {
    # get argument
    my $type = shift(@_);
    my @arguments = @_;

    # parse by command type
    given ($type) {
        when (/^string/) { # arguments: string
            print OUTFILE "\tchar $varname\[1024] = " . shift(@arguments) . ";\n";
        }
        default {
            error("unrecognized type \"$type\"");
        }
    }
}

