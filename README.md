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
delbrot is the working name of the Mkvsynth scripting language. AviSynth is controlled solely through scripts, and we hope to provide users with the power and portability of script-based video editing in MkvSynth. However, we will also be providing a GUI to allow for visual editing and frame comparisons. At present, it is unknown how delbrot will interface with the GUI; one approach is to directly translate UI actions into delbrot commands. This would facilitate easy sharing of scripts without requiring an understanding of delbrot itself.

Roles
-----
- David: project lead, x264 integration
- Forest: GUI wizard, delbrot design/implementation
- Luke: delbrot lead, unit testing
- Sean: FFmpeg filter implementation, general programming
- Wyler: GUI design/implementation, general programming

Questions?
----------
Contact david.vorick@gmail.com for general project questions.

Contact luke.champine@gmail.com for questions related to the Mkvsynth scripting language.

Contact trimble.forest@gmail.com for questions related to the GUI.

*an RCOS Project*
