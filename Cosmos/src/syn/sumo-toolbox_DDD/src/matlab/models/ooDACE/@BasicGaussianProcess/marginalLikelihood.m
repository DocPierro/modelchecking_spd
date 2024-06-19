% > @file "@BasicGaussianProcess/marginalLikelihood.m"
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
% >	[out, dout] = marginalLikelihood( this, dpsi, dsigma2 )
% 
%  ======================================================================
% > Used for Maximum Likelihood Estimation (MLE)
% >
% > Papers:
% > - "Gaussian Processes for Machine Learning" (Chapter 5),
% >   C. E. Rasmussen and C. K. I. Williams,
% >   MIT Press, 2006
% > - "An adjoint for likelihood maximization"
% >   D.J.J. Toal, A.I.J. Forrester, N.W. Bressloff, A.J. Keane, C. Holden,
% >   Proc. of the Royal Society, 2009
%  ======================================================================
%
