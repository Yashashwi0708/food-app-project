#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

double euclideanDistance(double a, double b) {
    return sqrt(pow((a - b), 2));
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

vector<double> extractUnitsSold(const vector<vector<string>>& data, int columnIndex) {
    vector<double> unitsSold;
    for (size_t i = 1; i < data.size(); ++i) {
        if (!data[i][columnIndex].empty()) {
            unitsSold.push_back(stod(data[i][columnIndex]));
        }
    }
    return unitsSold;
}

vector<vector<double>> initializeDist_Matrix(const vector<double>& data) {
    int n = data.size();
    vector<vector<double>> dist_Matrix(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = euclideanDistance(data[i], data[j]);
            dist_Matrix[i][j] = dist;
            dist_Matrix[j][i] = dist;
        }
    }
    return dist_Matrix;
}

void printMatrix(const vector<vector<double>>& matrix, const vector<int>& indices) {
    int n = matrix.size();
    cout << "\t";
    for (int i = 0; i < n; i++) {
        cout << indices[i] << "\t";
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << indices[i] << "\t";
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void removeCluster(vector<vector<double>>& dist_Matrix, int clusterIndex) {
    dist_Matrix.erase(dist_Matrix.begin() + clusterIndex);
    for (auto& row : dist_Matrix) {
        row.erase(row.begin() + clusterIndex);
    }
}

void agglomerativeClustering(vector<vector<double>>& dist_Matrix, int clusters) {
    int n = dist_Matrix.size();
    vector<vector<int>> clusterGroups(n);
    vector<int> indices(n);
    for (int i = 0; i < n; i++) {
        clusterGroups[i].push_back(i + 1);
        indices[i] = i + 1;
    }

    int currentClusters = n;

    while (currentClusters > clusters) {
        cout << "Distance Matrix at Iteration " << (n - currentClusters) << ":\n";
        printMatrix(dist_Matrix, indices);

        double minDistance = numeric_limits<double>::max();
        int cluster1 = -1, cluster2 = -1;

        for (int i = 0; i < dist_Matrix.size(); i++) {
            for (int j = i + 1; j < dist_Matrix.size(); j++) {
                if (dist_Matrix[i][j] < minDistance) {
                    minDistance = dist_Matrix[i][j];
                    cluster1 = i;
                    cluster2 = j;
                }
            }
        }

        if (cluster1 == -1 || cluster2 == -1) break;

        cout << "Merging: ";
        for (int book : clusterGroups[cluster1]) {
            cout << "Book " << book << " ";
        }
        cout << "with ";
        for (int book : clusterGroups[cluster2]) {
            cout << "Book " << book << " ";
        }
        cout << endl;

        clusterGroups[cluster1].insert(clusterGroups[cluster1].end(), clusterGroups[cluster2].begin(), clusterGroups[cluster2].end());
        clusterGroups.erase(clusterGroups.begin() + cluster2);

        for (int i = 0; i < dist_Matrix.size(); i++) {
            if (i != cluster1) {
                dist_Matrix[cluster1][i] = min(dist_Matrix[cluster1][i], dist_Matrix[cluster2][i]);
                dist_Matrix[i][cluster1] = dist_Matrix[cluster1][i];
            }
        }

        removeCluster(dist_Matrix, cluster2);
        indices.erase(indices.begin() + cluster2);
        currentClusters--;
    }

    cout << "\nFinal cluster labels:\n";
    for (int i = 0; i < clusterGroups.size(); i++) {
        cout << "Cluster " << i + 1 << ": ";
        for (int book : clusterGroups[i]) {
            cout << "Book " << book << " ";
        }
        cout << endl;
    }
}

int main() {
    string filename = "books_sales.csv";
    vector<vector<string>> data = readCSV(filename);
    if (data.empty()) return -1;

    int unitsSoldColumnIndex = 0;
    vector<double> unitsSold = extractUnitsSold(data, unitsSoldColumnIndex);
    if (unitsSold.empty()) return -1;

    vector<vector<double>> dist_Matrix = initializeDist_Matrix(unitsSold);

    int Clust_nums=1;

    agglomerativeClustering(dist_Matrix, Clust_nums);
    return 0;
}