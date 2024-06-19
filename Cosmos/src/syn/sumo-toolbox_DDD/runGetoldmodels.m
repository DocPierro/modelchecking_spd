function runGetoldmodels()

%% configuration

% simulator
%p = 'examples/Math/mosbo/vlmop2/'; % CHANGEME: Path to simulator
%addpath(p); % add p to matlab path
%func = @vlmop2; % function handle to your matlab function

bounds = [-5 0 ; 10 15]; % CHANGEME!: input bounds of the simulator (left is lower bounds right is upper bound, so it should be [0.01 0.01 ; 2.47 2.47]   )
               
transl = (bounds(2,:)+bounds(1,:))/2.0;
scale = (bounds(2,:)-bounds(1,:))/2.0;
[inFunc, outFunc] = calculateTransformationFunctions( [transl; scale] );

% general options
outputDir = 'oldmodels/';
nrMaxSamples = 30; % maximum number of samples % CHANGEME! (it is number of samples in the samples.txt file used)
nrInitialSamples = 14; % number of initial samples % CHANGEME! (dimension of the LHS used in the fitting)
nrIter = nrMaxSamples-nrInitialSamples; % number of iterations


inDim = 2; %  number of input variables

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
%d = LatinHypercubeDesign( nrInitialSamples, inDim, 1 );
TOT_FILE = load('samples.txt');  % CHANGEME! file containing the samples used in the COMPLETE model (it is assumed to be formatted as the default output of the fitting)
samples=inFunc(TOT_FILE(1:nrInitialSamples,1:inDim));
%values = load('values.txt');
values=TOT_FILE(1:nrInitialSamples,inDim+1);
outDim = 1;

% candidateRanker + optimizer to use
%ranker = expectedImprovement(inDim, 1);
%optimizer = DirectOptimizer(inDim, 1);
%optimizer = optimizer.setBounds(-ones(1,inDim), ones(1,inDim) );

% create weights
%if randomWeights
    % random
 %   weights = rand(nrIter,outDim);
 %   weights = weights ./ repmat(sum(weights,2),1,outDim); % normalize
%else
    % linear interpolation
 %   weights = linspace(0,1,nrIter)';
 %   weights(:,2) = 1-weights(:,1);
%end

%% main loop
mkdir(outputDir);
nrSamples = size(samples,1);
for i=1:(nrIter)
    
    txt = sprintf('Iteration %i of %i (sample %i of %i, %i left).', i, nrIter, nrSamples+1, nrMaxSamples, nrMaxSamples-nrSamples-1);
    disp(txt);
   % usingWeight = [0.1 0.1]
    
    % aggregate data
    
    %genValues = sum( usingWeight(ones(nrSamples,1),:) .* values(:,1:outDim), 2 );
    
    % build and fit Kriging object
    k = KrigingModel( opts, theta0, 'regpoly0', bf, 'useLikelihood' );
    k = k.constructInModelSpace( samples, values );
    basicPlotModel(k);
    % optimize it
    state.lastModels{1}{1} = k;
    state.samples = samples;
    state.values = values;
   % optimFunc = @(x) ranker.scoreMinimize(x, state);
   % [dummy, xmin, fmin] = optimizer.optimize(optimFunc);
    
    % evaluate new samples and add to set
   % fmin = func( outFunc(xmin) );
   %samples=load('samples.txt');
    samples = inFunc(TOT_FILE(1:(nrInitialSamples+i), 1:2));
    values=TOT_FILE(1:(nrInitialSamples+i),3);
    nrSamples = size(samples,1);
    
    % save data
    %save(fullfile(outputDir, 'samples.txt'), 'samples', '-ascii');
    %save(fullfile(outputDir, 'values.txt'), 'values', '-ascii');
    save(fullfile(outputDir, sprintf('sumo_model%i', nrSamples-1)), 'k');
end

disp('Finished');

%% plots
%% pareto plot
%figure;
%plotParetoFront( values );

% input plot
%figure;
%plot( samples(:,1), samples(:,2), 'rx' );

%% clean up
% remove p from matlab path
%rmpath(p);

end
