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
#ifndef MULTIANALYZER_H
#define MULTIANALYZER_H
#include "Global/vespucciworkspace.h"
using namespace std;
using namespace arma;
///
/// \brief The MultiAnalyzer class
/// This class handles the execution of multivariate analysis across multiple
/// datasets. Each analysis method will create an AnalysisResults object smart
/// pointer that is shared by all datasets used, plus an auxiliary matrix in each
/// dataset for data that corresponds to spectra.
class MultiAnalyzer
{
public:
    MultiAnalyzer(QSharedPointer<VespucciWorkspace> workspace);
    void VertexComponents(const QStringList &keys,
                          const QString &name,
                          uword endmembers);
    void KMeans(const QStringList & keys,
                size_t clusters,
                const QString &metric_text,
                const QString &name);
    void PrincipalComponents(const QStringList &keys,
                             const QString &name);
    void PrincipalComponents(const QStringList &keys,
                             const QString &name,
                             bool scale_data);
    void PartialLeastSquares(const QStringList &keys,
                             const QString &name,
                             uword components);
    bool CheckMergability(QStringList &keys, QStringList &problematic_keys);
private:
    QSharedPointer<VespucciWorkspace> workspace_;
    QList<QSharedPointer<VespucciDataset> > GetDatasets(QStringList keys);
    void ConcatenateSpectra(QList<QSharedPointer<VespucciDataset> > datasets,
                            mat &data,
                            uvec &start_indices,
                            uvec &end_indices);
};

#endif // MULTIANALYZER_H
