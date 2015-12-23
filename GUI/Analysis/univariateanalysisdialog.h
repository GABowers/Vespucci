#ifndef UNIVARIATEANALYSISDIALOG_H
#define UNIVARIATEANALYSISDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
class VespucciWorkspace;

namespace Ui {
class UnivariateAnalysisDialog;
}

class UnivariateAnalysisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnivariateAnalysisDialog(QWidget *parent,
                                      VespucciWorkspace *ws,
                                      int row);
    ~UnivariateAnalysisDialog();

private slots:
    void on_peakComboBox_currentIndexChanged(const QString &arg1);

    void on_buttonBox_accepted();

private:
    Ui::UnivariateAnalysisDialog *ui;

    ///
    /// \brief workspace
    /// Pointer to the workspace (which contains dataset list)
    VespucciWorkspace *workspace;


    ///
    /// \brief first_min_line_edit_
    /// User enteres first minimum here
    QLineEdit *first_min_line_edit_;

    ///
    /// \brief first_max_line_edit_
    /// User enters first maximum here
    QLineEdit *first_max_line_edit_;

    ///
    /// \brief second_min_line_edit_
    /// User enters second minimum here
    QLineEdit *second_min_line_edit_;

    ///
    /// \brief second_max_line_edit_
    /// User enteres second maximum here
    QLineEdit *second_max_line_edit_;


    ///
    /// \brief integration_method_selector_combo_box_
    /// User selects the integration method here
    QComboBox *integration_method_selector_combo_box_;

    ///
    /// \brief range_label_
    /// Displays the valid range
    QLabel *range_label_;

    ///
    /// \brief data_
    /// Points to the dataset we're working with.
    QSharedPointer<VespucciDataset> data_;


    ///
    /// \brief value_method_selector_combo_box_
    /// User selects the value determination method here
    QComboBox *value_method_selector_combo_box_;
};

#endif // UNIVARIATEANALYSISDIALOG_H
