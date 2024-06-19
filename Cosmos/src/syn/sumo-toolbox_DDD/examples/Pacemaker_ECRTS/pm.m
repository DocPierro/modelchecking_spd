function f = pm(x)
global t
global simTime
global maxConsumption

nParams=2;
maxParams=8;

ids=zeros(1,maxParams);
datas=zeros(1,maxParams);
ids(1,1)=1;
ids(1,2)=2;

%process inputs
datas(1,1)=max(1,min(247,round(x(:,1))*100)) + 7;
datas(1,2)=max(1,min(247,round(x(:,2))*100)) + 7;

error=1;
status=7;
totalCurrent=0;
while error
    fwrite(t, [nParams ids datas],'uint8');
    disp('data sent');
    tic;
    status=fread(t, 1,'int32');
    if toc<=30
        totalCurrent=fread(t, 1,'single');
        totalSamples=fread(t, 2,'int32');
        aps=fread(t, 1,'int32');
        vps=fread(t, 1,'int32');
        agets=fread(t, 1,'int32');
        vgets=fread(t, 1,'int32');
        disp('data received');
        disp(totalCurrent)
        error = (status==7 || totalCurrent<=0);
    end
    if error
        disp('Error...resending parameters')
    end
    
end
bpm = ((vps+vgets)/simTime)*60;
if bpm>=60 && bpm<=120
    f = totalCurrent;
else f = maxConsumption;
end

samplesFile=fopen('initDataSet.txt','a+');
fprintf(samplesFile,'%f %f %f\n',x,f);
fclose(samplesFile);

