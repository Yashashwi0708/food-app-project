#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

double calculateMedian(vector<double> &data, int start, int end) {
    int n = end-start+1;
    if (n % 2 == 0) {
        return (data[start+ n/2-1] + data[start +n/2])/2.0;
    } else {
        return data[start+ n/2];
    }
}

void readCSV(const string &filename, vector<double> &data) {
    ifstream file(filename);
    string line, value;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, value, ',');
        data.push_back(stod(value)); 
    }
}

void calculateFiveNumberSummary(vector<double> &data) {
    sort(data.begin(), data.end());

    int n = data.size();
    double min = data[0];             
    double max = data[n-1];         
    double Q1 = calculateMedian(data, 0, n/2-1);  
    double Q2 = calculateMedian(data, 0, n-1);
    double Q3;

    if (n % 2 == 0) {
        Q3 = calculateMedian(data, n/2, n-1);     
    } else {
        Q3 = calculateMedian(data, n/2+1, n-1); 
    }
    double IQR = Q3-Q1;  
    // cout<<Q1<<" "<<Q3<<" "<<IQR<<"\n";

    double lowerBound = Q1-1.5 * IQR;
    double upperBound = Q3+1.5 * IQR;

    cout<<"5-Number Summary:"<<endl;
    cout<<"Minimum: "<<min<<endl;
    cout<<"Q1 (First Quartile): "<<Q1<<endl;
    cout<<"Median (Q2): "<<Q2<<endl;
    cout<<"Q3 (Third Quartile): "<<Q3<<endl;
    cout<<"Maximum: "<<max<<endl;
    cout<<"Interquartile Range (IQR): "<<IQR<<endl;

    cout<<"\nOutliers: ";
    bool hasOutliers = false;
    // cout<<lowerBound<<" "<<upperBound<<"\n";
    for (double val : data) {
        if (val<lowerBound || val>upperBound) {
            cout<<val<<" ";
            hasOutliers = true;
        }
    }
    if (!hasOutliers) {
        cout<<"None";
    }
    cout<<endl;
}

int main() {
    vector<double> data;

    string filename = "box_Plot.csv";
    readCSV(filename, data);

    if (data.size()>0) {
        calculateFiveNumberSummary(data);
    } else {
        cout<<"No data available in the CSV file."<<endl;
    }

    return 0;
}
