function f = heart(x)
oldAV_anteDmin = evalin('base','AV_anteDMin');
oldAV_retroDmin = evalin('base','AV_retroDMin');
old_AV_vt = evalin('base','AV_Vt');


filteredECG=evalin('base','filteredECG');
tLimit_sim=evalin('base','tLimit_sim');
fs=evalin('base','fs');

assignin('base','AV_anteDMin',max(1,round(x(:,1)*1000)));
assignin('base','AV_retroDMin',max(1,round(x(:,1)*1000)));
assignin('base','AV_Vt',round(x(:,2)));

out=sim('HeartModelECG','StopTime', sprintf('%d',tLimit_sim),'SaveOutput','on','OutputSaveName','synth_ECG');
f=computeECGsdistance(filteredECG,fs,get(out,'synth_ECG'),1000,false);

assignin('base','AV_anteDMin',oldAV_anteDmin);
assignin('base','AV_retroDMin',oldAV_retroDmin);
assignin('base','AV_Vt',old_AV_vt);

end