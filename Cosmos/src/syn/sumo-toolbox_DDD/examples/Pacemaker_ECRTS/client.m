portNum=27016;
host='192.168.17.223';
t = tcpip(host, portNum, 'NetworkRole', 'client');
set(t,'Timeout',20);
set(t,'ByteOrder','littleEndian');

fopen(t);

nParams=2;
maxParams=8;

    ids=zeros(1,maxParams);
    datas=zeros(1,maxParams);
    ids(1,1)=1;
    ids(1,2)=2;
    
    datas(1,1)=211;
    datas(1,2)=40;
    disp(datas);
    fwrite(t, [nParams ids datas],'uint8');
    disp('data sent');

    % typedef struct SocketComm {
    %     enum SocketStatusComm status; 4 bytes //not needed
    %     float totalCurrent;           4 bytes
    %     unsigned int totalSamples[2]; 8 bytes //not needed
    % }
    
    status=fread(t, 1,'int32');
    totalCurrent=fread(t, 1,'single');
    totalSamples=fread(t, 2,'int32');
    aps=fread(t, 1,'int32');
    vps=fread(t, 1,'int32');
    agets=fread(t, 1,'int32');
    vgets=fread(t, 1,'int32');
    disp(totalCurrent)
    disp(aps)
    disp(vps)
    disp(agets)
    disp(vgets)
    if status==7 || totalCurrent<=0
        disp('Error...resending parameters')
    end

%terminate
fwrite(t, 0,'uint8');

fclose(t);
delete(t);
clear t
%
% ack=0;
% while ack~=1
% %send the value of each parameter, together with the id
%     fwrite(t, id1,'uint8');
%     fwrite(t, data1,'double');
%     id1_rcv=fread(t, 1,'uint8');
%     data1_rcv=fread(t, 1,'double');
%     ack_snd= (id1_rcv==id1)&&(data1_rcv==data1);
%     fwrite(t, ack_snd,'uint8');
%     ack = ack_snd;
% end
%
% ack=0;
% while ack~=1
% %send the value of each parameter, together with the id
%     fwrite(t, id2,'uint8');
%     fwrite(t, data2,'double');
%     id2_rcv=fread(t, 1,'uint8');
%     data2_rcv=fread(t, 1,'double');
%     ack_snd = (id2_rcv==id2)&&(data2_rcv==data2);
%     fwrite(t, ack_snd,'uint8');
%     ack = ack_snd;
% end
%
% ack=0;
% while ack~=1
%     %read computed value
%     f = fread(t, 1,'double');
%     disp(f);
%     fwrite(t, f,'double');
%     ack = fread(t, 1,'uint8');
% end

