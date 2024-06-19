function [score,scores,ECGmean_1,ECGsd_1,ECG_distr_1,meanphase_1,ECGmean_2,ECGsd_2,ECG_distr_2,meanphase_2] = computeECGsdistance(ecg_1,fs_1,ecg_2,fs_2,showplot,scaling,nbinsDiscreteDistr)

if nargin< 7
    nbinsDiscreteDistr = 10;
    if nargin < 6
        scaling = true;
        if nargin < 5
            showplot = 0;
        end
    end
end
ecgBins=floor(min(fs_1,fs_2)/2);
[ECGmean_1,ECGsd_1,ECG_distr_1,meanphase_1] = computeECGDistr(ecg_1',fs_1,ecgBins,showplot,scaling,'Data 1');
[ECGmean_2,ECGsd_2,ECG_distr_2,meanphase_2] = computeECGDistr(ecg_2',fs_2,ecgBins,showplot,scaling,'Data 2');

[score,scores] = discreteDistrsDistance(ECG_distr_1,ECG_distr_2,nbinsDiscreteDistr,ecgBins);
end

function [ECGmean,ECGsd,discreteDistrs,meanphase] = computeECGDistr(data,fs,bins,showplot,scaling,desc)

if nargin == 3
    showplot = 0;
    desc = '';
    scaling = true;
end

teta = 0;                                       % desired phase shift

% bsline = LPFilter(data,.7/fs);                 % baseline wander removal (may be replaced by other approaches)
% x = data-bsline;




% %Noise cancelation(Filtering)
% f1=0.5; %cuttoff low frequency to get rid of baseline wander
% f2=45; %cuttoff frequency to discard high frequency noise
% 
% if fs <= 90
%     f2 = floor((fs-1)/2);
% end
% 
% Wn=[f1 f2]*2/fs; % cutt off based on fs
% 
% N = 3; % order of 3 less processing
% [a,b] = butter(N,Wn); %bandpass filtering
% data = filtfilt(a,b,data);

x = data;

% peaks = PeakDetection(x,f/fs);                  % peak detection
[~,peaks_idxs] = pan_tompkin(x',fs,0);
peaks = zeros(size(x));
peaks(peaks_idxs) = 1;

% window = round(fs/25); % averaging window size
% x = smooth(x,window);

phase = PhaseCalculation(peaks);     % phase calculation

pphase = PhaseShifting(phase,teta);             % phase shifting

%bins = fs/2;                                     % number of phase bins
[ECGmean,ECGsd,meanphase,discreteDistrs] = MeanECGExtraction(x,pphase,bins,1); % mean ECG extraction

%scale
scaleFact=1;
if scaling
    scaleFact=max(ECGmean);
end
%scale the ECG mean and SD
ECGmean=ECGmean./scaleFact;
ECGsd=ECGsd./scaleFact;
%scale the distributions
for i = 1:length(discreteDistrs)
    discreteDistrs(i).vals=discreteDistrs(i).vals./scaleFact;
end

if showplot
    t = (0:length(data)-1)/fs;
    figure;
    plot(t,data*6/max(data),'b');
    hold on
    plot(t,peaks*2,'ro');
    plot(t,phase,'c--','linewidth',1);
    plot(t,pphase,'g','linewidth',1);
    grid;
    xlabel('time (sec.)');
    legend(strcat('Scaled ECG ',desc),strcat('ECG Peaks ',desc),strcat('phase ',desc),strcat('shifted phase ',desc));
    
    plotMeanECG(ECGmean,ECGsd,meanphase,desc);
end
end