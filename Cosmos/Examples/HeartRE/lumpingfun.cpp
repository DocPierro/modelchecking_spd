#include "markingImpl.hpp"

const int reducePL_RE_VRG = 0;
const int reducePL_RE_Ventricle = 1;
const int reducePL_RE_TAVIcmp = 2;
const int reducePL_RE_AVI = 3;
const int reducePL_RE_URI = 4;
const int reducePL_RE_Atrium = 5;
const int reducePL_RE_LRI = 6;
const int reducePL_RE_SANode = 7;
const int reducePL_RE_AVJOut = 8;
const int reducePL_RE_AVJ = 9;
const int reducePL_RE_RAConductor = 10;
const int reducePL_RE_RVConductor = 11;
void SPN::print_state(const vector<int> &vect){
	cerr << "RE_VRG = " << vect[reducePL_RE_VRG] << endl;
	cerr << "RE_Ventricle = " << vect[reducePL_RE_Ventricle] << endl;
	cerr << "RE_TAVIcmp = " << vect[reducePL_RE_TAVIcmp] << endl;
	cerr << "RE_AVI = " << vect[reducePL_RE_AVI] << endl;
	cerr << "RE_URI = " << vect[reducePL_RE_URI] << endl;
	cerr << "RE_Atrium = " << vect[reducePL_RE_Atrium] << endl;
	cerr << "RE_LRI = " << vect[reducePL_RE_LRI] << endl;
	cerr << "RE_SANode = " << vect[reducePL_RE_SANode] << endl;
	cerr << "RE_AVJOut = " << vect[reducePL_RE_AVJOut] << endl;
	cerr << "RE_AVJ = " << vect[reducePL_RE_AVJ] << endl;
	cerr << "RE_RAConductor = " << vect[reducePL_RE_RAConductor] << endl;
	cerr << "RE_RVConductor = " << vect[reducePL_RE_RVConductor] << endl;
}
bool SPN::precondition(const abstractMarking &Marking){return true;}

int mapVRG(int v){
    switch(v){
        case 268: return 268; //Wait4VBeat
        case 269: return 269; //Wait4VSynch;
        case 797: return 846; //491_Pre_Wait4VBeat;
        case 799: return 848; //266_Pre_Wait4VSynch;
    }
}

int mapVentricule(int v){
    switch(v){
        case 244: return 244; //Excitable
        case 245: return 245; //Refractory
        case 308: return 308; //Vsense

        case 246: return 245; //VSynch -> refractory
        case 247: return 245; //GenerateRetroWave -> refractory
        case 310: return 308; //VsenseAnte -> Vsense
    }
}

int mapAVI(int v){
    switch(v){
        case 737: return 758; //Idle0
        case 739: return 763; //AVI0
        case 740: return 773; //WaitURI
        case 741: return 760; //Idle1
        case 742: return 764; //AVI1
    }
}

int mapURI(int v){
    switch(v){
        case 730: return 525; //URI
        case 732: return 838; //q1
        case 781: return 840; //PreURI
    }
}

int mapAtrium(int v){
    switch(v){
        case 34: return 34; //Refractory
        case 35: return 35; //Exitable
        case 304: return 788; //ASense

        case 42: return 34;  //Refractory
        case 218: return 34; //Refractory
        case 306: return 788; //Refractory
    }
}

int mapSANode(int v){
    switch(v){
        case 14: return 14; // Wait4ABeat;
        case 88: return 88; // Wait4ASynch;
        case 801: return 850; // 492_Pre_Wait4ABeat;
        case 803: return 852; //  16_Pre_Wait4ASynch;
    }
}

int mapAVJOut(int v){
    switch(v){
       case 172: return 172;     //172 -> Retro;
       case 173: return 173;     //173 -> Empty;
       case 175: return 175;     //175 -> Ante;
       case 813: return 862;     //813 -> 179_Pre_Empty;
       case 815: return 864;     //815 -> 177_Pre_Empty;
    }
}

int mapAVJ(int v){
    switch(v){
        case 625: //Retro
        case 632: //Ante
        case 633: //Refractory
        case 787: // -> 638_Pre_Refractory;
        case 789: //-> 637_Pre_Refractory;
        case 791: // -> 668_Pre_Refractory;
        case 793: // -> 651_Pre_Refractory;
            return 633; // -> Refractory

        case 640: //Recovery
        case 795: //650_Pre_Recovery
            return 640; // -> Recovery

    }
}

int mapRAConductor(int v){
    switch(v){
        case 51: return 51; //-> Ante;
        case 52: return 52; // -> Retro;
        case 60: return 60; // -> Empty;
        case 817: return 858; // -> 56_Pre_Empty;
        case 819: return 860; //-> 58_Pre_Empty;
    }
}

int mapRVConductor(int v){
    switch(v){
        case 155: return 155; //-> Ante;
        case 156: return 156; // -> Retro;
        case 157: return 157; // -> Empty;
        case 807: return 854; // -> 161_Pre_Empty;
        case 809: return 856; //-> 160_Pre_Empty;
    }
}

int mapTAVIcmp(int v){
    switch (v) {
        case 761: return 794; //q0
        case 763: return 795; //-> q1;
        case 779: return 836; //-> 726_Pre_q1;
    }
}


int mapLRI(int v){
    switch (v) {
        case 720: return 510; // -> LRI;
        case 722: return 510; //-> ASensed -> LRI;
        case 783: return 842; //-> 714_Pre_LRI;
        case 785: return 844; //-> 723_Pre_LRI;
    }
}


void SPN::lumpingFun(const abstractMarking &Marking,vector<int> &vect){
	vect[reducePL_RE_VRG] = mapVRG(Marking.P->_PL_VRG);
	vect[reducePL_RE_Ventricle] = mapVentricule(Marking.P->_PL_Ventricle);
	vect[reducePL_RE_TAVIcmp] = mapTAVIcmp(Marking.P->_PL_TAVIcmp);
	vect[reducePL_RE_AVI] = mapAVI(Marking.P->_PL_AVI);
	vect[reducePL_RE_URI] = mapURI(Marking.P->_PL_URI);
	vect[reducePL_RE_Atrium] = mapAtrium(Marking.P->_PL_Atrium);
	vect[reducePL_RE_LRI] = mapLRI(Marking.P->_PL_LRI);
	vect[reducePL_RE_SANode] = mapSANode(Marking.P->_PL_SANode);
	vect[reducePL_RE_AVJOut] = mapAVJOut(Marking.P->_PL_AVJOut);
	vect[reducePL_RE_AVJ] = mapAVJ(Marking.P->_PL_AVJ);
	vect[reducePL_RE_RAConductor] = mapRAConductor(Marking.P->_PL_RAConductor);
	vect[reducePL_RE_RVConductor] = mapRVConductor(Marking.P->_PL_RVConductor);
}
