CC = gcc
CFLAGS = -Wall

DELBROT_OBJ = delbrot/lex.yy.o                                                 \
              delbrot/y.tab.o                                                  \
              delbrot/delbrot.o                                                \
              delbrot/plugins.o

FFMPEG_INCLUDES = $(shell pkg-config --cflags libavformat libavcodec libswscale)
FFMPEG_LIBS = $(shell pkg-config --libs libavformat libavcodec libswscale)
FFMPEG_OBJ = filters/coding/ffmpegDecode.o
$(FFMPEG_OBJ): EXTRA_INCLUDES := $(FFMPEG_INCLUDES)

GUI_LIBS = $(shell pkg-config --libs gtk+-3.0)
GUI_OBJ = gtk/imageViewer.o
$(GUI_OBJ): EXTRA_INCLUDES := $(shell pkg-config --cflags gtk+-3.0)

JARVIS_OBJ = jarvis/bufferAllocation.o                                         \
             jarvis/frameControl.o                                             \
             jarvis/spawn.o
JARVIS_DEPS = jarvis/jarvis.h                                                  \
              jarvis/datatypes.h

MPL_OBJ = mpl/pixels.o                                                         \
          mpl/properties.o
MPL_DEPS = mpl/datatypes.h

FILTERS_DEBUG_OBJ = filters/debug/gradientVideoGenerate.o                      \
                    filters/debug/testingGradient.o                            \
                    filters/debug/writeRawFile.o

FILTERS_UTIL_OBJ =  filters/utils/bilinearResize.o                             \
                    filters/utils/crop.o                                       \
                    filters/utils/removeRange.o                                \
                    filters/utils/convertColorspace.o

X264_OBJ = filters/coding/x264Encode.o

%.o: %.c
	$(CC) $(CFLAGS) $< $(EXTRA_INCLUDES) -c -o $@

mkvsynth: $(DELBROT_OBJ) $(FFMPEG_OBJ) $(GUI_OBJ) $(MPL_OBJ) $(JARVIS_OBJ) $(FILTERS_DEBUG_OBJ) $(FILTERS_UTIL_OBJ) $(X264_OBJ)
	$(CC) $(CFLAGS) $^ $(FFMPEG_LIBS) $(GUI_LIBS) -o $@

clean:
	find . -type f -name "*.o" -delete
	rm -rf mkvsynth test

delbrot/lex.yy.c: delbrot/delbrot.l
	@cd delbrot && flex delbrot.l && cd ..

delbrot/y.tab.c: delbrot/delbrot.y
	@cd delbrot && yacc -d delbrot.y && cd ..
