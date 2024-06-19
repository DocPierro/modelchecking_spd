% > @file "quickPlotModel.m"
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
% >	[minOut maxOut options] = quickPlotModel(model, outputIndex, inputs, options, fighandles)
% 
%  ======================================================================
% > @brief Plot 1 or 2 inputs of a model against one of it's outputs.
% >
% > Customize the plot through the options parameter.
% > If options.showAllOutputs is true, you must pass as many figure
% > handles as there are outputs. Otherwise you must pass 1
% > handle or no handle (to use the current figure).
% >
% > @param model the model to plot
% > @param outputIndex index of the output dimension to plot
% > @param inputs axis indices, bounds and values for each input
% > @param options plot customization options (must be complete!)
% > @param fighandles figure handles for plotting in
% >
% > Examples:
% > - [dummy1 dummy2 options] = quickPlotModel * Get the default options
% > - quickPlotModel(m,1) * Simple plot
%  ======================================================================
%
