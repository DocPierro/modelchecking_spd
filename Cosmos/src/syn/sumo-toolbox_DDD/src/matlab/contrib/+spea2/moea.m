%MOEA Multi-Objective Evolutionary Algorithm, implementing SPEA2
%        (Strengthen Pareto Evolutionary Algorithm 2) for MATLAB
%
% PLEASE READ THE LICENSE AGREEMENT BEFORE WORKING WITH THIS PROGRAM
% >> help spea2_copyrights
%
% PISA version created by Computer Engineering (TIK) ETH Zurich
%       www.tik.ee.ethz.ch/pisa/
%
% MATLAB version created by Institute of Control Systems
%       Hamburg University of Technology, 2005
%       http://www.tu-harburg.de/rts
%
%
% This function is only the interface between the user and SPEA2. Usage:
%
% [genes, fitness] = moea('file_name', bounds, options)
%
% genes     - the parameter vector of the Pareto-optimal individuals. The
%             number of individuals is equal the population size
% fitness   - objective values for the individuals in genes
%
% file_name - name of the M function, calculating the cost functions that
%             should be minimized (!! SPEA2 always minimizes !!)
%             The input arguments of this functions are the decision
%             variables and the output arguments are the minimized
%             functions.
% bounds    - Nx2 matrix, containing the lower and upper boundaries for
%             each decision variables, where N is the number of decision
%             variables
%             bounds = [Lower1, Upper1;
%                       Lower2, Upper2;
%                         ...
%                       LowerN, UpperN];
%             The number of decision variables is determined by the number
%             of rows in bounds. The algorithm searches ONLY WITHIN the
%             specified range.
% options   - structure, containing the optimization options:
% |-MaxGen  - maximal number of generations (integer > 0);
% |-PopSize - population size (integer > 0);
% |-Dim     - problem dimension (number of functions to be minimized)
% |           integer value > 0;
% |-Seed    - seed for the random generator (integer);
% |-Tourn   - number of individuals for Tournament selection (integer > 1);
% |-i_mut_p - individual mutation probability (real [0,1]);
% |-i_rec_p - individual recombination probability (real [0,1]);
% |-v_mut_p - variable mutation probability (real [0,1]);
% |-v_swa_p - variable swap probability (real [0,1]);
% |-v_rec_p - variable recombination probability (real [0,1]);
% |-eta_mut - eta mutation (real [0,1]);
% |-eta_rec - eta recombination (real [0,1]);
% |-Display - if different than 0, the number of remaining generations is
%             displayed;
%
% For more information on the meaning of the options, please refer to the
% PISA documentation.
%
% options = moea()  returns the structure with the default options.
%
% See also moea_copyrights
%                               v 1.0; A.Popov; www.tu-harburg/rts; 05.2005

% Copyright 2005-2007, Hamburg University of Technology, Institute of Control Systems

function [varargout] = moea(varargin)

if (nargin <= 1 && nargout <=1 )    % default structure
    options = struct(...
        'MaxGen',   50, ...     % Maximum number of generations
        'PopSize',  50, ...     % Population size
        'Dim',      2,  ...     % Problem dimension
        'Seed',     1,  ...     % Seed for the random generator
        'Tourn',    2,  ...     % See the manual of SPEA2
        'i_mut_p',  1,  ...
        'i_rec_p',  1,  ...
        'v_mut_p',  1,  ...
        'v_swa_p',  0.5,...
        'v_rec_p',  1,  ...
        'eta_mut',  15, ...
        'eta_rec',  5,  ...
        'Display',  1);         % when not 0 the remaining iter. are displayed
    varargout{1} = options;
    return;
end
    
if (nargin ~= 3 || nargout ~= 2)
    error('MOEA:SPEA2:WrongNumArg','Wrong number of input or output arguments for MOEA.\nPlase see "help moea" for details');
end

FName  = varargin{1};
Bounds = varargin{2};
Opt    = varargin{3};

m = size(Bounds,2);
if (m ~= 2)
    error('MOEA:SPEA2:WrongBounds','Wrong size of the Bounds matrix.\nPlease see "help moea" for usage details');
end

maxGen = round(Opt.MaxGen);
if (maxGen < 1 )
    error('MOEA:SPEA2:MaxGenIsNegativeOr0','The maximal number of generations should be positive integer.\nPlease see "help moea" for usage details');
end

popSize = round(Opt.PopSize);
if (popSize < 1 )
    error('MOEA:SPEA2:PopSizeIsNegativeOr0','The population size should be positive integer.\nPlease see "help moea" for usage details');
end
alpha  = popSize;   % initial population size
mu     = popSize;   % number of parent individuals
lambda = popSize;   % number of offspring individuals
% Sometimes SPEA 2 allows running with different values of those 3
% population sizes, but since there are still problems they are constrained
% to the same size

dim = round(Opt.Dim);
if (dim < 1 )
    error('MOEA:SPEA2:DimIsNegative','The number of minimized functions cannot be less than 1.\nSince you are using Multi-objective optimization it makes since to be at least 2.\nPlease see "help moea" for usage details');
end

seed = round(Opt.Seed);

tournament = round(Opt.Tourn);
if (tournament < 1 )
    error('MOEA:SPEA2:DimIsNegative','The number of parents for Tournament selection is at least 1\nPlease see "help moea" for usage details');
end

Options = [
    maxGen;
    alpha;
    mu;
    lambda;
    dim;
    seed;
    tournament;
    Opt.i_mut_p;
    Opt.i_rec_p;
    Opt.v_mut_p;
    Opt.v_swa_p;
    Opt.v_rec_p;
    Opt.eta_mut;
    Opt.eta_rec;
    Opt.Display
    ];

Genes = []; Fitness = [];

% Call the DLL file with SPEA2
[Genes,Fitness] = spea2.spea2(FName,Bounds,Options);

varargout{1} = Genes;
varargout{2} = Fitness;
