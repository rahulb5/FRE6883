# pragma once
#include "Group.h"
#include "Matrix.h"
#include "Stocks.h"
#include "bootstrap.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;
namespace fre
{
    class Bootstrap
    {
        private:
            int MCN = 10;   // no. of bootstrap (monte carlo) iterations 
            int M = 80;     // size of each sample from a group 
            int T; //number of days (2N)
            Group* GroupPtr;
            map <string, Stocks>* MapPtr;
            
            //populated as part of output of Bootstrap  
            Matrix AAR_STD; //group x time   
            Matrix CAAR_STD; //group x time   
            Matrix Avg_AAR; //group x time   
            Matrix Avg_CAAR; //group x time   
    
        public:
            Bootstrap() {};
            Bootstrap(Group* GroupPtr_, map<string, Stocks>* myMap_, int n);
            ~Bootstrap(){}
            
            Vector Cal_AAR(const vector<string>& sample); // return AAR calculation across sample stocks (of 1 sample) for 2N timesteps (2N x 1)
            Vector Cal_CAAR(const Vector& AAR); // return CAAR calculation across sample stocks (of 1 sample) for 2N timesteps (2N x 1)
     
            
            void SetMCN(int N_);
            void SetM(int M_);
            void RunBootstrap(); // return
            void plotResults();
            
            String generateSample(int gr_n);
            // Vector VSQRT(const Vector &V);
            Vector cumsum(const Vector& V);
            Vector AbnormRet(string ticker);
            Vector Cal_AAR(int gr_n);
            
            
            // Getter functions
            Vector GetAAR(int gr_index) const;
            Vector GetAAR_STD(int gr_index) const;
            Vector GetCAAR(int gr_index) const;
            Vector GetCAAR_STD(int gr_index) const;
            
    };
    
    
}