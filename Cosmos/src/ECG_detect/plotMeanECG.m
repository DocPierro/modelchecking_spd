function plotMeanECG(ECGmean,ECGsd,meanphase,desc)
    figure;
    errorbar(meanphase,ECGmean,ECGsd/2);
    hold on;
    plot(meanphase,ECGmean,'r');
    legend(strcat('SD Bar ',desc),strcat('Mean ECG ',desc));
    xlabel('phase (rads.)');
    grid
end