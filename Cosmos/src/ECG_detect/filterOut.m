function [ Data2 ] = filterOut( minV,maxV, Data )
%FILTEROUT Summary of this function goes here
%   Detailed explanation goes here
mini = 1;
while (Data(mini,1)<minV)
   mini=mini+1;
end
maxi = length(Data);
while (Data(maxi,1)>maxV)
    maxi=maxi-1;
end

Data2 = Data(mini:maxi,:);

end

