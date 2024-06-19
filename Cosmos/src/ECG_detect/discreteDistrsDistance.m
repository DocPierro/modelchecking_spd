function [score,scores] = discreteDistrsDistance(dist1,dist2,nbins,nbinsECG)
if length(dist1)~=length(dist2)
    disp 'The two ECGs are divided with different num of bins'
    score = inf;
    scores=ones(size(dist1));
    scores=scores*inf;
else
    score = 0;
    scores=zeros(size(dist1));
    for i = 1:length(dist1)
        %for all the elements of the distrs at the i-th bin of the ECG...
        d1 = dist1(i).vals;
        d2 = dist2(i).vals;
        els = union(d1,d2);
        domain = linspace(min(els),max(els),nbins);
        for j=1:(length(domain)-1)
            
            p1 = length(find(d1>=domain(j) & d1<domain(j+1)))/length(d1);
            p2 = length(find(d2>=domain(j) & d2<domain(j+1)))/length(d2);
            if j==(length(domain)-1)
                p1 = p1+length(find(d1==domain(j+1)))/length(d1);
                p2 = p2+length(find(d2==domain(j+1)))/length(d2);
            end
                
            score = score + abs(p1-p2)/2/nbinsECG;
            scores(i) = scores(i)+abs(p1-p2)/2/nbinsECG;
        end
    end
end
end

