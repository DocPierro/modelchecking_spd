#set -e

nb_points=${2:-10000}
size_poly=${3:-10}
loopsize=${4:-10}

outname=${1%.*}.grml

sage script_tocosmos_PTA.sage $1 $outname $size_poly -1 -unif

Cosmos $outname --loop "#$loopsize" --trace-place "x_1,x_2,x_3,t_a,t_b,t_c,t_a1,t_a2,t_b1,t_b2" --output-trace $1.dat 0.0 --max-run "$nb_points" --use-van-der-corput --set-Horizon "#$loopsize" --batch 0 --width 0
#grep 'time_' $1.dat > preplot.dat
grep 't_a' $1.dat > preplota.dat
grep 't_b' $1.dat > preplotb.dat
grep 't_c' $1.dat > preplotc.dat
#linecombinator $1.dat preplot.dat
gnuplot -c gnplotscript.gp $1_VDC.png

Cosmos $outname --loop "#$loopsize" --trace-place "x_1,x_2,x_3,t_a,t_b,t_c,t_a1,t_a2,t_b1,t_b2" --output-trace $1.dat 0.0 --max-run "$nb_points" --batch 0 --width 0
#grep 'time_' $1.dat > preplot.dat
#grep 'time_0_0\|time_1_0\|time_2_0' $1.dat > preplota.dat
#grep 'time_0_1\|time_1_1\|time_2_1\|time_3_0' $1.dat > preplotb.dat
#grep 'time_0_2\|time_1_2\|time_2_2\|time_3_1' $1.dat > preplotc.dat
grep 't_a' $1.dat > preplota.dat
grep 't_b' $1.dat > preplotb.dat
grep 't_c' $1.dat > preplotc.dat
#linecombinator $1.dat preplot.dat
gnuplot -c gnplotscript.gp $1_Unif.png

Cosmos Iso_$outname --loop "#$loopsize" --trace-place "x_1,x_2,x_3,t_a,t_b,t_c,t_a1,t_a2,t_b1,t_b2" --output-trace Iso_$1.dat 0.0 --max-run "$nb_points" --batch 0 --width 0
grep 't_a' Iso_$1.dat > preplota.dat
grep 't_b' Iso_$1.dat > preplotb.dat
grep 't_c' Iso_$1.dat > preplotc.dat
#grep 'tt_' Iso_$1.dat > preplot.dat
#grep 'tt_0_0\|tt_1_0\|tt_2_0' Iso_$1.dat > preplota.dat
#grep 'tt_0_1\|tt_1_1\|tt_2_1\|tt_3_0' Iso_$1.dat > preplotb.dat
#grep 'tt_0_2\|tt_1_2\|tt_2_2\|tt_3_1' Iso_$1.dat > preplotc.dat
#linecombinator Iso_$1H.dat preplot.dat
gnuplot -c gnplotscript.gp $1_Iso.png
