function R = demo_funct(p)
% Test function for MOEA (SPEA2)

R = [Inf, Inf];

R(1) = p(1);

m = length(p);
g = 9 + sum(p(2:m))/(m-1);

R(2) = 1 - sqrt(p(1)/g);

    
