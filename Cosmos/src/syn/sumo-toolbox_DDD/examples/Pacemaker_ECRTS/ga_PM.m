%we want to do at most 150 evaluations
popsFile=fopen('../Single_shot/ES_explored.txt','w+');
fprintf(popsFile,'');
fclose(popsFile);
options = gaoptimset('PopulationSize',20);
options = gaoptimset(options,'Generations',20);
options = gaoptimset(options,'OutputFcns',@saveGApopulation);
LB = [0.01 0.01];
UB = [2.5 2.5];
[gaX, gaFval, gaExitFlag,gaOutput] = ga(@pm1, 2, [],[],[],[],LB,UB,[],[], options);