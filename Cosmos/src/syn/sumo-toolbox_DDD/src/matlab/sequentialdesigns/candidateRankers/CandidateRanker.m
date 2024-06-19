% > @file "CandidateRanker.m"
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
% >	CandidateRanker
% 
%  ======================================================================
% > @brief An interface that allows the object to score a set of candidates
% >	according to its own system.
% >
% > This is used in the
% > PipelineSequentialDesign to score a set of externally generated
% > candidate samples according to some criterion.
% > Note: when a candidate ranker gives a HIGHER score to a candidate, the
% > sample selector will prefer this candidate above one with a LOWER
% > score.
% > @todo dscores (derivatives) are not scaled in CandidateRanker.score
%  ======================================================================
%
