#ifndef FILTEREDITORDIALOG_H
#define FILTEREDITORDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include "kernel.h"

class FilterEditorDialog : public QDialog {
    Q_OBJECT

public:
    explicit FilterEditorDialog(QWidget *parent = nullptr);
    QVector<QVector<int>> getKernel() const;
    int getDivisor() const;
    int getOffset() const;
    int getAnchorRow() const;
    int getAnchorCol() const;

signals:
    void filterApplied(Kernel customKer);

private slots:
    void updateKernelSize();
    void computeDivisor();
    void saveCustomFilter();
    void saveAsCustomFilter();
    void loadCustomFilter();
    void applyFilter();

private:
    QTableWidget *kernelTable;
    QSpinBox *rowsSpinBox;
    QSpinBox *colsSpinBox;
    QLineEdit *divisorEdit;
    QLineEdit *offsetEdit;
    QSpinBox *anchorRowSpinBox;
    QSpinBox *anchorColSpinBox;
    QString filterPath;
};

#endif // FILTEREDITORDIALOG_H
