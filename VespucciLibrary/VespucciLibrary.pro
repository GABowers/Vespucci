#    Copyright (C) 2015 Daniel P. Foose - All Rights Reserved

#    This file is part of Vespucci.

#    Vespucci is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    Vespucci is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.

################################################################################
##############               LibVespucci Qt Profile               ##############
################################################################################
QT       += core gui
QT       += widgets printsupport
QT       += svg
CONFIG   += shared debug_and_release c++11
#previously we handled this as a framework, but we're building dylib files now
#mac: CONFIG += lib_bundle
# Set the installation directory
isEmpty(PREFIX) {
    PREFIX = $$PWD/../../Vespucci-install
}
travis_ci = $$(TRAVIS_CI)

# it is assumed that casual windows users will not use the build system to install
!win32:TARGET = vespucci
win32:TARGET = libvespucci
TEMPLATE = lib
DEFINES += VESPUCCI_LIBRARY

#Boost, MLPACK, and Armadillo have code that produces warnings. Change the directory as appropriate.
unix:!macx: QMAKE_CXXFLAGS += -std=c++0x \
                        -isystem "/usr/local/include" \
                        -isystem "/usr/local/include/armadillo_bits" \
                        -isystem "/usr/local/include/boost" \
                        -isystem "/usr/include/mlpack" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.1/RcppArmadillo/include" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.1/Rcpp/include" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.1/RInside/include" \
                        -isystem "/usr/share/R/include" \
                        -isystem /usr/include \
                        -isystem /usr/local/include \
                        -isystem /usr/local/include/cminpack-1 \
                        -isystem /usr/local/opt/libxml2/include/libxml2 \
                        -isystem $$PWD/../../Vespucci-QCP-sharedlib/include \
                        -isystem $$PWD/include
#Warnings in boost library not suppressed with -isystem in Clang++. These are always
macx: QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++
macx: QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter


macx: CONFIG +=c++11


win32-g++: QMAKE_CXXFLAGS += -std=gnu++11 \
                         -pthread \
                         -isystem "C:/Projects/Vespucci/MinGW_libs/include" \
                         -isystem "C:/Projects/Vespucci/MinGW_libs/boost/boost" \
                         -isystem "C:/Projects/Vespucci/MinGW_libs/include/armadillo_bits" \
                         -isystem "C:/Projects/Vespucci/MinGW_libs/include/mlpack" \
                         -isystem "C:/Tools/R/R-3.1.3/include" \
                         -isystem "C:/Tools/R/R-3.1.3/library/Rcpp/include" \
                         -isystem "C:/Tools/R/R-3.1.3/library/RcppArmadillo/include/armadillo_bits"



SOURCES +=\
    src/Data/Import/binaryimport.cpp \
    src/Data/Import/textimport.cpp \
    src/Math/Accessory/accessory.cpp \
    src/Math/DimensionReduction/pls.cpp \
    src/Math/DimensionReduction/svds.cpp \
    src/Math/DimensionReduction/VCA.cpp \
    src/Math/Normalization/normalization.cpp \
    src/Math/Quantification/bandwidth.cpp \
    src/Math/Quantification/integration.cpp \
    src/Math/Quantification/maximum.cpp \
    src/Math/Smoothing/FIR.cpp \
    src/Math/Smoothing/nonlinear.cpp \
    src/Math/Smoothing/whittaker.cpp \
    src/Math/Transform/cwt.cpp \
    src/Math/PeakFinding/peakfinding.cpp \
    src/Math/Fitting/linleastsq.cpp \
    src/Global/vespucci.cpp \
    src/Math/Transform/fft.cpp \
    src/Math/Quantification/correlation.cpp \
    src/Math/Quantification/quantification.cpp \
    src/Math/Stats/confidenceinterval.cpp \
    src/Math/Stats/hyptothesistests.cpp \
    src/Math/PeakFinding/kernelpeakfinding.cpp \
    src/Math/Accessory/distancemetricwrapper.cpp \
    src/Math/Stats/histogram.cpp \
    src/Math/Clustering/agglomerativeclustering.cpp \
    src/Math/Clustering/ahcanode.cpp


HEADERS  += \
    include/Global/enums.h \
    include/Data/Import/binaryimport.h \
    include/Data/Import/textimport.h \
    include/Math/VespucciMath.h \
    include/Math/Accessory/accessory.h \
    include/Math/DimensionReduction/dimensionreduction.h \
    include/Math/Normalization/normalization.h \
    include/Math/Quantification/quantification.h \
    include/Math/Smoothing/smoothing.h \
    include/Math/Transform/cwt.h \
    include/Math/PeakFinding/peakfinding.h \
    include/Math/Accessory/accessory_impl.h \
    include/Math/Fitting/linleastsq.h \
    include/Global/vespucci.h \
    include/Global/libvespucci.h \
    include/Math/Transform/fft.h \
    include/Math/Smoothing/FIR.h \
    include/Math/Smoothing/whittaker.h \
    include/Math/Smoothing/nonlinear.h \
    include/Math/Quantification/integration.h \
    include/Math/Quantification/maximum.h \
    include/Math/Quantification/bandwidth.h \
    include/Math/Quantification/correlation.h \
    include/Math/Stats/confidenceinterval.h \
    include/Math/Stats/hypothesistests.h \
    include/Math/PeakFinding/kernelpeakfinding.h \
    include/Math/Accessory/distancemetricwrapper.h \
    include/Math/Stats/histogram.h \
    include/Math/Clustering/agglomerativeclustering.h \
    include/Math/Clustering/ahcanode.h

#For all platforms:
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx{
    QMAKE_CXX=/usr/bin/g++-4.9
    CONFIG += c++11
    QMAKE_CXXFLAGS += -fext-numeric-literals
    LIBS += -L$$PWD/../../mlpack/lib -lmlpack
    LIBS += -L$$PWD/../../armadillo/lib -larmadillo
    LIBS += -L/usr/lib -larpack
    PRE_TARGETDEPS += /usr/lib/libarpack.a
    LIBS += -L/usr/lib/x86_64-linux-gnu -lhdf5
    PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/libhdf5.a
    LIBS += -L/usr/lib -lblas
    LIBS += -L/usr/lib -llapack
    LIBS += -L$$PWD/../../yaml-cpp/lib -lyaml-cpp
    PRE_TARGETDEPS += $$PWD/../../yaml-cpp/lib/libyaml-cpp.a
    LIBS += -L$$PWD/../../quazip/lib -lquazip
    PRE_TARGETDEPS += $$PWD/../../quazip/lib/libquazip.a

    LIBS += -L/usr/lib/x86_64-linux-gnu/ -lz
    PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/libz.a

    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include

    INCLUDEPATH += /usr/include/libxml2
    DEPENDPATH += /usr/include/libxml2

    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include

    INCLUDEPATH += $$PWD/../../mlpack/include
    DEPENDPATH += $$PWD/../../mlpack/include

    INCLUDEPATH += $$PWD/../../armadillo/include
    DEPENDPATH += $$PWD/../../armadillo/include

    INCLUDEPATH += $$PWD/../../quazip/include
    DEPENDPATH += $$PWD/../../quazip/include

    INCLUDEPATH += $$PWD/../../yaml-cpp/include
    DEPENDPATH += $$PWD/../../yaml-cpp/include
}

#mac libraries. These are the same in Travis-CI as in most local environments
#with all dependencies of armadillo and mlpack installed using homebrew
#and armadillo and mlpack installed to the ../armadillo and ../mlpack directories
macx{
    CONFIG += app_bundle
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

    QMAKE_CXXFLAGS += --system-header-prefix=/usr \
                      --system-header-prefix=$$PWD/../../armadillo \
                      --system-header-prefix=$$PWD/../../mlpack \
                      --system-header-prefix=$$PWD/../../yaml-cpp \
                      --system-header-prefix=$$PWD/../../quazip

    ICON = vespuccilogo.icns
    LIBS += -L/usr/lib -lc++

    LIBS += -L$$PWD/../../mlpack/lib/ -lmlpack
    INCLUDEPATH += $$PWD/../../mlpack/include
    DEPENDPATH += $$PWD/../../mlpack/include

    LIBS += -L$$PWD/../../armadillo/lib/ -larmadillo
    INCLUDEPATH += $$PWD/../../armadillo/include
    DEPENDPATH += $$PWD/../../armadillo/include

    LIBS += -L/usr/local/lib/ -larpack
    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include

    LIBS += -framework Accelerate

    LIBS += -L/usr/local/opt/hdf5/lib -lhdf5

    INCLUDEPATH += $$PWD/../../quazip/include
    DEPENDPATH += $$PWD/../../quazip/include
    PRE_TARGETDEPS += $$PWD/../../quazip/lib/libquazip.a

    LIBS += -L$$PWD/../../yaml-cpp/lib/ -lyaml-cpp
    INCLUDEPATH += $$PWD/../../yaml-cpp/include
    DEPENDPATH += $$PWD/../../yaml-cpp/include
    PRE_TARGETDEPS += $$PWD/../../yaml-cpp/lib/libyaml-cpp.a

    LIBS += -lz.1

    INCLUDEPATH += /usr/local/opt/libxml2/include/libxml2
    DEPENDPATH += /usr/local/opt/libxml2/include/libxml2
}

#windows libraries for msvc (we don't currently build the libraries for g++ on windows)
win32:!win32-g++{
    CONFIG += release force_debug_info
    QMAKE_CXXFLAGS += /MP /openmp

    LIBS += -L$$PWD/../../Vespucci_dependencies/mlpack/lib/ -lmlpack
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/mlpack/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/mlpack/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/mlpack/lib/mlpack.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/armadillo/lib/ -larmadillo
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/armadillo/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/armadillo/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/armadillo/lib/armadillo.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/OpenBLAS/ -llibopenblas
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/OpenBLAS/libopenblas.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/LAPACK/ -llapack_x64
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/LAPACK/lapack_x64.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/HDF5/lib/ -lhdf5
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/HDF5/lib/hdf5.lib

    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/boost_1_61_0
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/boost_1_61_0

    LIBS += -L$$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/ -llibboost_program_options-vc140-mt-1_61
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/libboost_program_options-vc140-mt-1_61.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/ -llibboost_math_c99-vc140-mt-1_61
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/libboost_math_c99-vc140-mt-1_61.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/ -llibboost_random-vc140-mt-1_61
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/libboost_random-vc140-mt-1_61.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/ -llibboost_serialization-vc140-mt-1_61
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/libboost_serialization-vc140-mt-1_61.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/ -llibboost_unit_test_framework-vc140-mt-1_61
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/boost_1_61_0/lib64-msvc-14.0/libboost_unit_test_framework-vc140-mt-1_61.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/quazip/lib/ -lquazip
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/quazip/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/quazip/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/quazip/lib/quazip.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/yaml-cpp/lib/ -llibyaml-cppmdd
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/yaml-cpp/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/yaml-cpp/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/yaml-cpp/lib/libyaml-cppmdd.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/HDF5/lib/ -lzlib
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/HDF5/lib/zlib.lib
}
