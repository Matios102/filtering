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
};

#endif // IMAGEPROCESSOR_H
