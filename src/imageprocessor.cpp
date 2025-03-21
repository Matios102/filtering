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

QImage ImageProcessor::applyMedianFilter(const QImage &image, int kernelSize)
{
    const int width = image.width();
    const int height = image.height();
    const int halfKernelSize = kernelSize / 2;
    QImage result = image;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QVector<int> redValues;
            QVector<int> greenValues;
            QVector<int> blueValues;
            for (int ky = 0; ky < kernelSize; ++ky)
            {
                for (int kx = 0; kx < kernelSize; ++kx)
                {
                    int pixelX = qBound(0, x + kx - halfKernelSize, width - 1);
                    int pixelY = qBound(0, y + ky - halfKernelSize, height - 1);
                    QColor pixelColor = image.pixelColor(pixelX, pixelY);
                    redValues.append(pixelColor.red());
                    greenValues.append(pixelColor.green());
                    blueValues.append(pixelColor.blue());
                }
            }
            std::sort(redValues.begin(), redValues.end());
            std::sort(greenValues.begin(), greenValues.end());
            std::sort(blueValues.begin(), blueValues.end());
            int medianIndex = redValues.size() / 2;
            result.setPixelColor(x, y, QColor(redValues[medianIndex], greenValues[medianIndex], blueValues[medianIndex]));
        }
    }
    return result;
}

QImage ImageProcessor::applyOrderedDithering(const QImage &image, int thresholdMapSize)
{
    const int k = DITHERING_QUANTIZATION_LEVEL;
    QVector<QVector<int>> thresholdMap = getOrderedDitheringKernel(thresholdMapSize);
    int thresholdDivisor = thresholdMapSize * thresholdMapSize;

    QImage result(image.size(), QImage::Format_RGB888);

    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            QColor color = image.pixelColor(x, y);

            int tx = x % thresholdMapSize;
            int ty = y % thresholdMapSize;
            float thresholdNorm = thresholdMap[ty][tx] / float(thresholdDivisor);

            auto ditherChannel = [&](int value) -> int
            {
                float normalized = value / 255.0f;
                float scaled = normalized * k;
                int base = int(scaled);
                float frac = scaled - base;

                int quantized = base;
                if (frac > thresholdNorm)
                    quantized += 1;

                quantized = std::clamp(quantized, 0, k - 1);
                return (quantized * 255) / (k - 1);
            };

            int r = ditherChannel(color.red());
            int g = ditherChannel(color.green());
            int b = ditherChannel(color.blue());

            result.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    return result;
}

QImage ImageProcessor::applyUniformQuantization(const QImage &image, int rLevels, int gLevels, int bLevels)
{
    QImage result = image.convertToFormat(QImage::Format_RGB888);

    auto quantize = [](int value, int levels) -> int {
        if (levels <= 1) return 0;
        int quant = (value * (levels - 1)) / 255;
        return (quant * 255) / (levels - 1);
    };

    for (int y = 0; y < result.height(); ++y)
    {
        for (int x = 0; x < result.width(); ++x)
        {
            QColor color = result.pixelColor(x, y);

            int r = quantize(color.red(), rLevels);
            int g = quantize(color.green(), gLevels);
            int b = quantize(color.blue(), bLevels);

            result.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    return result;
}
