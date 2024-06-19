% > @file "@Model/evaluateDerivativeInModelSpace.m"
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
% >	[dvalues dsigma2] = evaluateDerivativeInModelSpace( s, points, outputIndex )
% 
%  ======================================================================
% > Approximate the derivative at the given points in model space. It will return the gradient in each dimension.
% > This is just a very simple implementation based on a simple symmetric approximation.
% > Note that models can override this with more efficient implementations. For example Rational/Polynomial models
% > would be able to provide exact derivatives.  The same goes for Kriging models.
% >
% > For a good tutorial/intro on gradients see: http://www-math.mit.edu/18.013A/HTML/chapter07/contents.html
%  ======================================================================
%
