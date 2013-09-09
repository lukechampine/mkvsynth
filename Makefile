CC = gcc
CFLAGS = -Wall

DELBROT_OBJ = delbrot/lex.yy.o                                                 \
              delbrot/y.tab.o                                                  \
              delbrot/delbrot.o                                                \
              delbrot/plugins.o

FFMPEG_INCLUDES = $(shell pkg-config --cflags libavformat libavcodec libswscale libavutil)
FFMPEG_LIBS = $(shell pkg-config --libs libavformat libavcodec libswscale libavutil)
FFMPEG_OBJ = filters/coding/ffmpegDecode.o

GUI_INCLUDES = $(shell pkg-config --cflags gtk+-3.0)
GUI_LIBS = $(shell pkg-config --libs gtk+-3.0)
GUI_OBJ = gtk/imageViewer.o

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

FILTERS_UTIL_OBJ = filters/utils/bilinearResize.o                             \
                    filters/utils/crop.o                                       \
                    filters/utils/removeRange.o                                \
                    filters/utils/convertColorspace.o

X264_OBJ = filters/coding/x264Encode.o

#all: $(DELBROT_OBJ) $(FFMPEG_OBJ) $(GUI_OBJ) $(MPL_OBJ) $(JARVIS_OBJ) $(FILTERS_DEBUG_OBJ) $(FILTERS_UTIL_OBJ) $(X264_OBJ)

$(FFMPEG_OBJ): EXTRA_INCLUDES := $(FFMPEG_INCLUDES)

$(GUI_OBJ): EXTRA_INCLUDES := $(GUI_INCLUDES)

%.o: %.c
	$(CC) $(CFLAGS) $< $(EXTRA_INCLUDES) -c -o $@

mkvsynth: $(DELBROT_OBJ) $(FFMPEG_OBJ) $(GUI_OBJ) $(MPL_OBJ) $(JARVIS_OBJ) $(FILTERS_DEBUG_OBJ) $(FILTERS_UTIL_OBJ) $(X264_OBJ)
	$(CC) $(CFLAGS) $^ $(FFMPEG_LIBS) $(GUI_LIBS) -o $@

#$(DELBROT_OBJ):
#	$(CC) $(CFLAGS) $< -c -o $@

#$(FFMPEG_OBJ):
	#$(CC) $(CFLAGS) $< $(FFMPEG_INCLUDES) -c -o $@

#$(GUI_OBJ):
	#$(CC) $(CFLAGS) $< $(GUI_INCLUDES) -c -o $@
#
#$(JARVIS_OBJ): $(JARVIS_DEPS)
	#$(CC) $(CFLAGS) $< -c -o $@
#
#$(MPL_OBJ): $(MPL_DEPS)
	#$(CC) $(CFLAGS) $< -c -o $@
#
#$(FILTERS_DEBUG_OBJ):
	#$(CC) $(CFLAGS) $< -c -o $@
#
#$(FILTERS_UTIL_OBJ):
	#$(CC) $(CFLAGS) $< -c -o $@
#
#$(X264_OBJ):
	#$(CC) $(CFLAGS) $< -c -o $@

#mkvsynth:
#	$(CC) -o $@ $^ $(CFLAGS) $(FFMPEG_LIBS) $(GUI_LIBS)

clean:
	find . -type f -name "*.o" -delete
	rm -rf mkvsynth test

delbrot/lex.yy.c: delbrot/delbrot.l
	@cd delbrot && flex delbrot.l && cd ..

delbrot/y.tab.c: delbrot/delbrot.y
	@cd delbrot && yacc -d delbrot.y && cd ..
