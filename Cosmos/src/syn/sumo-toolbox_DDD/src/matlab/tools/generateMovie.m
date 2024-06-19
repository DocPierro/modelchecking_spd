% > @file "generateMovie.m"
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
% >	 generateMovie(model, outputFile, numSlices, framesPerSecond, varyAxis, outputIndex, options)
% 
%  ======================================================================
% > @brief Create a movie of slices from a model with 3 inputs and n
% >	outputs, on the input interval [-1,1].
% >
% > Only one of the outputs is
% > plotted, specified by outputIndex. A subfolder named slices will
% > be created in the directory where the movie must be saved. In
% > that subdirectory the generated slices will be stored as png
% > files. The value of the varying axis will be appended to the title of
% > each slice.
% > NOTE: existing png files in the slices subdirectory will be deleted!
% > @param model	the model to generate slices from (3
% >                dimensional input, n dimensional output)
% > @param outputFile	file location to save the movie to
% > @param numSlices	number of slices to generate over the interval
% > @param framesPerSecond	number of frames per second in the movie
% > @param varyAxis		the axis index of the model to variate over
% >                  the different slices
% > @param outputIndex	index of the output dimension to plot
% >                    (ignored if output is 1D, defaults to 1)
% > @param options	plot options (to pass to plotScatteredData)
%  ======================================================================
%
