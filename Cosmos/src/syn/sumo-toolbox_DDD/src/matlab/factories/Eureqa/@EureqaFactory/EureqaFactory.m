% > @file "@EureqaFactory/EureqaFactory.m"
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
% >	EureqaFactory
% 
%  ======================================================================
% > @brief ModelFactory for the Eureqa Symbolic Regression Tool.
% >
% > @note For eureqa to work properly, the eureqa server executable must be
% > placed in the same directory as EureqaFactory.m. Also, an executable of
% > eureqa_select must be in the SUMO path. Since Eureqa performs a GP
% > search, no additional model builder needed.
% >
% > @note The Matlab interface to Eureqa and the Eureqa server do not
% > support all the functionalities of the "normal" version of Eureqa.
% > * Eureqa server: does not support the special differential building
% > blocks. To have the server look for this sort of solutions manually
% > calculate the derivatives and add them as a variable.
% > * Matlab interface: currently does not support all building blocks, only
% > constant, +, -, *, /, ^, exp, log, sin, cos, abs, tan are supported. ->
% > TODO.
% >
% > @note IMPORTANT! When constructing and evaluating the model via SUMO,
% > the data will be scaled from [-1 1] to their original bounds!
% >
% > @param fitness Fitness is an integer which tells the eureqa code what
% > measure to use when evaluating models. This is hardcoded and therefore
% > SUMO error function cannot be used. The list with available integers
% > and their corresponding measure can found here:
% > http://code.google.com/p/eureqa-api/wiki/doc_fitness_types.
% >
% > @param operators a string of mathematical operators which should be
% > included in the GP search. A comprehensive list of allowed operators can
% > be found in eureqa_select_SUMO.cpp
% >
% > website eureqa:
% > http://creativemachines.cornell.edu/eureqa
% > website eureqa_select:
% > http://www.mathworks.com/matlabcentral/fileexchange/32225-interface-with-eureqa-featuring-symbolic-regression
% >
% > TODO: input checks; support for more building blocks
%  ======================================================================
%
