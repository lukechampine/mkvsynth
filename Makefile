del: delbrot/lex.yy.c delbrot/y.tab.c delbrot/delbrot.c delbrot/plugins.c
	@cc -lm -lpthread -lavformat -lswscale                                       \
	jarvis/bufferAllocation.c            jarvis/frameControl.c                   \
	jarvis/spawn.c                       jarvis/properties.c                     \
	jarvis/pixels.c                                                              \
	delbrot/lex.yy.c                     delbrot/y.tab.c                         \
	delbrot/delbrot.c                    delbrot/plugins.c                       \
	filters/coding/ffmpegDecode.c        filters/coding/x264Encode.c             \
	filters/debug/writeRawFile.c                                                 \
	filters/toys/gradientVideoGenerate.c                                         \
	filters/utils/bilinearResize.c                                               \
	filters/utils/crop.c                 filters/utils/removeRange.c             \
	-o del

delbrot/lex.yy.c: delbrot/delbrot.l
	@cd delbrot && flex delbrot.l && cd ..

delbrot/y.tab.c delbrot/delbrot.c: delbrot/delbrot.y
	@cd delbrot && yacc -d delbrot.y && cd ..
