function [state,options,optchanged] = saveGApopulation(options,state,flag)
    popsFile=fopen('../Single_shot/ES_explored.txt','a+');
    pop=state.Population;
    score=state.Score;
    for i=1:length(score)
    	fprintf(popsFile,'%f   %f   %f\n',pop(i,:),score(i));
    end
    fclose(popsFile);
    optchanged=0;
end