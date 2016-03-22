/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#include "GUI/Analysis/kmeansdialog.h"
#include "ui_kmeansdialog.h"

///
/// \brief KMeansDialog::KMeansDialog
/// \param parent Parent QWidget
/// \param ws Current workspace
/// \param row Row of current dataset
///
KMeansDialog::KMeansDialog(QWidget *parent, VespucciWorkspace *ws, const QModelIndex &dataset_index) :
    QDialog(parent),
    ui(new Ui::KMeansDialog)
{
    ui->setupUi(this);
    name_line_edit_ = findChild<QLineEdit *>("nameLineEdit");
    cluster_spin_box_ = findChild<QSpinBox *>("clustersSpinBox");
    prediction_check_box_ = findChild<QCheckBox *>("predictionCheckBox");
    metric_combo_box_ = findChild<QComboBox *>("metricComboBox");
    workspace = ws;
    data_ = workspace->DatasetAt(dataset_index);
    name_line_edit_->setEnabled(false);
}

KMeansDialog::~KMeansDialog()
{
    delete ui;
}

///
/// \brief KMeansDialog::on_buttonBox_accepted
/// Triggers K-means method of dataset when "Ok" selected
void KMeansDialog::on_buttonBox_accepted()
{

    QString metric_text = metric_combo_box_->currentText();
    int clusters;
    QString name = name_line_edit_->text();
    if (prediction_check_box_->isChecked())
        clusters = 0;
    else
        clusters = cluster_spin_box_->value();
    try{
        data_->KMeans(clusters, metric_text, name);
    }catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }

    data_.clear();
    close();
}

///
/// \brief KMeansDialog::on_buttonBox_rejected
/// Closes window when "Cancel" selected.
void KMeansDialog::on_buttonBox_rejected()
{
    close();
    data_.clear();
}

void KMeansDialog::on_predictionCheckBox_clicked(bool checked)
{
    cluster_spin_box_->setEnabled(!checked);
}
