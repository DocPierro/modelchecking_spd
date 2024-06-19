Cosmos 802.11pOneMedium.grml simpleprop2.cpp --tmp-status 2 --batch 0 --max-run 100 --const "Arrival=$1" --njob=2 --HASL-formula "Throughput_CorrectPacketAckp1= AVG(Last(Garbled)); 
Throughput_CorrectPacketAckp2= AVG(Last(Garbled));
Throughput_CorrectPacketAckp3= AVG(Last(Garbled));
Throughput_CorrectPacketAckp4= AVG(Last(Garbled));
Throughput_CorrectPacketAck12= AVG(Last(Garbled));
Throughput_CorrectPacketAck21= AVG(Last(Garbled));
MeanToken_Idle= AVG(Last( Idle));
MeanToken_Medium= AVG(Last( Medium));
MeanToken_Garbled= AVG(Last( Garbled));" > res_$1.res