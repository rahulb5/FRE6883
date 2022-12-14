#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "Matrix.h"
using namespace std;

namespace fre {
    typedef vector<double> Vector;
    typedef vector<vector<double>> Matrix;
    typedef vector<vector<string>> Table;
    

    Matrix ConstMatrix(double c, int nrow, int ncol)
    {
        Matrix x;
        for(int i=0; i<nrow; i++)
        {
            Vector v;
            for(int j=0; j<ncol; j++)
            {
                v.push_back(c);
            }
            x.push_back(v);
        }
        return x;
    }

	Vector ConstVector(double c,int size)
	{
	   Vector x(size);
        for(Vector::iterator itr=x.begin(); itr!=x.end();++itr)
        {
            *itr = c;
        }
        return x; 
	}
    
    Vector operator*(const Matrix& C, const Vector& V)
    {
        int d = (int)C.size();
        Vector W(d);
        for (int j = 0; j < d; j++)
        {
            W[j] = 0.0;
            for (int l = 0; l < d; l++) W[j] = W[j] + C[j][l] * V[l];
        }
        return W;
    }

    Vector operator+(const Vector& V, const Vector& W)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = V[j] + W[j];
        return U;
    }
    
    Vector operator-(const Vector& V, const Vector& W)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = V[j] - W[j];
        return U;
    }

    Vector operator+(const double& a, const Vector& V)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = a + V[j];
        return U;
    }

    Vector& operator+=(Vector& V, const Vector& W)
    {
        int d = (int)V.size();
        for (int j = 0; j < d; j++) V[j] = V[j] + W[j];
        return V;
    }

    Vector operator*(const double& a, const Vector& V)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = a * V[j];
        return U;
    }

    Vector operator*(const Vector& V, const Vector& W)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = V[j] * W[j];
        return U;
    }

    Vector exp(const Vector& V)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = std::exp(V[j]);
        return U;
    }

    double operator^(const Vector& V, const Vector& W)
    {
        double sum = 0.0;
        int d = (int)V.size();
        for (int j = 0; j < d; j++) sum = sum + V[j] * W[j];
        return sum;
    }

    // overload cout for vector, cout every element in the vector
    ostream& operator<<(ostream& out, Vector& V)
    {
        for (Vector::iterator itr = V.begin(); itr != V.end(); itr++)
            out << *itr << "   ";
        out << endl;
        return out;
    }

    ostream& operator<<(ostream& out, Matrix& W)
    {
        for (Matrix::iterator itr = W.begin(); itr != W.end(); itr++)
            out << *itr;    // Use ostream & operator<<(ostream & out, Vector & V)
        out << endl;
        return out;
    }
}