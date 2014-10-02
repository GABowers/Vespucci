#ifndef VCADIALOG_H
#define VCADIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class VCADialog;
}

class VCADialog : public QDialog
{
    Q_OBJECT

public:
    explicit VCADialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~VCADialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::VCADialog *ui;
    VespucciWorkspace *workspace;
    QSpinBox *image_component_selector_;
    QSpinBox *components_selector_;
    QComboBox *color_selector_;
    QLineEdit *name_box_;
    QCheckBox *recalculate_box_;

    //index of the VespucciDataset object in the various lists
    int data_index_;

    //Pointer to the VespucciDataset object at data_index_
    QSharedPointer<VespucciDataset> data_;
};

#endif // VCADIALOG_H
