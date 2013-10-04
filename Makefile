CC = gcc
CFLAGS = -Wall

DELBROT_OBJ = delbrot/lex.yy.o                                                 \
              delbrot/y.tab.o                                                  \
              delbrot/delbrot.o                                                \
              delbrot/plugins.o
DELBROT_DEPS = delbrot/delbrot.h
DELBROT_LIBS = -lm

FFMPEG_CFLAGS = $(shell pkg-config --cflags libavformat libavcodec libswscale)
FFMPEG_LIBS = $(shell pkg-config --libs libavformat libavcodec libswscale)
FFMPEG_OBJ = filters/coding/ffmpegDecode.o
$(FFMPEG_OBJ): EXTRA_CFLAGS := $(FFMPEG_CFLAGS)

JARVIS_OBJ = jarvis/bufferAllocation.o                                         \
             jarvis/frameControl.o                                             \
             jarvis/spawn.o
JARVIS_DEPS = jarvis/jarvis.h
JARVIS_LIBS = -lpthread

MPL_OBJ = colorspacing/pixels.o                                                \
          colorspacing/properties.o
MPL_DEPS = colorspacing/colorspacing.h

FILTERS_DEBUG_OBJ = filters/debug/gradientVideoGenerate.o                      \
                    filters/debug/testingGradient.o                            \
                    filters/debug/writeRawFile.o                               \
                    filters/debug/colorspacingTests.o

FILTERS_UTIL_OBJ =  filters/utils/bilinearResize.o                             \
                    filters/utils/crop.o                                       \
                    filters/utils/removeRange.o                                \
                    filters/utils/convertColorspace.o

X264_OBJ = filters/coding/x264Encode.o

%.o: %.c                                                                       \
     $(JARVIS_DEPS)                                                            \
     $(MPL_DEPS)                                                               \
     $(DELBROT_DEPS)
	$(CC) $(CFLAGS) $< $(EXTRA_CFLAGS) -c -o $@

mkvsynth: $(DELBROT_OBJ)                                                       \
          $(FFMPEG_OBJ)                                                        \
          $(MPL_OBJ)                                                           \
          $(JARVIS_OBJ)                                                        \
          $(FILTERS_DEBUG_OBJ)                                                 \
          $(FILTERS_UTIL_OBJ)                                                  \
          $(X264_OBJ)
	$(CC) $(CFLAGS) $^ $(FFMPEG_LIBS) $(DELBROT_LIBS) $(JARVIS_LIBS) -o $@

clean:
	find . -type f -name "*.o" -delete
	rm -rf mkvsynth test unitTests/testOut1.mkv unitTests/testOut2.mkv

FLEX_VERSION := $(shell flex --version 2> /dev/null)
YACC_VERSION := $(shell yacc --version 2> /dev/null)

delbrot/lex.yy.c: delbrot/delbrot.l
ifdef FLEX_VERSION
	flex -o delbrot/lex.yy.c delbrot/delbrot.l
else
	$(error flex not found. Either install it or revert your changes to delbrot.l)
endif

delbrot/y.tab.c: delbrot/delbrot.y
ifdef YACC_VERSION
	@touch delbrot/delbrot.c # prevent makefile from clobbering delbrot.c
	yacc -o delbrot/y.tab.c -d delbrot/delbrot.y
else
	$(error yacc/bison not found. Either install it or revert your changes to delbrot.y)
endif
