#ifndef KMEANSDIALOG_H
#define KMEANSDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class KMeansDialog;
}

class KMeansDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KMeansDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~KMeansDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::KMeansDialog *ui;
    VespucciWorkspace *workspace;

    QSpinBox *cluster_spin_box_;
    QLineEdit *name_line_edit_;

    //index of the SpecMap object in the various lists
    int data_index_;

    //Pointer to the SpecMap object at data_index_
    QSharedPointer<SpecMap> data_;
};

#endif // KMEANSDIALOG_H
