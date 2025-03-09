#include "mainwindow.h"
#include "imageprocessor.h"
#include "filterconstants.h"
#include "filtereditordialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), originalImageLabel(new QLabel(this)), filteredImageLabel(new QLabel(this))
{
    originalImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    originalImageLabel->setAlignment(Qt::AlignCenter);
    originalImageLabel->setMinimumSize(100, 100); // Set minimum size
    originalImageLabel->setMaximumSize(800, 800); // Set maximum size

    filteredImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    filteredImageLabel->setAlignment(Qt::AlignCenter);
    filteredImageLabel->setMinimumSize(100, 100); // Set minimum size
    filteredImageLabel->setMaximumSize(800, 800); // Set maximum size

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QPushButton *loadButton = new QPushButton("Load Image", this);
    QPushButton *saveButton = new QPushButton("Save Image", this);
    QPushButton *resetButton = new QPushButton("Reset Image", this);

    QPushButton *invertButton = new QPushButton("Invert Colors", this);
    QPushButton *brightnessButton = new QPushButton("Brightness " + QString::number(BRIGHTNESS_ADJUSTMENT), this);
    QPushButton *contrastButton = new QPushButton("Contrast " + QString::number(CONTRAST_ADJUSTMENT), this);
    QPushButton *gammaButton = new QPushButton("Gamma " + QString::number(GAMMA_CORRECTION), this);

    QPushButton *blurButton = new QPushButton("Blur", this);
    QPushButton *gaussianBlurButton = new QPushButton("Gaussian Blur", this);
    QPushButton *sharpenButton = new QPushButton("Sharpen", this);
    QPushButton *edgeButton = new QPushButton("Edge Detection", this);
    QPushButton *embossButton = new QPushButton("Emboss", this);

    QPushButton *openFilterEditorButton = new QPushButton("Custom filter", this);

    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addWidget(originalImageLabel);
    imageLayout->addWidget(filteredImageLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(resetButton);

    QHBoxLayout *filterLayout1 = new QHBoxLayout();
    filterLayout1->addWidget(invertButton);
    filterLayout1->addWidget(brightnessButton);
    filterLayout1->addWidget(contrastButton);
    filterLayout1->addWidget(gammaButton);

    QHBoxLayout *filterLayout2 = new QHBoxLayout();
    filterLayout2->addWidget(blurButton);
    filterLayout2->addWidget(gaussianBlurButton);
    filterLayout2->addWidget(sharpenButton);
    filterLayout2->addWidget(edgeButton);
    filterLayout2->addWidget(embossButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(imageLayout);
    mainLayout->addLayout(filterLayout1);
    mainLayout->addLayout(filterLayout2);
    mainLayout->addWidget(openFilterEditorButton);

    setCentralWidget(centralWidget);

    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetImage);

    connect(invertButton, &QPushButton::clicked, this, &MainWindow::applyInversionFilter);
    connect(brightnessButton, &QPushButton::clicked, this, &MainWindow::applyBrightnessFilter);
    connect(contrastButton, &QPushButton::clicked, this, &MainWindow::applyContrastFilter);
    connect(gammaButton, &QPushButton::clicked, this, &MainWindow::applyGammaCorrectionFilter);

    connect(blurButton, &QPushButton::clicked, this, &MainWindow::applyBlurFilter);
    connect(gaussianBlurButton, &QPushButton::clicked, this, &MainWindow::applyGaussianBlurFilter);
    connect(sharpenButton, &QPushButton::clicked, this, &MainWindow::applySharpenFilter);
    connect(edgeButton, &QPushButton::clicked, this, &MainWindow::applyEdgeDetectionFilter);
    connect(embossButton, &QPushButton::clicked, this, &MainWindow::applyEmbossFilter);

    connect(openFilterEditorButton, &QPushButton::clicked, this, &MainWindow::openFilterEditorDialog);

    adjustSize();
    setWindowTitle("Image Filter Application");
}

MainWindow::~MainWindow() {}

void MainWindow::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty())
    {
        originalImage.load(fileName);
        originalPixmap = QPixmap::fromImage(originalImage);
        filteredImage = originalImage;

        QSize imageSize = originalImage.size();
        imageSize.setWidth(qBound(100, imageSize.width(), 800));
        imageSize.setHeight(qBound(100, imageSize.height(), 800));

        originalImageLabel->setPixmap(originalPixmap.scaled(imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        filteredImageLabel->setPixmap(originalPixmap.scaled(imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        originalImageLabel->setFixedSize(imageSize);
        filteredImageLabel->setFixedSize(imageSize);

        // Ensure widgets allow shrinking
        originalImageLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        filteredImageLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    }
}


void MainWindow::saveImage()
{
    if (!filteredImage.isNull())
    {
        QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty())
        {
            filteredImage.save(fileName);
        }
    }
}

void MainWindow::resetImage()
{
    filteredImage = originalImage;
    QPixmap scaledPixmap = QPixmap::fromImage(filteredImage).scaled(filteredImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    filteredImageLabel->setPixmap(scaledPixmap);
}

void MainWindow::updateFilteredImage(const QImage &newImage)
{
    filteredImage = newImage;
    QPixmap scaledPixmap = QPixmap::fromImage(filteredImage).scaled(filteredImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    filteredImageLabel->setPixmap(scaledPixmap);
}

void MainWindow::applyInversionFilter()
{
    updateFilteredImage(ImageProcessor::invertColors(filteredImage));
}

void MainWindow::applyBrightnessFilter()
{
    updateFilteredImage(ImageProcessor::adjustBrightness(filteredImage));
}

void MainWindow::applyContrastFilter()
{
    updateFilteredImage(ImageProcessor::adjustContrast(filteredImage));
}

void MainWindow::applyGammaCorrectionFilter()
{
    updateFilteredImage(ImageProcessor::gammaCorrection(filteredImage));
}

void MainWindow::applyBlurFilter()
{
    updateFilteredImage(ImageProcessor::blur(filteredImage));
}

void MainWindow::applyGaussianBlurFilter()
{
    updateFilteredImage(ImageProcessor::gaussianBlur(filteredImage));
}

void MainWindow::applySharpenFilter()
{
    updateFilteredImage(ImageProcessor::sharpen(filteredImage));
}

void MainWindow::applyEdgeDetectionFilter() 
{
    updateFilteredImage(ImageProcessor::edgeDetection(filteredImage));
}

void MainWindow::applyEmbossFilter()
{
    updateFilteredImage(ImageProcessor::emboss(filteredImage));
}

void MainWindow::openFilterEditorDialog()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }

    FilterEditorDialog dialog(this);
    connect(&dialog, &FilterEditorDialog::filterApplied, this, [this](QVector<QVector<int>> kernel, int divisor, int offset, int anchorX, int anchorY) {
        QImage newImage = ImageProcessor::applyConvolution(originalImage, kernel, 1.0/divisor, offset, anchorX, anchorY);
        updateFilteredImage(newImage);
    });
    dialog.exec();
}
