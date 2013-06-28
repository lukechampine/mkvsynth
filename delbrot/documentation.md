# delbrot documentation #
delbrot has grown appreciably in complexity since adopting a flex+bison implementation. This document aims to explain the inner workings of the delbrot.l, delbrot.y, and delbrot.c.

## basic workings ##
flex and bison are programs that take definition files and turn them into C functions. flex creates a *lexing function*, which reads in a segment of the input file, matches it to a rule, and returns the corresponding token. bison creates a *parsing function*, which reads in tokens, matches them to a rule, and executes the corresponding C code. The whole process looks something like this:

    /********** flex ***********\ /** bison **\
    scanner -> tokenizer -> lexer -> parser -> interpreter

If your language's grammar is very simple, bison can serve as your interpreter, and the two definition files are all you need. But for any sort of high-level language (delbrot included), bison alone isn't enough. Instead of being used to execute code, bison is used to generate an *abstract syntax tree*, or AST, which contains all the parsed values, function calls, etc. in a hierarchical structure. A simple AST might look something like this:

           if
          /  \
         /    \
        /      \
      cond    stmt
     / | \      |
    x  0  >    ...

The equivalent C code would be:

    if (x > 0) {
        ...
    }

A separate function is then used to walk down the AST and execute the code. This is the model that delbrot uses.

## delbrot.l ##
This file contains the lexing rules for the language. flex matches text using regexs and always chooses the longest match. The corresponding C code is then executed. In most cases, this amounts to little more than returning the matched value and/or appropriate token type. The special cases here are for identifiers and numbers. For our purposes, an identifier is a character sequence that does not begin with a number, and a number is a sequence of digits with an optional decimal portion. Since all text is scanned in as chars, we must first convert digits to numbers with the `atof()` function. Identifiers are a bit more complicated, but essentially we just try to match the identifier against a list of keywords and functions. If it's a keyword, we return the appropriate token. If it's a function name, we return the appropriate function pointer. If it's neither of those, we assume it's a variable and allocate space for it in our symbol table.

## delbrot.y ##
Here we define the grammar of the language. Now that we have a sequence of tokens, we read them in one at a time until they match one of the defined rules, at which point we execute the corresponding C code. First we define a union of types; these are the possible types that a rule can have. Since we are constructing an AST, all of our rules have type ASTnode. Then we define precedences for each operator, so the parser knows how to resolve expressions like `x + y / x`.

Next comes the main grammar section. For each rule, we define what arrangements of tokens qualify as an instance of that rule. For example, a statement can be an expression followed by a semicolon, a while loop, an if statement, etc. And for each instance, we define what we want to do with the tokens to turn them into ASTnodes. For numerical values we call `mkValNode()`, for variables we call `mkVarNode()`, and for just about everything else we call `mkOpNode()`. This is because a lot of our syntax is used to represent mathematical operations, like `+` and `^`. Below the grammar section, we simply define the functions that we use to create these nodes.

## delbrot.c ##
This file contains the `ex()` function, which is called each time we read in a full statement. The purpose of `ex()` is to reduce a section of the AST. In the example above, that would mean reducing the "cond" node (and all its children) down to a single node containing either "true" or "false." As such, this function must be highly recursive, since a node's children may need to be reduced before they can be meaningfully combined. This is also where we define the built-in functions of the language.

## memory concerns ##
Currently, the interpreter is more prone to leaks than the United States government. This situation has improved slightly now that the `freeNode()` function has been reenabled, but `ex()` is still quite leaky.
