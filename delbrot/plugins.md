## writing plugins ##
Plugins are user-defined functions that live in their own .c file. Since the parser deals only in ASTnodes, all functions are required to take two ASTnodes as arguments and return an ASTnode as their result. The first argument is the "result" node which the function will modify and return. The second node is a linked list of ASTnodes, each containing an argument. Some helpful macros are provided for translating these AST functions into "standard" C functions.

As an example, let's examine the provided "myPlugin" plugin, specifically the `myPlugin_AST()` function.

```c
#include "delbrot.h"
ASTnode* myPlugin_AST(ASTnode *p, ASTnode *args) {
    /* check that (mandatory) arguments are valid */
    checkArgs("myPlugin", args, 1);
    ...
```
The first crucial step is to include the delbrot.h header file. This file contains the definition of `ASTnode` and all its associated functions/macros. For most plugins, the first step should be to call the `checkArgs()` function, which ensures that the function received the correct number of mandatory arguments (later, type-checking will be added as well).

```c
    ...
    /* get arguments */
    char *str = args->str;
    /* get optional arguments */
    double frames = OPTVAL("frames", -1);
    char *output  = OPTSTR("output", NULL);
    ...
```
Then we can extract the arguments from `args`. Mandatory arguments are accessed directly. To get the next argument, use `args->next->`, `args->next->next->`, etc. This method will probably be replaced with a macro at some point to simplify things.

Next come the optional arguments. A different macro must be called depending on the expected return value. Simply provide the name of the argument and the default value, and the macro will do the rest.

```c
    ...
    myPlugin(str, frames, output);
    ...
```
If we so choose, we can pass these extracted values to a more traditional function that takes standard data types. This makes it easy to abstract away the argument handling and focus on the function logic. Alternatively, just write out the logic as usual and save yourself an extra function definition.

```c
    /* return value */
    p->type = typeVal;
    p->val = 0;
    return p;
}
```
Finally, we define what the function should return. In this case, our function just prints strings to STDOUT, so we return a dummy value of 0. This code will also probably be replaced with a macro.

To load your plugin into the interpreter and start using it, there's just one more step. Simply call the provided `loadplugin` program with the name of your plugin, and recompile everything (eventually, `loadplugin` will handle this step as well). In this case, we would run `./loadplugin myPlugin`. It is imperative that your function have the same name as your .c file! Otherwise, things may break.
