portNum=30000;
t = tcpip('localhost', portNum, 'NetworkRole', 'server');
fopen(t);

ack=0;
while ack~=1
    %read parameter id
    id1 = fread(t, 1,'uint8');
    %read parameter value
    data1 = fread(t, 1,'double');
    fwrite(t, id1,'uint8');
    fwrite(t, data1,'double');
    ack = fread(t, 1,'uint8');
end

ack=0;
while ack~=1
    %read parameter id
    id2 = fread(t, 1,'uint8');
    %read parameter value
    data2 = fread(t, 1,'double');
    fwrite(t, id2,'uint8');
    fwrite(t, data2,'double');
    ack = fread(t, 1,'uint8');
end

disp(id1)
disp(data1)
disp(id2)
disp(data2)

f = data1*data2/10;
disp(f);

ack=0;
while ack~=1
    %send the computed value
    fwrite(t, f,'double');
    f_rcv=fread(t, 1,'uint8');
    ack_snd= (f_rcv==f);
    fwrite(t, ack_snd,'uint8');
    ack = ack_snd;
end

fclose(t);

