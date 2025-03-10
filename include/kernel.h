#ifndef KERNEL_H
#define KERNEL_H

#include <QVector>
#include <QString>
#include <ostream>


class Kernel
{
    private:
    int rows;
    int cols;
    QVector<QVector<int>> kernel;
    int divisor;
    int offset;
    int anchorX;
    int anchorY;

    public:
        Kernel();
        Kernel(QString filterPath);
        Kernel(int rows, int cols, QVector<QVector<int>> kernel, int divisor, int offset, int anchorX, int anchorY);
        ~Kernel();

        QVector<QVector<int>> getKernel() const;
        int getDivisor() const;
        int getOffset() const;
        int getAnchorX() const;
        int getAnchorY() const;
        int getRows() const;
        int getCols() const;
};

#endif // KERNEL_H