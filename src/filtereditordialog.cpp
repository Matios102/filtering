#include "filtereditordialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>
#include <QHeaderView>

FilterEditorDialog::FilterEditorDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Custom Filter Editor");
    setMinimumSize(500, 450);

    kernelTable = new QTableWidget(3, 3, this);
    kernelTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    kernelTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    kernelTable->setStyleSheet("QTableWidget { gridline-color: gray; }");

    for (int r = 0; r < kernelTable->rowCount(); ++r)
    {
        for (int c = 0; c < kernelTable->columnCount(); ++c)
        {
            kernelTable->setItem(r, c, new QTableWidgetItem("0"));
        }
    }

    rowsSpinBox = new QSpinBox(this);
    colsSpinBox = new QSpinBox(this);
    rowsSpinBox->setRange(1, 9);
    colsSpinBox->setRange(1, 9);
    rowsSpinBox->setSingleStep(2);
    colsSpinBox->setSingleStep(2);
    rowsSpinBox->setValue(3);
    colsSpinBox->setValue(3);
    rowsSpinBox->setFixedWidth(50);
    colsSpinBox->setFixedWidth(50);

    connect(rowsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &FilterEditorDialog::updateKernelSize);
    connect(colsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &FilterEditorDialog::updateKernelSize);

    divisorEdit = new QLineEdit(this);
    divisorEdit->setFixedWidth(80);
    offsetEdit = new QLineEdit(this);
    offsetEdit->setFixedWidth(80);
    anchorRowSpinBox = new QSpinBox(this);
    anchorColSpinBox = new QSpinBox(this);
    anchorRowSpinBox->setRange(0, 8);
    anchorColSpinBox->setRange(0, 8);
    anchorRowSpinBox->setValue(1);
    anchorColSpinBox->setValue(1);
    anchorRowSpinBox->setFixedWidth(50);
    anchorColSpinBox->setFixedWidth(50);

    QPushButton *computeDivisorButton = new QPushButton("Compute", this);
    QPushButton *saveFilterButton = new QPushButton("Save", this);
    QPushButton *saveAsFiterButton = new QPushButton("Save As", this);
    QPushButton *loadFilterButton = new QPushButton("Load", this);
    QPushButton *applyFilterButton = new QPushButton("Apply", this);

    connect(computeDivisorButton, &QPushButton::clicked, this, &FilterEditorDialog::computeDivisor);
    connect(saveFilterButton, &QPushButton::clicked, this, &FilterEditorDialog::saveCustomFilter);
    connect(saveAsFiterButton, &QPushButton::clicked, this, &FilterEditorDialog::saveAsCustomFilter);
    connect(loadFilterButton, &QPushButton::clicked, this, &FilterEditorDialog::loadCustomFilter);
    connect(applyFilterButton, &QPushButton::clicked, this, &FilterEditorDialog::applyFilter);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QGridLayout *sizeLayout = new QGridLayout();
    QGridLayout *filterSettingsLayout = new QGridLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    sizeLayout->addWidget(new QLabel("Kernel Size:"), 0, 0);
    sizeLayout->addWidget(new QLabel("Rows:"), 0, 1);
    sizeLayout->addWidget(rowsSpinBox, 0, 2);
    sizeLayout->addWidget(new QLabel("Cols:"), 0, 3);
    sizeLayout->addWidget(colsSpinBox, 0, 4);

    QVBoxLayout *kernelLayout = new QVBoxLayout();
    kernelLayout->addWidget(new QLabel("Kernel Coefficients:"));
    kernelLayout->addWidget(kernelTable);

    filterSettingsLayout->addWidget(new QLabel("Divisor:"), 0, 0);
    filterSettingsLayout->addWidget(divisorEdit, 0, 1);
    filterSettingsLayout->addWidget(computeDivisorButton, 0, 2);

    filterSettingsLayout->addWidget(new QLabel("Offset:"), 1, 0);
    filterSettingsLayout->addWidget(offsetEdit, 1, 1);

    filterSettingsLayout->addWidget(new QLabel("Anchor Point:"), 2, 0);
    filterSettingsLayout->addWidget(new QLabel("Row:"), 2, 1);
    filterSettingsLayout->addWidget(anchorRowSpinBox, 2, 2);
    filterSettingsLayout->addWidget(new QLabel("Col:"), 2, 3);
    filterSettingsLayout->addWidget(anchorColSpinBox, 2, 4);

    buttonsLayout->addWidget(loadFilterButton);
    buttonsLayout->addWidget(saveFilterButton);
    buttonsLayout->addWidget(saveAsFiterButton);
    buttonsLayout->addWidget(applyFilterButton);
    buttonsLayout->setSpacing(15);

    mainLayout->addLayout(sizeLayout);
    mainLayout->addLayout(kernelLayout);
    mainLayout->addLayout(filterSettingsLayout);

    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator);

    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}

void FilterEditorDialog::updateKernelSize()
{
    int rows = rowsSpinBox->value();
    int cols = colsSpinBox->value();
    kernelTable->setRowCount(rows);
    kernelTable->setColumnCount(cols);

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            if (!kernelTable->item(r, c))
            {
                kernelTable->setItem(r, c, new QTableWidgetItem("0"));
            }
        }
    }
}

void FilterEditorDialog::computeDivisor()
{
    for (int r = 0; r < kernelTable->rowCount(); ++r)
    {
        for (int c = 0; c < kernelTable->columnCount(); ++c)
        {
            kernelTable->closePersistentEditor(kernelTable->item(r, c));
        }
    }

    int sum = 0;
    for (int r = 0; r < kernelTable->rowCount(); ++r)
    {
        for (int c = 0; c < kernelTable->columnCount(); ++c)
        {
            sum += kernelTable->item(r, c) ? kernelTable->item(r, c)->text().toInt() : 0;
        }
    }
    divisorEdit->setText(QString::number(sum == 0 ? 1 : sum));
}

void FilterEditorDialog::saveCustomFilter()
{
    if (filterPath.isEmpty())
    {
        saveAsCustomFilter();
        return;
    }

    QFile file(filterPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Could not save filter.");
        return;
    }

    QTextStream out(&file);
    out << kernelTable->rowCount() << " " << kernelTable->columnCount() << "\n";
    for (int r = 0; r < kernelTable->rowCount(); ++r)
    {
        for (int c = 0; c < kernelTable->columnCount(); ++c)
        {
            out << (kernelTable->item(r, c) ? kernelTable->item(r, c)->text() : "0") << " ";
        }
        out << "\n";
    }
    out << (divisorEdit->text().isEmpty() ? "0" : divisorEdit->text()) << "\n";
    out << (offsetEdit->text().isEmpty() ? "0" : offsetEdit->text()) << "\n";
    out << anchorRowSpinBox->value() << " " << anchorColSpinBox->value() << "\n";

    file.close();

    QMessageBox::information(this, "Success", "Filter saved.");
}

void FilterEditorDialog::saveAsCustomFilter()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Filter", "", "Filter Files (*.flt)");
    if (fileName.isEmpty())
        return;

    this->filterPath = fileName;
    saveCustomFilter();
}

void FilterEditorDialog::loadCustomFilter()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load Filter", "", "Filter Files (*.flt)");
    if (fileName.isEmpty())
        return;

    this->filterPath = fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Could not open filter file.");
        return;
    }

    QTextStream in(&file);
    int rows, cols;
    in >> rows >> cols;
    rowsSpinBox->setValue(rows);
    colsSpinBox->setValue(cols);
    kernelTable->setRowCount(rows);
    kernelTable->setColumnCount(cols);

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int value;
            in >> value;
            if (!kernelTable->item(r, c))
            {
                kernelTable->setItem(r, c, new QTableWidgetItem(QString::number(value)));
            }
            else
            {
                kernelTable->item(r, c)->setText(QString::number(value));
            }
        }
    }

    QString divisor, offset;
    in >> divisor >> offset;
    divisorEdit->setText(divisor);
    offsetEdit->setText(offset);

    int anchorX, anchorY;
    in >> anchorX >> anchorY;

    anchorRowSpinBox->setValue(anchorX);
    anchorColSpinBox->setValue(anchorY);

    file.close();
}

void FilterEditorDialog::applyFilter()
{
    QVector<QVector<int>> kernel(rowsSpinBox->value(), QVector<int>(colsSpinBox->value()));
    for (int r = 0; r < rowsSpinBox->value(); ++r)
    {
        for (int c = 0; c < colsSpinBox->value(); ++c)
        {
            kernel[r][c] = kernelTable->item(r, c) ? kernelTable->item(r, c)->text().toInt() : 0;
        }
    }
    Kernel customKernel(rowsSpinBox->value(), colsSpinBox->value(), kernel, divisorEdit->text().toInt(), offsetEdit->text().toInt(),
                        anchorRowSpinBox->value(), anchorColSpinBox->value());
    emit filterApplied(customKernel);
    accept();
}
