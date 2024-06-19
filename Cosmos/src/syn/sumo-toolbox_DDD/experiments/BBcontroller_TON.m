sumoPath='~/Projects/Cosmos/src/syn/sumo-toolbox_DDD/';
pyPath='~/Projects/pacemaker/PaceMakerModel/';
%sumoPath='~/work/gitFolders/Cosmos/src/syn/sumo-toolbox_DDD/';
%pyPath='~/work/gitFolders/pacemaker/PaceMakerModel/';

cd(sumoPath);
mat2py = strcat(pyPath,'mat2py.dat');
py2mat = strcat(pyPath,'py2mat.dat');

% default
saferegscript = 'saferegion';
% PM
% saferegscript = 'safeRegPM';
nParams=1;
ranges = [1,103];
paramNames = {'TON'};
safeRegionFile = 'experiments/safe_regions/bbcontroller_ton.mat';
experimentName = 'bbcontroller-ton';
numInitSamples = 10;
numTotalSamples = 100;


startSumoOptimization(mat2py, py2mat, saferegscript, ...
    nParams, ranges, paramNames, safeRegionFile, experimentName, ...
    numInitSamples, numTotalSamples) 