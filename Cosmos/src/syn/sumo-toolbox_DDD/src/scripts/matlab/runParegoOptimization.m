%> @file "runParegoOptimization.m"
%> @authors SUMO Lab Team
%> @version 2014a
%> @date Copyright 2006-2014
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
%>	 runParegoOptimization()
%
% ======================================================================
%> @brief The ParEGO optimization method.
%>
%> Adapt the CHANGEME lines for your own problem.
% ======================================================================
function runParegoOptimization()

%% configuration

% simulator
p = 'examples/Math/mosbo/vlmop2/'; % CHANGEME: Path to simulator
addpath(p); % add p to matlab path
func = @vlmop2; % function handle to your matlab function

bounds = [-2 -2 ; 2 2]; % CHANGEME: input bounds of the simulator

transl = (bounds(2,:)+bounds(1,:))/2.0;
scale = (bounds(2,:)-bounds(1,:))/2.0;
[inFunc, outFunc] = calculateTransformationFunctions( [transl; scale] );

% general options
outputDir = 'paregoResults/';
nrMaxSamples = 100; % maximum number of samples
nrInitialSamples = 21; % number of initial samples
nrIter = nrMaxSamples-nrInitialSamples; % number of iterations
randomWeights = true; % CHANGEME: true: random weights, false: linear interpolation

inDim = 2; % CHANGEME: number of input variables

% setup kriging model options
type = 'Kriging';
opts = eval( [type '.getDefaultOptions()'] );
opts.type = type;

theta0 = repmat(0.25,1,inDim);
lb = repmat(-2,1,inDim);
ub = repmat(2,1,inDim);
bf = BasisFunction( 'corrgauss', inDim, lb, ub, {'log'});

optimopts.GradObj = 'on';
optimopts.DerivativeCheck = 'off';
optimopts.Diagnostics = 'off';
optimopts.Algorithm = 'active-set';
%optimopts.MaxFunEvals = 1000000;
%optimopts.MaxIter = 500;
opts.hpOptimizer = MatlabOptimizer( inDim, 1, optimopts );

% generate initial data set
d = LatinHypercubeDesign( nrInitialSamples, inDim, 1 );
samples = d.generate();
values = func( outFunc(samples) );

outDim = size(values,2); % number of cost functions

% candidateRanker + optimizer to use
ranker = expectedImprovement(inDim, 1);
optimizer = DirectOptimizer(inDim, 1);
optimizer = optimizer.setBounds(-ones(1,inDim), ones(1,inDim) );

% create weights
if randomWeights
    % random
    weights = rand(nrIter,outDim);
    weights = weights ./ repmat(sum(weights,2),1,outDim); % normalize
else
    % linear interpolation
    weights = linspace(0,1,nrIter)';
    weights(:,2) = 1-weights(:,1);
end

%% main loop
mkdir(outputDir);
nrSamples = size(samples,1);
for i=1:nrIter
    
    txt = sprintf('Iterion %i of %i (sample %i of %i, %i left).', i, nrIter, nrSamples+1, nrMaxSamples, nrMaxSamples-nrSamples-1);
    disp(txt);
    usingWeight = weights(i,:)
    
    % aggregate data
    
    genValues = sum( usingWeight(ones(nrSamples,1),:) .* values(:,1:outDim), 2 );
    
    % build and fit Kriging object
    k = KrigingModel( opts, theta0, 'regpoly0', bf, 'useLikelihood' );
    k = k.constructInModelSpace( samples, genValues );
    
    % optimize it
    state.lastModels{1}{1} = k;
    state.samples = samples;
    state.values = genValues;
    optimFunc = @(x) ranker.scoreMinimize(x, state);
    [dummy, xmin, fmin] = optimizer.optimize(optimFunc);
    
    % evaluate new samples and add to set
    fmin = func( outFunc(xmin) );
    samples = [samples;xmin(1,:)];
    values = [values;fmin(1,:)];
    nrSamples = size(samples,1);
    
    % save data
    save(fullfile(outputDir, 'samples.txt'), 'samples', '-ascii');
    save(fullfile(outputDir, 'values.txt'), 'values', '-ascii');
    save(fullfile(outputDir, sprintf('sumo_model%i', nrSamples-1)), 'k');
end

disp('Finished');

%% plots
% pareto plot
figure;
plotParetoFront( values );

% input plot
figure;
plot( samples(:,1), samples(:,2), 'rx' );

%% clean up
% remove p from matlab path
rmpath(p);

end