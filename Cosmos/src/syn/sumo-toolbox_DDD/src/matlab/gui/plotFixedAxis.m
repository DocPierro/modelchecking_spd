% > @file "plotFixedAxis.m"
% > @authors SUMO Lab Team
% > @version 2014a
% > @date Copyright 2006-2014
% >
% > This file is part of the Surrogate Modeling Toolbox ("SUMO Toolbox")
% > and you can redistribute it and/or modify it under the terms of the
% > GNU Affero General Public License version 3 as published by the
% > Free Software Foundation.  With the additional provision that a commercial
% > license must be purchased if the SUMO Toolbox is used, modified, or extended
% > in a commercial setting. For details see the included LICENSE.txt file.
% > When referring to the SUMO Toolbox please make reference to the corresponding
% > publication:
% >   - A Surrogate Modeling and Adaptive Sampling Toolbox for Computer Based Design
% >   D. Gorissen, K. Crombecq, I. Couckuyt, T. Dhaene, P. Demeester,
% >   Journal of Machine Learning Research,
% >   Vol. 11, pp. 2051-2055, July 2010.
% >
% > Contact : sumo@sumo.intec.ugent.be - http://sumo.intec.ugent.be
% > Signature
% >	 plotFixedAxis(directory, outputformat, axisSettings)
% 
%  ======================================================================
% > @brief Replot a directory of .fig images in the specified output format, with
% > axis limits as specified.
% >
% > @param directory path to the folder containing the .fig files
% > @param outputformat extension for the output images
% > @param axisSettings fixed axis settings for each image (@see AXIS)
% >
% > Example:
% > - plotFixedAxis('slices', 'png', [-1 1 -1 1 -1 1 -1 1])
%  ======================================================================
%
