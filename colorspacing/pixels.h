#include "colorspacing.h"
#include <stdio.h>

MkvsynthPixel getPixel               (uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);
void          putPixel               (MkvsynthPixel *pixel, uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);

uint16_t      getRed                 (MkvsynthPixel *pixel);
uint16_t      getGreen               (MkvsynthPixel *pixel);
uint16_t      getBlue                (MkvsynthPixel *pixel);
MkvsynthPixel adjustRed              (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustGreen            (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustBlue             (MkvsynthPixel *pixel, double intensity);

uint16_t      getLuma                (MkvsynthPixel *pixel);
uint16_t      getCb                  (MkvsynthPixel *pixel);
uint16_t      getCr                  (MkvsynthPixel *pixel);
MkvsynthPixel adjustLuma             (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustCb               (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustCr               (MkvsynthPixel *pixel, double intensity);

uint16_t      getHue                 (MkvsynthPixel *pixel);
uint16_t      getLightness           (MkvsynthPixel *pixel);
uint16_t      getSaturation          (MkvsynthPixel *pixel);
uint16_t      getValue               (MkvsynthPixel *pixel);
MkvsynthPixel adjustHue              (MkvsynthPixel *pixel, double degrees);
MkvsynthPixel adjustLightness        (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustSaturation       (MkvsynthPixel *pixel, double intensity);
MkvsynthPixel adjustValue            (MkvsynthPixel *pixel, double intensity);

void          addPixel               (MkvsynthPixel *destination, MkvsynthPixel *source, uint16_t colorspace, double strength);
