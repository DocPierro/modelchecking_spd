% > @file "selectTestSet.m"
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
% >	[testSet, lastAdded] = selectTestSet(samples, testSet, n, type, invalidCandidates)
% 
%  ======================================================================
% > @brief Add n test samples from a given sample set to an existing test set
% > (may be empty).
% >
% > testSet is a vector of indices in the array samples
% > resembling the samples that are already in the set.
% > Type is one of 'random' or 'distance/uniform'.
% > invalidCandidates is an optional parameter which contains the indices
% > that resemble invalid candidates for adding in the list. These samples
% > are ignored as possible candidates for adding. Defaults to the empty
% > vector (all samples are allowed as candidates except for the ones
% > already in the test set).
%  ======================================================================
%
