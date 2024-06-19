% > @file "@BasisFunction/BasisFunction.m"
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
% >	BasisFunction
% 
%  ======================================================================
% > @brief This class represents a basis function for use in an approximation method
% >
% >	This could be a gaussian function for in an RBF model or a polynomial kernel function for in an SVM model, etc.
% >	Each basis function has a name (= name of the matlab script that implements it),
% >	parameter bounds, and a scale for each parameters (log/linear). The config comes from a <BasisFunction> tag
% >
% >	Example:
% >	   - a gaussian with one non-isotropic parameter (parameter gets duplicated dim times)
% >	        <BasisFunction name="gaussian">
% >	            <Parameter name="p" min="-5" max="3" scale="log" duplicate="off" />
% >	        </BasisFunction>
% >	   - a gaussian with two isotropic parameters
% >	        <BasisFunction name="gaussian">
% >	            <Parameter name="p1" min="-5" max="3" scale="log" duplicate="on" />
% >	            <Parameter name="p2" min="-5" max="3" scale="log" duplicate="on" />
% >	        </BasisFunction>
% >	   - a sigmoid with mixed parameters
% >	       <BasisFunction name="sigmoid"/>
% >	            <Parameter name="p1" min="-5" max="3" scale="log" duplicate="false" />
% >	            <Parameter name="p2" min="0.01" max="5" scale="lin" duplicate="true" />
% >	        </BasisFunction>
% >
%  ======================================================================
%
