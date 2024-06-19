%> @file "startup.m"
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
%>	 startup()
%
% ======================================================================
%> @brief Sets up the toolbox path
% ======================================================================
function startup

% Detect the matlab version
checkVersion();

warning off;

% configure the toolbox (set the paths)
if isdeployed()
    % csumo root is different (MCR)
    SUMORoot = pwd;
    
    % matlab addpath not needed for MCR (everything is in CTF archive)
    % ctfroot: temporary directory where archive is extracted
else % default
    % get location of this file (toolbox root path)
    p = mfilename('fullpath');
    SUMORoot = p(1:end-7);
    
    % add matlab class paths
    addpath(SUMORoot);
    addpath(genpath(fullfile(SUMORoot,'src','matlab','')));
    addpath(genpath(fullfile(SUMORoot,'src','scripts','matlab','')));
end

% add java paths
if exist(fullfile(SUMORoot,'bin','java','iminds','sumo','config','ContextConfig.class'), 'file')
    javaaddpath(fullfile(SUMORoot, 'bin','java'));
else
    javaaddpath(fullfile(SUMORoot,'lib','SUMO-Toolbox.jar'));
end
javaaddpath(fullfile(SUMORoot,'lib','jmf.jar'));
javaaddpath(fullfile(SUMORoot,'lib','dom4j-1.6.1.jar'));
javaaddpath(fullfile(SUMORoot,'lib','jaxen-1.1.1.jar'));
javaaddpath(fullfile(SUMORoot,'lib','kd.jar'));
javaaddpath(fullfile(SUMORoot,'lib','trilead-ssh2-build213.jar'));
javaaddpath(fullfile(SUMORoot,'lib','jfreechart-1.0.11.jar'));
javaaddpath(fullfile(SUMORoot,'lib','jcommon-1.0.14.jar'));
javaaddpath(fullfile(SUMORoot,'lib','swing-layout-1.0.3.jar'));

% vector graphics library
javaaddpath(fullfile(SUMORoot,'lib','vectorgraphics-2.1.1','freehep-graphics2d-2.1.1.jar'));
javaaddpath(fullfile(SUMORoot,'lib','vectorgraphics-2.1.1','freehep-graphicsio-2.1.1.jar'));
javaaddpath(fullfile(SUMORoot,'lib','vectorgraphics-2.1.1','freehep-graphicsio-ps-2.1.1.jar'));
javaaddpath(fullfile(SUMORoot,'lib','vectorgraphics-2.1.1','freehep-graphicsio-pdf-2.1.1.jar'));
javaaddpath(fullfile(SUMORoot,'lib','vectorgraphics-2.1.1','freehep-util-2.0.2.jar'));
javaaddpath(fullfile(SUMORoot,'lib','vectorgraphics-2.1.1','freehep-io-2.0.2.jar'));
javaaddpath(fullfile(SUMORoot,'lib','vectorgraphics-2.1.1','freehep-graphicsio-svg-2.1.1.jar'));
javaaddpath(fullfile(SUMORoot,'lib','vectorgraphics-2.1.1','freehep-xml-2.1.1.jar'));

% set up weka
java.lang.System.setProperty('WEKA_HOME', fullfile(SUMORoot, 'lib', 'wekafiles'));
javaaddpath(fullfile(SUMORoot, 'lib','weka.jar'));
weka.core.WekaPackageManager.loadPackages(false);

% workaround for jpegimages2movie
try
    com.sun.media.util.Registry.set('secure.allowSaveFileFromApplets', true);
catch ME
    % happens if jmf is not present, warn and then ignore
    disp('WARNING: jmf is not present (you will not be able to create movies.');
    disp(ME.getReport());
end

% check for some matlab toolbox'es and issue warnings
if ~checkLicense('Optimization Toolbox')
    disp('WARNING: the MATLAB optimization toolbox is not available. It is highly recommended to use the SUMO toolbox.')
    disp('WARNING: for instance, the default configuration (config/default.xml) will NOT work without it.')
end

if ~checkLicense('Global Optimization Toolbox')
    disp('WARNING: the MATLAB global optimization toolbox is not available. It is highly recommended to use the SUMO toolbox.')
    disp('WARNING: components of SUMO that use a genetic algorithm, pattern search, etc. will NOT work without it.')
end

if ~checkLicense('Fuzzy Logic Toolbox')
    disp('WARNING: the MATLAB Fuzzy Logic toolbox is not available. You will not be able to use the FLOLA-Voronoi sequential design.')
end

disp('* The SUMO-Toolbox path has been setup...');
disp('* To get started see http://www.sumowiki.intec.ugent.be/index.php/Running');

    function checkVersion()
        minVersion = '7.7';
        maxVersion = '8.3';
        
        if(verLessThan('matlab', minVersion))
            disp(['WARNING: the SUMO Toolbox needs at least MATLAB version ' minVersion ' or higher, which you don''t appear to have ! Depending on what you need it may, or may not work...']);
        elseif(~verLessThan('matlab', maxVersion))
            disp(['WARNING: the SUMO Toolbox has not been tested fully with MATLAB version ' maxVersion ' or higher! Depending on what you need it may, or may not work...']);
        % else version is ok
        end
    end
end