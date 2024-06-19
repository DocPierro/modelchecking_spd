#!env python
import sys

n= int(sys.argv[1]);
Dline=sys.argv[2];
wt=sys.argv[3]; 
B=sys.argv[4];

s="";
s+="NOTDETERMINISTIC\n\n";
s+="const double Dline="+Dline+";\n";
s+="const double wt="+wt+";\n";
s+="const double B = "+B+";\n";
s+="const double C = 10;\n\n";
s+="const double D = 9;\n\n";
s+="const double E = 50;\n\n";
s+="const double F = 100;\n\n";
s+="VariablesList = {time,DISC countT, x,t};\n"
states=["l_%d" %i for i in range(n)];
statesfin=["lf_%d" %i for i in range(n)];
statesbis=["ll_%d" %i for i in range(n)];
s+="LocationsList = {"
for i in range(n):
    s+=states[i]+","+statesfin[i]+","+statesbis[i]+",";
s+="lff};\n";
s+="PROB;\n\n"
s+="InitialLocations={l_0};\nFinalLocations={lff};\n";
s+="Locations={\n"
for i in range(n):
    s+="(l_%d, TRUE, (time:1,x:1,t:1));" %i+"\n(lf_%d, TRUE, (time:1,x:1,t:1));\n"%i;
    s+="(ll_%d, TRUE, (time:1,x:1,t:1));\n"%i;
s+="(lff, TRUE, (time:1,x:1,t:1));\n};\n\n"
s+="Edges={\n"

for i in range(n):
    s+="((l_%d" %i + ",l_%d"%i+"),ALL\{t_a,t_b,t_c,t_d,t_e},countT <=Dline ,#);\n";    
    s+="((ll_%d" %i + ",ll_%d"%i+"),ALL\{t_a,t_b,t_c,t_d,t_e},countT <=Dline ,#);\n";    
    s+="((lf_%d" %i + ",lf_%d"%i+"),ALL\{t_a,t_b,t_c,t_d,t_e},countT <=Dline ,#);\n";    
for i in range(n):
    s+="((lf_%d" %i + ",lff),ALL,countT =Dline+1,#);\n";
        
s+="%forward edges in the automaton\n"
for i in range(n-2):
    s+="((l_%d" %i + ",l_%d"%(i+1)+"),{t_a}, x<=B  ,{countT = countT+1});\n";
s+="((l_%d"%(n-2)+",l_%d"%(n-1) +"),{t_b}, x<=C  ,{countT = countT+1});\n";

s+="%back edges in the automaton\n";
s+="((l_%d"%(n-2)+",l_0),{t_c}, x>=C & x<=B , {countT = countT+1,x=0});\n";
s+="((l_%d"%(n-1)+",l_0),{t_e}, x<=wt , {countT = countT+1,x=0});\n";
for i in range(n-1):
    s+="((l_%d" %i + ",l_0),{t_d}, x<=B  ,{countT = countT+1});\n";


s+="\n%pattern to be recognised (leading to the final states)\n"
s+="((l_0,ll_1),{t_a}, x<=D  ,{countT = countT+1,t=0});\n";
s+="((ll_0,ll_1),{t_a}, x<=D  ,{countT = countT+1});\n";
for i in range(1,n-2):
    s+="((ll_%d" %i + ",ll_%d"%(i+1)+"),{t_a}, x<=D  ,{countT = countT+1});\n";
s+="((ll_%d"%(n-2)+",ll_%d"%(n-1) +"),{t_b}, x<=D & t<=E ,{countT = countT+1});\n";
s+="((ll_%d"%(n-1)+",lf_0),{t_e}, x<=wt& t>=F,  {countT = countT+1,x=0});\n";
for i in range(n-1):
    s+="((ll_%d" %i + ",ll_0),{t_d}, x<=B  ,{countT = countT+1});\n";


s+="%copy of the automaton when the final states are reached\n";
s+="%forward edges in the automaton\n"
for i in range(n-2):
    s+="((lf_%d" %i + ",lf_%d"%(i+1)+"),{t_a}, x<=B  ,{countT = countT+1});\n";
s+="((lf_%d"%(n-2)+",lf_%d"%(n-1) +"),{t_b}, x<=C  ,{countT = countT+1});\n";

s+="%back edges in the automaton\n";
s+="((lf_%d"%(n-2)+",lf_0),{t_c}, x>=C & x<=B , {countT = countT+1,x=0});\n";
s+="((lf_%d"%(n-1)+",lf_0),{t_e}, x<=wt , {countT = countT+1,x=0});\n";
for i in range(n-1):
    s+="((lf_%d" %i + ",lf_0),{t_d}, x<=B  ,{countT = countT+1});\n";
s+="};\n\n"

print(s);
