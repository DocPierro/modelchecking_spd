% > @file "genMovie.m"
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
% >	[movieOptions plotOptions] = genMovie(model, outputIndex, inputs, plotOptions, fighandle, movieOptions)
% 
%  ======================================================================
% > @brief Create a movie by varying over a chosen dimension.
% >
% > Get the default
% > movie options and plot options as a return value by specifying zero parameters.
% > The slices will be saved in a subdirectory 'slices' at the location
% > of the movie. All previously existing png and fig files will be
% > deleted from that directory. The plot and movie options will be
% > merged with the defaults, so you need not specify all fields.
% >
% > @param model			the model whereof a movie must be created
% > @param outputIndex	the index of the output dimension to plot
% > @param inputs			input settings: xIndex, yIndex, zIndex (varies over time), bounds and values
% > @param plotOptions	plot options
% > @param fighandle		handle of the figure to plot in
% > @param movieOptions	movie options
% > Examples:
% > - genMovie(model,2) * Create a movie of the second output.
% >
% > - [movieOptions plotOptions] = genMovie * Get the default movie and plot options.
%  ======================================================================
%
