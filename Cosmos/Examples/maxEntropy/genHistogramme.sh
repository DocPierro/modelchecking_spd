#set -e

nb_points=${2:-10000}
size_poly=${3:-10}
loopsize=${4:-10}

sage script_tocosmos_PTA.sage $1 $1.grml $size_poly $size_poly -unif


echo "$1_Unif.png";

Cosmos $1.grml taTacas2018.lha --tmp-status 2 --output-raw histdata.raw --max-run "$nb_points" --const "N=$loopsize" --tmp-status 2 --set-Horizon "$loopsize"
cp histdata.raw histUnif.raw
gnuplot -c histogram.gnu "$1_Unif.png"

Cosmos Iso_$1.grml taTacas2018.lha --tmp-status 2 --output-raw histdata.raw --max-run "$nb_points" --const "N=$loopsize" --tmp-status 2 --set-Horizon "$loopsize"
cp histdata.raw histIso.raw
gnuplot -c histogram.gnu  "$1_ISO.png"

Cosmos $1.grml taTacas2018.lha --tmp-status 2 --output-raw histdata.raw --max-run "$nb_points" --const "N=$loopsize" --tmp-status 2 --set-Horizon "$loopsize" --use-van-der-corput
cp histdata.raw histVDC.raw
gnuplot -c histogram.gnu  "$1_VDC.png"
