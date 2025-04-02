#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QVector>
#include "kernel.h"

class ImageProcessor
{
public:
    enum class Channel
    {
        H,
        S,
        V
    };

    static QImage invertColors(const QImage &image);
    static QImage adjustBrightness(const QImage &image);
    static QImage adjustContrast(const QImage &image);
    static QImage gammaCorrection(const QImage &image);

    static QImage applyConvolution(const QImage &image, const Kernel &kernel);
    static QImage applyMedianFilter(const QImage &image, int kernelSize);

    static QImage applyOrderedDithering(const QImage &image, int thresholdMapSize, int k);
    static QImage applyUniformQuantization(const QImage &image, int rLevels, int gLevels, int bLevels);

    static QImage applyGreyscaleFilter(const QImage &image);

    static QImage convertToHSV(const QImage &image);
    static QImage extractChannel(const QImage &hsvImage, Channel channel);
    static QImage convertHSVToRGB(const QImage &hChannel, const QImage &sChannel, const QImage &vChannel);
};

#endif // IMAGEPROCESSOR_H
