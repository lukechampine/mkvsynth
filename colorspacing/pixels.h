#include "colorspacing.h"

MkvsynthPixel getPixel               (uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);
void          putPixel               (MkvsynthPixel *pixel, uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);

uint16_t getRed                      (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
uint16_t getGreen                    (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
uint16_t getBlue                     (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
//increments or decrements by intensity
MkvsynthPixel adjustRed              (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
MkvsynthPixel adjustGreen            (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
MkvsynthPixel adjustBlue             (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
//sets color to value
MkvsynthPixel setRed                 (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);
MkvsynthPixel setGreen               (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);
MkvsynthPixel setBlue                (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);

int getLuma                          (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
int getCb                            (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
int getCr                            (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
MkvsynthPixel adjustLuma             (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
MkvsynthPixel adjustCb               (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
MkvsynthPixel adjustCr               (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
MkvsynthPixel setLuma                (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
MkvsynthPixel setCb                  (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
MkvsynthPixel setCr                  (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);

int getHue                           (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
int getLightness                     (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
int getSaturation                    (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
int getValue                         (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
MkvsynthPixel adjustHue              (MkvsynthPixel *pixel, double degrees, MkvsynthMetaData *metaData);
MkvsynthPixel adjustLightness        (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
MkvsynthPixel adjustSaturation       (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
MkvsynthPixel adjustValue            (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);



void          addPixel               (MkvsynthPixel *destination, MkvsynthPixel *source, uint16_t colorspace, double strength);
