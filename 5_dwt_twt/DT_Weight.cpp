#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    string line;

    getline(file, line);
    
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
    vector<vector<string>> csvData = readCSV("td_Weight.csv");

    if (csvData.empty()) {
        cout << "No data found!" << endl;
        return 1;
    }

    size_t numColumns = csvData[0].size();
    vector<double> totals(numColumns - 1, 0);
    double grandTotal = 0;

    for (const auto& row : csvData) {
        for (size_t i = 1; i < row.size(); ++i) {
            double value = stod(row[i]);
            totals[i - 1] += value;
            grandTotal += value;
        }
    }

    cout << fixed << setprecision(2);
    cout << left << setw(15) << "Region";
    for (size_t j = 1; j < numColumns; ++j) {
        cout << setw(15) << "D-weight " + to_string(j);
    }
    cout << setw(15) << "T-weight" << endl;
    cout << string(15 + 15 * (numColumns - 1), '-') << endl;

    for (size_t i = 0; i < csvData.size(); ++i) {
        cout << left << setw(15) << csvData[i][0];
        double rowTotal = 0;

        for (size_t j = 1; j < csvData[i].size(); ++j) {
            double value = stod(csvData[i][j]);
            double dWeight = value / totals[j - 1];

            cout << setw(15) << dWeight;
            rowTotal += value;
        }

        double tWeight = rowTotal / grandTotal;
        cout << setw(15) << tWeight << endl;
    }

    return 0;
}
