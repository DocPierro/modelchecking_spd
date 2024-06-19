function fid = readLines(fid,nLines)
i=0;
while i<nLines
    fgetl(fid);
    i=i+1;
end