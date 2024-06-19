% > @file "createGridParameters.m"
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
% >	gridParameters = createGridParameters(importanceParameters, points)
% 
%  ======================================================================
% > @brief Creates grid parameters.
% >
% > Solve equation:
% > { ax + by      = 0
% > {      cy + dz = 0
% > {          xyz = e
% > <=>
% > { ax = -by
% > { cy = -dz
% > { xyz = e
% > <=>
% > { x = (-b/a)y
% > { y = (-d/c)z
% > { xyz = e
% > <=>
% > { x/y = -b/a
% > { y/z = -d/c
% > { xyz = e
% > <=>
% > { logx - logy = -log(b/a)
% > { logy - logz = -log(d/c)
% > { logx + logy + logz = loge
% >
% >	Example: createGridParameters([2,5,10], 50)
% > { logx - logy = -log(2/5)
% > { logy - logz = -log(5/10)
% > { logx + logy + logz = log(50)
% > solution: [2,4,8]
%  ======================================================================
%
