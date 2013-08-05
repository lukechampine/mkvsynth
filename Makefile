del: delbrot/lex.yy.c delbrot/y.tab.c delbrot/delbrot.c delbrot/plugins.c
	@cc -lm -lpthread -lavformat -lswscale \
	jarvis/bufferAllocation.c jarvis/frameControl.c jarvis/spawn.c \
	delbrot/lex.yy.c delbrot/y.tab.c delbrot/delbrot.c delbrot/plugins.c \
	filters/ffmpegDecode.c filters/gradientVideoGenerate.c filters/rgb24Crop.c filters/writeRawFile.c filters/x264Encode.c \
	-o del

delbrot/lex.yy.c: delbrot/delbrot.l
	@cd delbrot && flex delbrot.l && cd ..

delbrot/y.tab.c delbrot/delbrot.c: delbrot/delbrot.y
	@cd delbrot && yacc -d delbrot.y && cd ..
