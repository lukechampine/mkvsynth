# mkvsynth #

mkvsynth is a non-linear video editor for Linux, designed with parallel video processing in mind. It allows users to manipulate and encode videos through an easy-to-learn scripting system. mkvsynth was created out of frustration with Avisynth, the current standard for non-linear video editing. Avisynth is Windows-only, unmaintained, and has an ugly and unintuitive scripting language. For these reasons, we feel that starting from scratch is a better approach than simply porting Avisynth to Linux.

mkvsynth is comprised of two parts: Jarvis, the control program, which manages the video processing; and delbrot, the scripting language interpreter. These programs are described in more detail below.

Installation
------------
To build mkvsynth, you need the FFmpeg and x264 libraries:
```
$ git clone https://github.com/FFmpeg/FFmpeg
$ cd FFmpeg && ./configure
$ make && make install

$ git clone https://github.com/DarkShikari/x264-devel
$ cd x264-devel && ./configure
$ make && make install
```
You may also need more dev packages depending on your system. For Ubuntu, you need:

- libavcodec-dev
- libavformat-dev
- libavutil-dev

Now you can install mkvsynth:
```
$ git clone https://github.com/mkvsynth/mkvsynth
$ cd mkvsynth
$ make && make install
```

If you want to hack on the interpreter, you'll have to install Flex and Bison, which can be found through your package manager. The makefile will automatically detect changes to `delbrot.l` and `delbrot.y` and will call Flex and/or Bison accordingly. For convenience, `make delbrot` will build just the bare interpreter without any video processing capabilities. This has the dual advantage of 1) faster compilation times, and 2) no need to install FFmpeg or x264 if you just want to contribute to delbrot.

delbrot
-------
delbrot is the interpreter for the mkvsynth scripting language. AviSynth is controlled solely through scripts, and we hope to provide users with the power and portability of script-based video editing in mkvsynth. However, we will also be providing a GUI to allow for visual editing and frame comparisons. Check out delbrot's [documentation](delbrot/documentation.md) for the full language specification, and the [plugin guide](delbrot/plugins.md) for information on writing video plugins.

Jarvis
------
Jarvis is a control program that spawns and manages filters and frame streams. Jarvis operates as a parent thread that spawns and monitors filters in child threads. If a filter has a run-time error, it will be handled by Jarvis. Part of Jarvis is also a small set of functions and structs that help filters to communicate eachother. Read the [documentation](filters/Creating Filters.md) for instructions on writing filters with Jarvis.

Questions?
----------
Contact david.vorick@gmail.com for general project questions.

Contact luke.champine@gmail.com for questions related to delbrot.

*an RCOS Project*
