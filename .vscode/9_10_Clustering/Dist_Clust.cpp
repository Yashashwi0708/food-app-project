#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;


vector<double> computeCenter(const vector<vector<double>>& points) {
    int dimensions = points[0].size();
    vector<double> center(dimensions, 0.0);

    for (const auto& point : points) {
        for (int i = 0; i < dimensions; ++i) {
            center[i] += point[i];
        }
    }

    for (int i = 0; i < dimensions; ++i) {
        center[i] /= points.size();
    }

    return center;
}

double calculateDistance(const vector<double>& point1, const vector<double>& point2) {
    double distance = 0.0;
    for (int i = 0; i < point1.size(); ++i) {
        distance += pow(point1[i] - point2[i], 2);
    }
    return sqrt(distance);
}

void printDistanceMatrix(const vector<double>& distances) {
    int n = distances.size();
    cout << "\nLower Triangular Distance Matrix:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            if (i == j)
                cout << setw(10) << fixed << setprecision(4) << 0.0;
            else
                cout << setw(10) << fixed << setprecision(4) << distances[j];
        }
        cout << endl;
    }
}

vector<vector<double>> readCSV(const string& filename) {
    vector<vector<double>> points;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<double> point;
        while (getline(ss, token, ',')) {
            point.push_back(stod(token));
        }
        points.push_back(point);
    }

    return points;
}

int main() {
    srand(time(nullptr));

    string filename = "points.csv";

    vector<vector<double>> points = readCSV(filename);

    int randomIndex = rand() % points.size();
    vector<double> initialCenter = points[randomIndex];
    
    cout << "Initial Cluster Center: ";
    for (const auto& val : initialCenter)
        cout << val << " ";
    cout << endl;

    vector<double> finalCenter = computeCenter(points);
    
    cout << "Final Cluster Center: ";
    for (const auto& val : finalCenter)
        cout << val << " ";
    cout << endl;

    vector<double> distances;
    for (const auto& point : points) {
        distances.push_back(calculateDistance(point, finalCenter));
    }

    printDistanceMatrix(distances);

    return 0;
}
