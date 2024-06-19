function f = pm1(x)
simTime=60;
maxConsumption=1e08;
assignin('base','PM_TLRI',max(1,round(x(:,1)*1000)));
assignin('base','PM_TAVI',max(1,round(x(:,2)*1000)));
%used in TURI experiment
%assignin('base','PM_TURI',max(1,round(x(:,1)*1000)));
%assignin('base','PM_TPVARP',max(1,round(x(:,2)*1000)));

sim('idhp_model_whole')
agets=sum(Agets.signals.values);
vgets=sum(Vgets.signals.values);
aps=sum(APs.signals.values);
vps=sum(VPs.signals.values);
basicCurrent = 1.6e06;
probVariation = rand*1e04 - 5e03;
total=agets+vgets+aps+vps;
bluetoothConsumption=normrnd(total*1e2,sqrt(5e1));
pacingConsumption = normrnd(aps*2e2+vps*3e2,sqrt(5e1));
%used in TURI experiment
% pacingConsumption = normrnd(aps*2e3+vps*3e3,sqrt(5e2));
bpm = ((vps+vgets)/simTime)*60;
f=basicCurrent+probVariation+bluetoothConsumption+pacingConsumption;
if bpm<60 || bpm>120
    f = f + maxConsumption;
end
assignin('base','PM_TLRI',1000);
assignin('base','PM_TAVI',150);
%used in TURI experiment
% assignin('base','PM_TURI',500);
% assignin('base','PM_TPVARP',250);

end