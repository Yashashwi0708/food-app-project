#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    string line;
   
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<string> row;

        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        
        data.push_back(row);
    }

    return data;
}

int main() {
    vector<vector<string>> temp = readCSV("td_Weight.csv");

    int numColumns = temp[0].size();
    vector<double> columnTotals(numColumns - 1, 0.0); 
    vector<double> rowTotals(temp.size(), 0.0); 
    double grandTotal = 0.0; 

    vector<vector<string>> csvData;
    vector<string> header;

    header = temp[0];
    for(int i = 1; i < temp.size(); i++) {
        csvData.push_back(temp[i]);
    }


    for (int i = 0; i < csvData.size(); ++i) {
        double rowSum = 0.0;
        for (int j = 1; j < csvData[i].size(); ++j) {
            double value = stod(csvData[i][j]);
            columnTotals[j - 1] += value;
            rowSum += value; 
        }
        rowTotals[i] = rowSum; 
        grandTotal += rowSum; 
    }



    cout << fixed << setprecision(2);
    cout << left << setw(20) << header[0];
    for (int j = 1; j < header.size(); ++j) {
        cout << setw(30) << "D-weight " + header[j];
        cout << setw(15) << "T-weight " + header[j];
    }
    cout << endl;
    cout << string(15 + 30 * (numColumns - 1), '-') << endl;

    for (int i = 0; i < csvData.size(); ++i) {
        cout << left << setw(15) << csvData[i][0];

        for (int j = 1; j < csvData[i].size(); ++j) {
            double value = stod(csvData[i][j]);

            double dWeight = (value / columnTotals[j - 1]) * 100;

            double tWeight = (value / rowTotals[i]) * 100;

            cout << setw(15) << dWeight << setw(15) << tWeight;
        }
        cout << endl;
    }

    return 0;
}