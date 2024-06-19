% > @file "@ValidationSet/ValidationSet.m"
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
% >	ValidationSet
% 
%  ======================================================================
% > @brief This measure uses a set of validation samples, not used for construction of
% >	the model, to estimate the accuracy of the model.
% >
% > There are two sources for this set of validation samples:
% >
% > - They are taken as a subset of the list of evaluated samples. If this
% >  option is chosen, the samples are chosen in such a way that the entire
% >  domain is covered as optimally as possible (using a distance metric).
% > - They are loaded from a file. This file is provided in the same way
% >  as a dataset file for the sample evaluator is.
% >
% > This measure is very useful for validation purposes.
%  ======================================================================
%
