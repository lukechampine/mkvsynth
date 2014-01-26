# mkvsynth #

mkvsynth is a modular set of specifications designed to enable parallel video processing. Right now, the major features include a parser and a control program. mkvsynth itself is nothing more than documentation and specification, but each piece is being implemented by separate programs (included in this project) titled 'delbrot' (the parser) and 'Jarvis' (the control program). The goal is to keep these programs fully modular, such that if either was substituted out for a different program that still perfectly followed spec, the other piece would still cooperate.

Installation
------------
To build mkvsynth, you need the FFmpeg and x264 libraries:
```
$ git clone https://github.com/FFmpeg/FFmpeg.git
$ cd FFmpeg && ./configure
$ make && make install

$ git clone https://github.com/DarkShikari/x264-devel.git
$ cd x264-devel && ./configure
$ make && make install
```
You may also need more dev packages depending on your system. For ubuntu, you need:

- libavcodec-dev
- libavformat-dev
- libavutil-dev

Now you can run `make` to build mkvsynth.

If you want to hack on the interpreter, you'll have to install Flex and Bison, which can be found through your package manager. The makefile will automatically detect changes to `delbrot.l` and `delbrot.y` and will call Flex and/or Bison accordingly. For convenience, `make delbrot` will build just the bare interpreter without any video processing capabilities. This has the dual advantage of 1) faster compilation times, and 2) no need to install FFmpeg or x264 if you just want to contribute to delbrot.

delbrot
-------
delbrot is the interpreter for the mkvsynth scripting language. AviSynth is controlled solely through scripts, and we hope to provide users with the power and portability of script-based video editing in mkvsynth. However, we will also be providing a GUI to allow for visual editing and frame comparisons. At present, it is unknown how mkvsynth's scripting language will interface with the GUI; one approach is to directly translate UI actions into delbrot commands. This would facilitate easy sharing of scripts without requiring the user to manually write any code.

Jarvis
------
Jarvis is the (new) codename for the program that spawns and manages filters and frame streams. Jarvis operates as a parent thread that spawns and monitors filters in child threads. If a filter has a run-time error, it will be handled by Jarvis. Part of Jarvis is also a small set of functions and structs that help filters to communicate eachother. Full details can be found in the documentaion inside of the 'filters' folder, but these functions include 'getFrame()' 'putFrame()' and the structs include mkvsynthFrame and mkvsynthControlNode.

Roles
-----
- David: project lead, filter creation, x264 integration
- Luke: scripting language lead, delbrot implementation
- Forest: GUI wizard, scripting language design

Questions?
----------
Contact david.vorick@gmail.com for general project questions.

Contact luke.champine@gmail.com for questions related to delbrot.

Contact trimble.forest@gmail.com for questions related to the GUI.

*an RCOS Project*
