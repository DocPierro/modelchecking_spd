% > @file "guiPlotModelErrors.m"
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
% >	[fighandle options trainingStats validationStats] = guiPlotModelErrors(model, outputIndex, validation, options, parentHandle)
% 
%  ======================================================================
% > @brief Plot a model's error stats for a single output and print or return a
% >	summary.
% >
% > If the summaries aren't requested as return values, they
% > will be printed on standard output.
% > All parameters except the model are optional. The options provided
% > are not required to be complete, they will be merged with the
% > defaults.
% >
% > @param model the model to get the stats for
% > @param outputIndex index of the output dimension to show
% > @param validation validation function or matrix (may be empty)
% > @param options options for plotting and validation
% > @param parentHandle handles of figure or panels to plot in
% > @retval fighandle handle of the figure were the errors were plotted
% > @retval options plot and validation options, defaults if not specified
% > @retval trainingStats cell array of strings with training stats summary
% > @retval validationStats cell array of strings with validation stats summary
% >
% >Examples:
% > - guiPlotModelErrors(model, 1) * Show errors of a model's first output.
% > - [dummy1 dummy2 trainingStats] = guiPlotModelErrors(model, 2, [], struct, panelHandle)
% > * Show errors of a model's second output in a predefined panel and
% > return the summary instead of printing it. Use the default options.
% > - [dummy options] = guiPlotModelErrors * Get the default options.
%  ======================================================================
%
