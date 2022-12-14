#pragma once
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include "Stocks.h"
using namespace std;
using namespace fre;

typedef vector<double> Vector;
typedef vector<vector<double>> Matrix;
typedef vector<vector<string>> Table;
typedef map<string,Stocks> Map;

class Group
{
    protected:
        //data members
        Table Group_Mapping; //table of tickers
        Map* stockMapPtr;
        int N_group;
        vector<string> Group_Names;
        
        bool (*cmp)(Stocks& a, Stocks& b);
        
        //constructor
        Group(Map* data_);
        
        //destructor
        virtual ~Group();
        
    public:
        //get, set number of groups
        void SetN(int n);
        int GetN() const;
        
        //get, set group names
        void SetGroupNames(vector<string>& names);
        vector<string> GetGroupNames() const;
        
        //get, set stock map
        Map GetStockMap() const;
        void UpdateStockMap(Map* stockMapPtr_);
        
        //get Group Vector Pointer
        Table GetGroup_Mapping();
        
        //create Groups
        virtual void CreateGroups(String skipped_tickers) = 0;
        virtual void CreateGroups(int n, String skipped_tickers) = 0;
        virtual void CreateGroups(Map* stockMapPtr_, int n, String skipped_tickers) = 0;
        
        //overloaded subscript operator to return by group name
        vector<string> operator[] (const string& groupname);
};

class Groupby_Surprise: public Group
{
    //comparator to sort stocks by attribute
    //protected:
        //bool compare(Stocks& a,Stocks& b);
        
    public:
        //constructor
        Groupby_Surprise(Map* stockMapPtr_);
        
        //destructor
        ~Groupby_Surprise();
        
        //create groups
        void CreateGroups(String skipped_tickers);
        void CreateGroups(int n, String skipped_tickers);
        void CreateGroups(Map* StockMapPtr_, int n,String skipped_tickers);
        
        //friend function
        friend bool compare(Stocks& a, Stocks& b);
};