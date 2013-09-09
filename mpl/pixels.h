#ifndef pixels_h_
#define pixels_h_

#include "datatypes.h"

MkvsynthPixel getPixel               (uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);
void          putPixel               (MkvsynthPixel *pixel, uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);

short getRed                         (MkvsynthPixel *pixel);
short getGreen                       (MkvsynthPixel *pixel);
short getBlue                        (MkvsynthPixel *pixel);
MkvsynthPixel adjustRed              (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustGreen            (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustBlue             (MkvsynthPixel *pixel, double intensity);

short getLuma                        (MkvsynthPixel *pixel);
short getCb                          (MkvsynthPixel *pixel);
short getCr                          (MkvsynthPixel *pixel);
MkvsynthPixel adjustLuma             (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustCb               (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustCr               (MkvsynthPixel *pixel, double intensity);

short getHue                         (MkvsynthPixel *pixel);
short getLightness                   (MkvsynthPixel *pixel);
short getSaturation                  (MkvsynthPixel *pixel);
short getValue                       (MkvsynthPixel *pixel);
MkvsynthPixel adjustHue              (MkvsynthPixel *pixel, double degrees);
MkvsynthPixel adjustLightness        (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustSaturation       (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustValue            (MkvsynthPixel *pixel, double intensity);

void addPixel                        (MkvsynthPixel *destination, MkvsynthPixel *source, short colorspace, double strength);

#endif
