% Demo of MOEA (Using SPEA2)

% Get the default options
opt = moea();

% Set the number of generations
opt.MaxGen = 100;

% Set the boundaries (specifies also the number of variables)
bounds = [0 10; 0 2];

% Make the optimization run
[genes, fitness] = moea('demo_funct', bounds, opt);

% Visualize the result
plot(fitness(:,1), fitness(:,2), 'ro');
