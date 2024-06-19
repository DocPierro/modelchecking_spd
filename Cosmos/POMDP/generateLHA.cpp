#include <iomanip>
#include <iostream>
#include <fstream>
#include "generateLHA.hpp"

void generateLHAfun(double ron,int N,vector< vector< vector<double> > >& OldPlast ,vector< vector< vector<double> > >& Plast ,vector<double>& RewardArray, double H, vector< vector<double> >& M, bool discounted){

  unsigned long Xlastmax = Plast[0].size()-1;
  double discount = 0.9;

  vector<vector<bool> > hasChanged(Plast.size(),vector<bool>(Plast[0].size(),false) );

  for (int v=0 ; v<N; v++)
    for(int Xlast=1;Xlast<=Xlastmax;Xlast++)
      for(int x=1;x<N;x++)
        hasChanged[v][Xlast] = hasChanged[v][Xlast] || ((ron + Plast[v][Xlast-1][x]*RewardArray[x] >=0) !=
                                                        (ron + OldPlast[v][Xlast-1][x]*RewardArray[x] >=0));

  cout << "generate LHA" << endl;


  ofstream LhaFile("generated.lha", ios::out | ios::trunc);
  ofstream StrategieFile("Strat",ios::out | ios::trunc);

  if(discounted){LhaFile << "NbVariables = " << 3 << ";" << endl;
  }else LhaFile << "NbVariables = " << 2 << ";" << endl;

  if(discounted){LhaFile << "NbLocations = "<< N*(Xlastmax+1)+2 << ";" << endl;
  }else LhaFile << "NbLocations = "<< N*(Xlastmax+1)+2 << ";" << endl;

  LhaFile << "const double H = "<< H << ";" << endl;


  /* time record the elapsed time;
     Xlast is equal to 0 if the drone is currently detected and 1 otherwise
  */

  if(discounted){LhaFile << "VariablesList = {time, Reward,Discount};"<< endl;
  }else LhaFile << "VariablesList = {time, Reward} ;"<< endl;

  LhaFile << "LocationsList = {lf";
  if(discounted)LhaFile << ",li";
  else LhaFile << ",ln";
  for(int i=0;i<N;i++){
    for(int Xlast=0;Xlast<=Xlastmax;Xlast++){
      LhaFile << ", lp"<<i<<"_"<<Xlast;
    }
  }
  LhaFile << "} ;"<< endl;


  LhaFile << "Reward=AVG(Last(Reward));"<<endl;

  if(discounted){LhaFile << "InitialLocations = {li};"<< endl;
  }else LhaFile << "InitialLocations = {lp0_0};"<< endl;

  if(discounted)LhaFile << "FinalLocations = {lf};"<< endl;
  else LhaFile << "FinalLocations = {ln};"<< endl;

  LhaFile << "Locations={"<< endl;
  //LhaFile << "(li, TRUE, (time:1));"<< endl;
  if(discounted)LhaFile << "(li, TRUE , (time:1));"<< endl;
  else LhaFile << "(ln, TRUE , (time:1));" << endl;
  LhaFile << "(lf, TRUE, (time:0));"<< endl;
  //LhaFile << "(la, TRUE, (time:1));"<< endl;
  for(int i=0;i<N;i++){
    LhaFile << "(lp"<<i<<"_0, V"<<i<<"=1 , (time:0));"<< endl;

    StrategieFile << "Strategy for drone last located in "<<i;

    //Print strategy
    StrategieFile << ", "<< 0 << " round ago" << endl;
    //The drone must not be in a activate sensor location
    for (int j =1;j<N;j++){
      StrategieFile << "\tSensor " << j;
      //j start at 1 because 0 is the bottom position no sensor
      if (ron + Plast[i][0][j]*RewardArray[j] >=0){
        StrategieFile << " on" << endl;
      }else{
        StrategieFile << " off" << endl;
      }
    }

    cout <<setw(4)<< i<< "[";
    for (int j=0; j< Plast[0][0].size(); j++){
      cout << "->"<<setw(3)<<j<<"[";
      for (int Xlast=0; Xlast<Plast[0].size(); Xlast++){
        if (ron + Plast[i][Xlast][j]*RewardArray[j] >=0|| hasChanged[i][Xlast]){
          cout << " on ";
        }else{
          cout << " off";
        }

      }
      cout << "]";
    }
    cout << "]"<< endl;

    for(int Xlast=1;Xlast<=Xlastmax;Xlast++){

      /*

        for(int Xlast=1;Xlast<=Xlastmax;Xlast++){*/
      LhaFile << "(lp"<<i<<"_"<<Xlast<< ", (TRUE";
      StrategieFile << "Strategy for drone last located in "<< i;
      StrategieFile <<", "<< Xlast << " round ago" << endl;
      //The drone must not be in a activate sensor location
      for (int j =1;j<N;j++){
        StrategieFile << "\tSensor " << j;
        //j start at 1 because 0 is the bottom position no sensor
        if (ron + Plast[i][Xlast][j]*RewardArray[j] >=0 || hasChanged[i][Xlast] ){
          //LhaFile<<"& V"<<j<<"=0";
          StrategieFile << " on" << endl;
        }else{
          StrategieFile << " off" << endl;
        }
      }

      LhaFile << ") );"<< endl;
      //}
    }
  }


  LhaFile << "};"<<endl;

  LhaFile << "Edges={"<< endl;

  if(discounted)LhaFile << "((li,lp0_0),#,time<=0, {Discount = 1} );"<<endl;

  //Edges to the final state.
  for (int v=0 ; v<N; v++) {
    for(int Xlast=0;Xlast<=Xlastmax;Xlast++)
      LhaFile << "((lp"<<v<<"_"<< Xlast <<",lf),ALL, time>=H,#);"<<endl;
  }

  if(!discounted)LhaFile << "((lf,ln),ALL,#, {Reward=Reward/H} );"<<endl;

  for (int v=0 ; v<N; v++) {

    for(int Xlast=0;Xlast<=Xlastmax;Xlast++){

      //count the number of on sensor
      int nbSensorOn = 0;
      for(int x=1;x<N;x++)
        if (ron + Plast[v][Xlast][x]*RewardArray[x] >=0|| hasChanged[v][Xlast])nbSensorOn++;

      //Drone undetected => Xlast++
      LhaFile << "((lp"<<v<<"_"<< Xlast <<",lp"<<v<<"_";
      LhaFile << min(Xlast+1,(int)Xlastmax) <<"),ALL";
      int compt = 0;
      for (int x=0 ; x<N; x++) {
        if ((M[v][x]>0) & ((ron + Plast[v][Xlast][x]*RewardArray[x] >=0 || hasChanged[v][Xlast]) & (x>0))){
          for(int xnb=0 ; xnb<N; xnb++){
            if(compt >0){LhaFile << " ,";}
            else LhaFile << "\\{";
            compt++;
            LhaFile << "Tr" << xnb << "_" << x << " ";
          }
        }
      }
      if(compt>0)LhaFile << "}";
      LhaFile << ", time <=H,";
      LhaFile << "{time = time+1,Reward = Reward + " << ron * nbSensorOn;
      if(discounted){LhaFile << "*Discount , Discount = "<< discount <<"* Discount }";
      }else LhaFile << "}";

      LhaFile << ");"<< endl;

      //Drone detected in location x_0
      for (int x=1 ; x<N; x++) {
        //x start in 1 because there is no sensor in 0
        if ((ron + Plast[v][Xlast][x]*RewardArray[x]>=0 || hasChanged[v][Xlast]) &(Xlast>0 | M[v][x]>0) ){
          LhaFile << "((lp"<<v<<"_"<< Xlast <<",lp"<<x<<"_0),";
          LhaFile << "ALL, time <=H, { ";

          LhaFile << "time=time+1,Reward = Reward + (" << RewardArray[x] << " + "<< ron * nbSensorOn;
          //LhaFile <<", Plast"<<v<<"_"<<Xlast<<"_"<<x<<"=Plast"<<v<<"_"<< Xlast <<"_"<<x<<"+1";
          //LhaFile << ", Out"<<v<<"=Out"<<v<<"+1";
          if(discounted){LhaFile << ")*Discount , Discount = "<< discount <<"* Discount }";
          }else LhaFile << ")}";

          LhaFile << ");"<<endl;
        }
      }
    }
  }

  LhaFile << "};"<< endl;

  LhaFile.close();
  StrategieFile.close();
}


int NumberOfSetBits(int i){
  i = i - ((i >> 1) & 0x55555555);
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
  return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

void generateLHAfromStrat(double ron,int N, vector< pair<int, vector<int> > >& strat, vector<double>& reward, double H,vector< vector<double> >& M,bool discounted){
  double discount = 0.9;

  cout << "generate LHA" << endl;


  ofstream LhaFile("generatedFromStrat.lha", ios::out | ios::trunc);

  if(discounted){LhaFile << "NbVariables = " << 3 << ";" << endl;
  }else LhaFile << "NbVariables = " <<  2 << ";" << endl;

  LhaFile << "NbLocations = "<< strat.size()+3 << ";" << endl;

  LhaFile << "const double H = "<< H << ";" << endl;


  /* time record the elapsed time;
     Xlast is equal to 0 if the drone is currently detected and 1 otherwise
  */

  if(discounted){LhaFile << "VariablesList = {time, Reward,Discount};"<< endl;
  }else LhaFile << "VariablesList = {time, Reward} ;"<< endl;


  LhaFile << "LocationsList = { li, lf, ln";
  for(int i=0;i<strat.size();i++){
    LhaFile << ", la"<<i;
  }
  LhaFile << "} ;"<< endl;


  LhaFile << "Reward=AVG(Last(Reward));"<<endl;

  //LhaFile << "InitialLocations = {la"<<strat.size()-1<<"};"<< endl;
  LhaFile << "InitialLocations = {li};"<< endl;

  if(discounted)LhaFile << "FinalLocations = {lf};"<< endl;
  else LhaFile << "FinalLocations = {ln};"<< endl;

  LhaFile << "Locations={"<< endl;
  LhaFile << "(li, TRUE, (time:1));"<< endl;
  LhaFile << "(lf, TRUE, (time:0));"<< endl;
  LhaFile << "(ln, TRUE, (time:0));"<< endl;
  //LhaFile << "(la, TRUE, (time:1));"<< endl;
  for(int i=0;i<strat.size();i++){
    LhaFile << "(la"<<i<<", TRUE , (time:0));"<< endl;

  }


  LhaFile << "};"<<endl;

  LhaFile << "Edges={"<< endl;

  LhaFile << "((lf,ln),ALL,#, {Reward=Reward/H} );"<<endl;

  //Edges to the initial state.
  if(discounted){
    LhaFile << "((li,la"<< strat.size()-1 <<"),#,time<=0, {Discount = 1} );"<<endl;
  }else{
    LhaFile << "((li,la"<< strat.size()-1 <<"),#,time<=0, # );"<<endl;
  }

  //Edges to the final state.
  for (int v=0 ; v<strat.size(); v++)
    LhaFile << "((la"<<v<<",lf),ALL, time>=H,#);"<<endl;

  for (int v=0 ; v<strat.size(); v++) {

    for (int x=0 ; x<N; x++) {
      if (strat[v].second[x]>=0) {
        bool firsttrans=true;
        LhaFile << "((la"<<v<<",la"<<strat[v].second[x]<<"),{";

        if(x==0){
          for (int lo =0; lo<N; lo++) {
            for(int li=0;li<N; li++){
              if(((strat[v].first>>(li-1)) %2)==0){
                if (firsttrans){firsttrans=false;}
                else LhaFile<< ", ";

                LhaFile<< "Tr"<<lo<<"_"<<li;
              }
            }
          }

          LhaFile << "}, time <=H, {time=time+1,Reward= Reward+ "
                  << ron * NumberOfSetBits(strat[v].first);
          if(discounted)
            LhaFile << "*Discount ,Discount = " << discount << "*Discount";

        }else{
          for (int lo =0; lo<N; lo++) {
            if (firsttrans){firsttrans=false;}
            else LhaFile<< ", ";

            LhaFile<< "Tr"<<lo<<"_"<<x;
          }

          LhaFile << "},time <=H,{ time=time+1,Reward = Reward+ ("
                  << reward[x]<<" + "<< ron*NumberOfSetBits(strat[v].first)<<")";
          if(discounted)
            LhaFile << "*Discount ,Discount = "<<discount<<"*Discount";
        }

        LhaFile << "});"<<endl;
      }
    }
  }

  LhaFile << "};"<< endl;

  LhaFile.close();


}
