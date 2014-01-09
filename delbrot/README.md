# delbrot #
This is the current implementation of delbrot, the mkvsynth script interpreter.

## features ##
delbrot has come a long way since its first incarnation. Function chaining, recursion, and optional parameters are all supported. The result is a language that is both cleaner and more powerful than what Avisynth has to offer. Here's a quick overview; see [here](documentation.md) for more comprehensive documentation.

```ruby
# Project Euler problem 1
function euler(num acc, num x) {
    if (x == 1000)
        return acc;
    if (x % 3 == 0 || x % 5 == 0)
        acc += x;
    return (euler acc (x + 1));
}
print (euler 0 0); # prints "233168"

# function chaining
clp -> Lanczos4Resize dx dy
    -> TurnLeft
    -> SangNom aa:aath
    -> TurnRight
    -> SangNom aa:aath
    -> LanczosResize ox oy;
# compare to Avisynth: clp.Lanczos4Resize(dx,dy).TurnLeft().SangNom(aa=aath).TurnRight().SangNom(aa=aath) \
#   .LanczosResize(ox,oy)

# optional arguments
function foo(:string str) {
    default str: "World";
    print "Hello," str;
}
foo;            # prints "Hello, World"
foo str:"Luke"; # prints "Hello, Luke"

# simulating a guard with ternary expressions
speed = x < 100   ? "slow"
      | x < 300   ? "medium"
      | otherwise ? "fast";
```

Like Avisynth, users can write their own filters/functions in C and distribute them as plugins. 

## syntax ##
The syntax of delbrot has fluctuated wildly over the course of its development. Until recently, it largely resembled C and other curly-brace, semicolon-terminated languages. In its current incarnation, a strong Haskell influence can be seen, especially in function calls. We've found that in most cases, this syntax increases readability and reduces keystrokes.

## implementation ##
delbrot uses Flex and Bison for lexing and parsing operations. Flex scans the input and returns tokens that satisfy a set of regex-based rules. Bison matches the tokens to a BNF-style grammar and executes any associated C code. These tools confer all the advantages of a native C interpreter without the hassle of scanning, tokenizing, and parsing the source file.

## a short history ##

delbrot has had a rough life. It started out as a C "transpiler" written in Perl by Luke, which offended the sensibilities of a lot of people. In response, Forest started work on a native C interpreter, but his implementation suffered from a lack of good compiler principles (i.e. the standard model of scanner -> tokenizer -> parser, etc.).

Soon it was demo time, and a big push was made to bring the Perl implementation up the required level of sophistication. Once the demo was over, though, all work on delbrot promptly ceased.

A few months later, Luke heard about Lex and Yacc. Seeing the power of these tools, he decided to scrap his Perl kludge in favor of a native C interpreter powered by Flex and Bison. The result is faster, easier to develop, and much more sane.
