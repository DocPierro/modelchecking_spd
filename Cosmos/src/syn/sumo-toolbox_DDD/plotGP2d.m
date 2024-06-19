function plotGP2d(model, samples, paramNames,ranges,experimentName)
SIZE_OF_GRID=100;
Bx=ranges(1,:);   
By=ranges(2,:);      
X=linspace( Bx(1), Bx(2) , SIZE_OF_GRID );
Y=linspace( By(1), By(2) , SIZE_OF_GRID );
meanVal=zeros(SIZE_OF_GRID,SIZE_OF_GRID);
varianceVal=zeros(SIZE_OF_GRID,SIZE_OF_GRID);
for ii=1:length(X)
   for jj=1:length(Y)
      % evaluating variance of kriging model  at point x(ii)  y(jj) which correspond to z(jj,ii) for surf plot
       [meanVal(jj,ii),varianceVal(jj,ii)]=evaluate(model,[X(ii), Y(jj)]);    
   end
end
h = figure;
hold on
surf(X,Y,meanVal)
title(strcat('Expected value. Experiment: ',experimentName))
xlabel(paramNames{1}) 
ylabel(paramNames{2})
scatter3(samples(:,1),samples(:,2),samples(:,3),50,[0 0 0],'filled')
[min_val,min_idxs]=min(samples(:,3));
minEntry = samples(min_idxs(1),:);
scatter3(minEntry(:,1),minEntry(:,2),minEntry(:,3),100,[1 0 0],'filled')
hold off
savefig(h, strcat('experiments/out_plots/',experimentName,'_plot_mean.fig'))
minfile = fopen(strcat('experiments/out_plots/',experimentName,'_minSample.txt'),'w');
fprintf(minfile,'%f,%f,%f',minEntry);
fclose(minfile);
h = figure;
surf(X,Y,sqrt(varianceVal))
title(strcat('Standard deviation. Experiment: ',experimentName))
xlabel(paramNames{1}) 
ylabel(paramNames{2})
savefig(h,strcat('experiments/out_plots/',experimentName,'_plot_sd.fig'))
end