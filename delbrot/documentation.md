# scripting language documentation #

The Mkvsynth scripting language is pretty simple and easy to learn. Those who are used to Avisynth scripting may have to unlearn a few habits, but overall the two languages are fairly similar (e.g. neither has a real name!).

The main differences are:

| Property       | Avisynth                                        | Mkvsynth                                |
|:---------------|:------------------------------------------------|:----------------------------------------|
| Terminator     | Newline (use `\` for line continuation)         | Semicolon                               |
| Case sensitive | No                                              | Yes                                     |
| Types          | `int`, `float`, `bool`, `string`, `clip`, `val` | `num`, `bool`, `string`, `clip`         |
| Functions      | `Function foo(int i, bool "b") {...`            | `function foo(num n, :bool b) {...`     | 
| Function calls | `foo(arg1, arg2 + 3, optArg=val)`               | `foo arg1 (arg2 + 3) optArg:val`        |
| Chaining       | `clip.Foo().Bar().Baz()`                        | `clip -> Foo -> Bar -> Baz`             |
| Default        | `radius = Default(radius, 2)`                   | `default radius: 2`                     |
| if/else        |  Not supported                                  | Supported                               |
| Ternary        | `cond ? foo : bar`                              | `cond ? foo ¦ bar`                      |

## Full specification ##

### types ###
| type     | literal              | description                                   |
|:---------|:---------------------|:----------------------------------------------|
| `num`    | `3`, `1.5`, `.1`     | A number. Internally represented as a double. |
| `bool`   | `True`, `False`      | A boolean. Case-sensitive.                    |
| `string` | `"Hello"`, `"World"` | A string. Most escape sequences supported.    |
| `clip`   | N/A                  | A video. Actually a pointer to a video file.  |

### keywords ###
| keyword      | description             | example                                     |
|:-------------|:------------------------|:--------------------------------------------|
| `if`, `else` | Conditional statement   | `if (x == 1) print "yes"; else print "no";` |
| `function`   | Function definition     | `function foo(num x) { ...`                 |
| `default`    | Specify default value   | `default radius = 2;`                       |
| `return`     | Function return value   | `return (result * 3);`                      |
| `otherwise`  | Ternary syntactic sugar | `y = x == 1 ? "yes" ¦ otherwise ? "no";`    |

### built-in functions ###
| function     | description                | example                                     |
|:-------------|:---------------------------|:--------------------------------------------|
| `print`      | Generalized print function | `print 12 False "hi"`                       |
| `assert`     | Assertion statement        | `assert (frames > 2) "not enough frames"`   |
| `show`       | Convert to string          | `show 12.3 == "12.3"`                       |
| `read`       | Convert string to num      | `read "12.3" == 12.3`                       |

### operators ###
| operator                | description                                                            |
|:------------------------|:-----------------------------------------------------------------------|
| `+`,`-`,`*`,`/`,`^`,`%` | Standard binary arithmetic operators. `^` is exponentiation, not XOR.  |
| `==`,`!=`,`¦¦`,`&&`     | Standard binary boolean operators.                                     |
| `>`,`<`,`<=`,`>=`       | Standard binary relational operators.                                  |
| `+=`, `-=`, `*=`...     | Arithmetic assignment operators.                                       |
| `!`, `-`                | Standard unary negation operators.                                     |
| `->`                    | Function chaining operator. Appends LHS to front of RHS argument list. |
| `=>`                    | Chaining assignment operator. `a =>` is equivalent to `a = a ->`       |
| `? ¦`                   | Standard ternary operator, using `¦` in place of `:`                   |
| `:`                     | Optional argument operator. Marks function arguments as optional.      |

### misc. syntax ###
| syntax                  | description                                                            |
|:------------------------|:-----------------------------------------------------------------------|
| `#`                     | Comment signifier. There is no multi-line comment signifier.           |
| `;`                     | Statement terminator.                                                  |


### statements ###
**function declarations:**
```ruby
function foo(bool b, :string s) {
    default s: "bar";
    if (b)
        return s;
    else
        return 0;
}
```
This tiny example showcases everything you need to know about function definitions. Functions do not have an explicit return type; this example returns either a `num` or a `string`. Optional arguments are marked by a `:` preceding their type. Inside the function body, a `default` statement is used to set the value of an optional argument if it is not supplied in the function call. Next, an `if`/`else` statement is used to determine the return value of the function.

**function calls:**

Now we can call `foo`:
```ruby
foo True            # returns "bar"
foo (True && False) # returns 0
foo True s:"baz"    # returns "baz"
```
Function arguments are separated by spaces, as in Haskell. Note that this means that you may need to enclose arguments in parentheses, or you might get some strange error messages. Optional arguments are specified using their name, and can be declared in any order. Note how `:` is used in all expressions relating to optional arguments: function definitions, function calls, and default statements.

**assignment statements:**
```ruby
x = "Hello";  # x is "Hello"
x = 12;       # x is 12
y = (x %= 7); # x is 5, y is 5
```
Assignment statements are pretty straightforward. Note that variable names are not preceded by a type, and a variable can easily be reassigned to a different type. Since assignment "statements" are really just expressions, they also return the value of the variable they are assigning to, as shown in the last line of the example.

**ternary expressions:**
```ruby
x = y < 100 ? 50 | 1000;

speed = x < 100   ? "slow"
      | x < 300   ? "medium"
      | otherwise ? "fast";
```
In Avisynth, ternary expressions are used heavily to compensate for the language's lack of `if`/`else` statements. This is not an issue in Mkvsynth, but the ternary expression is still nice to have. In addition, the `otherwise` construct is provided to make long ternary chains more readable. `otherwise` acts as a catch-all, as in Haskell (or `default` in C).

**function chains:**
```ruby
clp => Lanczos4Resize dx dy
    -> TurnLeft
    -> SangNom aa:aath
    -> TurnRight
    -> SangNom aa:aath
    -> LanczosResize ox oy;
```
The `->` operator is used to pass a variable through a series of functions, allowing the script author to avoid lengthy, unreadable nested function calls. Internally, the value on the left side is appended to the front of the argument list on the right side. This is why all video filters should have the input clip as their first argument. The `=>` operator can be used to assign the resulting value to the original variable.
