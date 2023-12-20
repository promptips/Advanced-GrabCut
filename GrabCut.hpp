/*
Copyright (C) 2012 David Doria, daviddoria@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GrabCut_HPP
#define GrabCut_HPP

#include "GrabCut.h"

// Submodules
#include "Helpers/Helpers.h"
#include "ITKHelpers/ITKHelpers.h"

#include "ExpectationMaximization/ExpectationMaximization.h"
#include "ExpectationMaximization/GaussianModel.h"

// ITK
#include "itkImageRegionIterator.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkMaskImageFilter.h"

// STL
#include <cmath>
#include <sstream>

// Boost
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>

template <typename TImage>
GrabCut<TImage>::GrabCut()
{
    this->Image = TImage::New();
    this->InitialMask = ForegroundBackgroundSegmentMask::New();
    this->SegmentationMask = ForegroundBackgroundSegmentMask::New();
}

template <typename TImage>
void GrabCut<TImage>::SetImage(TImage* const image)
{
    ITKHelpers::DeepCopy(image, this->Image.GetPointer());
}

template <typename TImage>
void GrabCut<TImage>::SetInitialMask(ForegroundBackgroundSegmentMask* const mask)
{
    // Save the initial mask
    ITKHelpers::DeepCopy(mask, this->InitialMask.GetPointer());

    // Initialize the segmentation mask from the initial mask
    ITKHelpers::DeepCopy(mask, this->SegmentationMask.GetPointer());
}

template <typename TImage>
Eigen::MatrixXd GrabCut<TImage>::CreateMatrixFromPixels(const std::vector<typename TImage::PixelType>& pixels)
{
    unsigned int dimensionality = this->GetDimensionality();
    Eigen::MatrixXd data(dimensionality, pixels.size());

    for(unsigned int i = 0; i < pixels.size(); i++)
    {
        Eigen::VectorXd v(dimensionality);
        PixelType p = pixels[i];
        for(unsigned int d = 0; d < dimensionality; ++d)
        {
            v(d) = p[d];
        }

        data.col(i) = v;
    }

    return data;
}

template <typename TImage>
void GrabCut<TImage>::InitializeModels(const unsigned int numberOfModels)
{
    unsigned int dimensionality = this->GetDimensionality();

    // Initialize the foreground and background mixture models
    std::vector<Model*> foregroundModels;
    std::vector<Model*> backgroundModels;
    foregroundModels.resize(numberOfModels);
    backgroundModels.resize(numberOfModels);

    for(unsigned int i = 0; i < numberOfModels; i++)
    {
      Model* foregroundModel = new GaussianModel(dimensionality);
      foregroundModels[i] = foregroundModel;

      Model* backgroundModel = new GaussianModel(dimensionality);
      backgroundModels[i] = backgroundModel;
    }

    this->ForegroundModels.SetModels(foregroundModels);
    this->BackgroundModels.SetModels(background