#pragma once

#include <iostream>
#include <vector>
#include <map>

using namespace std;

namespace fre
{
	typedef vector<string> String;
	typedef vector<double> Vector;
	typedef vector<Vector> Matrix;
	
	vector<string> GenerateDates();
	
	class Stocks
	{
	  protected:
		String Date;
		Vector Open_price;
		Vector High_price;
		Vector Low_price;
		Vector Close_price;
		Vector Adjusted_close;
		Vector volume;
		Vector pct_returns;
		Vector Abnormal_returns;
		Vector Cumulative_returns;
		Matrix Stock_info;
		
		double reported_earnings;
		double estimated_earnings;
		double surprise_earnings;
		double surprise_perecent;
		string earnings_date;
		string start_date;
		string end_date;
		string ticker;
		
		int N;
		int start_index;
		int end_index;
		
	  public:
		Stocks(){}
		Stocks(String d_, Vector op_, Vector hp_, Vector lp_, Vector cp_, Vector acp_, Vector v_ ):Date(d_), Open_price(op_), High_price(hp_), Low_price(lp_), Close_price(cp_), Adjusted_close(acp_), volume(v_) {}
		String GetDate() const { return Date; }
        Vector GetOpen_price() const { return Open_price; }
		Vector GetHigh_price() const { return High_price; }
		Vector GetLow_price() const { return Low_price; }
		Vector GetClose_price() const { return Close_price; }
		Vector GetAdjusted_close() const { return Adjusted_close; }
		Vector Getvolume() const { return volume; }
		Vector GetReturns() const {return pct_returns;}
		Vector GetAbnormalReturns() const { return Abnormal_returns; }
		Vector GetCumulativeReturns() const { return Cumulative_returns; }
		
		void SetDate(const String & dt) { Date = dt; }
		void SetOP(const Vector & OP) { Open_price = OP; }
        void SetHP(const Vector& HP) { High_price = HP; }
        void SetLP(const Vector& LP) { Low_price = LP; }
        void SetCP(const Vector& CP) { Close_price = CP; }
        void SetACP(const Vector& ACP) { Adjusted_close = ACP; }
        void SetVol(const Vector& Vol) { volume = Vol; }
        void CalculateReturns();
        void SetStartIndex(int N_) {start_index = N_;}
        void SetEndIndex(int N_) {end_index = N_;}
        
        
        int SetN(int N_);
        void CalculateAbnormalReturns(map<string, Stocks> &GlobalStockMap);
        void CalculateCumulativeReturns();
        void DisplayDetails();
        
        void SetReportedEarnings(const double& re) { reported_earnings = re;}
        void SetEstimatedEarnings(const double& ee) { estimated_earnings = ee;}
        void SetSurpriseEarnings(const double& se) { surprise_earnings = se;}
        void SetSurprisePerecent(const double& sp) { surprise_perecent = sp;}
        void SetTicker(const string& ticker_) { ticker = ticker_;}
        void SetEarningsDate(const string& earnings_date_) { earnings_date = earnings_date_;}
        int SetDates();
        
        
        double GetReportedEarnings() { return reported_earnings; }
        double GetEstimatedEarnings() { return estimated_earnings; }
        double GetSurpriseEarnings() { return surprise_earnings; }
        double GetSurprisePerecent() { return surprise_perecent; }
        string GetTicker(){ return ticker;}
        string GetEarningsDate() { return earnings_date; }
        string GetStartDate() { return start_date;}
        string GetEndDate() { return end_date;}
        int GetStartIndex() const { return start_index; }
        int GetEndIndex() const { return end_index; }
        int GetN() const {return N; }
	};


}
