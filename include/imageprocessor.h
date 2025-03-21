#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QVector>
#include "kernel.h"

class ImageProcessor {
public:
    static QImage invertColors(const QImage &image);
    static QImage adjustBrightness(const QImage &image);
    static QImage adjustContrast(const QImage &image);
    static QImage gammaCorrection(const QImage &image);

    static QImage applyConvolution(const QImage &image, const Kernel &kernel);
    static QImage applyMedianFilter(const QImage &image, int kernelSize);

    static QImage applyOrderedDithering(const QImage &image, int thresholdMapSize);
};

#endif // IMAGEPROCESSOR_H
