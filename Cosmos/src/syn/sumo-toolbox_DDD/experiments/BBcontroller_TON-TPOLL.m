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
nParams=2;
ranges = [1,110;1,20];
paramNames = {'TON','TPOLL'};
safeRegionFile = 'experiments/safe_regions/bbcontroller_ton_tpoll.mat';
experimentName = 'bbcontroller-ton-tpoll';
numInitSamples = 10;
numTotalSamples = 100;


startSumoOptimization(mat2py, py2mat, saferegscript, ...
    nParams, ranges, paramNames, safeRegionFile, experimentName, ...
    numInitSamples, numTotalSamples) 