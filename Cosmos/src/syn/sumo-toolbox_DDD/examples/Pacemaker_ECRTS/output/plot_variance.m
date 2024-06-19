%load('model[out].mat'); %loading the model
SIZE_OF_GRID=100;
%[Bx,By]=getBounds(model); % getting bounds (doesn't work!)
Bx=[0.01, 120];    % setting bound manually
By=[0.01, 20];      % setting bound manually 
X=linspace( Bx(1), Bx(2) , SIZE_OF_GRID );
Y=linspace( By(1), By(2) , SIZE_OF_GRID );
Z1=zeros(SIZE_OF_GRID,SIZE_OF_GRID);
Z2=zeros(SIZE_OF_GRID,SIZE_OF_GRID);
for ii=1:length(X)
    
   for jj=1:length(Y)
      
       [Z1(jj,ii),Z2(jj,ii)]=evaluate(model,[X(ii), Y(jj)]);    % evaluating variance of kriging model  at point x(ii)  y(jj) which correspond to z(jj,ii) for surf plot
        
   end
    
    
end
figure
hold on
surf(X,Y,Z1)
scatter3(samples(:,1),samples(:,2),samples(:,3),50,[0 0 0],'filled')
hold off
figure
surf(X,Y,sqrt(Z2))
