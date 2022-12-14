#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <stdexcept> 
#include <iomanip>
#include <vector>
#include <map>
#include <chrono>
#include <sstream> 
#include <typeinfo>
#include "ReadLoadData.h"
#include "Stocks.h"
#include "Group.h"
#include "bootstrap.h"
#include "Matrix.h"

using namespace std;
using namespace std::chrono;
using namespace fre;
// vector output print function 
void print_v(vector<double> v)
{
    auto itr = v.begin();
    for (;itr != v.end(); itr++)
    {
        cout << *itr << " " << fixed << setprecision(3);
        
    }
    cout <<endl;
}

//map and vector of vector prints?? are they included in header files ?
String SetN(int N, map<string, Stocks> &stock_map)
{
//    cout<<"Printing all tickers: ";
    auto itr2 = stock_map.begin();
    
    String skipped_tickers;
    
    for(;itr2 != stock_map.end(); itr2++)
    {
        //Stocks temp = (itr->second);
        //cout<<itr2->first;
        int flag = (itr2->second).SetN(N);
        if(flag == -1)
        {
            skipped_tickers.push_back(itr2->first);
            //cout<<" unable to set"   ;
        }
        //cout<<endl;
    }
    
    cout<<"\nsize inside SetN: "<<skipped_tickers.size()<<endl;
    return skipped_tickers;
}

void CalAbnormalReturns(map<string, Stocks> &stock_map)
{
    auto itr = stock_map.begin();
    for(; itr != stock_map.end(); itr++)
    {
        (itr->second).CalculateAbnormalReturns(stock_map);
    }
    cout<<"Finished AAR\n";
}


int main() 
{
    
    int x,group,test =0;  // menu option and number of days 
    string tick;
    int N=0;
    int integer1;
    
    // map<string, Stocks> GlobalStockMap;
    // LoadEarnings(GlobalStockMap);
    // String DateList = GenerateDates();
    // FetchData(GlobalStockMap);
    // write2file(GlobalStockMap);
    // Stocks Russel;
    // map<string, Stocks> RusselMap;
    // RusselMap["IWV"] = Russel;
    // FetchData(RusselMap);

    map<string, Stocks> GlobalStockMap;
    Stocks Russel;
    
    LoadEarnings(GlobalStockMap);
    
    
    String skipped_tickers;
    
    Groupby_Surprise gobj(&GlobalStockMap);
    cout << "group obj created" << endl;
    gobj.CreateGroups(skipped_tickers);
    cout << "groups created" << endl;
    Table groupTable = gobj.GetGroup_Mapping();
    cout << "Table created" << endl; 
    cout << groupTable[0].size() <<endl ;
    
    //String DateList = GenerateDates();
    GlobalStockMap["IWV"] = Russel;
    
    FetchData(GlobalStockMap);
    CalAbnormalReturns(GlobalStockMap);
    
    
    skipped_tickers = SetN(90,GlobalStockMap);
    
    write2file(GlobalStockMap);
    
    /*
    for (int i = 0; i < groupTable[0].size(); i++)
    {
        cout << groupTable[0][i] << " " << groupTable[1][i] << " " << groupTable[2][i] << endl;
    }
    for(auto it = GlobalStockMap.begin(); it != GlobalStockMap.end(); it++)
    {
        cout << "stock - " << it->first << endl;
    }
    */
     
    //temporary vector 
    vector<double> v;
    v.insert(v.begin(),2.0);
    v.push_back(3.0);
    v.push_back(4.0);
    v.push_back(5.0);
    
    // create object of bootstrap or our stock object with our data Bootstrap test;
    
    while(true)
    {
    cout<<"\t \t \t :Menu: "<<endl;
    cout<<"1) Enter N to retrieve 2N+1 historical prices for all stocks."<<endl;
    cout<<"2) Pull information for one stock from one group. "<<endl;
    cout<<"3) Display AAR, AAR-std, CAAR, CAAR-std for one group. "<<endl;
    cout<<"4) Display gnuplot plot for CAAR of all 3 groups.  "<<endl;
    cout<<"5) Exit program "<<endl<<endl;
    cout<<"Please select appropriate option number:  "<<endl;;
    cin>>x;
   
    
    // switch structure for each option
   
    
        switch(x)
        {
            case 1:
            {
                cout << "option 1 selected" << endl;
                cout << "N set for all stocks" << endl;
                while(test==0)
                {
                    cout<<"Enter N value between 60 and 90: "<<endl;
                    cin>>N;
                    integer1 = (int) N;
                    skipped_tickers = SetN(integer1, GlobalStockMap);
                    gobj.CreateGroups(skipped_tickers);
                    
                    if (integer1 != N)
                    {
                        cout<<"Error, please enter an integer value error"<<endl;
                        cin.clear();
                    }
                    else if(N>=60 && N<=90 && !cin.fail())  // n is an int 
                    {
                        cout<<"Check for case working cout "<<endl;
                        // object.load data for stocks
                        auto start = high_resolution_clock::now();
                        // object.operations to return historical data
                        print_v(v);
                        auto stop = high_resolution_clock::now();
                        auto duration = duration_cast<seconds>(stop - start);
                        // object.display
                        cout<<"function execution time in seconds: "<<duration.count()<<endl;
                        test =1;
                        //break;
                    }
                    else
                    {
                        cout<<"Error, Enough data points not found. Please select N above 60 and below 90 "<<endl;
                        cin.clear();
                        //test =1;
                        //cin.ignore(10000,'\n');
                        //break;
                    }
                }
                break;
            }
            case 2:
            {   
                if (N>=60 && N<=90)
                {
                    while(test==0)
                    {
                        cout<<"Please provide ticker of stock: "<<endl;
                        cin>> tick;
                        
                        if(GlobalStockMap.find(tick) != GlobalStockMap.end())
                        {
                            GlobalStockMap[tick].DisplayDetails();
                        }
                        else
                        {
                            cout<<"Please enter a valid ticker\n";
                            test = 1;
                        }
                        
                        
                        if(!cin.fail())
                        {
                            cout<<"check case 2 working "<<tick<<endl;
                            print_v(v);
                            test = 1;
                        }
                        else 
                        { 
                            cout<<"Error, Invalid Input field. Example: Try GOOG ";   // try and error c++ 
                        }
                    }
                }
                else
                {
                    cout<<"Data has not yet been retrieved! Try retrieving data first using Option 1 in Main Menu!"<<endl;
                }
                break;
            }
            case 3:
            {
                Bootstrap boot(&gobj, &GlobalStockMap);
                boot.RunBootstrap();
                cout << "Bootstrap object created" << endl;
                if(N>=60 && N<=90)
                {
                    while(test==0)
                    {
                        cout<<"1) Beat \t 2) Meet \t 3) Miss"<<endl;
                        cout<< "Please select appropriate group number: "<<endl; 
                        cin>>group;
                        int integer2 = (int) group;
                        
                        if(integer2 != group)
                        {
                            cout<<"Invalid Input field"<<endl;
                            cin.clear();
                            
                        }
                        else if(group == 1)   //switch?
                        {
                            cout<<" Beat Estimate Group summary "<<endl;
                            cout<<" Average Abnormal Returns (AAR) "<<endl;
                            //print object.missaar
                            // cout << "' date - " << GlobalStockMap["GOOG"].GetStartIndex() << endl;
                            // Vector vec = boot.AbnormRet("GOOG");
                            // cout << vec << endl;
                    
                            cout << "AAR - Group 1 "<< endl;
                            Vector vec0_mean; 
                            vec0_mean = 100*boot.GetAAR(0);
                            cout << vec0_mean << endl;
                            
                            cout << "AAR_STD - Group 1 "<< endl;
                            Vector vec0_std;
                            vec0_std = 100*boot.GetAAR_STD(0);
                            cout << vec0_std << endl;

                            cout << "AAR - Group 2 "<< endl;
                            Vector vec1_mean; 
                            vec1_mean = 100*boot.GetAAR(1);
                            cout << vec1_mean << endl;

                            cout << "AAR_STD - Group 2 "<< endl;
                            Vector vec1_std;
                            vec1_std = 100*boot.GetAAR_STD(1);
                            cout << vec1_std << endl;
                            
                            cout << "AAR - Group 3 "<< endl;
                            Vector vec2_mean; 
                            vec2_mean = 100*boot.GetAAR(2);
                            cout << vec2_mean << endl;
                            
                            cout << "AAR_STD - Group 3 "<< endl;
                            Vector vec2_std;
                            vec2_std = 100*boot.GetAAR_STD(2);
                            cout << vec2_std<<endl;
                            
                            test = 1;
                        }
                        else if(group == 2)
                        {
                            cout<<" Beat Estimate Group summary "<<endl;
                            cout<<" Average Abnormal Returns (AAR) "<<endl;
                            //print object.missaar
                            print_v(v);
                    
                            cout<<" Average Abnormal Returns standard deviation (AAR-std) "<<endl;
                            //print object.missaarstd
                            print_v(v);
                        
                            cout<<" Cumilative Average Abnormal Returns (CAAR) "<<endl;
                            //print object.misscarr
                            print_v(v);
                        
                            cout<<" Cumilative Average Abnormal Returns (CAAR-std) "<<endl;
                            //print object.misscaarstd
                            print_v(v);
                            test = 1;
                        }
                        else if(group == 3)
                        {
                            cout<<" Meet Estimate Group summary "<<endl;
                            cout<<" Average Abnormal Returns (AAR) "<<endl;
                            //print object.missaar
                            print_v(v);
                
                            cout<<" Average Abnormal Returns standard deviation (AAR-std) "<<endl;
                            //print object.missaarstd
                            print_v(v);
                        
                            cout<<" Cumilative Average Abnormal Returns (CAAR) "<<endl;
                            //print object.misscarr
                            print_v(v);
                        
                            cout<<" Cumilative Average Abnormal Returns (CAAR-std) "<<endl;
                            //print object.misscaarstd
                            print_v(v);
                            test = 1;
                        }
                        else 
                        {
                            cout<<"Error, Please select valid option number"<<endl;
                        }
                    
                    }
                    
                }
                
                else
                {
                    cout<<"Data has not yet been retrieved! Try retrieving data first using Option 1 in Main Menu!"<<endl;
                }
                break;
            }
            case 4:
            {   
                if(N>=60 && N<=90)
                {
                    cout<<"CAAR for all 3 groups "<<endl;
                    //object.plot
                }
                else
                {
                    cout<<"Data has not yet been retrieved! Try retrieving data first using Option 1 in Main Menu!"<<endl;
                }
                break;
            }
            case 5:
            {
                exit(0);
            }
            default: 
                cout<<"Error, Please select valid option "<< endl;
                cin.clear();
                test = 1;
                break;
            
        }
         
    
    

        test = 0;
        
    }
    
    return 0;
     
}