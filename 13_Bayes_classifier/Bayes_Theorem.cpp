#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <iomanip>

using namespace std;

double calculatePrior(string target, vector<vector<string>>& dataset, int colIndex) {
    double count = 0.0;
    for (int i = 0; i < dataset.size(); i++) {
        if (dataset[i][colIndex] == target) {
            count++;
        }
    }
    return dataset.size() > 0 ? count / dataset.size() : 0.0;  // Prevent division by zero
}

double likelihood(string feature, string target, int featureIndex, int targetIndex, vector<vector<string>>& dataset) {
    double countFeatureTarget = 0.0;
    double countTarget = 0.0;
    set<string> uniqueFeatures;

    for (int i = 0; i < dataset.size(); i++) {
        if (dataset[i][targetIndex] == target) {
            countTarget++;
            if (dataset[i][featureIndex] == feature) {
                countFeatureTarget++;
            }
        }
        uniqueFeatures.insert(dataset[i][featureIndex]);
    }

    int uniqueFeatureCount = uniqueFeatures.size();
    
    if (countTarget == 0) return 1.0 / (uniqueFeatureCount + 1); 

    return (countFeatureTarget + 1) / (countTarget + uniqueFeatureCount);
}


string predict(vector<string> data, vector<vector<string>>& dataset) {
    cout << fixed << setprecision(6); 

    double yesProb = calculatePrior("yes", dataset, 4);
    double noProb = calculatePrior("no", dataset, 4);

    cout << "P(Buys_Computer = 'yes') = " << yesProb << endl;
    cout << "P(Buys_Computer = 'no') = " << noProb << endl;

    for (int i = 0; i < data.size(); i++) {
        double yesLikelihood = likelihood(data[i], "yes", i, 4, dataset);
        double noLikelihood = likelihood(data[i], "no", i, 4, dataset);
        
        yesProb *= yesLikelihood;
        noProb *= noLikelihood;

        cout << "Likelihood P(Feature = '" << data[i] << "' | Buys_Computer = 'yes') = " << yesLikelihood << endl;
        cout << "Likelihood P(Feature = '" << data[i] << "' | Buys_Computer = 'no') = " << noLikelihood << endl;
    }

    cout << "Posterior probability of 'yes': " << yesProb << endl;
    cout << "Posterior probability of 'no': " << noProb << endl;

    return yesProb > noProb ? "yes" : "no";
}

vector<vector<string>> readCSV(string filename) {
    ifstream file(filename);
    vector<vector<string>> dataset;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<string> row;

        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        dataset.push_back(row);
    }

    return dataset;
}

int main() {
    string filename = "Bayes.csv";

    vector<vector<string>> dataset = readCSV(filename);

    vector<string> data = {"<=30", "low", "yes", "excellent"};

    string result = predict(data, dataset);
    cout << "Prediction for new data: " << result << endl;

    return 0;
}
