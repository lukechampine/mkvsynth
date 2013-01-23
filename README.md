Mkvsynth
also documented as 'Linux Video Processing' and 'Linux Video Processing Program'

Linux Video Processing Program

Goal: We aim to develop a frame-accurate linear video editing program with minimum functionality, including crop and spline resize, using as much code from existing projects as is efficient (that will mean importing lots of code)

Immediate Goal: GUI that accepts video input, a frame number, and produces a frame accurate screenshot

Long Term Goal: A Linux compatible program that is better than avisynth

Similar Projects: Avxsynth, ffmpeg, avidemux, openshot:

Avxsynth is not unix friendly, it's just an attempt to reproduce avisynth on linux, a somewhat silly idea. Nonetheless, they have lots of good filters written in easily stolen/ported C code and we will be piggybacking off of parts of this project.

Ffmpeg is a command line tool that does lots of what we need, but the screenshots are not frame-accurate, and the video editing capabilities are limited.

Avidemux is a lot like Windows Movie Maker, it's confusing and overall not streamlined in the same efficient way that avisynth is.

We don't know much about OpenShot, we will look into it

Week 1: explore avidemux, and figure out how they get frame-accurate screenshots
Week 2: port that functionality to our program
Week 3: write GUI
Week 4: add crop function and resize function
Week 5: start pulling functions over from avxsynth
Week 6-9: debugging
Week 10-12: adding program to linux repositories (ubuntu, fedora, arch, gentoo, debian)
Week 13-15: Adapting to Problems

Group member roles:
David: leader/visionary, gui writer, programmer
Sean: research/search engine expert/graphic designer
Luke: writes unit tests/general programming
Forest: Bug finder, cross-distro compatibility expert, ports code from other projects


contact david.vorick@gmail.com with questions
or trimble.forest@gmail.com   
or luke.champine@gmail.com

an RCOS Project
