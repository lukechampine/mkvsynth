# MkvSynth #

### Project Goal:
We aim to develop a frame-accurate linear video editing program with minimum functionality, including crop and spline resize, using as much code from existing projects as is efficient.

### Immediate Goal:
A GUI that accepts video input, a frame number, and produces a frame accurate screenshot

### Long Term Goal:
A Linux compatible program that is better than AviSynth

### Similar Projects:
Avxsynth, FFmpeg, Avidemux, OpenShot:

delbrot
-------
delbrot is the interpreter for the MkvSynth scripting language. AviSynth is controlled solely through scripts, and we hope to provide users with the power and portability of script-based video editing in MkvSynth. However, we will also be providing a GUI to allow for visual editing and frame comparisons. At present, it is unknown how MkvSynth's scripting language will interface with the GUI; one approach is to directly translate UI actions into delbrot commands. This would facilitate easy sharing of scripts without requiring the user to manually write any code.

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
