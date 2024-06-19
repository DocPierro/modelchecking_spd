%> @file "runCoKrigingOptimization.m"
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
%>	 runCoKrigingOptimization()
%
% ======================================================================
%> @brief Multifidelity optimization (cokriging + EGO)
%>
%> Applies sequential design to the high fidelity data as well as the low fidelity
%> data.
%> Adapt the CHANGEME lines for your own problem.
% ======================================================================
function runCoKrigingOptimization()

close all;

%% configuration

% constants
CHEAP = 1;
EXPENSIVE = 2;

desc{CHEAP} = 'cheap';
desc{EXPENSIVE} = 'expensive';

% simulator
p = 'src/matlab/playground/CoKrigingExamples/'; % CHANGEME: Path to simulator
addpath(p); % add p to matlab path
func{CHEAP} = @cheapMath1D; % function handle to your matlab function
func{EXPENSIVE} = @expensiveMath1D; % function handle to your matlab function

bounds = func{CHEAP}(); % CHANGEME: bounds of input variables
inDim = size(bounds,2); % number of input variables
nrFunc = length(func); % number different fidelity simulators (should be 2)

transl = (bounds(2,:) + bounds(1,:))/2.0;
scale = (bounds(2,:) - bounds(1,:))/2.0;
[inFunc, outFunc] = calculateTransformationFunctions( [transl; scale] );

% general options
debug = true;
outputDir = 'results_egoCoKriging/';
nrMaxSamples = 100; % maximum number of samples (cheap+expensive ones)
nrInitialSamples = [3 2]; % parameter for initial designs [levels nrPoints]

% CHANGEME: how much the cheap simulator is used
% -Inf (always) ... Inf (never)
minimumDeviation = -Inf; % minimum deviation needed before considering cheap function
distanceThreshold = 0.05; % and then only if no other cheap sample lies near

% setup kriging model options
type = 'CoKriging';
opts = eval( [type '.getDefaultOptions()'] );
opts.type = type;

theta0 = repmat(0.25,1,inDim);

lb = repmat(-2,1,inDim);
ub = repmat(2,1,inDim);

% CHANGEME: correlation function to use
bf = BasisFunction( 'corrgauss', inDim, lb, ub, {'log'});
%bf = BasisFunction( 'correxp', inDim, lb, ub, {'log'});
%bf = BasisFunction( 'corrmatern32', inDim, lb, ub, {'log'});

optimopts.DerivativeCheck = 'off';
optimopts.Diagnostics = 'off';
optimopts.Algorithm = 'active-set';
optimopts.MaxFunEvals = 1000000;
optimopts.MaxIter = 500;
optimopts.GradObj = 'on';
opts.hpOptimizer = MatlabOptimizer( inDim, 1, optimopts );

% use gradients for all types of kriging except for cokriging's second sub-GP
optimopts.GradObj = 'off';
opts.hpOptimizer = {opts.hpOptimizer MatlabOptimizer( inDim, 1, optimopts )};

% generate initial data sets
samples = cell(nrFunc,1);
values = cell(nrFunc,1);

d = FactorialDesign( inDim, nrInitialSamples(:,1) );
samples{CHEAP} = d.generate();
values{CHEAP} = func{CHEAP}( outFunc(samples{CHEAP}) );

d = LatinHypercubeDesign( nrInitialSamples(:,2), inDim, 1 );
samples{EXPENSIVE} = d.generate();
values{EXPENSIVE} = func{EXPENSIVE}( outFunc(samples{EXPENSIVE}) );

% select optimizer to use
candidatesPerSample = 50; % monte carlo

%optimopts.GradObj = 'on';
%optimopts.DerivativeCheck = 'off';
%optimopts.Diagnostics = 'off';
%optimopts.Algorithm = 'active-set';
%optimizer = MatlabOptimizer(inDim, 1, optimopts);

optimizer = MatlabPatternSearch(inDim, 1);

%optimopts.ep = 1e-6;
%optimopts.maxevals = 20000;
%optimopts.maxits = 1000;
%optimizer = DirectOptimizer(inDim, 1, optimopts);

optimizer = optimizer.setBounds(-ones(1,inDim), ones(1,inDim));

% candidateRankers to use
rankers = {expectedImprovement(inDim, 1) ...
    maxvar(inDim, 1) };

%rankers = {knowledgeGradient(optimizer, inDim, 1) ...
%            maxvar(inDim, 1) };


%% main loop
nrSamples = cellfun( 'size', samples, 1 );
nrIter = nrMaxSamples - sum(nrSamples, 1); % number of iterations

mkdir(outputDir);

modelPlot = [];
rankersPlots = [];

k = KrigingModel( opts, theta0, '', bf, 'useLikelihood' );

for i=1:nrIter
    
    fprintf('Iterion %i of %i (cheap samples %i : expensive samples %i).\n', i, nrIter, nrSamples(CHEAP), nrSamples(EXPENSIVE) );
    
    % build and fit Kriging object
    k = k.constructInModelSpace( samples, values );
    
    % optimize it
    state.lastModels{1}{1} = k;
    state.samples = samples{EXPENSIVE};
    state.values = values{EXPENSIVE};
    
    for j=1:length(rankers)
        
        rankers{j} = rankers{j}.initNewSamples(state);
        
        %% monte carlo
        nCandidates = nrSamples*candidatesPerSample;
        initialPopulation = rand(nCandidates, inDim) .* 2 - 1;
        
        foundvalues = rankers{j}.score(initialPopulation, state);
        [dummy, idx] = sort( foundvalues, 1, 'descend' );
        
        fprintf('\t- Generated %i candidates.\n', nCandidates );
        
        %% optimize best candidate
        
        % set initial population
        maxPopSize = optimizer.getPopulationSize();
        initialPopulation = initialPopulation(idx(1:maxPopSize,:),:);
        
        fprintf('\t- Optimizing best candidate...\n');
        %initialPopulation
        
        optimizer = optimizer.setInitialPopulation(initialPopulation);
        
        % give the state to the optimizer - might contain useful info such as # samples
        optimizer = optimizer.setState(state);
        
        optimFunc = @(x) rankers{j}.scoreMinimize(x, state);
        [dummy, xmin, fmin] = optimizer.optimize(optimFunc);
        
        dups = buildDistanceMatrix( xmin, samples{EXPENSIVE}, 1 );
        index = find(all(dups > 2.*eps, 2));
        xmin = xmin(index,:);
        fmin = fmin(index,:);
        
        if ~isempty( xmin )
            break;
        end
        
    end
    
    if isempty( xmin )
        xmin = 2.*(rand(1,inDim) - 0.5);
        
        fprintf('\t- No unique samples found, randomizing.\n' );
    else
        fprintf('\t- Samples found using %s ranker.\n', class(rankers{j}));
    end
    
    %% evaluate new samples and add to set
    % which function to evaluate ? depends on uncertainty
    [dummy, s2] = k.evaluateInModelSpace( xmin );
    s = sqrt(abs(s2));
    
    % if uncertainty is high enough consider using the cheap function first
    if s > minimumDeviation
        
        % but only if no other cheap sample lies near
        cheapDist = min( buildDistanceMatrixPoint( samples{CHEAP}, xmin, true) );
        
        if cheapDist < distanceThreshold
            idx = EXPENSIVE;
        else
            idx = CHEAP;
        end
    else
        idx = EXPENSIVE;
    end
    
    fmin = func{idx}( outFunc(xmin) );
    samples{idx} = [samples{idx} ; xmin(1,:)];
    values{idx} = [values{idx} ; fmin(1,:)];
    
    nrSamples = cellfun( 'size', samples, 1 );
    
    fprintf('\t- Using %s function.\n', desc{idx} );
    
    %% nice plotjes
    if debug
        % sumo model
        plotOpts = struct('state', state, 'fixedSamples', xmin); %, 'plotDerivatives', true);
        modelPlot = sliceContourPlot( modelPlot, k, plotOpts );
        rankersPlots = sliceContourPlot( rankersPlots, rankers{j}, plotOpts );
        pause(1); % wait 1 second
    end
    
    samplesSimulator{CHEAP} = outFunc(samples{CHEAP});
    samplesSimulator{EXPENSIVE} = outFunc(samples{EXPENSIVE});
    
    fprintf('\t- Best optimum so far:\n' );
    [fmin, idx] = min( values{EXPENSIVE} );
    xmin = samplesSimulator{EXPENSIVE}(idx,:)
    fmin
    
    %% save data
    save(fullfile(outputDir, 'samplesSimulator.mat'), 'samplesSimulator' );
    save(fullfile(outputDir, 'samples.mat'), 'samples' );
    save(fullfile(outputDir, 'values.mat'), 'values' );
    save(fullfile(outputDir, sprintf('sumo_model%i', sum(nrSamples)-1)), 'k');
end

disp('Finished');

%% final model + samples plot
sliceContourPlot( modelPlot, k );

%% clean up
% remove p from matlab path
rmpath(p);

end

function values = cheapMath1D(samples)
if nargin <= 0
    values = [0; 1]; % bounds
    return;
end

A = 0.5; B = 10; C = -5;

% cheap
values2 = (6.*samples(:,1) - 2).^2 .* sin(12.*samples(:,1)-4);
values = A.*values2 + B.*(samples(:,1)-0.5) + C;
end

function values = expensiveMath1D(samples)
if nargin <= 0
    values = [0; 1]; % bounds
    return;
end

% expensive
values = (6.*samples(:,1) - 2).^2 .* sin(12.*samples(:,1)-4);
end