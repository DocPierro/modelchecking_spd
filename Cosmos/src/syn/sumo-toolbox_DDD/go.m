%> @file "go.m"
%> @authors: SUMO Lab Team
%> @version 7.0.3 ($Revision$)
%> @date $LastChangedDate$
%> @date Copyright 2006-2010
%>
%> This file is part of the Surrogate Modeling Toolbox ("SUMO Toolbox")
%> and you can redistribute it and/or modify it under the terms of the
%> GNU Affero General Public License version 3 as published by the
%> Free Software Foundation.  With the additional provision that a commercial
%> license must be purchased if the SUMO Toolbox is used, modified, or extended
%> in a commercial setting. For details see the included LICENSE.txt file.
%> When referring to the SUMO Toolbox please make reference to the corresponding
%> publication:
%>   - A Surrogate Modeling and Adaptive Sampling Toolbox for Computer Based Design
%>   D. Gorissen, K. Crombecq, I. Couckuyt, T. Dhaene, P. Demeester,
%>   Journal of Machine Learning Research,
%>   Vol. 11, pp. 2051-2055, July 2010. 
%>
%> Contact : sumo@sumo.intec.ugent.be - http://sumo.intec.ugent.be
%> Signature
%>	[surrogateModels, outputDirs, errorInfo] = go(varargin)
%
% ======================================================================
%> @brief Main entry point for the SUMO Toolbox.
%>
%> Example:
%> Usages:
%> - "go"
%> - "go('MyConfigFile.xml')"
%> - "go('MyConfigFile.xml', runFilter)"
%> - "go('MyConfigFile.xml',xValues, yValues)"
%> - "go('MyConfigFile.xml',xValues, yValues, options)"
%> - "go('MyConfigFile.xml',xValues, yValues, options, runFilter)"
%> - "[surrogateModels outputDirs] = go(...)"
%>
%> @param options a cell array containing one or more of:
%>   "-merge" : merge MyConfigFile.xml with the default configuration
%> @param runFilter a number or vector (with range [1 numRuns]) that specifies which runs to execute
%> @retval surrogateModels nested cell array representing the different runs best surrogate models
%> @retval outputDirs cell array of the output directories of each run (string)
%>
%> NB: the default configuration file is /path/to/SUMO/config/default.xml
% ======================================================================
function [surrogateModels, outputDirs, errorInfo] = go(varargin)

%dbstop if error % excluding try/catch blocks
%dbstop if caught % only for try/catch blocks (dont use)

% No point in running the toolbox if there is no JVM
if ~usejava('jvm')
	error('The SUMO Toolbox requires Java to run, normally this is enabled by default.  Check the FAQ for more information');
end

% Check if the toolbox path has been set
res = exist('iminds.sumo.config.ConfigUtil','class');
if(res == 0)
    disp('It seems the SUMO-Toolbox path has not yet been setup, running startup now..')
    startup
end

% csumo root is different (MCR)
if isdeployed()
    SUMORoot = pwd;
else % default
    % get location of this file
    p = mfilename('fullpath');
    % get the toolbox root directory
    SUMORoot = p(1:end-2);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Is the toolbox activated?
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%{
% code not needed for open-source edition.
actFile = fullfile(SUMORoot,'bin','java','iminds','sumo','config','ContextConfig.class');
activated = fopen(actFile,'r');

if(activated < 0)
	s = sprintf('---------------------------------------------------------\n');
	s = sprintf('%s Your SUMO Toolbox installation has not been activated yet\n',s);
	s = sprintf('%s Please contact sumo@intec.ugent.be for further information\n',s);
	s = sprintf('%s---------------------------------------------------------\n',s);

	error(s);
else
  fclose(activated);
end
%}

switch nargin
	case 0
		configFile = fullfile(SUMORoot,'config','default.xml');
		samples = [];
		values = [];
		options = {};
		runFilter = [];
	case 1
		configFile = varargin{1};
		samples = [];
		values = [];
		options = {};
		runFilter = [];
	case 2
		configFile = varargin{1};
		samples = [];
		values = [];
		options = {};
		runFilter = varargin{2};
	case 3
		configFile = varargin{1};
		samples = varargin{2};
		values = varargin{3};
		options = {};
		runFilter = [];
	case 4
		configFile = varargin{1};
		samples = varargin{2};
		values = varargin{3};
		options = varargin{4};
		runFilter = [];
	case 5
		configFile = varargin{1};
		samples = varargin{2};
		values = varargin{3};
		options = varargin{4};
		runFilter = varargin{5};
	otherwise
		error('Invalid arguments given, type "help go" for usage instructions');
	end

[surrogateModels, outputDirs, errorInfo] = SUMODriver(configFile,samples,values,options,runFilter);
