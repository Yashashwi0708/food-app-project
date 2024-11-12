#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <fstream>  
#include <iomanip> 

using namespace std;

vector<double> zScoreNormalization(const vector<double>& data) {
    double sum = 0, mean, stddev = 0;
    for (double val : data) {
        sum += val;
    }
    mean = sum / data.size();
    for (double val : data) {
        stddev += pow(val - mean, 2);
    }
    stddev = sqrt(stddev / data.size());
    vector<double> zScoreData;
    for (double val : data) {
        double zScoreVal = (val - mean) / stddev;
        zScoreData.push_back(zScoreVal);
    }
    return zScoreData;
}

vector<double> minMaxNormalization(const vector<double>& data) {
    int new_min = 1, new_max = 10;
    double minVal = *min_element(data.begin(), data.end());
    double maxVal = *max_element(data.begin(), data.end());

    vector<double> normalizedData;

    for (double val : data) {
        double normVal = (((val - minVal) * (new_max - new_min)) / (maxVal - minVal)) + new_min;
        normalizedData.push_back(normVal);
    }

    return normalizedData;
}

vector<vector<string>> readCSV(const string& filename) {
    ifstream file(filename);
    vector<vector<string>> data;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> row;
        string e;

        while (getline(ss, e, ',')) {
            row.push_back(e);
        }

        data.push_back(row);
    }

    return data;
}

int main() {
    string filename = "samsung_sales.csv"; 

    vector<vector<string>> csvData = readCSV(filename);
    vector<double> samsungSales;

    for(auto i:csvData){
        for(auto j:i){
            cout<<j<<" ";
        }
        cout<<endl;
    }

    for (const vector<string>& row : csvData) {
        if (!row.empty()) {
            try {
                samsungSales.push_back(stod(row[0]));
            } catch (const invalid_argument& e) {
                cout<<row[0]<<" Hello";
                cerr << "Error: Invalid value in CSV file: " << e.what() << endl;
            }
        }
    }

    vector<double> minMaxNormalized = minMaxNormalization(samsungSales);

    vector<double> zScoreNormalized = zScoreNormalization(samsungSales);

    cout << setw(15) << left << "Original Values" << setw(20) << left << "Min-Max Normalized" << setw(20) << left << "Z-Score Normalized" << endl;
    cout << string(55, '-') << endl;

    for (size_t i = 0; i < samsungSales.size(); ++i) {
        cout << setw(15) << left << samsungSales[i] << setw(20) << left << fixed << setprecision(2) << minMaxNormalized[i] << setw(20) << left << fixed << setprecision(2) << zScoreNormalized[i] << endl;
    }

    return 0;

























    
}