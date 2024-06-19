function [ecg_sig,fs] = extractSignal(id, tLimit)

headerFile = fopen(strcat('MIT_DB/data/',id,'.hea'),'r');
if headerFile == -1
    error(strcat('Could not open header file for recording ',id))
end
signalFile = fopen(strcat('MIT_DB/data/',id,'.dat'),'r');
if signalFile == -1
    error(strcat('Could not open signal file for recording ',id))
end
parsedHeader = fscanf(headerFile,'%d',4);
fclose(headerFile);

fs = parsedHeader(3);
nSamples = parsedHeader(4);
nChannels = parsedHeader(2);

if nargin>1
    nSamples = min(nSamples,tLimit*fs);
end

ecg_sig=zeros(nSamples,1);
for i=1:nSamples 
%while readSamples<1000
fread(signalFile,1,'bit4');
sample1 = fread(signalFile,1,'bit12');
fread(signalFile,1,'bit8');
% tmpFile = fopen(strcat('tmp/',id,'.bin'),'w+');
% fwrite(tmpFile,sample2_sndpart,'bit8');
% fwrite(tmpFile,sample2_fstpart,'bit4');
% %rewind, read and close
% frewind(tmpFile)
% sample2 = fread(tmpFile,1,'bit12');
% fclose(tmpFile);
ecg_sig(i) = sample1;
end
fclose(signalFile);

end