rm -rf tmp
cp distrAVB.cpp distr.cpp 
Cosmos HeartModelWithDistr.slx CheckAVB.grml  --tmp-status 2 --const "PM_TAVI=1500,PM_TLRI=5000,PM_TURI=5000,VRG_ectoD=100000000000" --max-run 1000000 --width 0 --njob 2
rm -rf tmp
Cosmos HeartModelWithDistr.slx CheckAVB.grml  --tmp-status 2 --max-run 1000000 --width 0 --njob 2 --const "PM_TAVI=190,VRG_ectopD=10000000000"
rm -rf tmp
cp distrNormal.cpp distr.cpp
Cosmos HeartModelWithDistr.slx CheckAVB.grml  --tmp-status 2 --const "PM_TAVI=1500,PM_TLRI=5000,PM_TURI=5000,VRG_ectopD=1000000000" --max-run 1000000 --width 0 --njob 2
