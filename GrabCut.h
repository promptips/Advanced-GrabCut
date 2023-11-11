
/*
Copyright (C) 2015 David Doria, daviddoria@gmail.com

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

#ifndef GrabCut_H
#define GrabCut_H

// Submodules
#include "Mask/ForegroundBackgroundSegmentMask.h"
#include "ImageGraphCutSegmentation/ImageGraphCut.h"

// ITK
#include "itkImage.h"

// STL
#include <vector>

// Eigen
#include <Eigen/Dense>

#include "ExpectationMaximization/MixtureModel.h"

/** Perform GrabCut segmentation on an image.  */
template <typename TImage>
class GrabCut
{
public:
    // Typedefs
    typedef typename TImage::PixelType PixelType;

    /** Constructor */
    GrabCut();

    /** The type of a list of pixels/indexes. */
    typedef std::vector<itk::Index<2> > IndexContainer;