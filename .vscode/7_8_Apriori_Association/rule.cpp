#include <bits/stdc++.h>
using namespace std;

map<set<string>, int> freq;

vector<set<string>> generate_subsets(const set<string>& itemset) {
    vector<set<string>> subsets;
    int n = itemset.size();
    vector<string> items(itemset.begin(), itemset.end());

    for (int i = 1; i < (1 << n); i++) {
        set<string> subset;
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) subset.insert(items[j]);
        }
        subsets.push_back(subset);
    }
    return subsets;
}

void generate_rules(double min_confidence) {
    cout << "Association Rules " << min_confidence * 100 << "%:\n";

    for (const auto& itemset_pair : freq) {
        const set<string>& itemset = itemset_pair.first;
        int itemset_support = itemset_pair.second;

        if (itemset.size() < 2) continue;

        vector<set<string>> subsets = generate_subsets(itemset);

        for (const auto& LHS : subsets) {
            if (LHS.size() == itemset.size()) continue;

            set<string> RHS;
            set_difference(itemset.begin(), itemset.end(), LHS.begin(), LHS.end(), inserter(RHS, RHS.begin()));

            int lhs_support = freq[LHS];
            double confidence = (double)itemset_support / lhs_support;

            if (confidence >= min_confidence) {
                cout << "{ ";
                for (const string& item : LHS) cout << item << " ";
                cout << "} => { ";
                for (const string& item : RHS) cout << item << " ";
                cout << "} (Confidence: " << confidence * 100 << "%)\n";
            }
        }
    }
}

void read_frequent_itemsets(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.find("Frequent") != string::npos || line.empty()) continue;

        size_t brace_pos = line.find('{');
        size_t end_brace_pos = line.find('}');
        string items_str = line.substr(brace_pos + 1, end_brace_pos - brace_pos - 1);

        set<string> itemset;
        istringstream item_stream(items_str);
        string item;
        while (item_stream >> item) {
            itemset.insert(item);
        }

        int support = stoi(line.substr(end_brace_pos + 1));
        freq[itemset] = support;
    }

    file.close();
}

int main() {
    string filename = "freqitem_op.txt";
    double min_confidence;

    cout << "Enter minimum confidence (in %): ";
    cin >> min_confidence;
    min_confidence /= 100.0;

    read_frequent_itemsets(filename);

    generate_rules(min_confidence);

    return 0;
}
