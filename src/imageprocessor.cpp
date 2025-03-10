#include "imageprocessor.h"
#include "filterconstants.h"
#include <QtMath>

QImage ImageProcessor::invertColors(const QImage &image)
{
    QImage result = image;
    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            QColor color = image.pixelColor(x, y);
            color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
            result.setPixelColor(x, y, color);
        }
    }
    return result;
}

QImage ImageProcessor::adjustBrightness(const QImage &image)
{
    int brightness = BRIGHTNESS_ADJUSTMENT;
    QImage result = image;
    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            QColor color = image.pixelColor(x, y);
            color.setRgb(qBound(0, color.red() + brightness, 255),
                         qBound(0, color.green() + brightness, 255),
                         qBound(0, color.blue() + brightness, 255));
            result.setPixelColor(x, y, color);
        }
    }
    return result;
}

QImage ImageProcessor::adjustContrast(const QImage &image)
{
    double contrast = CONTRAST_ADJUSTMENT;
    QImage result = image;
    double factor = (259 * (contrast + 255)) / (255 * (259 - contrast));

    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            QColor color = image.pixelColor(x, y);
            color.setRgb(qBound(0, static_cast<int>(factor * (color.red() - 128) + 128), 255),
                         qBound(0, static_cast<int>(factor * (color.green() - 128) + 128), 255),
                         qBound(0, static_cast<int>(factor * (color.blue() - 128) + 128), 255));
            result.setPixelColor(x, y, color);
        }
    }
    return result;
}

QImage ImageProcessor::gammaCorrection(const QImage &image)
{
    double gamma = GAMMA_CORRECTION;
    QImage result = image;
    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            QColor color = image.pixelColor(x, y);
            color.setRgb(qBound(0, static_cast<int>(255 * pow(color.red() / 255.0, gamma)), 255),
                         qBound(0, static_cast<int>(255 * pow(color.green() / 255.0, gamma)), 255),
                         qBound(0, static_cast<int>(255 * pow(color.blue() / 255.0, gamma)), 255));
            result.setPixelColor(x, y, color);
        }
    }
    return result;
}

QImage ImageProcessor::applyConvolution(const QImage &image, const Kernel &kernel)
{
    const int divisor = kernel.getDivisor();
    const double factor = 1.0 / divisor;
    const int bias = kernel.getOffset();
    const int offsetRow = kernel.getAnchorX();
    const int offsetCol = kernel.getAnchorY();
    const int width = image.width();
    const int height = image.height();
    const QVector<QVector<int>> kernelTable = kernel.getKernel();
    const int kernelSize = kernelTable.size();
    QImage result = image;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int r = 0, g = 0, b = 0;
            for (int ky = 0; ky < kernelSize; ++ky)
            {
                for (int kx = 0; kx < kernelSize; ++kx)
                {
                    int pixelX = qBound(0, x + kx - offsetCol, width - 1);
                    int pixelY = qBound(0, y + ky - offsetRow, height - 1);
                    QColor pixelColor = image.pixelColor(pixelX, pixelY);
                    r += pixelColor.red() * kernelTable[ky][kx];
                    g += pixelColor.green() * kernelTable[ky][kx];
                    b += pixelColor.blue() * kernelTable[ky][kx];
                }
            }
            r = qBound(0, static_cast<int>(factor * r + bias), 255);
            g = qBound(0, static_cast<int>(factor * g + bias), 255);
            b = qBound(0, static_cast<int>(factor * b + bias), 255);
            result.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return result;
}
