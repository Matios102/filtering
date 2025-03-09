#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QVector>

class ImageProcessor {
public:
    static QImage invertColors(const QImage &image);
    static QImage adjustBrightness(const QImage &image);
    static QImage adjustContrast(const QImage &image);
    static QImage gammaCorrection(const QImage &image);

    static QImage applyConvolution(const QImage &image, const QVector<QVector<int>> &kernel, double factor, int bias);
    static QImage blur(const QImage &image);
    static QImage gaussianBlur(const QImage &image);
    static QImage sharpen(const QImage &image);
    static QImage edgeDetection(const QImage &image);
    static QImage emboss(const QImage &image);
};

#endif // IMAGEPROCESSOR_H
