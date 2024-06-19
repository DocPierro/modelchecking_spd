rm -rf tmp
cp distrBrach.cpp distr.cpp 
Cosmos HeartModelWithDistr.slx CheckSafe.grml  --tmp-status 2 --const "PM_TAVI=1500,PM_TLRI=5000,PM_TURI=5000" --max-run 1000000 --width 0 --njob 2
rm -rf tmp
Cosmos HeartModelWithDistr.slx CheckSafe.grml  --tmp-status 2 --max-run 1000000 --width 0 --njob 2 --const "PM_TAVI=200"
rm -rf tmp
cp distrNormal.cpp distr.cpp
Cosmos HeartModelWithDistr.slx CheckSafe.grml  --tmp-status 2 --const "PM_TAVI=1500,PM_TLRI=5000,PM_TURI=5000" --max-run 1000000 --width 0 --njob 2