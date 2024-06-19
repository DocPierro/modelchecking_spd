% > @file "DelaunayCandidateGenerator.m"
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
% >	DelaunayCandidateGenerator
% 
%  ======================================================================
% > @brief Generates samples based on a Delaunay triangulation
% >
% > Generates a set of candidate samples at the centroids of all the
% > simplices of the delaunay triangulation of the samples.
% > Also adds the mapping of candidates to simplices to the state.
% > In addition halfway points (midpoints) and random points for each
% > simplex can be generated as well. Random points are always generated
% > together with midpoints.
%  ======================================================================
%
