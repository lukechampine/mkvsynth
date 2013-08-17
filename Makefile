del: delbrot/lex.yy.c delbrot/y.tab.c delbrot/delbrot.c delbrot/plugins.c
	@cc -lm -lpthread -lavformat -lswscale                                       \
	jarvis/bufferAllocation.c            jarvis/frameControl.c                   \
	jarvis/spawn.c                       jarvis/properties.c                     \
	jarvis/pixels.c                                                              \
	delbrot/lex.yy.c                     delbrot/y.tab.c                         \
	delbrot/delbrot.c                    delbrot/plugins.c                       \
	filters/coding/ffmpegDecode.c        filters/coding/x264Encode.c             \
	filters/debug/writeRawFile.c         filters/debug/gradientVideoGenerate.c   \
	filters/debug/testingGradient.c                                              \
	filters/utils/bilinearResize.c                                               \
	filters/utils/crop.c                 filters/utils/removeRange.c             \
	-o del

test: unitTests/testAll.c
	@cc unitTests/testAll.c jarvis/pixels.c jarvis/properties.c unitTests/pixelTest.c -o test

delbrot/lex.yy.c: delbrot/delbrot.l
	@cd delbrot && flex delbrot.l && cd ..

delbrot/y.tab.c delbrot/delbrot.c: delbrot/delbrot.y
	@cd delbrot && yacc -d delbrot.y && cd ..
