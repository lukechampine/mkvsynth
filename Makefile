del: delbrot/lex.yy.c delbrot/y.tab.c delbrot/delbrot.c delbrot/plugins.c
	@cc -lm -lpthread -lavformat -lswscale                                       \
	jarvis/bufferAllocation.c            jarvis/frameControl.c                   \
	jarvis/spawn.c                       jarvis/properties.c                     \
	jarvis/pixels.c                      jarvis/unitTests/testPixels.c           \
	delbrot/lex.yy.c                     delbrot/y.tab.c                         \
	delbrot/delbrot.c                    delbrot/plugins.c                       \
	filters/coding/ffmpegDecode.c        filters/coding/x264Encode.c             \
	filters/debug/writeRawFile.c                                                 \
	filters/unitTesting/gradientVideoGenerate.c filters/unitTesting/testingGradient.c \
	filters/utils/bilinearResize.c                                               \
	filters/utils/crop.c                 filters/utils/removeRange.c             \
	-o del

test: unitTest.c
	@cc unitTest.c jarvis/pixels.c jarvis/properties.c jarvis/unitTests/testPixels.c -o test

delbrot/lex.yy.c: delbrot/delbrot.l
	@cd delbrot && flex delbrot.l && cd ..

delbrot/y.tab.c delbrot/delbrot.c: delbrot/delbrot.y
	@cd delbrot && yacc -d delbrot.y && cd ..
