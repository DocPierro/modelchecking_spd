currDir = pwd;
cd('../../ECG_detect/');
addpath(pwd);
initHeartModel;
cd(currDir);
go('config/personalized_heart.xml');