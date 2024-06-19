% > @file "genNewOptimizationSeed.m"
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
% >	seed = genNewOptimizationSeed(searchHistory, scores, reliability, numNew, LB, UB )
% 
%  ======================================================================
% > @brief Given a history of previous model parameter optimization runs, a set of scores (lower is better, >= 0)
% > where each score corresponds to each point in the history, and a measure of how reliable each run in
% > the history is (in [0 1]), predict the n most interesting starting points for a new optimization.
% > LB, UB denote the lower and upperbounds of the search space.
%  ======================================================================
%
