load workspace_unstruct.mat
% idRecording = '118';
% tLimit = 120; %2 minutes
% tLimit_sim = 120;
% [ecg_sig,fs] = extractSignal(idRecording, tLimit);
ecg_sig=EKG_andrea_light_activity;
% ecg_sig=EKG_chris(1:end/4);
fs=100;
% ecg_sig=EKG3;
% fs=360;
tLimit_sim = min(length(ecg_sig)/fs,120);
[ParECG,filteredECG] = genECGParam(ecg_sig,fs,10,false,0.1);
out=sim('HeartModelECG2','StopTime', sprintf('%d',tLimit_sim),'SaveOutput','on','OutputSaveName','heart_Events');
attempts = 20;

synth_ECGs = [];
ECGmeans_synth=[];
ECGsds_synth=[];
meanPhases_synth=[];
minScore=1;
bestI=0;
for i = 1:attempts
    synth_ECG=ECGfromTrace(get(out,'heart_Events'),ParECG,1/fs,0.001);
    
    [score,scores,ECGmean_orig,ECGsd_orig,ECG_distr_orig,meanphase_orig,ECGmean_synth,ECGsd_synth,ECG_distr_synth,meanphase_synth]=computeECGsdistance(filteredECG,fs,synth_ECG,fs,false,false);
    if score < minScore
        minScore = score;
        bestI = i;
    end
    synth_ECGs = [synth_ECGs; struct('at',i,'synth_ecg',synth_ECG, 'score', score)];
    ECGmeans_synth = [ECGmeans_synth; ECGmean_synth];
    ECGsds_synth = [ECGsds_synth; ECGsd_synth];
    meanPhases_synth = [meanPhases_synth; meanphase_synth];
end

plotMeanECG(ECGmean_orig,ECGsd_orig,meanphase_orig,'Original')
plotMeanECG(ECGmeans_synth(bestI,:),ECGsds_synth(bestI,:),meanPhases_synth(bestI,:),'Synthesized')


mean(ParECG)
%out=sim('HeartModelECG','StopTime', sprintf('%d',tLimit_sim),'SaveOutput','on','OutputSaveName','synth_ECG');
%synth_ECG=get(out,'synth_ECG');
%score=computeECGsdistance(filteredECG,fs,synth_ECG,fs,true);