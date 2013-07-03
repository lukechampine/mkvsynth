# MkvSynth #

### Overview:

Mkvsynth is a modular set of specifications designed to enable parallel video processing. Right now, the major features include a parser and a control program. Mkvsynth itself is nothing more than documentation and specification, but each piece is being implemented by separate programs (included in this project) titled 'Delbrot' (the parser) and 'Jarvis' (the control program). The goal is to keep these programs fully modular, such that if either was substituted out for a different program that still perfectly followed spec, the other piece would still cooperate.

### Nearest Milestone:

Create a working command line program 'mkvsynth' that can take in a text file according to a (reduced) spec that will create a raw video, filter the raw video, and then save the filtered video to a file.

### Long Term Goal:

Create a linux compatible program that is superior to avisynth.

### Similar Projects:
Avxsynth, FFmpeg, Avidemux, OpenShot:

delbrot
-------
delbrot is the interpreter for the MkvSynth scripting language. AviSynth is controlled solely through scripts, and we hope to provide users with the power and portability of script-based video editing in MkvSynth. However, we will also be providing a GUI to allow for visual editing and frame comparisons. At present, it is unknown how MkvSynth's scripting language will interface with the GUI; one approach is to directly translate UI actions into delbrot commands. This would facilitate easy sharing of scripts without requiring the user to manually write any code.

Jarvis
------
Jarvis is the (new) codename for the program that spawns and manages filters and frame streams. Jarvis operates as a parent thread that spawns and monitors filters in child threads. If a filter has a run-time error, it will be handled by Jarvis. Part of Jarvis is also a small set of functions and structs that help filters to communicate eachother. Full details can be found in the documentaion inside of the 'filters' folder, but these functions include 'getFrame()' 'putFrame()' and the structs include mkvsynthFrame and mkvsynthControlNode.

Roles
-----
- David: project lead, filter creation, x264 integration
- Forest: GUI wizard, scripting language design
- Luke: scripting language lead, delbrot implementation

Questions?
----------
Contact david.vorick@gmail.com for general project questions.

Contact luke.champine@gmail.com for questions related to delbrot.

Contact trimble.forest@gmail.com for questions related to the GUI.

*an RCOS Project*
