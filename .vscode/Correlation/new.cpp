#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

vector<string> split(const string& line, char delimiter = ',') {
    vector<string> result;
    stringstream ss(line);
    string item;
    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

vector<set<string>> readCSV(const string& filename) {
    vector<set<string>> itemSets;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        vector<string> row = split(line);
        set<string> itemSet(row.begin(), row.end());
        itemSets.push_back(itemSet);
    }
    file.close();
    return itemSets;
}

vector<string> getUniqueItems(const vector<set<string>>& itemSets) {
    set<string> uniqueItems;
    for (const auto& itemSet : itemSets) {
        uniqueItems.insert(itemSet.begin(), itemSet.end());
    }
    return vector<string>(uniqueItems.begin(), uniqueItems.end());
}

vector<vector<int>> createBinaryMatrix(const vector<set<string>>& itemSets, const vector<string>& items) {
    vector<vector<int>> binaryMatrix;
    for (const auto& itemSet : itemSets) {
        vector<int> binaryVector;
        for (const auto& item : items) {
            binaryVector.push_back(itemSet.count(item) ? 1 : 0);
        }
        binaryMatrix.push_back(binaryVector);
    }
    return binaryMatrix;
}

double calculateCorrelation(const vector<int>& x, const vector<int>& y) {
    int n = x.size();
    int sumX = 0, sumY = 0, sumXSquare = 0, sumYSquare = 0, sumXY = 0;
    
    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumXSquare += x[i] * x[i];
        sumYSquare += y[i] * y[i];
        sumXY += x[i] * y[i];
    }

    double numerator = n * sumXY - sumX * sumY;
    double denominator = sqrt((n * sumXSquare - sumX * sumX) * (n * sumYSquare - sumY * sumY));
    
    if (denominator == 0) return NAN;
    return numerator / denominator;
}

int main() {
    string filename = "transaction.csv";

    vector<set<string>> itemSets = readCSV(filename);
    vector<string> items = getUniqueItems(itemSets);
    vector<vector<int>> binaryMatrix = createBinaryMatrix(itemSets, items);

    cout << "Shape of the binary matrix: " << binaryMatrix.size() << " x " << items.size() << endl;

    map<pair<string, string>, double> correlationResults;
    for (size_t i = 0; i < items.size(); i++) {
        for (size_t j = i + 1; j < items.size(); j++) {
            vector<int> x, y;
            for (const auto& row : binaryMatrix) {
                x.push_back(row[i]);
                y.push_back(row[j]);
            }
            double correlation = calculateCorrelation(x, y);
            correlationResults[{items[i], items[j]}] = correlation;
        }
    }

    cout << "Pearson Correlation Coefficients:" << endl;
    for (const auto& [pair, correlation] : correlationResults) {
        cout << pair.first << " and " << pair.second << ": ";
        if (isnan(correlation)) {
            cout << "Undefined" << endl;
        } else {
            cout << fixed << setprecision(4) << correlation << endl;
        }
    }

    return 0;
}
