function plotGP1d(model, samples, paramName,range,experimentName)
SIZE_OF_GRID=1000;
Bx=range;   
X=linspace( Bx(1), Bx(2) , SIZE_OF_GRID );
meanVal=zeros(1,SIZE_OF_GRID);
varianceVal=zeros(1,SIZE_OF_GRID);
for ii=1:length(X)
      % evaluating variance of kriging model  at point x(ii)
       [meanVal(ii),varianceVal(ii)]=evaluate(model,X(ii));    
end

sd = sqrt(varianceVal);
sd_plot=[(meanVal-sd)',(sd+sd)'];
h = figure;
hold on
area(X,sd_plot);
plot(X,meanVal,'Color',[1,0,0])
xlabel(paramName) 
title(strcat('Expected value \pm SD. Experiment: ',experimentName))
scatter(samples(:,1),samples(:,2),50,[0 0 0],'filled')
[min_val,min_idxs]=min(samples(:,2));
minEntry = samples(min_idxs(1),:);
scatter(minEntry(:,1),minEntry(:,2),100,[1 0 0],'filled')
hold off
savefig(h,strcat('examples/OneParam/output_',experimentName,'/plot_mean_sd.fig'))
minfile = fopen(strcat('experiments/out_plots/',experimentName,'_minSample.txt'),'w');
fprintf(minfile,'%f,%f',minEntry);
fclose(minfile);
end