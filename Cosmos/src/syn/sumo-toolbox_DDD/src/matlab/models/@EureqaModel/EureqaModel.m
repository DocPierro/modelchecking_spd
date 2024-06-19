% > @file "@EureqaModel/EureqaModel.m"
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
% >	EureqaModel
% 
%  ======================================================================
% > @brief Wrapper for the Eureqa Symbolic Regression Tool. The model
% > returned is a string of the equation with the best fitness value.
% >
% > @note Important for the code to work properly, eureqa_select_SUMO.cpp
% > has to be compiled (in the same way as) instead of eureqa_select.cpp
% >
% > @note Important, right now the code waits 10 (arbitrary) seconds before
% > starting the Eureqa server. In case two or more servers are run
% > concurrently.
% >
% > TODO: find a better than the delay.
% >
% > Reference: http://creativemachines.cornell.edu/eureqa
% > http://www.mathworks.com/matlabcentral/fileexchange/32225-interface-with-eureqa-featuring-symbolic-regression
%  ======================================================================
%
