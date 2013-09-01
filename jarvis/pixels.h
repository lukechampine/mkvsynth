#ifndef pixels_h_
#define pixels_h_

#include "datatypes.h"

MkvsynthPixel getPixel               (uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);
void          putPixel               (MkvsynthPixel *pixel, uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);

int getRed                           (MkvsynthPixel *pixel);
int getGreen                         (MkvsynthPixel *pixel);
int getBlue                          (MkvsynthPixel *pixel);
MkvsynthPixel adjustRed              (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustGreen            (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustBlue             (MkvsynthPixel *pixel, double intensity);

int getLuma                          (MkvsynthPixel *pixel);
int getCb                            (MkvsynthPixel *pixel);
int getCr                            (MkvsynthPixel *pixel);
MkvsynthPixel adjustLuma             (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustCb               (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustCr               (MkvsynthPixel *pixel, double intensity);

int getHue                           (MkvsynthPixel *pixel);
int getLightness									   (MkvsynthPixel *pixel);
int getSaturation                    (MkvsynthPixel *pixel);
int getValue                         (MkvsynthPixel *pixel);
MkvsynthPixel adjustHue              (MkvsynthPixel *pixel, double degrees);
MkvsynthPixel adjustLightness        (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustSaturation       (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustValue            (MkvsynthPixel *pixel, double intensity);

void addPixel                        (MkvsynthPixel *destination, MkvsynthPixel *source, short colorspace, double strength);

#endif
