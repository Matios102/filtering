#include "mainwindow.h"
#include "imageprocessor.h"
#include "filterconstants.h"
#include "filtereditordialog.h"
#include <QMessageBox>
#include <QStackedWidget>
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

    // --- Image view setup ---
    originalImageLabel->setAlignment(Qt::AlignCenter);
    originalImageLabel->setMinimumSize(100, 100);
    originalImageLabel->setMaximumSize(500, 500);
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

    // --- Core layout setup ---
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Top row: Load / Save / Reset
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *loadButton = new QPushButton("Load Image", this);
    QPushButton *saveButton = new QPushButton("Save Image", this);
    QPushButton *resetButton = new QPushButton("Reset Image", this);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(resetButton);

    // --- Image display ---
    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addWidget(originalImageScrollArea);
    imageLayout->addWidget(filteredImageScrollArea);

    // --- Functional filters  ---
    QHBoxLayout *filterLayout1 = new QHBoxLayout();
    QPushButton *invertButton = new QPushButton("Invert Colors", this);
    QPushButton *brightnessButton = new QPushButton("Brightness " + QString::number(BRIGHTNESS_ADJUSTMENT), this);
    QPushButton *contrastButton = new QPushButton("Contrast " + QString::number(CONTRAST_ADJUSTMENT), this);
    QPushButton *gammaButton = new QPushButton("Gamma " + QString::number(GAMMA_CORRECTION), this);
    QPushButton *greyScaleButton = new QPushButton("Greyscale", this);
    filterLayout1->addWidget(invertButton);
    filterLayout1->addWidget(brightnessButton);
    filterLayout1->addWidget(contrastButton);
    filterLayout1->addWidget(gammaButton);
    filterLayout1->addWidget(greyScaleButton);

    // --- Convolution filter section ---
    QHBoxLayout *convolutionLayout = new QHBoxLayout();
    FilterSelector = new QComboBox(this);
    FilterSelector->addItem("None");                 // Page 0
    FilterSelector->addItem("Blur");                 // Page 1
    FilterSelector->addItem("Gaussian Blur");        // Page 2
    FilterSelector->addItem("Sharpen");              // Page 3
    FilterSelector->addItem("Edge Detection");       // Page 4
    FilterSelector->addItem("Emboss");               // Page 5
    FilterSelector->addItem("Median Filter");        // Page 6
    FilterSelector->addItem("Ordered Dithering");    // Page 7
    FilterSelector->addItem("Uniform Quantization"); // Page 8

    QStackedWidget *convolutionFilterConfigStack = new QStackedWidget(this);

    // Pages 0-5: No config needed
    for (int i = 0; i < 6; ++i)
        convolutionFilterConfigStack->addWidget(new QWidget());

    // Page 6: Median filter config
    QWidget *medianWidget = new QWidget();
    QHBoxLayout *medianLayout = new QHBoxLayout(medianWidget);
    medianSpinBox = new QSpinBox(this);
    medianSpinBox->setRange(1, 15);
    medianSpinBox->setSingleStep(2);
    medianLayout->addWidget(new QLabel("Kernel Size:"));
    medianLayout->addWidget(medianSpinBox);
    convolutionFilterConfigStack->addWidget(medianWidget);

    // Page 7: Ordered dithering config
    QWidget *ditheringWidget = new QWidget();
    QHBoxLayout *ditheringLayout = new QHBoxLayout(ditheringWidget);
    orderedDitheringComboBox = new QComboBox(this);
    for (int v : QVector<int>{2, 3, 4, 6})
        orderedDitheringComboBox->addItem(QString::number(v), v);
    ditheringLayout->addWidget(new QLabel("Threshold Map Size:"));
    ditheringLayout->addWidget(orderedDitheringComboBox);
    convolutionFilterConfigStack->addWidget(ditheringWidget);

    // Page 8: Uniform quantization config
    QWidget *quantizationWidget = new QWidget();
    QHBoxLayout *quantizationLayout = new QHBoxLayout(quantizationWidget);
    rQuantSpinBox = new QSpinBox(this);
    gQuantSpinBox = new QSpinBox(this);
    bQuantSpinBox = new QSpinBox(this);
    for (QSpinBox *box : {rQuantSpinBox, gQuantSpinBox, bQuantSpinBox})
    {
        box->setRange(2, 32);
        box->setValue(4);
    }
    quantizationLayout->addWidget(new QLabel("R:", this));
    quantizationLayout->addWidget(rQuantSpinBox);
    quantizationLayout->addWidget(new QLabel("G:", this));
    quantizationLayout->addWidget(gQuantSpinBox);
    quantizationLayout->addWidget(new QLabel("B:", this));
    quantizationLayout->addWidget(bQuantSpinBox);
    convolutionFilterConfigStack->addWidget(quantizationWidget);

    QPushButton *applyConvolutionButton = new QPushButton("Apply Filter", this);
    connect(FilterSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            convolutionFilterConfigStack, &QStackedWidget::setCurrentIndex);
    connect(applyConvolutionButton, &QPushButton::clicked, this, &MainWindow::applySelectedConvolutionFilter);

    convolutionLayout->addWidget(FilterSelector);
    convolutionLayout->addWidget(convolutionFilterConfigStack);
    convolutionLayout->addWidget(applyConvolutionButton);

    QPushButton *openFilterEditorButton = new QPushButton("Custom Filter", this);

    // --- Assembling final layout ---
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(imageLayout);
    mainLayout->addLayout(filterLayout1);
    mainLayout->addLayout(convolutionLayout);
    mainLayout->addLayout(ditheringLayout);
    mainLayout->addWidget(openFilterEditorButton);

    setCentralWidget(centralWidget);
    setWindowTitle("Image Filter Application");

    // --- Connections ---
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetImage);
    connect(invertButton, &QPushButton::clicked, this, &MainWindow::applyInversionFilter);
    connect(brightnessButton, &QPushButton::clicked, this, &MainWindow::applyBrightnessFilter);
    connect(contrastButton, &QPushButton::clicked, this, &MainWindow::applyContrastFilter);
    connect(gammaButton, &QPushButton::clicked, this, &MainWindow::applyGammaCorrectionFilter);
    connect(openFilterEditorButton, &QPushButton::clicked, this, &MainWindow::openFilterEditorDialog);
    connect(greyScaleButton, &QPushButton::clicked, this, &MainWindow::applyGreyscaleFilter);

    adjustSize();
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

void MainWindow::applyUniformQuantization()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::applyUniformQuantization(filteredImage, rQuantSpinBox->value(), gQuantSpinBox->value(), bQuantSpinBox->value()));
}

void MainWindow::applySelectedConvolutionFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }

    int index = FilterSelector->currentIndex();
    switch (index)
    {
    case 1:
        applyBlurFilter();
        break;
    case 2:
        applyGaussianBlurFilter();
        break;
    case 3:
        applySharpenFilter();
        break;
    case 4:
        applyEdgeDetectionFilter();
        break;
    case 5:
        applyEmbossFilter();
        break;
    case 6:
        applyMedianFilter();
        break;
    case 7:
        applyOrderedDithering();
        break;
    case 8:
        applyUniformQuantization();
        break;
    default:
        break;
    }
}

void MainWindow::applyGreyscaleFilter()
{
    if (originalImage.isNull())
    {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }
    updateFilteredImage(ImageProcessor::applyGreyscaleFilter(filteredImage));
}