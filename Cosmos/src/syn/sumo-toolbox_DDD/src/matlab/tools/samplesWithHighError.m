% > @file "samplesWithHighError.m"
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
% >	[points items] = samplesWithHighError(model, relative, n, varargin)
% 
%  ======================================================================
% > @brief Given a model, returns for each output the n samples with the highest error
% >
% >[points items] = samplesWithHighError(model, relative, n, [samples], [values] )
% >
% > model: the model to evaluate
% > relative: true -> relative error , false -> absolute error
% > n: the number of points to return
% > samples, values : use these samples/values instead of the model ones
% >
% > points: a cell array with one entry for each output.  Each entry contains a matrix with
% > 	        the following columns: sample points, model prediction, true values, error
% >
% > items: the same information as above only now in struct form for
% > convenience
%  ======================================================================
%
