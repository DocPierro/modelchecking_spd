% > @file "lrm.m"
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
% >	lrm
% 
%  ======================================================================
% > @brief The LRM candidate ranker.
% >
% > Compares the current best model with a
% > linear interpolation (based on a Delaunay triangulation). The
% > candidates samples are ranked based on the deviation of the model and
% > the interpolation at their location. If there are several outputs,
% > the average deviation of these outputs is taken.
% > OPTIONS:
% > 1) perpendicularToAxis: if true, the deviation between the two
% >    models will be calculated as simply the difference between the
% >    predicted values; else the deviation is taken as point-plane distance
% >    between the candidate sample and the hyperplane corresponding to the
% >    tesselation of the candidate sample
% > 2) power: by default = 1, takes the deviation to this power as the LRM
% >    score
% > 3) powerPointPlane: default = 1, the power to which the point-plane
% >    distance is raised
% > 4) angleThreshold: default = 1.54, is a threshold for the angle between
% >    input space and the hyperplane of a tesselation. In some cases the
% >    hyperplanes of the tesselation can be (near) vertical although the
% >    problem itself show little dynamics (see Bird example). Setting this
% >    threshold reduces spurious samples being selected in such cases.
% >    Setting the threshold to pi/2 will allow any angle.
%  ======================================================================
%
