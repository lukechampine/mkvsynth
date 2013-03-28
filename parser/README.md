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

## Syntax ##
The syntax of delbrot is still a point of contention among team members, so it is unknown at this point what the final syntax of the language will look like. The current model is pseudo-class based, with video objects implementing filter methods. You should be able to grok the basic structure of the language from the example script file.

## Implementation ##
Script files are scanned, lexed, and parsed using Perl, then translated into C code, and finally compiled and executed. Recently, the entirely implementation has be rewritten to use the standard scanner -> lexer -> parser -> compiler approach, instead of a single Perl program doing all of the work. Forest is working on a native C implementation that will not have to be compiled. There are advantages to both strategies:

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

Note that there are ways of calling C functions from inside Perl; once some library functions are implemented, we will definitely be investigating this avenue.
