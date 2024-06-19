#include "exPapier.hpp"
#include <iostream>
#include <stdlib.h>

vector< vector<double> > generateMatrix(int n){
    int N =5;
    vector< vector<double> > M(N,vector<double>(N,0) );
    
    M[0][1]=1;
    M[1][1]=0.3;
    M[1][2]=0.4;
    M[1][3]=0.3;
    M[2][0]=0.5;
    M[2][3]=0.5;
    M[3][3]=0.3;
    M[3][4]=0.7;
    M[4][2]=1;
    
    return M;
}

double dist(double i, double j){
	return (1.0 / ((i-j)*(i-j)));
}
vector< vector<double> > generateLinearMatrix(int n, double p, double q){
   
    vector< vector<double> > M(n+1,vector<double>(n+1,0) );
    double c1 = 0.75;
    double c2 = 0.75;
    double c3 = 1.2;
	
	int k = n/2+1;
	
	{
		double out0 = c3;
		M[0][0] = c3;
		for(int j= 1;j<=k;j++){M[0][j]= c1*dist(0,j) ; out0+=c1*dist(0,j);}
		for(int j= k+1;j<=n;j++){M[0][j]= c1*dist(2*k,j); out0+=c1*dist(2*k,j);}
		for(int j= 0;j<=n;j++)M[0][j]/= out0;
	}
	for(int i=1;i<=n;i++){
		
		if (i<=k) M[i][0] = c2*dist(0,i);
		else M[i][0] = c2*dist(2*k,i);
		
		double outi = M[i][0];
		for(int j=1;j<=n;j++){
			if (i==j)M[i][j] = c3;
			else {
				double d = dist(i,j);
				if(i ==k) M[i][j] = c2*d;
				else 
					if((i<j && i<=k) || (i>j && i>=k)) M[i][j]= c1*d;
					else 
						if((i<j && i>=k) || (i>j && i<=k)) M[i][j]= c2*d;
			}
			outi += M[i][j];
		}
		for(int j= 0;j<=n;j++)M[i][j]/= outi;
	}
	
	for(int i=0;i<=n;i++){
		std::cout << "\t[] state = "<<i<<" -> ";
		for(int j=0;j<=n;j++){
			if(j>0)std::cout << " + ";
			std::cout << M[i][j] << " : (state'="<<j<<") ";
		}
		std::cout << ";" << std::endl;
	}
	
	for(int i=0;i<=n;i++){
		for(int j=0;j<=n;j++){
			std::cout << M[i][j]<< "\t";
		}
		std::cout << endl;
	}
	
    /*M[0][0]=1-p-q;
    M[0][1]=q;
    M[0][n]=p;
    
    M[n][n]=1-p-q;
    M[n][0]=q;
    M[n][n-1]=p;
    
    for(int i=1;i<n;i++){
      M[i][i]=1-p-q;
      M[i][i+1]=q;
      M[i][i-1]=p;
    }*/
    
    
    return M;
}

vector<double> generateLinearReward(int n,double Rmax){
	double Rmax2 = Rmax * n;
	vector<double> reward(n+1,0.0);
	int k = n/2+1;
	reward[0]=0.0;
	for(int i =1 ; i<=n ;i++){
		reward[i]= (Rmax2/(abs(k-i)+1));
		std::cout <<"reward"<< i <<" : "<<"k:"<<k <<" : "<< reward[i] << endl;
	}
		
	return reward;
}



vector< vector<double> > generateGridMatrix(int n, int m, double u, double r, double d, double l){
  int Dim=n*m+1;
  
  vector< vector<double> > M(Dim,vector<double>(Dim,0) );
  int s;
  M[0][0]=1-u-r-d-l;
  for(int i=1;i<=n;i++)
    for(int j=1;j<=m;j++)
      {s=(i-1)*m+j;
	
	  if(i==1){
		  if(j==1){ M[0][s]=l;
			  M[s][0]=l+d;
			  if(m>1)
			    M[s][s+1]=r;
			  if(n>1)
			    M[s][i*m+j]=u;
			  M[s][s]=1-u-r-d-l;
		  }
		  else if(j==m){ M[0][s]=d;
			  M[s][0]=r+d;
			  if(m>1)
			    M[s][s-1]=l;
			  if(n>1)
			    M[s][i*m+j]=u;
			  M[s][s]=1-u-r-d-l;
		  }
		  else{ M[s][0]=d;
			  if(n>1)
			    M[s][i*m+j]=u;			  
			  M[s][s+1]=r;
			  M[s][s-1]=l;			  
			  M[s][s]=1-u-r-d-l;
		  }
	  }
	  else if(i==n){
		  if(j==1){ M[0][s]=u;
			  M[s][0]=l+u;
			  if(m>1)
			    M[s][s+1]=r;
			  if(n>1)
			    M[s][(i-2)*m+j]=d;
			  M[s][s]=1-u-r-d-l;
		  }
		  else if(j==m){ M[0][s]=r;
			  M[s][0]=r+u;
			  if(m>1)
			    M[s][s-1]=l;
			  if(n>1)
			    M[s][(i-2)*m+j]=d;
			  M[s][s]=1-u-r-d-l;
		  }
		  else{M[s][0]=u;
			  if(n>1)
			    M[s][(i-2)*m+j]=d;			  
			  M[s][s+1]=r;
			  M[s][s-1]=l;			  
			  M[s][s]=1-u-r-d-l;
		  }
	  }
	 
	  else{
		  if(j==1){M[s][0]=l;
			  M[s][i*m+j]=u;
			  if(m>1)
			    M[s][s+1]=r;
			  M[s][(i-2)*m+j]=d;
			  M[s][s]=1-u-r-d-l;
		  }
		  else if(j==m){ M[s][0]=r;
			  M[s][(i+1)*m+j]=u;
			  if(m>1)
			    M[s][s-1]=l;
			  M[s][(i-2)*m+j]=d;
			  M[s][s]=1-u-r-d-l;
		  }
		  else{M[s][i*m+j]=u;
			  M[s][s+1]=r;
			  M[s][(i-2)*m+j]=d;			 
			  M[s][s-1]=l;			  
			  M[s][s]=1-u-r-d-l;
		  }
	  }
	  
	
      }
       
    return M;
  
}

vector<double> generateGridReward(int n,int m){
	return vector<double>(n*m+1,10);
}
