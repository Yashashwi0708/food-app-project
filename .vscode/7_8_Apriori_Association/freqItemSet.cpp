#include <bits/stdc++.h>
using namespace std;

ifstream fin;
double minfre;
vector<set<string>> datatable;
set<string> products;
map<string, int> freq;

vector<string> wordsof(string str) {
    vector<string> tmpset;
    string tmp = "";
    for (char c : str) {
        if (isalnum(c))
            tmp += c;
        else {
            if (!tmp.empty())
                tmpset.push_back(tmp);
            tmp = "";
        }
    }
    if (!tmp.empty())
        tmpset.push_back(tmp);
    return tmpset;
}

string combine(vector<string> &arr, int miss) {
    string str;
    for (int i = 0; i < arr.size(); i++)
        if (i != miss)
            str += arr[i] + " ";
    return str.substr(0, str.size() - 1);
}

set<string> apriori_gen(set<string> &sets, int k) {
    set<string> set2;
    for (auto it1 = sets.begin(); it1 != sets.end(); it1++) {
        auto it2 = next(it1);
        for (; it2 != sets.end(); it2++) {
            vector<string> v1 = wordsof(*it1);
            vector<string> v2 = wordsof(*it2);
            if (equal(v1.begin(), v1.begin() + k - 1, v2.begin())) {
                v1.push_back(v2[k - 1]);
                sort(v1.begin(), v1.end());
                string combined = combine(v1, -1);
                set2.insert(combined);
            }
        }
    }
    return set2;
}

int main() {
    fin.open("transaction.csv", ios::in);
    if (!fin.is_open()) {
        perror("Error in opening file: ");
        return 1;
    }

    cout << "Frequency %: ";
    cin >> minfre;
    string str;

    while (getline(fin, str)) {
        vector<string> arr = wordsof(str);
        set<string> tmpset(arr.begin(), arr.end());
        datatable.push_back(tmpset);
        for (const string &item : tmpset) {
            products.insert(item);
            freq[item]++;
        }
    }
    fin.close();

    cout << "No of transactions: " << datatable.size() << endl;
    minfre = minfre * datatable.size() / 100;
    cout << "Min frequency: " << minfre << endl;

    vector<pair<int, set<string>>> all_frequent_itemsets;

    for (auto it = products.begin(); it != products.end();) {
        if (freq[*it] < minfre) {
            it = products.erase(it);
        } else {
            ++it;
        }
    }

    int pass = 1;
    cout << "\nFrequent " << pass << " - item set:\n";
    set<string> current_level;
    for (const auto &item : products) {
        cout << "{" << item << "} " << freq[item] << endl;
        current_level.insert("{" + item + "}");
    }
    if (!current_level.empty())
        all_frequent_itemsets.push_back({1, current_level});

    int i = 2;
    set<string> prev = products;

    while (true) {
        set<string> cur = apriori_gen(prev, i - 1);
        if (cur.empty()) break;

        set<string> next_level;
        for (const auto &item : cur) {
            vector<string> arr = wordsof(item);
            int tot = 0;
            for (const auto &transaction : datatable) {
                if (includes(transaction.begin(), transaction.end(), arr.begin(), arr.end())) {
                    tot++;
                }
            }
            if (tot >= minfre) {
                freq[item] = tot;
                next_level.insert("{" + item + "}");
            }
        }

        if (next_level.empty()) break;

        cout << "\n\nFrequent " << i << " - item set:\n";
        for (const auto &item : next_level) {
            cout << item << " " << freq[item.substr(1, item.size() - 2)] << endl;
        }
        all_frequent_itemsets.push_back({i, next_level});

        prev = next_level;
        i++;
    }

    ofstream fw("freqitem_op.txt", ios::out);
    for (const auto &level : all_frequent_itemsets) {
        int k = level.first;
        const set<string>& itemsets = level.second;
        fw << "\nFrequent " << k << " - item set:\n";
        for (const auto &itemset : itemsets) {
            fw << itemset << " " << freq[itemset.substr(1, itemset.size() - 2)] << endl;
        }
    }

    return 0;
}
