function f = computeVal(x)

inFile = load('tmp/inFile.mat');
inFile = inFile.inFile;
outFile = load('tmp/outFile.mat');
outFile = outFile.outFile;

currentSamples = load('tmp/samples.mat');
smpls = currentSamples.smpls;
currentSampleNum = size(smpls,1)+1;


fprintf(outFile,'%d,',ceil(x(:,1)));
fprintf(outFile,'%d\n',ceil(x(:,2)));
disp(strcat(sprintf('%d',currentSampleNum),'-th sample. Value: ',sprintf('%f,%f',x)));
disp('Waiting for simulation results...');

tline = fgetl(inFile);


while strcmp(tline,'')
    tline = fgetl(inFile);
end


f=sscanf(tline,'%f');
disp(strcat('Result: ',tline));

smpls(currentSampleNum,:)=[x,f];
save('tmp/samples.mat','smpls');
