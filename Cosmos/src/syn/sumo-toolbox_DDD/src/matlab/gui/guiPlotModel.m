% > @file "guiPlotModel.m"
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
% >	[fighandle options] = guiPlotModel(model, outputIndex, inputs, options)
% 
%  ======================================================================
% > @brief This plotModel implementation visualises the model with a GUI to
% > choose the way the model is plotted.
% >
% >All parameters are optional. If you specify a second output parameter
% >and no input parameters, the default plot options are returned
% >without further action. The options parameter will be merged with the
% >default options, so you need not specify all fields.
% >
% > @param model			the model to plot
% > @param outputIndex	index of the output to plot
% > @param inputs			input settings (axis indices, bounds, values)
% > @param options		plot options (will be merged with defaults)
% > @retval fighandle		handle of the plotted figure
% > @retval options		the initial plot options
% >
% > Examples:
% > - guiPlotModel * Show a load model dialog, use all default settings.
% > - guiPlotModel(model,1,defaultInputSettings(model),options) * Specify custom plot options, but use the default input settings.
% > - [dummy options] = guiPlotModel * Just get the default plot options.
%  ======================================================================
%
