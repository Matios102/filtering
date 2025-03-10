#include "kernel.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMessageBox>

Kernel::Kernel(int rows, int cols, QVector<QVector<int>> kernel, int divisor, int offset, int anchorX, int anchorY)
    : rows(rows), cols(cols), kernel(kernel), divisor(divisor), offset(offset), anchorX(anchorX), anchorY(anchorY)
{
}

Kernel::Kernel()
{
}

Kernel::Kernel(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw std::runtime_error("Could not open filter file " + filePath.toStdString());
        return;
    }

    QTextStream in(&file);
    int rows, cols;
    in >> rows >> cols;
    this->rows = rows;
    this->cols = cols;

    this->kernel = QVector<QVector<int>>(rows, QVector<int>(cols));

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int value;
            in >> value;
            this->kernel[r][c] = value;
        }
    }

    int divisor, offset;
    in >> divisor >> offset;
    this->divisor = divisor;
    this->offset = offset;

    int anchorX, anchorY;
    in >> anchorX >> anchorY;

    this->anchorX = anchorX;
    this->anchorY = anchorY;

    file.close();
}

Kernel::~Kernel()
{
}

QVector<QVector<int>> Kernel::getKernel() const
{
    return kernel;
}

int Kernel::getDivisor() const
{
    return divisor;
}

int Kernel::getOffset() const
{
    return offset;
}

int Kernel::getAnchorX() const
{
    return anchorX;
}

int Kernel::getAnchorY() const
{
    return anchorY;
}

int Kernel::getRows() const
{
    return rows;
}

int Kernel::getCols() const
{
    return cols;
}