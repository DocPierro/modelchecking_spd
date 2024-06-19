% > @file "buildMinimumDistanceMatrix.m"
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
% >	[minDistances, idx] = buildMinimumDistanceMatrix(samples, targets, distObj)
% 
%  ======================================================================
% > @brief Both `samples' and `targets' give an array of d-dimensional points.
% >
% > If `samples' is N x d and `targets' is M x d, then
% > this function returns an N x 1 matrix, which contains the minimum
% > distance of each sample from all of the targets. This computation is done
% > in a memory-safe manner, in which it is ensured that no memory limitations
% > of Matlab are hit in the process.
% >
% > Example:
% > >> build_distance_matrix( [ 0 0 ; 1 1 ; 2 2 ], [ 1 0 ; 0 1 ] )
% > ans =
% >  1.0000    1.0000
% >  1.0000    1.0000
% >  2.2361    2.2361
%  ======================================================================
%
