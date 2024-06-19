% > @file "inputParserSUMO.m"
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
% >	inputParserSUMO
% 
%  ======================================================================
% > @brief SUMO version of an inputParser
% >
% > Basic validation and convertion functions:
% > - integer/double: @isscalar - @str2double
% > - vector: @isvector or @(x) any(x)>5 - @str2num
% > - cell: @iscell - @cellstr
% > - string: @ischar - @char
% > - functon_handle: @(f) isa(f, 'function_handle') - @str2func
% > - boolean: @islogical - @inputParserSUMO.str2bool
% > - SUMO class: @(x) inputParserSUMO.isa(x, 'InitialDesign')
% >
% > More rigorous validation functions:
% > @(x) validateattributes(x,{'numeric'},{'nonempty','integer','positive'})
% > @(x) validateattributes(x,{'cell'},{'ndims',2})
% > @(x) validateattributes(x,{'char'},{'nonempty'})
% > @(x) validatestring(x, {'value1' 'value2' 'value3'}, '', 'optionName')
% >
% > Limitations:
% > - default values dependent on other variables (e.g., inDim, outDim)
% > - makes NodeConfig.getOption methods obsolute ?
% > - base classes can not be constructed without some extra logic (ModelBuilder)
% > - DatasetDesign and CombinedSequentialDesign under convertion
%  ======================================================================
%
