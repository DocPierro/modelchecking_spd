% > @file "corrmatern52.m"
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
% >	[corr, dx, dhp, rho] = corrmatern52(hp, d)
% 
%  ======================================================================
% > @brief Class of Matern covariance functions (nu = {1/2 3/2 5/2}
% >
% > Implementation based on the book of Rasmussen et al.
% > - "Gaussian Processes for Machine Learning" (Chapter 4),
% >   C. E. Rasmussen and C. K. I. Williams,
% >   MIT Press, 2006
% >
% > The correlation function is:
% >   corr = f( sqrt(nud)*dist2 ) * exp(-sqrt(nud)*dist2)
% >   with nud = 2.*nu
%  ======================================================================
%
