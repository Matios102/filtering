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

private:
    QLabel *originalImageLabel;
    QLabel *filteredImageLabel;
    QPixmap originalPixmap;
    QImage originalImage;
    QImage filteredImage;

    void updateFilteredImage(const QImage &newImage);
    void openFilterEditorDialog();
};

#endif // MAINWINDOW_H
