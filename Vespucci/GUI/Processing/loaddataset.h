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
#ifndef LOADDATASET_H
#define LOADDATASET_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class LoadDataset;
}

///
/// \brief The LoadDataset class
/// Dialog that allows the user to import files into the program
class LoadDataset : public QDialog
{
    Q_OBJECT

public:
    explicit LoadDataset(QWidget *parent, VespucciWorkspace * ws);
    ~LoadDataset();
public slots:
    void FilenameChanged(QString new_filename);
private slots:
    void on_browseButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_filenameLineEdit_textChanged(const QString &arg1);

private:
    Ui::LoadDataset *ui;
    ///
    /// \brief workspace
    /// The current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief directory_
    /// The current working directory
    QString *directory_;
    QDialogButtonBox *button_box_;
    QLabel *file_size_label_;
    QCheckBox *swap_check_box_;
    QLineEdit *filename_line_edit_;
    QLineEdit *name_line_edit_;
    QLineEdit *y_description_line_edit_;
    QLineEdit *x_description_line_edit_;
    QLineEdit *y_units_line_edit_;
    QLineEdit *x_units_line_edit_;
    QComboBox *data_format_combo_box_;
};

#endif // LOADDATASET_H
