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
#include "Data/Analysis/plsdata.h"

PLSData::PLSData(QSharedPointer<VespucciDataset> parent, QString *directory)
{
    parent_ = parent;
    directory_ = directory;
}

///
/// \brief PLSData::Apply
/// \param spectra Input matrix
/// \param wavelength Spectral abscissa
/// \param components Number of components to calculate
/// \return
/// Performs PLS analysis on a copy of the spectra matrix.
bool PLSData::Apply(mat spectra, vec wavelength, int components)
{
    mat Y = repmat(wavelength, 1, components);
    bool success = Vespucci::Math::DimensionReduction::plsregress(spectra, Y, components,
                                        X_loadings_, Y_loadings_,
                                        X_scores_, Y_scores_,
                                        coefficients_, percent_variance_,
                                        fitted_);

    return success;

}

bool PLSData::Calibrate(mat spectra, mat controls)
{
    //spectra is y
    //controls are X

    bool success = Vespucci::Math::DimensionReduction::plsregress(controls, spectra, controls.n_cols,
                                                                  X_loadings_, Y_loadings_,
                                                                  X_scores_, Y_scores_,
                                                                  coefficients_, percent_variance_,
                                                                  fitted_);
    return success;
}

///
/// \brief DiscriminantAnalysis
/// \param spectra
/// \param labels
/// \param components
/// \return
/// Perform PLS-DA
bool DiscriminantAnalysis(mat spectra, vec labels, int components)
{
    //mat Y = repmat();
}

mat *PLSData::X_loadings()
{
    return &X_loadings_;
}

mat *PLSData::Y_loadings()
{
    return &Y_loadings_;
}

mat *PLSData::X_scores()
{
    return &X_scores_;
}

mat* PLSData::Y_scores()
{
    return &Y_scores_;
}

mat* PLSData::coefficients()
{
    return &coefficients_;
}

mat *PLSData::percent_variance()
{
    return &percent_variance_;
}

mat *PLSData::value(QString key)
{
    if (key == "PLS Variance")
        return &percent_variance_;
    else if (key == "PLS Predictor Loadings")
        return &X_loadings_;
    else if (key == "PLS Response Loadings")
        return &Y_loadings_;
    else if (key == "PLS Predictor Scores")
        return &X_scores_;
    else if (key == "PLS Response Scores")
        return &Y_scores_;
    else if (key == "PLS Coefficients")
        return &coefficients_;
    else
        return NULL;
}

int PLSData::NumberComponents()
{
    return coefficients_.n_cols;
}

colvec PLSData::Results(const uword i, bool &valid)
{
    if (coefficients_.n_cols < i){
        valid = false;
        return X_loadings_.col(coefficients_.n_cols - 1);
    }

    else{
        valid = true;
        return X_loadings_.col(i);
    }

}
