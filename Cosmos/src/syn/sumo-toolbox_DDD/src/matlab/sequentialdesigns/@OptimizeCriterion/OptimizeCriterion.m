% > @file "@OptimizeCriterion/OptimizeCriterion.m"
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
% >	OptimizeCriterion
% 
%  ======================================================================
% > @brief This sample selector selects one or more samples that optimizes a certain candidateRanker
% >
% > A candidateRanker is also known as a sample infill criterion, figure of merit, (utility) function, etc.
% > Any CandidateRanker can be used as a criterion. If more than one CandidateRanker is defined they will
% > be used as fallback criterions
%  ======================================================================
%
