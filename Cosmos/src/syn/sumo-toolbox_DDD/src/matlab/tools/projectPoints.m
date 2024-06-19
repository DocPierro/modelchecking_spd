% > @file "projectPoints.m"
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
% >	[samples values sizes] = projectPoints(samples, values, bounds, fixedSample, plotIdx, opts)
% 
%  ======================================================================
% > @brief Filter the points by clipping along x and (optionally) y axis, and by
% > comparing their value in the unplotted dimensions to the current value
% > in those dimensions.
% >
% > @param samples points matrix
% > @param values  corresponding output values matrix
% > @param bounds input domain bounds
% > @param fixedSample the sample to which you want to project to
% > @param plotIdx vector of the plot indices
% > @param opts struct of options (optional)
% > @retval samples trimmed down samples
% > @retval values trimmed down values
% > @retval sizes vector of sizes for each sample (used for plotting)
% >
% > The deviation percentage corresponds to the current
% > interval size in each unplotted dimension.
%  ======================================================================
%
