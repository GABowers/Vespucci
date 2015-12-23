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
#include "GUI/Display/dataviewer.h"
#include "ui_dataviewer.h"
#include "GUI/Display/spectrumviewer.h"

///
/// \brief DataViewer::DataViewer
/// \param parent Parent QWidget
/// \param ws Current workspace
/// \param row Row of current dataset
///
DataViewer::DataViewer(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    ui->setupUi(this);
    workspace = ws;

    dataset_ = workspace->DatasetAt(row);

    QString title = "Data Viewer (" + dataset_->name() + ")";
    setWindowTitle(title);

    directory_ = workspace->directory();

    export_tool_button_ = findChild<QToolButton *>("exportToolButton");
    plot_tool_button_ = findChild<QToolButton *>("plotToolButton");
    data_selector_combo_box_ = findChild<QComboBox *>("comboBox");
    extract_tool_button_ = findChild<QToolButton *>("extractToolButton");
    stats_tool_button_ = findChild<QToolButton *>("statsToolButton");
    plot_tool_button_->setDisabled(true);
    export_tool_button_->setDisabled(true);
    extract_tool_button_->setDisabled(true);
    table_view_ = findChild<QTableView *>("tableView");

    const mat *matrix;
    matrix = dataset_->wavelength_ptr();
    data_objects_.insert("Spectral Abscissa", matrix);
    matrix = dataset_->x_ptr();
    data_objects_.insert("Spatial Data", matrix);
    matrix = dataset_->indices_ptr();
    data_objects_.insert("Indices", matrix);

    partial_least_squares_calculated_ = dataset_->partial_least_squares_calculated();
    vertex_components_calculated_ = dataset_->vertex_components_calculated();
    k_means_calculated_ = dataset_->k_means_calculated();
    univariate_count_ = dataset_->UnivariateCount();
    principal_components_calculated_ = dataset_->principal_components_calculated();

    if (univariate_count_ > 0){
        QList<QSharedPointer<UnivariateData> > univariate_data_list = dataset_->univariate_datas();
        for (int i = 0; i < univariate_data_list.size(); ++i){
            matrix = (mat *) univariate_data_list[i]->results_ptr();
            data_objects_.insert(univariate_data_list[i]->name(), matrix);
            if(univariate_data_list[i]->calibrated()){
                metadata_objects_.insert(univariate_data_list[i]->name() +
                                         "Calibration Statistics",
                                         univariate_data_list[i]->calibration_stats());
                matrix = univariate_data_list[i]->calibration_curve_ptr();
                data_objects_.insert(univariate_data_list[i]->name() +
                                     "Calibration Curve",
                                     matrix);
            }
        }
    }

    if (principal_components_calculated_){
        matrix = (mat *) dataset_->principal_components_data()->latent();
        data_objects_.insert("PCA Eigenvalues of Covariance Matrix", matrix);
        matrix = (mat *) dataset_->principal_components_data()->percent_variance();
        data_objects_.insert("PCA Percent Variance", matrix);
    }

    if (vertex_components_calculated_){
        matrix = dataset_->vertex_components_data()->endmember_spectra();
        data_objects_.insert("VCA Endmembers", matrix);
        matrix = dataset_->vertex_components_data()->fractional_abundances();
        data_objects_.insert("VCA Fractional Abundances", matrix);
        matrix = dataset_->vertex_components_data()->indices();
        data_objects_.insert("VCA Pure Pixel Indices", matrix);

        vca_endmembers_ = dataset_->vertex_components_data()->NumberComponents();
    }

    if (partial_least_squares_calculated_){
        matrix = dataset_->partial_least_squares_data()->percent_variance();
        data_objects_.insert("PLS Variance", matrix);
        matrix = dataset_->partial_least_squares_data()->coefficients();
        data_objects_.insert("PLS Coefficients", matrix);

    }

    if (k_means_calculated_){
        matrix = dataset_->k_means_data();
        data_objects_.insert("k-Means Assignments", matrix);
    }

    if (dataset_->meta()){
        matrix = dataset_->parent_dataset_indices();
        data_objects_.insert("Parent Dataset Coordinates", matrix);
    }
    QString key;

    if (dataset_->AnalysisResultsList().size() > 0){
        for (int i = 0; i < dataset_->AnalysisResultsList().size(); ++i){
            key = dataset_->AnalysisResultsList()[i];
            matrix = dataset_->AnalysisResult(key);
            //attempting to display very large matrices will freeze the dataviewer
            //items will have to be saved and then opened.
            if (matrix->n_rows <= 100 || matrix->n_cols <= 100){
                data_objects_.insert(key, matrix);
            }
        }
    }
    data_selector_combo_box_->addItems(data_objects_.keys());
    data_selector_combo_box_->setCurrentIndex(0);
}

DataViewer::~DataViewer()
{
    delete ui;
}

///
/// \brief DataViewer::on_comboBox_currentTextChanged
/// \param arg1
/// Changes the table model when a user changes the selected data object
void DataViewer::on_comboBox_currentTextChanged(const QString &arg1)
{
    RefreshComboBox();
    current_text_ = arg1;
    if (arg1 == "Spatial Data"){
        VespucciTableModel *table_model = new VespucciTableModel(this, dataset_, "spatial");
        table_view_->setModel(table_model);
        current_data_ = table_model->GetData();
        export_tool_button_->setDisabled(true);
        plot_tool_button_->setDisabled(true);
        extract_tool_button_->setDisabled(true);
        stats_tool_button_->setDisabled(true);
    }
    else if (data_objects_.contains(arg1)){
        current_data_ = data_objects_[arg1];
        VespucciTableModel *table_model = new VespucciTableModel(this, current_data_);
        table_view_->setModel(table_model);
        export_tool_button_->setDisabled(false);

        if (current_data_->n_cols == dataset_->spectra_ptr()->n_cols 
                || current_data_->n_rows == dataset_->spectra_ptr()->n_cols){
            extract_tool_button_->setDisabled(false);
            stats_tool_button_->setDisabled(false);
        }
        else{
            extract_tool_button_->setDisabled(true);
            stats_tool_button_->setDisabled(true);
        }


        if (current_data_->n_rows == dataset_->spectra_ptr()->n_rows)
            plot_tool_button_->setDisabled(false);
        else
            plot_tool_button_->setDisabled(true);

    }
    else if (metadata_objects_.contains(arg1)){
        extract_tool_button_->setDisabled(true);
        stats_tool_button_->setDisabled(true);
    }
    else{
        current_data_ = data_objects_["Spectral Abscissa"];
        VespucciTableModel *table_model = new VespucciTableModel(this, current_data_);
        table_view_->setModel(table_model);
        export_tool_button_->setDisabled(false);
        plot_tool_button_->setDisabled(true);
        stats_tool_button_->setDisabled(true);
    }

    table_view_->resizeColumnsToContents();
    table_view_->resizeRowsToContents();
}

///
/// \brief DataViewer::on_plotToolButton_clicked
/// Triggers a dialog that allows the plotting of data
void DataViewer::on_plotToolButton_clicked()
{
    bool ok;
    if (current_text_ == "VCA Endmembers"){
        int endmember = QInputDialog::getInt(this,
                                             tr("Select Endmember"),
                                             tr("Endmember"), 1, 1,
                                             vca_endmembers_, 1, &ok, 0);
        endmember--; //fix indexing from human-intuitve 1 to programmer-intuitive 0

        if (ok){

            spectrum_viewer_ = new SpectrumViewer(this, dataset_, endmember, directory_, "VCA");
            spectrum_viewer_->show();
        }
    }
    else{
        int column = QInputDialog::getInt(this,
                                          "Select Column",
                                          "Column", 1, 1,
                                          current_data_->n_cols,
                                          1, &ok, 0);
        if(ok){
            vec data = current_data_->col(column);
            spectrum_viewer_ = new SpectrumViewer(this, dataset_, data, current_text_);
            spectrum_viewer_->show();
        }

    }

}

///
/// \brief DataViewer::on_exportToolButton_clicked
/// Triggers a save dialog to save the current dataset
void DataViewer::on_exportToolButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Export As..."),
                                                    directory_,
                                                    tr("Comma-separated Variables (*.csv);;"
                                                       "Tab-delimited Text (*.txt);;"
                                                       "Armadillo Binary (*.arma);;"));
    QFileInfo file_info(filename);
    bool success = false;
    if (filename.isEmpty())
        return;
    workspace->set_directory(file_info.dir().path());
    QString extension = file_info.suffix();
    if (extension == "arma")
        success = current_data_->save(filename.toStdString(), arma_binary);
    else if (extension == "csv")
        success = current_data_->save(filename.toStdString(), csv_ascii);
    else
        success = current_data_->save(filename.toStdString(), raw_ascii);

    if (success)
        QMessageBox::information(this, "Success!", "File " + filename + " written successfully!");
    else
        QMessageBox::warning(this, "File Not Saved", "File was not written successfully.");

    return;
}

void DataViewer::RefreshComboBox()
{
    mat *matrix;

    if (univariate_count_ != dataset_->UnivariateCount()){
        univariate_count_ = dataset_->UnivariateCount();
        QList<QSharedPointer<UnivariateData> > univariate_data_list = dataset_->univariate_datas();

        for (int i = 0; i < univariate_data_list.size(); ++i){
            matrix = (mat *) univariate_data_list[i]->results_ptr();
            if (!data_objects_.values().contains(matrix)){
                data_objects_.insertMulti(univariate_data_list[i]->name(), matrix);
                data_selector_combo_box_->addItem(univariate_data_list[i]->name());
            }
        }
    }

    if (dataset_->principal_components_calculated() && !principal_components_calculated_){
        principal_components_calculated_ = true;

        matrix = dataset_->principal_components_data()->coeff();
        data_objects_.insert("PCA Coefficients", matrix);
        data_selector_combo_box_->addItem("PCA Cofficients");

        matrix = (mat *) dataset_->principal_components_data()->tsquared();
        data_objects_.insert("PCA t² Values", matrix);
        data_selector_combo_box_->addItem("PCA t² Values");

        matrix = (mat *) dataset_->principal_components_data()->latent();
        data_objects_.insert("PCA Eigenvalues of Covariance Matrix", matrix);
        data_selector_combo_box_->addItem("PCA Eigenvalues of Covariance Matrix");
    }

    if (dataset_->vertex_components_calculated() && !vertex_components_calculated_){
        vertex_components_calculated_ = true;

        matrix = dataset_->vertex_components_data()->endmember_spectra();
        data_objects_.insert("VCA Endmembers", matrix);
        data_selector_combo_box_->addItem("VCA Endmembers");

        matrix = dataset_->vertex_components_data()->fractional_abundances();
        data_objects_.insert("VCA Fractional Abundances", matrix);
        data_selector_combo_box_->addItem("VCA Fractional Abundances");

        matrix = dataset_->vertex_components_data()->indices();
        data_objects_.insert("VCA Pure Pixel Indices", matrix);
        data_selector_combo_box_->addItem("VCA Pure Pixel Indices");

        vca_endmembers_ = dataset_->vertex_components_data()->NumberComponents();
    }

    if (dataset_->partial_least_squares_calculated() && !partial_least_squares_calculated_){
        partial_least_squares_calculated_ = true;

        matrix = dataset_->partial_least_squares_data()->percent_variance();
        data_objects_.insert("PLS Variance", matrix);
        data_selector_combo_box_->addItem("PLS Variance");

        matrix = dataset_->partial_least_squares_data()->X_loadings();
        data_objects_.insert("PLS Predictor Loadings", matrix);
        data_selector_combo_box_->addItem("PLS Predictor Loadings");

        matrix = dataset_->partial_least_squares_data()->Y_loadings();
        data_selector_combo_box_->addItem("PLS Response Loadings");
        data_objects_.insert("PLS Response Loadings", matrix);


        matrix = dataset_->partial_least_squares_data()->X_scores();
        data_objects_.insert("PLS Predictor Scores", matrix);
        data_selector_combo_box_->addItem("PLS Predictor Scores");

        matrix = dataset_->partial_least_squares_data()->Y_scores();
        data_objects_.insert("PLS Response Scores", matrix);
        data_selector_combo_box_->addItem("PLS Response Scores");


        matrix = dataset_->partial_least_squares_data()->coefficients();
        data_objects_.insert("PLS Coefficients", matrix);
        data_selector_combo_box_->addItem("PLS Coefficients");

    }

    if (dataset_->k_means_calculated() && !k_means_calculated_){
        matrix = dataset_->k_means_data();
        data_objects_.insert("k-Means Assignments", matrix);
        data_selector_combo_box_->addItem("k-Means Assignments");
    }
}

void DataViewer::on_extractToolButton_clicked()
{
    int column_number;
    vec column;
    if (current_data_->n_cols != 1){
        column_number = QInputDialog::getInt(this,
                                             "Select Column",
                                             "Column number",
                                             1, 1,
                                             current_data_->n_cols);
        column = current_data_->col(column_number - 1);

    }
    else{
        column = current_data_->col(0);
    }
    DataExtractorDialog *extractor_dialog =
            new DataExtractorDialog(this,
                                    column,
                                    dataset_,
                                    workspace->main_window(),
                                    data_selector_combo_box_->currentText());
    extractor_dialog->show();
}


void DataViewer::on_statsToolButton_clicked()
{
    vec column;
    int column_number;

    if (current_data_->n_cols == 0){
        return;
    }
    else if (current_data_->n_cols > 1){
             column_number = QInputDialog::getInt(this,
                                                  "Select Column",
                                                  "Column number",
                                                  1, 1,
                                                  current_data_->n_cols);
             column = current_data_->col(column_number - 1);

    }
    else{
        column = current_data_->col(0);
    }

    StatsDialog *stats_dialog = new StatsDialog(this, column);
    stats_dialog->show();

}
