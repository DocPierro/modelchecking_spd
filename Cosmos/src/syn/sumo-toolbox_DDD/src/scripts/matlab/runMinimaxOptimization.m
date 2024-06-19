%> @file "runMinimaxOptimization.m"
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
%>	 runMinimaxOptimization()
%
% ======================================================================
%> @brief Minimax optimization using Kriging and expected improvement
%>
%> Based on:
%> Min-max hyperparameter tuning, with application to fault detection
%> Julien Marzat, Hélène Piet-Lahanier and Eric Walter
%>
%> Adapt the CHANGEME lines for your own problem.
% ======================================================================
function runMinimaxOptimization()

%% configuration

% constants
MINX = 1; % outer min (hyperparameters to be tuned)
MAXX = 2; % inner max (environmental parameters)

% simulator
origFunc = @minimaxExample; % CHANGEME: function handle to your matlab function

% bounds and dimensions
bounds = origFunc();
inDim = [size(bounds{MINX}, 2) size(bounds{MAXX}, 2)]; % [MINX MAXX]

% created scaled function
transl = (bounds{MINX}(2,:) + bounds{MINX}(1,:))/2.0;
scale = (bounds{MINX}(2,:) - bounds{MINX}(1,:))/2.0;
[inFunc{MINX}, outFunc{MINX}] = calculateTransformationFunctions( [transl; scale] );

transl = (bounds{MAXX}(2,:) + bounds{MAXX}(1,:))/2.0;
scale = (bounds{MAXX}(2,:) - bounds{MAXX}(1,:))/2.0;
[inFunc{MAXX} outFunc{MAXX}] = calculateTransformationFunctions( [transl; scale] );

func = @(minx, maxx) origFunc( outFunc{MINX}(minx), outFunc{MAXX}(maxx) );

% general options
debug = false;
debugPlot = [];
outputDir = 'minimaxResults/';
mkdir(outputDir);

epsilon = 0.1; % CHANGEME: from paper
nrInitialSamples = [10 5]; % CHANGEME: [MINX MAXX]
nrMaxSamples = [90 20]; % CHANGEME: [MINX MAXX]
%eiMax = [0.1 0.2]; % NOT USED: [MINX MAXX];

% initial designs (fixed throughout)
d = LatinHypercubeDesign( nrInitialSamples(:,MINX), inDim(:,MINX), 1 );
initialSamples{MINX} = d.generate();

d = LatinHypercubeDesign( nrInitialSamples(:,MAXX), inDim(:,MAXX), 1 );
initialSamples{MAXX} = d.generate();

% setup kriging model options
opts = Kriging.getDefaultOptions();
opts.type = 'Kriging';
opts.hpOptimizer = SQPLabOptimizer( inDim(:,MINX), 1 );

theta0{MINX} = zeros(1,inDim(:,MINX));
lb = repmat(-2,1,inDim(:,MINX));
ub = repmat(2,1,inDim(:,MINX));
bf{MINX} = BasisFunction( 'corrmatern32', inDim(:,MINX), lb, ub, {'log'});

theta0{MAXX} = zeros(1,inDim(:,MAXX));
lb = repmat(-2,1,inDim(:,MAXX));
ub = repmat(2,1,inDim(:,MAXX));
bf{MAXX} = BasisFunction( 'corrmatern32', inDim(:,MAXX), lb, ub, {'log'});

% candidateRankers + optimizer to use
rankers = {expectedImprovement(inDim(:,MINX), 1) expectedImprovement(inDim(:,MAXX), 1); ...
    maxvar(inDim(:,MINX), 1) maxvar(inDim(:,MAXX), 1) };

optimopts.GradObj = 'on'; %'off';
optimopts.DerivativeCheck = 'off';
optimopts.Diagnostics = 'off';
optimopts.Algorithm = 'active-set';
%optimopts.MaxFunEvals = 1000000;
%optimopts.MaxIter = 500;

% monte carlo + matlab optimization algorithm for EI
candidatesPerSample = 100;
optimizer = MatlabOptimizer(inDim(:,MINX), 1, optimopts);
%optimizer = DirectOptimizer(inDim(:,MINX), 1);

%% main loop
modelPlot = [];
rankersPlots = [];

plotOpts = sliceContourPlot();
%plotOpts.plotDerivatives = true;

initialValues{MINX} = repmat( -Inf, nrInitialSamples(:,MINX), 1 );
initialValues{MAXX} = [];

%% Step 1: initialize finite set of MAXX
Re = 2.*rand(1, inDim(:,MAXX)) - 1;

nrIter = 50; % CHANGEME
for i=1:nrIter
    
    fprintf('\nGlobal loop iteration %i of %i\n\n', i, nrIter);
    
    %% Step 2: Solve relaxed problem
    fprintf('Step 2:\n');
    
    % update initial design
    tmp = func(initialSamples{MINX}, repmat(Re(end,:), nrInitialSamples(:,MINX), 1));
    initialValues{MINX} = max( [initialValues{MINX} tmp], [], 2 );
    
    % initial kriging model
    samples{MINX} = initialSamples{MINX};
    values{MINX} = initialValues{MINX};
    
    sumomodel = KrigingModel( opts, theta0{MINX}, '', bf{MINX}, 'useLikelihood' );
    
    % EGO loop
    optimizer = optimizer.setDimensions(inDim(:,MINX), 1 );
    optimizer = optimizer.setBounds(-ones(1,inDim(:,MINX)), ones(1,inDim(:,MINX)));
    
    nrIterEGO = nrMaxSamples(:,MINX) - nrInitialSamples(:,MINX);
    for j=1:nrIterEGO
        
        fprintf('\tIterion %i of %i (%i samples).\n', j, nrIterEGO, size(samples{MINX},1) );
        
        % construct kriging model
        sumomodel = sumomodel.constructInModelSpace( samples{MINX}, values{MINX} );
        
        % optimize it
        state.lastModels{1}{1} = sumomodel;
        state.samples = samples{MINX};
        state.values = values{MINX};
        
        for k=1:length(rankers)
            
            rankers{k,MINX} = rankers{k,MINX}.initNewSamples(state);
            
            optimFunc = @(x) rankers{k,MINX}.scoreMinimize(x, state);
            pop = 2.*rand(size(samples{MINX},1).*candidatesPerSample, inDim(:,MINX))-1;
            optimizer = optimizer.setInitialPopulation( pop( 1:optimizer.getPopulationSize(),:) );
            [dummy, xmin, fmin] = optimizer.optimize(optimFunc);
            
            dups = buildDistanceMatrix( xmin, samples{MINX}, 1 );
            index = find(all(dups > 2.*eps, 2));
            xmin = xmin(index,:);
            fmin = fmin(index,:);
            
            if ~isempty( xmin )
                break;
            end
            
        end
        
        if isempty( xmin )
            xmin = 2.*(rand(1,inDim(:,MINX)) - 0.5);
            
            fprintf('\t- No unique samples found, randomizing.\n' );
        else
            fprintf('\t- Samples found using ranker %i.\n', k)
        end
        
        % plot
        if inDim(:,MINX) == 1
            debugPlot = sumomodel.plotModel;
        else
            plotOpts.fixedSamples = xmin;
            plotOpts.state = state;
            debugPlot = sliceContourPlot( debugPlot, rankers{k}, plotOpts );
        end
        
        % evaluate samples on finite set
        tmp = func( repmat(xmin, size(Re,1), 1), Re );
        
        % add to dataset
        samples{MINX} = [samples{MINX}; xmin];
        values{MINX} = [values{MINX}; max(tmp)];
    end
    
    %% Step 3: Solve the maximization problem
    fprintf('Step 3:\n');
    
    % get current best minx
    [dummy index] = min( values{MINX} );
    minxi = samples{MINX}(index,:);
    
    fprintf('\tCurrent minxi (scaled - original):\n');
    minxi
    outFunc{MINX}(minxi)
    if debug
        disp('Press a key to continue...');
        pause;
    end
    
    % update initial design (NOTE: minus before func())
    initialValues{MAXX} = -func( repmat(minxi, nrInitialSamples(:,MAXX), 1), initialSamples{MAXX});
    
    % initial kriging model
    samples{MAXX} = initialSamples{MAXX};
    values{MAXX} = initialValues{MAXX};
    
    sumomodel = KrigingModel( opts, theta0{MAXX}, '', bf{MAXX}, 'useLikelihood' );
    
    % EGO loop
    optimizer = optimizer.setDimensions(inDim(:,MAXX), 1 );
    optimizer = optimizer.setBounds(-ones(1,inDim(:,MAXX)), ones(1,inDim(:,MAXX)));
    
    nrIterEGO = nrMaxSamples(:,MAXX) - nrInitialSamples(:,MAXX);
    for j=1:nrIterEGO
        
        fprintf('\tIterion %i of %i (%i samples).\n', j, nrIterEGO, size(samples{MAXX},1) );
        
        % construct kriging model
        sumomodel = sumomodel.constructInModelSpace( samples{MAXX}, values{MAXX} );
        
        % optimize it
        state.lastModels{1}{1} = sumomodel;
        state.samples = samples{MAXX};
        state.values = values{MAXX};
        
        for k=1:length(rankers)
            
            rankers{k,MAXX} = rankers{k,MAXX}.initNewSamples(state);
            
            optimFunc = @(x) rankers{k,MAXX}.scoreMinimize(x, state);
            
            optimizer = optimizer.setInitialPopulation( 2.*rand(size(samples{MAXX},1).*candidatesPerSample, inDim(:,MAXX))-1 );
            [dummy xmin fmin] = optimizer.optimize(optimFunc);
            
            dups = buildDistanceMatrix( xmin, samples{MAXX}, 1 );
            index = find(all(dups > 2.*eps, 2));
            xmin = xmin(index,:);
            fmin = fmin(index,:);
            
            if ~isempty( xmin )
                break;
            end
            
        end
        
        if isempty( xmin )
            xmin = 2.*(rand(1,inDim(:,MAXX)) - 0.5);
            
            fprintf('\t- No unique samples found, randomizing.\n' );
        else
            fprintf('\t- Samples found using ranker %i.\n', k)
        end
        
        % plot
        if inDim(:,MAXX) == 1
            debugPlot = sumomodel.plotModel;
        else
            plotOpts.fixedSamples = xmin;
            plotOpts.state = state;
            debugPlot = sliceContourPlot( debugPlot, rankers{k}, plotOpts );
        end
        
        % evaluate sample (NOTE: minus)
        tmp = -func( minxi, xmin );
        
        % add to dataset
        samples{MAXX} = [samples{MAXX}; xmin];
        values{MAXX} = [values{MAXX}; tmp];
    end
    
    %% Step 4: Stopping criterion
    [dummy index] = min( values{MAXX} );
    xei1 = samples{MAXX}(index,:); % xe i+1
    
    %{
    TODO: stopping criterion
        candidateSolution = func( minxi, xei1 );

        tmp = func( repmat(minxi, size(Re,1), 1), Re );
        if (candidateSolution - max(tmp)) < epsilon
            break;
        end
    end
    %}
    %else
    Re = [Re; xei1]; % augment finite set
    
    % user feedback
    disp('minxi (original):')
    outFunc{MINX}(minxi)
    
    disp('Re (original):');
    outFunc{MAXX}(Re)
    
    if debug
        disp('Press a key to continue...')
        pause;
    end
    
    %% save data
    %save(fullfile(outputDir, 'samples.mat'), 'samples' );
    %save(fullfile(outputDir, 'values.mat'), 'values' );
    %save(fullfile(outputDir, sprintf('sumo_model%i', sum(nrSamples)-1)), 'k');
end

disp('Finished');

%% final model + samples plot
%sliceContourPlot( modelPlot, k );

    function y = minimaxExample( minx, maxx )
        if nargin == 0
            % Return bounds
            y{1} = [0; 1]; % MINX = a
            y{2} = [0; 1]; % MAXX = x
        else
            n = 10;
            k = 0:n-1;
            
            minx = repmat( minx, 1, n );
            k = repmat( k, size(minx,1), 1 );
            
            p1 = prod( repmat(maxx, 1, n) + minx.^k, 2); % p(x,a)
            p2 = prod(-repmat(maxx, 1, n) + minx.^k, 2); % p(-x,a)
            
            r = maxx .* ((p1-p2) ./ (p1+p2)); % r(x,a)
            
            %y = abs( maxx - r );
            %y = ( maxx - r ).^2; % MSE
            y = log10( abs(maxx - r ) + eps );
        end
    end
end