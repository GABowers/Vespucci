#include "dataviewer.h"
#include "ui_dataviewer.h"
#include "spectrumviewer.h"

DataViewer::DataViewer(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);
    directory_ = workspace->directory();
    export_button_ = this->findChild<QPushButton *>("exportPushButton");
    plot_button_ = this->findChild<QPushButton *>("plotPushButton");
    data_selector_ = this->findChild<QComboBox *>("comboBox");
    plot_button_->setDisabled(true);
    export_button_->setDisabled(true);
    table_ = this->findChild<QTableView *>("tableView");

    //cout << "populate list" << endl;
    QStringList object_list("Spectral Abcissa");
    object_list << "Spatial Data";

    if (dataset_->principal_components_calculated()){
        object_list << "PCA Coefficents" << "Eigenvalues of Covariance Matrix"
                    << "PCA t² Values";
    }
    if (dataset_->vertex_components_calculated()){
        object_list << "VCA Coefficients" << "VCA Endmembers" << "VCA Pure Components";
        vca_endmembers_ = dataset_->vertex_components_data()->NumberComponents();
    }
    if (dataset_->partial_least_squares_calculated()){
        object_list << "PLS Variance" << "PLS Predictor Loading"
                    << "PLS Response Loading" << "PLS Predictor Scores"
                    << "PLS Response Scores" << "PLS Coefficients";
    }
    if (dataset_->k_means_calculated()){
        object_list << "K-means Assignments";
    }

    //cout << "assign list" << endl;
    data_selector_->addItems(object_list);
    data_selector_->setCurrentIndex(0);
    //cout << "end of constructor";
}

DataViewer::~DataViewer()
{
    delete ui;
}

void DataViewer::on_comboBox_currentTextChanged(const QString &arg1)
{
    current_text_ = arg1;
    //cout << "DataViewer::on_comboBox_currentTextChanged" << endl;
    //cout << arg1.toStdString();
    if (arg1 == "Spatial Data"){
        VespucciTableModel *table_model = new VespucciTableModel(this, dataset_, "spatial");
        table_->setModel(table_model);
        current_data_ = table_model->GetData();
        export_button_->setDisabled(true);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "Spectral Abcissa"){
        VespucciTableModel *table_model = new VespucciTableModel(this, dataset_->wavelength_ptr());
        current_data_ = table_model->GetData();
        table_->setModel(table_model);
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PCA Coefficients"){
        current_data_ = dataset_->principal_components_data()->coeff();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "Eigenvalues of Covariance Matrix"){
        current_data_ = (mat*) dataset_->principal_components_data()->latent();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PCA t² Values"){
        current_data_ = (mat*) dataset_->principal_components_data()->tsquared();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "VCA Coefficients"){
        current_data_ = dataset_->vertex_components_data()->fractional_abundances();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "VCA Endmembers"){
        current_data_ = dataset_->vertex_components_data()->endmember_spectra();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(false);
    }
    else if (arg1 == "K-means Assignments"){
        current_data_ = dataset_->k_means_data();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Variance"){
        current_data_ = dataset_->partial_least_squares_data()->percent_variance();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Predictor Loading"){
        current_data_ = dataset_->partial_least_squares_data()->X_loadings();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Response Loading"){
        current_data_ = dataset_->partial_least_squares_data()->Y_loadings();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Predictor Scores"){
        current_data_ = dataset_->partial_least_squares_data()->X_scores();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Response Scores"){
        current_data_ = dataset_->partial_least_squares_data()->Y_scores();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Coefficients"){
        current_data_ = dataset_->partial_least_squares_data()->coefficients();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else{
        VespucciTableModel *table_model = new VespucciTableModel(this, dataset_->wavelength_ptr());
        current_data_ = table_model->GetData();
        table_->setModel(table_model);
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }

    table_->resizeColumnsToContents();
    table_->resizeRowsToContents();
}


void DataViewer::on_plotPushButton_clicked()
{
    if (current_text_ == "VCA Endmembers"){
        bool ok;
        int endmember = QInputDialog::getInt(this,
                                             tr("Select Endmember"),
                                             tr("Endmember"), 1, 1,
                                             vca_endmembers_, 1, &ok, 0);
        endmember--; //fix indexing from human-intuitve 1 to programmer-intuitive 0

        if (ok){
            spectrum_viewer_ = new SpectrumViewer(this, dataset_.data(), endmember, directory_, tr("VCA"));
            spectrum_viewer_->show();
        }
    }
}

void DataViewer::on_exportPushButton_clicked()
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

    QString extension = file_info.suffix();
    cout << "save" << endl;
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
