#ifndef FILTERCONSTANTS_H
#define FILTERCONSTANTS_H

#include <QVector>

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

// Scaling Factors and Biases
const double BLUR_FACTOR = 1.0 / 9;
const double GAUSSIAN_BLUR_FACTOR = 1.0 / 64;
const double SHARPEN_FACTOR = 1.0;
const double EDGE_DETECTION_FACTOR = 1.0;
const double EMBOSS_FACTOR = 1.0;
const int EMBOSS_BIAS = 128;

#endif // FILTERCONSTANTS_H
