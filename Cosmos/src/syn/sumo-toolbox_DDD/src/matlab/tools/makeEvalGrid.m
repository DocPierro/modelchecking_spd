% > @file "makeEvalGrid.m"
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
% >	evalgrid = makeEvalGrid( gridpoints, gridsize )
% 
%  ======================================================================
% > @brief Low-level procedure, makes a `prod(gridsize)' by `length(gridsize)'
% >	array, where each row is a different vector, where the i'th
% >	element is a number out of `gridpoints{i}'.
% >
% >	The `gridsize' parameter may be omitted, in that case it's generated
% >	from `gridpoints'.
% >	This function produces grids in the expected order, which is now also
% >	used for gridded dataset files. Use this instead of
% >	makeEvalGridInverted if possible.
% >
% >	Example:
% >	makeEvalGrid( { [-1, .5], [-1 0 1], [.2 .3] }, [2 3 2] )
% >	ans =
% >	-1.0000    -1.0000     0.2000
% >	-1.0000    -1.0000     0.3000
% >	-1.0000     0.0000     0.2000
% >	-1.0000     0.0000     0.3000
% >	-1.0000     1.0000     0.2000
% >	-1.0000     1.0000     0.3000
% >	0.5000    -1.0000     0.2000
% >	0.5000    -1.0000     0.3000
% >	0.5000     0.0000     0.2000
% >	0.5000     0.0000     0.3000
% >	0.5000     1.0000     0.2000
% >	0.5000     1.0000     0.3000
%  ======================================================================
%
