function [ delayTab, heightTab, widthTab] = extractDelay( R_data,S_data,S_filtered_data,T_data,Q_data,Q_filtered_data,P_data,rate )
%EXTRACTDELAY Summary of this function goes here
%   Detailed explanation goes here

% minT = P_data(1,1);
% maxT = T_data(length(T_data),1);

% P_data = filterOut(minT,maxT,P_data);
% Q_data = filterOut(minT,maxT,Q_data);
% R_data = filterOut(minT,maxT,R_data);
% S_data = filterOut(minT,maxT,S_data);
% T_data = filterOut(minT,maxT,T_data);

S_data_p=[];
Q_data_p=[];

for i=1:size(T_data,1)
    idx = find(S_data(:,2)==T_data(i,2),1);
    S_data_p = [S_data_p; S_data(idx,:)];
    idx = find(Q_data(:,2)==T_data(i,2),1);
    Q_data_p = [Q_data_p; Q_data(idx,:)];
end

S_data=S_data_p;
Q_data=Q_data_p;


S_width_filtered = S_data(:,4);
Q_width_filtered = Q_data(:,4);

for i=1:length(S_width_filtered)
    idx=find(S_filtered_data(:,2)==S_data(i,2),1);
    if ~isempty(idx)
        S_width_filtered(i)=S_filtered_data(idx,4);
    end
    idx=find(Q_filtered_data(:,2)==Q_data(i,2),1);
    if ~isempty(idx)
        Q_width_filtered(i)=Q_filtered_data(idx,4);
    end
end
S_data(:,4)=S_width_filtered;
Q_data(:,4)=Q_width_filtered;



%comment if you want to keep original (and not filtered) detection
% S_data = S_filtered_data;
% Q_data = Q_filtered_data;


delayTab = [ (P_data(1:end,1) - [0.0; P_data(1:end-1,1)]) ...
    ,(Q_data(:,1)-P_data(:,1)) ...
    ,(R_data(:,1)-P_data(:,1)) ...
    ,(S_data(:,1)-P_data(:,1)) ...
    ,(T_data(:,1)-P_data(:,1))]/rate;

heightTab = [ P_data(:,3) ...
    , Q_data(:,3) ...
    , R_data(:,2) ...
    , S_data(:,3) ...
    , T_data(:,3) ];

widthTab  = [ P_data(:,4) ...
    , Q_data(:,4) ...
    , R_data(:,3) ...
    , S_data(:,4)...
    , T_data(:,4) ]/rate;

end

