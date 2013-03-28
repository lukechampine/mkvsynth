# scanner for the delbrot scripting language
# translates delbrot source file into annotated list of ASCII values
# v0.1 authored on 03/27/13 by Luke Champine

# open files for reading/writing
open(INFILE, 'script.del');
open(OUTFILE, '>scannedsource');

# set initial linenumber
my $linenumber = 0;

# main lexing loop
while (<INFILE>) {
    
    # read in a line
    my $inputline = $_;
    
    # break line into array of ASCII values
    my @chars = unpack 'C*', $inputline;

    # reset char number variable
    my $charnumber = 0;

    # iterate through characters in line
    for (@chars) {
        # add a formatted line to the output file
        print OUTFILE "$linenumber $charnumber $_\n";

        # increment char number
        $charnumber++;
    }

    # increment line number
    $linenumber++;
}

# add terminating statement
print OUTFILE "END\n";

# close files
close(OUTFILE);
close(INFILE);
