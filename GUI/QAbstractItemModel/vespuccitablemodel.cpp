/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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
#include "GUI/QAbstractItemModel/vespuccitablemodel.h"
///
/// \brief VespucciTableModel::VespucciTableModel
/// \param parent See QDialog
/// \param input_data The matrix that is displayed by the model
///
VespucciTableModel::VespucciTableModel(QObject *parent, const mat *input_data) :
    QAbstractTableModel(parent)
{
    data_ = input_data;
    rows_ = data_->n_rows;
    cols_ = data_->n_cols;
    composite_ = false;
}

///
/// \brief VespucciTableModel::VespucciTableModel
/// \param parent
/// \param dataset Dataset containing data to be displayed
/// \param type The type of data this is "spatial" or not
///
VespucciTableModel::VespucciTableModel(QObject *parent, QSharedPointer<VespucciDataset> dataset, QString type) :
    QAbstractTableModel(parent)
{
    dataset_ = dataset;
    composite_ = true;
    if (type == "spatial"){
        rows_ = dataset->x_ptr()->n_rows;
        cols_ = 2;
        spatial_only_ = true;
    }
    else{
        rows_ = dataset->x_ptr()->n_rows + 1;
        cols_ = dataset->wavelength_ptr()->n_cols + 2;
        spatial_only_ = false;
    }
}

///
/// \brief VespucciTableModel::data
/// \param index
/// \param role
/// \return
/// See QAbstractTableModel::data()
QVariant VespucciTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }

    int row = index.row();
    int column = index.column();
    double value;
    if (!composite_){
        value = data_->at(row, column);
        return QString::number(value);
    }

    else
    {
        if (spatial_only_){
            switch (column)
            {
              case 0:
                return QString::number(dataset_->x_ptr()->at(row));
              default:
                return QString::number(dataset_->y_ptr()->at(row));
            }

        }
        switch (row)
        {
          case 0:
            switch (column)
            {
              case 0:
              case 1:
                return "";
              default:
                return QString::number(dataset_->wavelength_ptr()->at(column));
            }
          default:
            switch (column)
            {
              case 0:
                return QString::number(dataset_->x_ptr()->at(row));
              case 1:
                return QString::number(dataset_->y_ptr()->at(row));
              default:
                return QString::number(dataset_->spectra_ptr()->at(row - 1, column - 2));
            }

        }
    }

}

///
/// \brief VespucciTableModel::columnCount
/// \param parent
/// \return
/// See QAbstractTableModel::columnCount()
int VespucciTableModel::columnCount(const QModelIndex &parent) const
{
    //we don't need to use the model index here (our model only uses one object)
    return cols_;
}

///
/// \brief VespucciTableModel::rowCount
/// \param parent
/// \return
/// See QAbstractTableModel::rowCount
int VespucciTableModel::rowCount(const QModelIndex &parent) const
{
    //we don't need to use the model index here (our model only uses one object)
    return rows_;
}

///
/// \brief VespucciTableModel::GetData
/// \return
/// Get a pointer to the displayed data
const mat* VespucciTableModel::GetData() const
{
    return data_;
}
