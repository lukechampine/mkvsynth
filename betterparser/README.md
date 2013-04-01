# delbrot++ #

This is the current implementation of delbrot++, the better scripting language 
for MkvSynth. Similarly to delbrot, it's functionality currently only extends 
to string operations as a proof of concept more than anything else. However,
it was designed to be fairly extensible, and it should prove not too challenging
to add the functionality once the interaction has been written up by the other
members of the team. 

## Implementation ##

This is where delbrot++ really differs from delbrot. Personally, I did not like
the fact that the script files were written to C and then compiled; it seemed 
like a strategy that was 
- Not as fun as having it run on the fly
- Really not our own scripting language, but rather some wrappers to C
- Non-homogeneous
Nonetheless, C seems a bit inferior to Perl when it comes to string 
parsing. However, I think that this advantage is overshadowed by the overall 
integration with the project that we can achieve with a native parser. 

## Features ##
Things that are supported right now:
- Technically, variables can have a variety of different types, but the only 
  one that has really been implemented so far is the string type.
    - Strings can be appended and output.
- Support has been added for dynamic function and variable lookup. 
    - Dynamic function lookup should be a pretty exciting concept if we ever have
   any plugins, as it provides for a very extensible parser.
    - Dynamic variable lookup is simply a necessity in order to allow for any sort
      of real scripting language.

### Other Important Information ###

Note that this folder is called betterparser. As such, you can rest assured that
Luke's implementation is entirely inferior. It may as well be in a folder called
worseparser. 