#set -e

nb_points=${2:-10000}
size_poly=${3:-10}
loopsize=${4:-10}

sage script_tocosmos_PTA.sage $1 $1.grml $size_poly 1 -unif

Cosmos $1.grml --loop "#$loopsize" --trace-place "x_1,x_2,x_3,t_a,t_b,t_c,t_a1,t_a2,t_b1,t_b2" --output-trace $1.dat 0.0 --max-run "$nb_points" --use-van-der-corput --set-Horizon "$loopsize" --batch 0 --width 0 --seed 2705179413
#grep 'time_' $1.dat > preplot.dat

grep "t_\|New" $1.dat | tr '\n' '\t' | sed 's/New Path/ \
/g' | sed 's/->//g' > preplotTmp.dat

grep 't_c.*t_c' preplotTmp.dat > preplota.dat
grep 't_a.*t_b' preplotTmp.dat > preplotb.dat
grep 't_c.*t_a' preplotTmp.dat > preplotc.dat
grep 't_a.*t_a' preplotTmp.dat > preplotaa.dat
#linecombinator $1.dat preplot.dat
head -n 300 preplotb.dat > preplot500b.dat

#gnuplot -c gnplotscriptTmp.gp $1_VDC_cc.png preplota.dat
#gnuplot -c gnplotscriptTmp.gp $1_VDC_ab.png preplotb.dat
#gnuplot -c gnplotscriptTmp.gp $1_VDC_ca.png preplotc.dat

#gnuplot -c gnplotscriptTmp.gp $1_VDC_ab.png preplot500b.dat
gnuplot -c gnplotscriptTmp.gp $1_VDC_aa.png preplotaa.dat

Cosmos $1.grml --loop "#$loopsize" --trace-place "x_1,x_2,x_3,t_a,t_b,t_c,t_a1,t_a2,t_b1,t_b2" --output-trace $1.dat 0.0 --max-run "$nb_points" --batch 0 --width 0 --seed 2705179413
#grep 'time_' $1.dat > preplot.dat
#grep 'time_0_0\|time_1_0\|time_2_0' $1.dat > preplota.dat
#grep 'time_0_1\|time_1_1\|time_2_1\|time_3_0' $1.dat > preplotb.dat
#grep 'time_0_2\|time_1_2\|time_2_2\|time_3_1' $1.dat > preplotc.dat

grep "t_\|New" $1.dat | tr '\n' '\t' | sed 's/New Path/ \
/g' | sed 's/->//g' > preplotTmp.dat

grep 't_c.*t_c' preplotTmp.dat > preplota.dat
grep 't_a.*t_b' preplotTmp.dat > preplotb.dat
grep 't_c.*t_a' preplotTmp.dat > preplotc.dat
grep 't_a.*t_a' preplotTmp.dat > preplotaa.dat
#linecombinator $1.dat preplot.dat
head -n 300 preplotb.dat > preplot500b.dat

#gnuplot -c gnplotscriptTmp.gp $1_Unif_cc.png preplota.dat
#gnuplot -c gnplotscriptTmp.gp $1_Unif_ab.png preplotb.dat
#gnuplot -c gnplotscriptTmp.gp $1_Unif_ca.png preplotc.dat

#gnuplot -c gnplotscriptTmp.gp $1_Unif_ab.png preplot500b.dat
gnuplot -c gnplotscriptTmp.gp $1_Unif_aa.png preplotaa.dat

Cosmos Iso_$1.grml --loop "#$loopsize" --trace-place "x_1,x_2,x_3,t_a,t_b,t_c,t_a1,t_a2,t_b1,t_b2" --output-trace $1.dat 0.0 --max-run "$nb_points" --batch 0 --width 0 --use-van-der-corput --set-Horizon "$loopsize" --seed 2705179413

grep "t_\|New" $1.dat | tr '\n' '\t' | sed 's/New Path/ \
/g' | sed 's/->//g' > preplotTmp.dat

grep 't_c.*t_c' preplotTmp.dat > preplota.dat
grep 't_a.*t_b' preplotTmp.dat > preplotb.dat
grep 't_c.*t_a' preplotTmp.dat > preplotc.dat
grep 't_a.*t_a' preplotTmp.dat > preplotaa.dat
#linecombinator $1.dat preplot.dat
head -n 300 preplotb.dat > preplot500b.dat

#gnuplot -c gnplotscriptTmp.gp $1_Iso_cc.png preplota.dat
#gnuplot -c gnplotscriptTmp.gp $1_Iso_ab.png preplotb.dat
#gnuplot -c gnplotscriptTmp.gp $1_Iso_ca.png preplotc.dat

#gnuplot -c gnplotscriptTmp.gp $1_Iso_ab.png preplot500b.dat
gnuplot -c gnplotscriptTmp.gp $1_IsoLD_aa.png preplotaa.dat

Cosmos Iso_$1.grml --loop "#$loopsize" --trace-place "x_1,x_2,x_3,t_a,t_b,t_c,t_a1,t_a2,t_b1,t_b2" --output-trace $1.dat 0.0 --max-run "$nb_points" --batch 0 --width 0 --seed 2705179413

grep "t_\|New" $1.dat | tr '\n' '\t' | sed 's/New Path/ \
/g' | sed 's/->//g' > preplotTmp.dat

grep 't_c.*t_c' preplotTmp.dat > preplota.dat
grep 't_a.*t_b' preplotTmp.dat > preplotb.dat
grep 't_c.*t_a' preplotTmp.dat > preplotc.dat
grep 't_a.*t_a' preplotTmp.dat > preplotaa.dat
#linecombinator $1.dat preplot.dat
head -n 300 preplotb.dat > preplot500b.dat

#gnuplot -c gnplotscriptTmp.gp $1_Iso_cc.png preplota.dat
#gnuplot -c gnplotscriptTmp.gp $1_Iso_ab.png preplotb.dat
#gnuplot -c gnplotscriptTmp.gp $1_Iso_ca.png preplotc.dat

#gnuplot -c gnplotscriptTmp.gp $1_Iso_ab.png preplot500b.dat
gnuplot -c gnplotscriptTmp.gp $1_IsoUnif.png preplotaa.dat
