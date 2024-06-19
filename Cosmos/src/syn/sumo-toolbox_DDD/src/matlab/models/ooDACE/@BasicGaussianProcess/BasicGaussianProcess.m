% > @file "@BasicGaussianProcess/BasicGaussianProcess.m"
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
% >	BasicGaussianProcess
% 
%  ======================================================================
% > @brief A kriging surrogate model (also known as a Gaussian Process)
% >
% > Papers:
% > - "Design and Analysis of Computer Experiments",
% >   J. Sacks, W. Welch, T. Mitchell, H. Wynn,
% >   1989
% > - "Design and Analysis of Simulation Experiments",
% >   J.P.C. Kleijnen,
% >   Springer, 2008
% > - "Engineering Design Via Surrogate Modelling: A Practical Guide",
% >   A. Forrester and A. Sobester and A. Keane,
% >   Wiley, 2008
% > - "Gaussian Processes for Machine Learning",
% >   C. E. Rasmussen and C. K. I. Williams,
% >   MIT Press, 2006
% >
% > @todo Refactor correlation functions into proper basis function class
% > hierarchy.
% > @todo solve the correlation matrix vs covariance matrix issue
%  ======================================================================
%
