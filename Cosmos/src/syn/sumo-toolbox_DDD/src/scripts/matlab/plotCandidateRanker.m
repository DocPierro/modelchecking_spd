function plotHandles = plotCandidateRanker(models, rankers)

    function y = func(x)
        angle = pi ./ 5;
        x1 = x(:,1).*cos(angle) - x(:,2).*sin(angle);
        y = x1.^2;
    end

    density = 500;
    
    if ~exist('models', 'var' )
        inDim = 2;
        inNames = {'x' 'y'};
        LB = [-1 -1];
        UB = [1 1];

        lhd = LatinHypercubeDesign(inDim, 25);
        samples = lhd.generate();
        values = func(samples);

        % kriging
        % Set initial value, upper and lower bounds of hyperparameters

        hp0 = ones(1,inDim);
        lb = 0.01;
        ub = 4;
        bf = BasisFunction( 'corrgauss', inDim, lb, ub, {'lin'});
        opts = Kriging.getDefaultOptions();
        opts.hpOptimizer = SQPLabOptimizer(inDim,1);
        opts.hpBounds = log10( repmat( [lb ; ub], 1, inDim ) );
        opts.type = 'Kriging';
        %opts.debug = true;

        models = KrigingModel(opts, hp0, 'regpoly0', bf );
        models = models.construct( samples, values );
    else
        samples = models.getSamples();
        values = models.getValues();
        
        inNames = models.getInputNames();
        [LB UB] = models.getBounds()
        
        [inFunc outFunc] = models.getTransformationFunctions();
        
        inDim = size(samples,2);
    end
    
    % plots
    x = cell(inDim,1);
    for i=1:inDim
        x{i} = linspace( LB(:,i), UB(:,i), density );
    end
    
    gridx = makeEvalGrid( x );
    gridy = models.evaluate( gridx );
    %func( gridx );
    
    deltas = [0 0; 5e-6 5e-6; 10e-6 10e-6; 20e-6 20e-6; 40e-6 40e-6; 80e-6 80e-6; 160e-6 160e-6];
    
    if ~exist( 'rankers', 'var' )
        rankers = ...
            { ...
             %probabilityImprovement_inputUncertainty(inDim, 'none', [-Inf 0.02], false, false, delta) ...
             %probabilityImprovement(inDim, 'none', 0.02, false) ...
             gProbabilityImprovement(inDim, 'none', [0.5 Inf], false, true) ...
             %gProbabilityImprovement(inDim, 'none', [15 25], false, false, false) ...
             %expectedImprovement(inDim, 'none', 0.02) ...
             %expectedImprovement(inDim, 'none', []) ...
             %expectedImprovement(inDim, 'none', [], true) ...
             %expectedDistance(inDim, 'none', 25) ...
             %maxvar(inDim, 'none') ...
             %maxmodel(inDim, 'none') ...
             %minmodel(inDim, 'none') ...
             maximinDistance(inDim, 'none', true, true) ...
             %aggregateObjective(inDim, 'none', ranker, ranker2 ) ...
            };
        %ranker2 = maximinDistance(inDim, 'none', true, true);
    end
        
    plotHandles = zeros(1,length(rankers));
    for i=1:length(rankers);
        ranker = rankers{i};
        
        delta = deltas(i,:);
       
        scale = (UB-LB)/2.0;
        scale = repmat( scale, size(delta,1), 1);
        delta = delta ./ scale;

        state.lastModels{1}{1} = models;
        state.samples = models.getSamplesInModelSpace();
        state.values = models.getValues();
        
        tic
            if iscell( ranker )
                score = [];
                for j=1:length(ranker)-1
                    ranker{j} = ranker{j}.initNewSamples(state);
                    score(:,j) = ranker{j}.score( inFunc( gridx ), state);
                end
                
                score = ranker{end}.processScores( score );
            else
                ranker = ranker.initNewSamples(state);
            
                %[score dscore] = ranker.score( inFunc( gridx ), state);
                score = ranker.score( inFunc( gridx ), state);

                %score2 = ranker2.score( inFunc( gridx ), state);
                %score = mean( [score, score2], 2 );
            end
        toc

        %gridy = reshape( gridy, density, density );
        score = reshape( score, density, density );
        %dscore = dscore .* repmat( scale, size(dscore,1), 1 );
        %dscore = dscore ./ scale.^2;
        
        plotHandles(i) = figure;
        %grayscale = linspace(1, 0, 50)';
        %colormap( grayscale(:,[1 1 1] ) );
        %colormap('default');
        [dummy h] = contourf( x{1}, x{2}, score);
        colorbar;
        set(h, 'ShowText', 'on', 'TextStep', get(h,'LevelStep')*2);
        %surfc( x{1}, x{2}, score );

        hold on;
        scatter(gca(gcf), samples(:,1), samples(:,2), ...
			28, 'ko', 'MarkerEdgeColor','k', 'MarkerFaceColor','w' );

        xlabel( inNames{1}, 'FontSize', 14 );
        ylabel( inNames{2}, 'FontSize', 14 );
        %legend( {class(ranker) 'samples'}, 'FontSize', 14 );
        set( gca, 'FontSize', 14 );
        
        %hold on;
        %quiver( gridx(:,1), gridx(:,2), dscore(:,1), dscore(:,2), 'k' );
        
        %savePlot( gcf, sprintf('images/%s%i_%i', class(ranker), i, size(samples,1) ) )
    end 
end