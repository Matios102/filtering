#ifndef FILTERCONSTANTS_H
#define FILTERCONSTANTS_H

#include <QVector>
#include <QString>

const QString PREDEFINED_FILTERS_DIR = "./assets/filters/";
const QString BLUR_FILTER = "blur.flt";
const QString GAUSSIAN_BLUR_FILTER = "gaussian_blur.flt";
const QString SHARPEN_FILTER = "sharpen.flt";
const QString EDGE_DETECTION_FILTER = "edge_detection.flt";
const QString EMBOSS_FILTER = "emboss.flt";

// Functional Filter Constants
const int BRIGHTNESS_ADJUSTMENT = 10;
const double CONTRAST_ADJUSTMENT = 20;
const double GAMMA_CORRECTION = 1.5;

// Convolution Kernels
inline QVector<QVector<int>> getBlurKernel()
{
    return {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}};
}

inline QVector<QVector<int>> getGaussianBlurKernel()
{
    return {
        {0, 1, 2, 1, 0},
        {1, 4, 8, 4, 1},
        {2, 8, 16, 8, 2},
        {1, 4, 8, 4, 1},
        {0, 1, 2, 1, 0}};
}

inline QVector<QVector<int>> getSharpenKernel()
{
    return {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}};
}

inline QVector<QVector<int>> getEdgeDetectionKernel()
{
    return {
        {-1, -1, -1},
        {-1, 8, -1},
        {-1, -1, -1}};
}

inline QVector<QVector<int>> getEmbossKernel()
{
    return {
        {-2, -1, 0},
        {-1, 1, 1},
        {0, 1, 2}};
}

inline QVector<QVector<int>> getOrderedDitheringKernel(int thresholdMapSize)
{
    if (thresholdMapSize == 2)
    {
        return {
            {  0, 170 },
            {255,  85 }
        };
    }
    else if (thresholdMapSize == 3)
    {
        return {
            {204, 238, 170},
            { 68,   0, 102},
            {187,  34, 221}
        };
    }
    else if (thresholdMapSize == 4)
    {
        return {
            {  0, 136,  34, 170},
            {204,  68, 238, 102},
            { 51, 187,  17, 153},
            {255, 119, 221,  85}
        };
    }
    else if (thresholdMapSize == 6)
    {
        return {
            {  0, 136,  34, 170,  8, 144},
            {204,  68, 238, 102, 212,  76},
            { 51, 187,  17, 153,  59, 195},
            {255, 119, 221,  85, 247, 111},
            { 12, 148,  42, 178,  20, 156},
            {216,  80, 230,  94, 224,  88}
        };
    }

    return {};
}



// Kernel Constants
const int BLUR_DIVISOR = 9;
const int BLUR_OFFSET = 0;
const int BLUR_ANCHOR_X = 1;
const int BLUR_ANCHOR_Y = 1;

const int GAUSSIAN_BLUR_DIVISOR = 64;
const int GAUSSIAN_BLUR_OFFSET = 0;
const int GAUSSIAN_BLUR_ANCHOR_X = 2;
const int GAUSSIAN_BLUR_ANCHOR_Y = 2;

const int SHARPEN_DIVISOR = 1;
const int SHARPEN_OFFSET = 0;
const int SHARPEN_ANCHOR_X = 1;
const int SHARPEN_ANCHOR_Y = 1;

const int EDGE_DETECTION_DIVISOR = 1;
const int EDGE_DETECTION_OFFSET = 0;
const int EDGE_DETECTION_ANCHOR_X = 1;
const int EDGE_DETECTION_ANCHOR_Y = 1;

const int EMBOSS_DIVISOR = 1;
const int EMBOSS_OFFSET = 128;
const int EMBOSS_ANCHOR_X = 1;
const int EMBOSS_ANCHOR_Y = 1;

const int DITHERING_QUANTIZATION_LEVEL = 4;

#endif // FILTERCONSTANTS_H
