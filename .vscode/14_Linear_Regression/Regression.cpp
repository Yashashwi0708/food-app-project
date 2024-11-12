#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void readCSV(const string &filename, vector<double> &X, vector<double> &Y) {
    ifstream file(filename);
    string line, value;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string xValue, yValue;

        getline(ss, xValue, ',');
        getline(ss, yValue, ',');
        
        X.push_back(stod(xValue));
        Y.push_back(stod(yValue));
    }
}

int main() {
    vector<double> X, Y;

    string filename = "Linear_Regression.csv";
    readCSV(filename, X, Y);
    int n = X.size();

    double sum_X = 0, sum_Y = 0;
    for (int i = 0; i < n; i++) {
        sum_X += X[i];
        sum_Y += Y[i];
    }
    double mean_X = sum_X / n;
    double mean_Y = sum_Y / n;

    double num = 0, denom = 0;
    for (int i = 0; i < n; i++) {
        num += (X[i] - mean_X) * (Y[i] - mean_Y);
        denom += (X[i] - mean_X) * (X[i] - mean_X);
    }
    double m = num / denom;
    double c = mean_Y - m * mean_X;

    cout << "Slope: " << m << endl;
    cout << "Intercept: " << c << endl;

    double new_X;
    cout << "Enter years of experience to predict salary: ";
    cin >> new_X;
    double predicted_Y = c + m * new_X;

    cout << "Predicted Salary for " << new_X << " years of experience: " << predicted_Y << "k" << endl;

    return 0;
}
