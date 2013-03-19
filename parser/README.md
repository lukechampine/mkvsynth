# delbrot #
This is the current implementation of delbrot, the scripting language for MkvSynth. At the moment, it's very rudimentary: it only supports string operations. This will change once some actual video filters are written by the rest of the team. 

## Features ##
Here's what commands are currently supported:
- string: assign a string to a variable
- filter: apply a filter to a variable
- output: output the contents of a variable

And here are what filters are currently supported:
- append: add more text to the end of an existing string

So yes, pretty limited at the moment, but it demonstrates the core concepts of the language well.

Function chaining is working, at least for append operations. You can chain as many appends as you want.

## Syntax ##
The syntax of delbrot is still a point of contention. This directory contains a parser for the "old" syntax, which I support, andthe revisedsyntax subdirectory contains a parser for the "new" syntax, which David and Forest support. Both directories contain an example file which gives a basic outline of the syntax. Official documentation is forthcoming.

## Implementation ##
Scripts are parsed using Perl and translated into C code, which is then compiled and executed. This is somewhat ugly, but doing the parsing natively in C wouldn't be a walk in the park either.

### Perl ###
- + Great parsing capabilities
- + Easy to extend
- + Generated C file can be edited directly if desired
- - Can't natively call MkvSynth libraries; must compile every run
- - Parser cannot be compiled, must be interpreted

### C ###
- + Allows direct access to MkvSynth libraries
- + Homogenous codebase: project is entirely C/C++, no additional Perl dependency
- + Fast, can be compiled
- - Parsing is ugly and more of a pain to extend
- - Variable storage/lookup is ugly

Note that there are ways of calling C functions from inside Perl; once some library functions are implemented, we will definitely be investigating this avenue. At present, Forest and I are pursuing the C and Perl implementations separately (and respectively).
