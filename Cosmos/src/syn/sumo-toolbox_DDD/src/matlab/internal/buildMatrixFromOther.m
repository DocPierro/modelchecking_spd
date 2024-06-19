% > @file "buildMatrixFromOther.m"
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
% >	A = buildMatrixFromOther(varargin)
% 
%  ======================================================================
% > @brief Initializes matrix A with the data in matrix B
% >
% > The dimensions need not match, data in B will
% > be pruned or appended with random values in [min, max]
% > if necessary.
% > The idea is to preserve the information in B as much as possible.
% > Usage: A = buildMatrixFromOther(A,B,[min],[max])
% >	      if neigher min/max are specified no random values are added, the original A values are used
%  ======================================================================
%
