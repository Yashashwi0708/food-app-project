#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>

using namespace std;

double gini(const vector<string> &data) {
    map<string, int> freq;
    for (const string &value : data) {
        freq[value]++;
    }

    double gini_index = 1.0;
    int total = data.size();

    for (const auto &pair : freq) {
        double p = static_cast<double>(pair.second) / total;
        gini_index -= p * p;
    }

    return gini_index;
}

pair<double, double> gini_gain(const vector<string> &target, const vector<string> &attribute) {
    double total_gini = gini(target);

    map<string, vector<string>> subsets;
    for (size_t i = 0; i < attribute.size(); ++i) {
        subsets[attribute[i]].push_back(target[i]);
    }

    double subset_gini = 0.0;
    int total = target.size();

    for (const auto &subset : subsets) {
        double weight = static_cast<double>(subset.second.size()) / total;
        subset_gini += weight * gini(subset.second);
    }

    return {total_gini - subset_gini, subset_gini};
}

vector<string> get_column(const vector<vector<string>> &data, int col_index) {
    vector<string> column;
    for (const auto &row : data) {
        if (col_index < row.size()) {
            column.push_back(row[col_index]);
        }
    }
    return column;
}

vector<vector<string>> load_csv(const string &filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    string line, cell;

    while (getline(file, line)) {
        stringstream line_stream(line);
        vector<string> row;
        while (getline(line_stream, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    return data;
}

int main() {
    string filename = "Info_Gain_Dataset.csv";
    vector<vector<string>> data = load_csv(filename);

    vector<string> attributes = data[0];
    data.erase(data.begin());

    vector<string> target = get_column(data, attributes.size() - 1);
    attributes.pop_back();

    double total_gini = gini(target);

    cout << left << setw(15) << "Attribute"
         << setw(15) << "Gini"
         << setw(20) << "Gini Gain" << endl;

    cout << string(50, '-') << endl;

    for (size_t i = 0; i < attributes.size(); ++i) {
        vector<string> attribute_column = get_column(data, i);

        pair<double, double> g_gain = gini_gain(target, attribute_column);

        cout << left << setw(15) << attributes[i]
             << setw(15) << g_gain.second
             << setw(20) << g_gain.first << endl;
    }

    return 0;
}
