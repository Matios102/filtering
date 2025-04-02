#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QSpinBox>
#include <QComboBox>
#include "kernel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage();
    void saveImage();
    void resetImage();

    void applyInversionFilter();
    void applyBrightnessFilter();
    void applyContrastFilter();
    void applyGammaCorrectionFilter();

    void applyBlurFilter();
    void applyGaussianBlurFilter();
    void applySharpenFilter();
    void applyEdgeDetectionFilter();
    void applyEmbossFilter();
    void applyMedianFilter();

    void applyOrderedDithering();
    void applyUniformQuantization();

    void applySelectedConvolutionFilter();

    void applyGreyscaleFilter();

    void applyHSVFilter();

private:
    QLabel *originalImageLabel;
    QLabel *filteredImageLabel;
    QLabel *HfilteredImageLabel;
    QLabel *VfilteredImageLabel;
    QLabel *SfilteredImageLabel;
    QLabel *RGBfilteredImageLabel;
    QPixmap originalPixmap;
    QImage originalImage;
    QImage filteredImage;
    QImage HfilteredImage;
    QImage VfilteredImage;
    QImage SfilteredImage;
    QImage RGBfilteredImage;

    Kernel blurKernel;
    Kernel gaussianBlurKernel;
    Kernel sharpenKernel;
    Kernel edgeDetectionKernel;
    Kernel embossKernel;

    QSpinBox *medianSpinBox;
    QComboBox *orderedDitheringComboBox;

    QComboBox *FilterSelector;

    QSpinBox *rQuantSpinBox;
    QSpinBox *gQuantSpinBox;
    QSpinBox *bQuantSpinBox;

    QSpinBox *ditheringQuantizationSpinBox;

    void updateFilteredImage(const QImage &newImage);
    void openFilterEditorDialog();
};

#endif // MAINWINDOW_H
