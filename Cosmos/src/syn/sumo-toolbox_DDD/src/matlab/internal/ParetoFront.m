% > @file "ParetoFront.m"
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
% >	ParetoFront
% 
%  ======================================================================
% > @brief Updates the cells (integral bounds) for the pareto front.
% >
% > Refactored from PoI and EI to get this code into one place.
% > Could have been a simple function, but has some persistent state in the
% > form of a logger, some profilers and the previous pareto front.
% > @todo ideas to improve the speed: update the front on new samples
% > instead of recalculating everything, do a binarysearch (per dimension,
% > not octree or branch&bound-like), this comes close to classification
% > (finding the boundary where condition test 1 or 0) -> perhaps this can
% > be solved using an SVM classifier ? :-)
% >
% > Construct separately with:
% > pf = ParetoFront(levelsOfImprovement);
%  ======================================================================
%
