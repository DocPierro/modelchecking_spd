% > @file "defaultInputSettings.m"
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
% >	settings = defaultInputSettings(model)
% 
%  ======================================================================
% > @brief Get the default bounds and values in each input dimension, and
% >	default indices for 3 axes. All indices will be in range of the
% >	model's inputs.
% >
% >	Example:
% >	% model has 5 inputs
% >	settings = defaultInputSettings(model)
% >	settings =
% >	 xIndex: 1
% >	 yIndex: 2
% >	 zIndex: 3
% >	 bounds: [5x2 double]
% >	 values: [5x1 double]
%  ======================================================================
%
