#include "colorspacing.h"

MkvsynthPixel getPixel                    (uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);
void putPixel                             (MkvsynthPixel *pixel, uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset);

uint16_t getRed                           (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
uint16_t getGreen                         (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
uint16_t getBlue                          (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
//increments or decrements by intensity
void adjustRed                            (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
void adjustGreen                          (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
void adjustBlue                           (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
//sets color to value
void setRed                               (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);
void setGreen                             (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);
void setBlue                              (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);

uint16_t getLuma                          (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
uint16_t getCb                            (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
uint16_t getCr                            (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
void adjustLuma                           (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
void adjustCb                             (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
void adjustCr                             (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
void setLuma                              (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);
void setCb                                (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);
void setCr                                (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);

uint16_t getHue                           (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
uint16_t getHSVSaturation                 (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
uint16_t getValue                         (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
void adjustHue                            (MkvsynthPixel *pixel, double degrees, MkvsynthMetaData *metaData);
void adjustHSVSaturation                  (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
void adjustValue                          (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
void setHue                               (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);
void setHSVSaturation                     (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);
void setValue                             (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);

//H is the same for both HSV and HSL, so there is no need to add another set of hue functions
uint16_t getLightness                     (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
uint16_t getHSLSaturation                 (MkvsynthPixel *pixel, MkvsynthMetaData *metaData);
void adjustLightness                      (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
void adjustHSLSaturation                  (MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData);
void setLightness                         (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);
void setHSLSaturation                     (MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData);

void addPixel                             (MkvsynthPixel *destination, MkvsynthPixel *source, uint16_t colorspace, double strength);
