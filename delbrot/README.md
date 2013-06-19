# delbrot #
This is the current implementation of delbrot, the MkvSynth script interpreter.

## features ##
At present, delbrot is essentially a glorified calculator. It only has support for doubles (strings will be added soon, I promise). But already it showcases a level of sophistication greater than any previous implementation. Control structures like if statements and while loops are fully supported, and expressions are evaluated recursively before being passed to functions:

```c
x = y = 3;
if (x == y) {
    while (x > 0) {
        print(x,x*y);
        x = x - 1;
        y = y^x; // exponentiation, not XOR
    }
}
// output: 3 9
//         2 18
//         1 9
```
These are basic operations, to be sure, but they demonstrate the advantages of using flex and bison over writing our own interpreter from scratch.

## syntax ##
The syntax of delbrot has fluctuated wildly over the course of its development, so it is unknown at this point what the final syntax of the language will look like. At present, it's strongly influenced by C. You should be able to grok the basic structure of the language from the example script file.

## implementation ##
delbrot uses flex and bison for lexing and parsing operations. flex scans the input and returns tokens that satisfy a set of regex-based rules. bison matches the tokens to a BNF-style grammar and executes any associated C code. For a more detailed explanation, see the documentation file. These tools confer all the advantages of a native C interpreter without the hassle of scanning, tokenizing, and parsing the source file.

To try out the interpreter, simply run `make && ./del` to enter an interactive session. Control-D exits.

## a short history ##

delbrot has had a rough life. It started out as a C translator written in Perl by Luke, which offended the sensibilities of a lot of people. In response, Forest started work on a native C interpreter, but his implementation suffered from a lack of good compiler principles (i.e. the standard model of scanner -> tokenizer -> parser, etc.).

Soon it was demo time, and a big push was made to bring the Perl implementation up the required level of sophistication. Once the demo was over, though, all work on delbrot promptly ceased.

A few months later, Luke heard about lex and yacc from his father, and did something unheard of in the realm of software: he *accepted that his approach was inferior*. Seeing the power of these tools, he decided to scrap all of his Perl code in favor of a native C interpreter powered by flex and bison. And that brings us to today.
