# delbrot #
This is the current implementation of delbrot, the mkvsynth script interpreter.

## features ##
delbrot has come a long way since its first incarnation. Functions can be defined (and called recursively), method calls can be chained, and optional parameters are supported:

```ruby
# Project Euler problem 1
total = 0;
for(x = 0; x < 1000; x++)
	if (!(x % 3 && x % 5))
		total += x;
print(total); # prints "233168"

# recursive version
function euler(int acc, int x) {
	if (x == 1000)
		return acc;
	if (!(x % 3 && x % 5))
		acc += x;
	return euler(acc, x + 1);
}
print(euler(0,0)); # prints "233168"

# method chaining
0.sin.cos.print; # equivalent to print(cos(sin(0))), prints "1"

# optional arguments
ffmpegDecode("example.mkv");            # prints "decoded example.mkv"
ffmpegDecode("example.mkv", frames:10); # prints "decoded 10 frames of example.mkv"
```

There is also some rudimentary support for plugins now, so users can define their own filters/functions using the provided tools and integrate them into the parser without recompiling.

## syntax ##
The syntax of delbrot has fluctuated wildly over the course of its development, so it is unknown at this point what the final syntax of the language will look like. However, one goal of the language is to avoid diverging significantly from what people are used to, i.e. AviSynth's scripting language. 100% compatibility with AviSynth would be undoubtedly convenient, but at the moment that is not a design goal.

## implementation ##
delbrot uses Flex and Bison for lexing and parsing operations. Flex scans the input and returns tokens that satisfy a set of regex-based rules. Bison matches the tokens to a BNF-style grammar and executes any associated C code. These tools confer all the advantages of a native C interpreter without the hassle of scanning, tokenizing, and parsing the source file. For a more detailed explanation of how the interpreter works, see the documentation file.

## a short history ##

delbrot has had a rough life. It started out as a C "transpiler" written in Perl by Luke, which offended the sensibilities of a lot of people. In response, Forest started work on a native C interpreter, but his implementation suffered from a lack of good compiler principles (i.e. the standard model of scanner -> tokenizer -> parser, etc.).

Soon it was demo time, and a big push was made to bring the Perl implementation up the required level of sophistication. Once the demo was over, though, all work on delbrot promptly ceased.

A few months later, Luke heard about Lex and Yacc from his father, and did something unheard of in the realm of software: he *accepted that his approach was inferior*. Seeing the power of these tools, he decided to scrap all of his Perl code in favor of a native C interpreter powered by Flex and Bison. And that brings us to today.
