#include <vector>
 double DiscreteTab1[73] = {876.000000, 880.000000, 840.000000, 804.000000, 864.000000, 916.000000, 868.000000, 840.000000, 884.000000, 924.000000, 892.000000, 860.000000, 856.000000, 876.000000, 824.000000, 824.000000, 872.000000, 932.000000, 876.000000, 920.000000, 932.000000, 872.000000, 820.000000, 832.000000, 880.000000, 860.000000, 864.000000, 948.000000, 900.000000, 876.000000, 904.000000, 912.000000, 872.000000, 824.000000, 852.000000, 928.000000, 880.000000, 820.000000, 864.000000, 924.000000, 900.000000, 876.000000, 940.000000, 912.000000, 884.000000, 828.000000, 832.000000, 908.000000, 960.000000, 900.000000, 868.000000, 936.000000, 912.000000, 872.000000, 848.000000, 896.000000, 948.000000, 884.000000, 856.000000, 888.000000, 920.000000, 880.000000, 868.000000, 876.000000, 920.000000, 888.000000, 912.000000, 920.000000, 872.000000, 812.000000, 848.000000, 848.000000, 852.000000, };
 double DiscreteTab2[73] = {72.000000, 76.000000, 84.000000, 100.000000, 72.000000, 72.000000, 80.000000, 88.000000, 76.000000, 92.000000, 84.000000, 72.000000, 80.000000, 72.000000, 76.000000, 80.000000, 104.000000, 76.000000, 84.000000, 92.000000, 80.000000, 68.000000, 84.000000, 96.000000, 84.000000, 80.000000, 96.000000, 72.000000, 76.000000, 80.000000, 84.000000, 84.000000, 72.000000, 84.000000, 88.000000, 68.000000, 80.000000, 92.000000, 88.000000, 80.000000, 76.000000, 92.000000, 72.000000, 92.000000, 84.000000, 76.000000, 80.000000, 76.000000, 84.000000, 80.000000, 84.000000, 68.000000, 76.000000, 76.000000, 88.000000, 84.000000, 76.000000, 76.000000, 84.000000, 84.000000, 84.000000, 96.000000, 76.000000, 84.000000, 88.000000, 84.000000, 76.000000, 84.000000, 76.000000, 92.000000, 84.000000, 96.000000, 84.000000, };
 double DiscreteTab3[73] = {26.666667, 24.000000, 24.000000, 24.000000, 26.666667, 26.666667, 26.666667, 26.666667, 29.333333, 24.000000, 26.666667, 26.666667, 26.666667, 29.333333, 26.666667, 24.000000, 26.666667, 26.666667, 26.666667, 24.000000, 26.666667, 26.666667, 26.666667, 24.000000, 29.333333, 26.666667, 26.666667, 24.000000, 24.000000, 26.666667, 26.666667, 26.666667, 29.333333, 24.000000, 24.000000, 26.666667, 24.000000, 24.000000, 26.666667, 24.000000, 26.666667, 24.000000, 24.000000, 21.333333, 24.000000, 24.000000, 29.333333, 29.333333, 26.666667, 26.666667, 26.666667, 24.000000, 24.000000, 24.000000, 26.666667, 26.666667, 26.666667, 26.666667, 26.666667, 26.666667, 26.666667, 24.000000, 26.666667, 24.000000, 26.666667, 26.666667, 26.666667, 24.000000, 29.333333, 24.000000, 24.000000, 24.000000, 24.000000, };
 double DiscreteTab4[73] = {13.333333, 12.000000, 12.000000, 12.000000, 13.333333, 13.333333, 13.333333, 13.333333, 14.666667, 12.000000, 13.333333, 13.333333, 13.333333, 14.666667, 13.333333, 12.000000, 13.333333, 13.333333, 13.333333, 12.000000, 13.333333, 13.333333, 13.333333, 12.000000, 14.666667, 13.333333, 13.333333, 12.000000, 12.000000, 13.333333, 13.333333, 13.333333, 14.666667, 12.000000, 12.000000, 13.333333, 12.000000, 12.000000, 13.333333, 12.000000, 13.333333, 12.000000, 12.000000, 10.666667, 12.000000, 12.000000, 14.666667, 14.666667, 13.333333, 13.333333, 13.333333, 12.000000, 12.000000, 12.000000, 13.333333, 13.333333, 13.333333, 13.333333, 13.333333, 13.333333, 13.333333, 12.000000, 13.333333, 12.000000, 13.333333, 13.333333, 13.333333, 12.000000, 14.666667, 12.000000, 12.000000, 12.000000, 12.000000, };
 double DiscreteTab5[73] = {252.000000, 256.000000, 248.000000, 252.000000, 240.000000, 256.000000, 252.000000, 248.000000, 252.000000, 256.000000, 252.000000, 248.000000, 252.000000, 252.000000, 248.000000, 256.000000, 256.000000, 248.000000, 252.000000, 256.000000, 252.000000, 252.000000, 256.000000, 252.000000, 252.000000, 236.000000, 248.000000, 260.000000, 256.000000, 248.000000, 256.000000, 244.000000, 252.000000, 252.000000, 256.000000, 256.000000, 252.000000, 252.000000, 252.000000, 256.000000, 256.000000, 248.000000, 260.000000, 256.000000, 256.000000, 248.000000, 248.000000, 244.000000, 256.000000, 252.000000, 252.000000, 260.000000, 256.000000, 252.000000, 252.000000, 252.000000, 252.000000, 256.000000, 252.000000, 252.000000, 252.000000, 248.000000, 252.000000, 260.000000, 248.000000, 240.000000, 256.000000, 260.000000, 248.000000, 240.000000, 260.000000, 252.000000, 252.000000, };
double magicUDDD(std::vector<double> const& param,unsigned int i){
	switch((int)param[0]){
		case 1: return DiscreteTab1[i % 73];
		case 2: return DiscreteTab2[i % 73];
		case 3: return DiscreteTab3[i % 73];
		case 4: return DiscreteTab4[i % 73];
		case 5: return DiscreteTab5[i % 73];
default: exit(1);
	}
}