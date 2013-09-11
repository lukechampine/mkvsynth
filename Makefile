CC = gcc
CFLAGS = -Wall

DELBROT_OBJ = delbrot/lex.yy.o                                                 \
              delbrot/y.tab.o                                                  \
              delbrot/delbrot.o                                                \
              delbrot/plugins.o
DELBROT_LIBS = -lm

FFMPEG_CFLAGS = $(shell pkg-config --cflags libavformat libavcodec libswscale libavutil)
FFMPEG_LIBS = $(shell pkg-config --libs libavformat libavcodec libswscale libavutil)
FFMPEG_OBJ = filters/coding/ffmpegDecode.o
$(FFMPEG_OBJ): EXTRA_CFLAGS := $(FFMPEG_CFLAGS)

GUI_LIBS = $(shell pkg-config --libs gtk+-3.0)
GUI_OBJ = gtk/imageViewer.o
$(GUI_OBJ): EXTRA_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)

JARVIS_OBJ = jarvis/bufferAllocation.o                                         \
             jarvis/frameControl.o                                             \
             jarvis/spawn.o
JARVIS_DEPS = jarvis/jarvis.h                                                  \
              jarvis/datatypes.h
JARVIS_LIBS = -lpthread

MPL_OBJ = colorspacing/pixels.o                                                \
          colorspacing/properties.o
MPL_DEPS = colorspacing/colorspacing.h

FILTERS_DEBUG_OBJ = filters/debug/gradientVideoGenerate.o                      \
                    filters/debug/testingGradient.o                            \
                    filters/debug/writeRawFile.o

FILTERS_UTIL_OBJ =  filters/utils/bilinearResize.o                             \
                    filters/utils/crop.o                                       \
                    filters/utils/removeRange.o                                \
                    filters/utils/convertColorspace.o

X264_OBJ = filters/coding/x264Encode.o

%.o: %.c                                                                       \
     $(JARVIS_DEPS)                                                            \
     $(MPL_DEPS)
	$(CC) $(CFLAGS) $< $(EXTRA_CFLAGS) -c -o $@

mkvsynth: $(DELBROT_OBJ)                                                       \
          $(FFMPEG_OBJ)                                                        \
          $(GUI_OBJ)                                                           \
          $(MPL_OBJ)                                                           \
          $(JARVIS_OBJ)                                                        \
          $(FILTERS_DEBUG_OBJ)                                                 \
          $(FILTERS_UTIL_OBJ)                                                  \
          $(X264_OBJ)
	$(CC) $(CFLAGS) $^ $(FFMPEG_LIBS) $(GUI_LIBS) $(DELBROT_LIBS) $(JARVIS_LIBS) -o $@

clean:
	find . -type f -name "*.o" -delete
	rm -rf mkvsynth test unitTests/testOut1.mkv unitTests/testOut2.mkv

delbrot/lex.yy.c: delbrot/delbrot.l
	@cd delbrot && flex delbrot.l && cd ..

delbrot/y.tab.c: delbrot/delbrot.y
	@cd delbrot && yacc -d delbrot.y && cd ..
