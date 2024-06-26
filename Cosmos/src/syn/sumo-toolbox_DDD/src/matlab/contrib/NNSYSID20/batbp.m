function [W1,W2,PI_vector,iter]=batbp(NetDef,W1,W2,PHI,Y,trparms)
%
%  BATBP
%  -----
%           Batch version of the back-propagation algorithm.
%
%  Given a set of corresponding input-output pairs and an initial network
%  [W1,W2,critvec,iter]=batbp(NetDef,W1,W2,PHI,Y,trparms) trains the
%  network with backpropagation.
%
%  The activation functions must be either linear or tanh. The network
%  architecture is defined by the matrix 'NetDef' consisting of two
%  rows. The first row specifies the hidden layer while the second
%  specifies the output layer.
%
%  E.g.,    NetDef = ['LHHHH'
%                     'LL---']
%
%  (L = Linear, H = tanh)
% 
%  Notice that the bias is included as the last column in the weight
%  matrices.
%
%  See alternatively INCBP for the incremental/recursive back-propagation.
% 
%  INPUT:
%  NetDef : Network definition 
%  W1     : Input-to-hidden layer weights. The matrix dimension is
%           dim(W1) = [(# of hidden units) * (inputs + 1)] (the 1 is due to the bias)
%           Use [] for a random initialization.
%  W2     : hidden-to-output layer weights.
%           dim(W2) = [(outputs)  *  (# of hidden units + 1)]
%           Use [] for a random initialization.
%  PHI    : Input vector. dim(PHI) = [(inputs)  *  (# of data)]
%  Y      : Output data. dim(Y) = [(outputs)  * (# of data)]
%  trparms: Data structure containing parameters associated with the
%           training algorithm (optional). Use the function SETTRAIN if
%           you do not want to use the default values.
%
%  OUTPUT: 
%  W1, W2   : Weight matrices after training.
%  critvec  : Vector containing the criterion evaluated after each iteration.
%  iter     : # of iterations.
%  
%  Programmed by : Magnus Norgaard, IAU/IMM, DTU
%  LastEditDate  : Jan 15, 2000

 
%----------------------------------------------------------------------------------
%--------------             NETWORK INITIALIZATIONS                   -------------
%----------------------------------------------------------------------------------
if nargin<6 | isempty(trparms) % Default training parameters
   trparms = settrain;   
   eta = trparms.eta;
   alpha = trparms.alph;
else                                    % User specified values
  if ~isstruct(trparms),
     error('''trparms'' must be a structure variable.');
  end
  if ~isfield(trparms,'infolevel')
     trparms = settrain(trparms,'infolevel','default');
  end
  if ~isfield(trparms,'maxiter')
     trparms = settrain(trparms,'maxiter','default');
  end
  if ~isfield(trparms,'critmin')
     trparms = settrain(trparms,'critmin','default');
  end
  if ~isfield(trparms,'critterm')
     trparms = settrain(trparms,'critterm','default');
  end
  if ~isfield(trparms,'gradterm')
     trparms = settrain(trparms,'gradterm','default');
  end
  if ~isfield(trparms,'paramterm')
     trparms = settrain(trparms,'paramterm','default');
  end
  if ~isfield(trparms,'eta')
     trparms = settrain(trparms,'eta','default');
  end
  eta    = trparms.eta;
  if ~isfield(trparms,'alph')
     trparms = settrain(trparms,'alph','default');
  end
  alpha    = trparms.alph;
end
[outputs,N] = size(Y);                % # of outputs and # of data
[inputs,N] = size(PHI);                 % # of hidden units
L_hidden = find(NetDef(1,:)=='L')';   % Location of linear hidden units
H_hidden = find(NetDef(1,:)=='H')';   % Location of tanh hidden units
L_output = find(NetDef(2,:)=='L')';   % Location of linear output units
H_output = find(NetDef(2,:)=='H')';   % Location of tanh output units
hidden = length(L_hidden)+length(H_hidden);
if isempty(W1) | isempty(W2),           % Initialize weights if nescessary
  W1 = rand(hidden,inputs+1)-0.5;
  W2 = rand(outputs,hidden+1)-0.5;
end
if (size(W1,2)~=inputs+1 | size(W1,1)~=hidden |... % Check dimensions
      size(W2,2)~=hidden+1 | size(W2,1)~=outputs)
   error('Dimension mismatch in weights, data, or NetDef.');
end
PI_vector  = zeros(trparms.maxiter,1); % Vector containing the SSE for each iteration
y1 = zeros(hidden,N);                 % Hidden layer outputs
aug_y1=[y1;ones(1,N)];
delta1 = y1;
y2 = zeros(outputs,N);                % Network output
delta2 = y2;
Y_train = zeros(size(Y));
PHI = [PHI;ones(1,N)];                % Augment PHI with a row containg ones
dW1_old = 0*W1;                       % Weights from previous iteration
dW2_old = 0*W2;                       % Weights from previous iteration
critdif  = trparms.critterm+1;        % Initialize stopping variables

%---------------------------------------------------------------------------------
%-------------                   TRAIN NETWORK                       -------------
%---------------------------------------------------------------------------------
clc;
c = fix(clock);
%fprintf('Network training started at %2i.%2i.%2i\n\n',c(4),c(5),c(6));
for iter=1:trparms.maxiter,

% >>>>>>>>>>>>>>    Compute network output (Presentation phase)    <<<<<<<<<<<<<<< 
    h1 = W1*PHI;  
    y1(H_hidden,:) = pmntanh(h1(H_hidden,:));
    y1(L_hidden,:) = h1(L_hidden,:);
    
    aug_y1(1:hidden,1:N)=y1;
    h2 = W2*aug_y1;
    y2(H_output,:) = pmntanh(h2(H_output,:));
    y2(L_output,:) = h2(L_output,:);

    E            = Y - y2;                     % Training error
    SSE          = sum(sum(E.*E));             % Sum of squared errors (SSE)



% >>>>>>>>>>>>>>>>>>>>>    Backpropagation of the error     <<<<<<<<<<<<<<<<<<<<<< 
    % Delta for output layer
    delta2(H_output,:) = (1-y2(H_output,:).*y2(H_output,:)).*E(H_output,:);
    delta2(L_output,:) = E(L_output,:);
  
    % delta for hidden layer
    E1 = W2(:,1:hidden)'*delta2; 
    delta1(H_hidden,:) = (1-y1(H_hidden,:).*y1(H_hidden,:)).*E1(H_hidden,:);
    delta1(L_hidden,:) = E1(L_hidden,:);


    
% >>>>>>>>>>>>>>>>>>>>>>>>        Update weights         <<<<<<<<<<<<<<<<<<<<<<<<<
    % -- Update weights without momentum --
    G2 = delta2*aug_y1';
    G1 = delta1*PHI';
    if (alpha==0),
      W2 = W2 + eta*G2; % Update weights between hidden and ouput
      W1 = W1 + eta*G1;    % Update weights between input and hidden
    else
    
    % -- Update weights with momentum --
      dW2 = alpha*dW2_old + (1-alpha)*eta*G2;
      W2  = W2 + dW2;
      dW1 = alpha*dW1_old + (1-alpha)*eta*G1;
      W1  = W1 + dW1;
      dW2_old = dW2;
      dW1_old = dW1;
    end

    
%>>>>>>>>>>>>>>>>>>>>        UPDATES FOR NEXT ITERATION        <<<<<<<<<<<<<<<<<<<
  PI = SSE/(2*N);
  PI_vector(iter)=PI;
  if iter>1,
     critdif  = abs(PI_vector(iter-1)-PI);        % Criterion difference
  end   
  paramdif = eta*max(max(max(abs(G1))),max(max(abs(G2)))); % Maximum parameter dif.
  gradmax  = paramdif/eta/N;                      % Maximum gradient
  switch(trparms.infolevel)                                % Print on-line inform
     case 1
        %fprintf('# %i   W=%4.3e  critdif=%3.2e  maxgrad=%3.2e  paramdif=%3.2e\n',...
        %   iter,PI,critdif,gradmax,paramdif);
     otherwise
        %fprintf('iteration # %i   W = %4.3e\r',iter,PI);
  end
  if (PI<trparms.critmin | ...                    % Check if stop condition is satisfied
       (critdif<trparms.critterm & gradmax<trparms.gradterm & ...
       paramdif<trparms.paramterm))
    break, end
end
%---------------------------------------------------------------------------------
%-------------              END OF NETWORK TRAINING                  -------------
%---------------------------------------------------------------------------------
PI_vector = PI_vector(1:iter);
c = fix(clock);
%fprintf('\n\nNetwork training ended at %2i.%2i.%2i\n',c(4),c(5),c(6));
