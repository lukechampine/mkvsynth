## writing plugins ##
Plugins are collections of functions that are written in C and loaded at runtime. Since the parser deals only in ASTnodes, all functions are required to take two ASTnodes as arguments and return an ASTnode as their result. The first argument is the "result" node which the function will modify and return. The second node is a linked list of ASTnodes, each containing an argument. Some helpful macros are provided for translating these AST functions into "standard" C functions.

As an example, let's walk through the creation of a plugin, "fact", that implements just a single function, `factorial`. First, we'll navigate to `mkvsynth/plugins/`, which contains `plugins.c` and `makeplugin`. We'll create a new file here called `fact.c` and start coding.

The first crucial step is to include the delbrot.h header file. This file contains the definition of `ASTnode` and all its associated functions/macros. y.tab.h should also be included if your function deals with booleans, because it contains the TRUE and FALSE macros. The function arguments must look exactly like they do below, or the plugin macros will break:

```c
#include "../delbrot/delbrot.h"
#include "../delbrot/y.tab.h"
ASTnode* factorial(ASTnode *p, ASTnode *args) {
    /* check that mandatory arguments are valid */
    checkArgs("factorial", args, 1, typeNum);
    ...
```
Most plugin functions should call `checkArgs()` before doing anything else. `checkArgs()` ensures that the proper number and type of arguments were supplied. In this case, our function only takes one argument of type number.

Now we can extract the function arguments from `args`:

```c
    ...
    /* get mandatory arguments */
    int n = (int) MANDNUM();
    /* get optional arguments, if we had any */
    /* bool_t factFlag = OPTBOOL("flag", TRUE); */
    ...
```

Mandatory arguments are accessed using `MANDNUM()`, `MANDSTR()`, etc., and optional arguments are accessed using `OPTNUM(argName, defaultValue)`. **Important: mandatory arguments must be accessed in order.** This is because the `MAND*()` macros all advance `args` by one step. Of course, these macros are just conveniences; you are free to traverse and manipulate `args` as you see fit.
 
Next, we'll define the actual function logic. In this case, it's pretty straightforward:

```c
    ...
    double i = 1.0;
    for (n; n > 0; n--)
        i *= n;
    ...
```

And finally, we'll define the function's return value:

```c
    ...
    RETURNNUM(i);
}
```

The `RETURN*(arg)` macros place their argument in `p`, give it the proper type, and call `return p`. Again, this can be written by hand if you so choose. We're done!

## compiling and loading plugins ##

To load your plugin into the interpreter and start using it, you'll have to compile it as a shared library using the provided `makeplugin` script. In this case, we'll run `./makeplugin fact`, which will create `libfact.so` and move it to the mkvsynth plugin directory, `~/.config/mkvsynth`. This library can easily be distributed on the internet. **It is strongly suggested** that you distribute your plugin as an archive containing the `.so` library, the source code, and a README documenting each of the functions contained in your plugin.

Finally, we're ready to use our function in mkvsynth. All we have to do is `import fact`, and then we can call our function as `fact.factorial`. Further information on plugin syntax and usage is provided in [documentation.md](documentation.md).
