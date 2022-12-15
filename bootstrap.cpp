#include <stdio.h>
#include <cstring>
#include <string> 
#include <iostream>
#include <vector>
#include <locale>
#include <iomanip>
#include <cassert>
#include <map>
#include <chrono>
#include <time.h>
#include <random>
#include<algorithm>
#include<iterator>
#include "Stocks.h"
#include "bootstrap.h"
#include "Matrix.h"
#include "Group.h"

namespace fre
{
    Bootstrap::Bootstrap(Group* GroupPtr_, map<string, Stocks>* MapPtr_, int n): GroupPtr(GroupPtr_),MapPtr(MapPtr_)
    {
        T = 2*n;
             
    }

    String Bootstrap :: generateSample(int gr_n) // To be unit tested - with integration 
    {
        int max_size = (GroupPtr->GetGroup_Mapping())[gr_n].size();
        String grouptickers(max_size);
        grouptickers = (GroupPtr->GetGroup_Mapping())[gr_n];
        String W(M);
        vector<int> numbers;
        vector<int> sample;
        for (int i = 0; i < max_size; i++) {numbers.push_back(i);}
	    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	    std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));
    	for (int i = 0; i < M; i++)
    	{
    		sample.push_back(numbers[i]);
    	}
    	for (int i = 0; i < (int)sample.size(); i++)
    	{
    		W[i] = grouptickers[sample[i]];  // operator overloading
            //cout << W[i] << endl;
    	}
	    return W;
    }
    

    // Vector Bootstrap :: VSQRT(const Vector &V)    // unit tested
    // {
    //     int d = (int)V.size();
    //     Vector U(d);
    //     for (int j = 0; j < d; j++) U[j] = sqrt(V[j]);
    //     return U;
    // }
    
    // return CAAR calculation across sample stocks (of 1 sample) for 2N timesteps (2N x 1)
    Vector Bootstrap :: cumsum(const Vector& V) // unit tested
    {
        int d = (int)V.size();
        Vector U(d);
        U[0] = V[0];
        for(int j = 1; j < d; j++)
        {
            U[j] = U[j-1] + V[j];
        }
        return U;
    }
    
    
    Vector Bootstrap :: AbnormRet(string ticker) // To be unit tested - with integration 
    {
        Vector AbnormReturn = ConstVector(0,T);
        int start, end;
        start = (*MapPtr)[ticker].GetStartIndex();
        end = (*MapPtr)[ticker].GetEndIndex();
        cout << start - end << endl;
        if ((end - start) != T){
            cout << "Corrupt asset:  " << start << "  " << end << endl;
            
            return AbnormReturn;
        }

        Vector R = (*MapPtr)[ticker].GetReturns();
        Vector Benchmark = (*MapPtr)["IWV"].GetReturns();
        for(int i=0; i<T; i++)
        {
         //cout << "TIMEPERIOD " << i << endl;
         // cout << "start + i = " <<  start+i << "  " << R[start+1+i] << "  " << Benchmark[start+1+i] << endl;
         AbnormReturn[i] = R[start+i] - Benchmark[start+i];
         // cout << AbnormReturn[i] << endl;
        }
        return AbnormReturn;
    }
 
    // return AAR calculation across sample stocks (of 1 sample) for 2N timesteps (2N x 1)
    Vector Bootstrap :: Cal_AAR(int gr_n) // To be unit tested - with integration 
    {
        cout << "Entering Cal_AAR function." << endl;
        String sample(M);
        
        cout<<"generating sample\n";
        Matrix Ave = ConstMatrix(0,M,T);
        sample = generateSample(gr_n);
        cout<<"finished sampling\n";
        for(int i = 0; i< M; i++)
        {
            cout << "STARTED STOCK " << sample[i] << endl;
            Vector tmp = AbnormRet(sample[i]);
            for (int k = 0; k< T; k++){
                Ave[i][k] = tmp[k];
            }
            //cout << "CALCULATED ABONORMAL RETURN FOR STOCK " << i << endl;
            //cout << "FINISHED STOCK "<< i << endl;
        }
        Vector average_AAR(T);
        for (int i = 0; i < T; i++){
            for (int k = 0; k < M; k++ ){
                average_AAR[i] += Ave[k][i];
            }
            average_AAR[i] = average_AAR[i] / M;
        }
        cout << "Exiting Cal_AAR function. " << endl;
        return average_AAR;
        
    }

    //number of timesteps: 2N)
    void Bootstrap :: RunBootstrap()  // To be unit tested - with integration 
    {
        int N_Group = GroupPtr->GetN(); // number of groups. In this case - 3
        //initialize result matrices to 0s
        Avg_AAR = ConstMatrix(0,N_Group,T);     //group x time 
        Avg_CAAR = ConstMatrix(0,N_Group,T);    //group x time 
        AAR_STD = ConstMatrix(0,N_Group,T);   //group x time 
        CAAR_STD = ConstMatrix(0,N_Group,T); //group x time 
        
        cout << "Started Run Bootstrap" << endl;

        for(int n = 0; n < N_Group; n++) //iterate through each group
        {
            Vector Sum_AAR_tmp(T), Sum_CAAR_tmp(T);
            for(int i = 0;i < MCN; i++) //iterate through each monte carlo iteration (i.e. Bootstrap iteration 1-40) 
            {
                Vector AAR_tmp(T), CAAR_tmp(T);
                cout << "n = " << n << ", i = " << i << endl;
                AAR_tmp = Cal_AAR(n);
                Sum_AAR_tmp += AAR_tmp;
                cout << Sum_AAR_tmp << endl;
                AAR_STD[n] = AAR_STD[n] + AAR_tmp*AAR_tmp;
                CAAR_tmp = cumsum(AAR_tmp);
                Sum_CAAR_tmp += CAAR_tmp;
                CAAR_STD[n] += CAAR_tmp*CAAR_tmp;
            }
            cout << Sum_AAR_tmp << endl;
            double one_by_MCN = (double)(1.0/MCN);
            cout << one_by_MCN << endl;
            for(int j=0;j<T;j++)
            {
                Avg_AAR[n][j] = one_by_MCN*Sum_AAR_tmp[j]; 
                Avg_CAAR[n][j] = one_by_MCN*Sum_CAAR_tmp[j];
                AAR_STD[n][j] = sqrt((one_by_MCN*AAR_STD[n][j] - Avg_AAR[n][j]*Avg_AAR[n][j]));
                CAAR_STD[n][j] = sqrt(one_by_MCN*CAAR_STD[n][j] - Avg_CAAR[n][j]*Avg_CAAR[n][j]);
            }
            cout << Avg_AAR[n] << endl;
            // Avg_AAR[n] = (1/MCN)*Avg_AAR[n]; 
            // Avg_CAAR[n] = (1/MCN)*Avg_CAAR[n];
            // AAR_STD[n] = VSQRT((1/(MCN)*AAR_STD[n] - Avg_AAR[n]*Avg_AAR[n]));
            // for (int i = 0; i < T; i ++){
            //     cout << "Avg_AAR["<<i<<"]: " << Avg_AAR[i] << "AAR_STD["<<i<<"]: " << AAR_STD[i] << endl;
            // }
            // CAAR_STD[n] = VSQRT((1/(MCN))*CAAR_STD[n] - Avg_CAAR[n]*Avg_CAAR[n]); 
        }
    }
    
    Vector Bootstrap::GetAAR(int gr_index) const
    {
        return Avg_AAR[gr_index];
    }
    Vector Bootstrap::GetAAR_STD(int gr_index) const
    {
        return AAR_STD[gr_index];
    }
    Vector Bootstrap::GetCAAR(int gr_index) const
    {
        return Avg_CAAR[gr_index];
    }
    Vector Bootstrap::GetCAAR_STD(int gr_index) const
    {
        return CAAR_STD[gr_index];
    }
    
    void Bootstrap :: plotResults() 
    {
        
        FILE *gnuplotPipe,*tempDataFile;
        
        int N = (int) T/2;
        
        const char *tempDataFileName1 = "Beat";
        const char *tempDataFileName2 = "Meet";
        const char *tempDataFileName3 = "Miss";
        
        //set up the gnu plot
        gnuplotPipe = popen("/usr/bin/gnuplot", "w");
        fprintf(gnuplotPipe,"set terminal png size 800,600; set output 'Results.png'\n");
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "set title 'Avg CAAR for 3 groups'\n");
        fprintf(gnuplotPipe, "set arrow from 0,graph(0,0) to 0,graph(1,1) nohead lc rgb 'red'\n");
        fprintf(gnuplotPipe, "set xlabel 'Announcement Date'\nset ylabel 'Avg CAAR (%)'\n");
        
        if (gnuplotPipe) 
        {
            fprintf(gnuplotPipe,"plot \"%s\" with lines lw 3, \"%s\" with lines lw 3, \"%s\" with lines lw 3 \n",tempDataFileName1, tempDataFileName2, tempDataFileName3);
            fflush(gnuplotPipe);
            
            //plot figure
            tempDataFile = fopen(tempDataFileName1,"w");
            for (int j = -N; j < N; j++)
            {
                fprintf(tempDataFile, "%i %lf\n", j, Avg_CAAR[0][j + N]);
            }
            fclose(tempDataFile);
            
            //plot figure 2
            tempDataFile = fopen(tempDataFileName2,"w");
            for (int j = -N; j < N; j++)
            {
                fprintf(tempDataFile, "%i %lf\n", j, Avg_CAAR[1][j + N]);
            }
            fclose(tempDataFile);
            
            //plot figure 3
            tempDataFile = fopen(tempDataFileName3,"w");
            for (int j = -N; j < N; j++)
            {
                fprintf(tempDataFile, "%i %lf\n", j, Avg_CAAR[2][j + N]);
            }
            fclose(tempDataFile);
            
        } 
        else 
        {        
            printf("gnuplot not found...");    
        }
    }
}



