#include "mainwindow.h"
#include "imageprocessor.h"
#include "filterconstants.h"
#include "filtereditordialog.h"
#include <QMessageBox>
#include <iostream>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), originalImageLabel(new QLabel(this)), filteredImageLabel(new QLabel(this))
{
    try
    {
        blurKernel = Kernel(PREDEFINED_FILTERS_DIR + BLUR_FILTER);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading blur filter: " << e.what() << std::endl;
        QVector<QVector<int>> kernel = getBlurKernel();
        int rows = kernel.size();
        int cols = kernel[0].size();

        blurKernel = Kernel(rows, cols, kernel, BLUR_DIVISOR, BLUR_OFFSET, BLUR_ANCHOR_X, BLUR_ANCHOR_Y);
    }

    try
    {
        gaussianBlurKernel = Kernel(PREDEFINED_FILTERS_DIR + GAUSSIAN_BLUR_FILTER);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading gaussian blur filter: " << e.what() << std::endl;
        QVector<QVector<int>> kernel = getGaussianBlurKernel();
        int rows = kernel.size();
        int cols = kernel[0].size();

        gaussianBlurKernel = Kernel(rows, cols, kernel, BLUR_DIVISOR, BLUR_OFFSET, BLUR_ANCHOR_X, BLUR_ANCHOR_Y);
    }

    try
    {
        sharpenKernel = Kernel(PREDEFINED_FILTERS_DIR + SHARPEN_FILTER);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading sharpen filter: " << e.what() << std::endl;
        QVector<QVector<int>> kernel = getSharpenKernel();
        int rows = kernel.size();
        int cols = kernel[0].size();

        sharpenKernel = Kernel(rows, cols, kernel, BLUR_DIVISOR, BLUR_OFFSET, BLUR_ANCHOR_X, BLUR_ANCHOR_Y);
    }

    try
    {
        edgeDetectionKernel = Kernel(PREDEFINED_FILTERS_DIR + EDGE_DETECTION_FILTER);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading edge detection filter: " << e.what() << std::endl;
        QVector<QVector<int>> kernel = getEdgeDetectionKernel();
        int rows = kernel.size();
        int cols = kernel[0].size();

        edgeDetectionKernel = Kernel(rows, cols, kernel, BLUR_DIVISOR, BLUR_OFFSET, BLUR_ANCHOR_X, BLUR_ANCHOR_Y);
    }

    try
    {
        embossKernel = Kernel(PREDEFINED_FILTERS_DIR + EMBOSS_FILTER);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading emboss filter: " << e.what() << std::endl;
        QVector<QVector<int>> kernel = getEmbossKernel();
        int rows = kernel.size();
        int cols = kernel[0].size();

        embossKernel = Kernel(rows, cols, kernel, BLUR_DIVISOR, BLUR_OFFSET, BLUR_ANCHOR_X, BLUR_ANCHOR_Y);
    }


    originalImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    originalImageLabel->setAlignment(Qt::AlignCenter);
    originalImageLabel->setMinimumSize(100, 100);
    originalImageLabel->setMaximumSize(500, 500);

    filteredImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    filteredImageLabel->setAlignment(Qt::AlignCenter);
    filteredImageLabel->setMinimumSize(100, 100);
    filteredImageLabel->setMaximumSize(500, 500);

    QScrollArea *originalImageScrollArea = new QScrollArea(this);
    originalImageScrollArea->setWidget(originalImageLabel);
    originalImageScrollArea->setWidgetResizable(true);
    originalImageScrollArea->setFixedSize(500, 500);
    
    
    QScrollArea *filteredImageScrollArea = new QScrollArea(this);
    filteredImageScrollArea->setWidget(filteredImageLabel);
    filteredImageScrollArea->setWidgetResizable(true);
    filteredImageScrollArea->setFixedSize(500, 500);



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
    QPushButton *medianButton = new QPushButton("Median Filter", this);
    QPushButton *orderedDitheringButton = new QPushButton("Ordered Dithering", this);
    this->medianSpinBox = new QSpinBox(this);
    this->medianSpinBox->setRange(1, 15);
    this->medianSpinBox->setSingleStep(2);
    this->orderedDitheringComboBox = new QComboBox(this);
    QVector<int> orderedDitheringValues = {2, 3, 4, 6};
    for (int value : orderedDitheringValues)
    {
        this->orderedDitheringComboBox->addItem(QString::number(value), value);
    }


    QPushButton *openFilterEditorButton = new QPushButton("Custom filter", this);

    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addWidget(originalImageScrollArea);
    imageLayout->addWidget(filteredImageScrollArea);

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
    filterLayout2->addWidget(medianButton);
    filterLayout2->addWidget(medianSpinBox);
    filterLayout2->addWidget(orderedDitheringButton);
    filterLayout2->addWidget(orderedDitheringComboBox);

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

    connect(medianButton, &QPushButton::clicked, this, &MainWindow::applyMedianFilter);

    connect(orderedDitheringButton, &QPushButton::clicked, this, &MainWindow::applyOrderedDithering);

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

        originalImageLabel->setPixmap(originalPixmap);
        filteredImageLabel->setPixmap(originalPixmap);

        originalImageLabel->setFixedSize(originalPixmap.size());
        filteredImageLabel->setFixedSize(originalPixmap.size());
    }
}

void MainWindow::saveImage()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty())
    {
        filteredImage.save(fileName);
    }
}

void MainWindow::resetImage()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
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
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::invertColors(filteredImage));
}

void MainWindow::applyBrightnessFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::adjustBrightness(filteredImage));
}

void MainWindow::applyContrastFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::adjustContrast(filteredImage));
}

void MainWindow::applyGammaCorrectionFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::gammaCorrection(filteredImage));
}

void MainWindow::applyBlurFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::applyConvolution(filteredImage, blurKernel));
}

void MainWindow::applyGaussianBlurFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::applyConvolution(filteredImage, gaussianBlurKernel));
}

void MainWindow::applySharpenFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::applyConvolution(filteredImage, sharpenKernel));
}

void MainWindow::applyEdgeDetectionFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::applyConvolution(filteredImage, edgeDetectionKernel));
}

void MainWindow::applyEmbossFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::applyConvolution(filteredImage, embossKernel));
}

void MainWindow::openFilterEditorDialog()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }

    FilterEditorDialog dialog(this);
    connect(&dialog, &FilterEditorDialog::filterApplied, this, [this](Kernel kernel)
            {
        QImage newImage = ImageProcessor::applyConvolution(originalImage, kernel);
        updateFilteredImage(newImage); });
    dialog.exec();
}

void MainWindow::applyMedianFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::applyMedianFilter(filteredImage, medianSpinBox->value()));
}

void MainWindow::applyOrderedDithering()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::applyOrderedDithering(filteredImage, orderedDitheringComboBox->currentData().toInt()));
}