CC = gcc
CFLAGS = -Wall
DEPS = delbrot/delbrot.h jarvis/jarvis.h jarvis/bufferAllocation.h             \
       jarvis/frameControl.h jarvis/pixels.h jarvis/properties.h               \
       jarvis/spawn.h
LIBS = -lm -lpthread -lavformat -lswscale
OBJ = jarvis/bufferAllocation.o jarvis/frameControl.o jarvis/spawn.o           \
      jarvis/properties.o jarvis/pixels.o                                      \
			delbrot/lex.yy.o delbrot/y.tab.o delbrot/delbrot.o delbrot/plugins.o     \
			filters/coding/ffmpegDecode.o filters/coding/x264Encode.o                \
			filters/debug/writeRawFile.o filters/debug/gradientVideoGenerate.o       \
			filters/debug/testingGradient.o filters/utils/bilinearResize.o           \
			filters/utils/crop.o filters/utils/removeRange.o                         \
			filters/utils/convertColorspace.o                                        \
			gtk/imageViewer.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mkvsynth: $(OBJ)
	$(CC) `pkg-config --cflags gtk+-3.0` -o $@ $^ $(CFLAGS) $(LIBS) `pkg-config --libs gtk+-3.0`

clean:
	find . -type f -name "*.o" -delete
	rm -rf mkvsynth test

test: unitTests/testAll.c
	@cc unitTests/testAll.c jarvis/pixels.c jarvis/properties.c unitTests/pixelTest.c -o test

delbrot/lex.yy.c: delbrot/delbrot.l
	@cd delbrot && flex delbrot.l && cd ..

delbrot/y.tab.c delbrot/delbrot.c: delbrot/delbrot.y
	@cd delbrot && yacc -d delbrot.y && cd ..
