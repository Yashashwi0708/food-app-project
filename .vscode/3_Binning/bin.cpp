#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>

using namespace std;

void readCSV(const string &filename, vector<int> &data) {
    ifstream file(filename);
    string line, value;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, value, ',');
        data.push_back(stoi(value)); 
    }
}

double calculateMedian(vector<int>& bin) {
    if (bin.empty()) return 0.0;
    size_t size = bin.size();
    sort(bin.begin(), bin.end());
    if (size % 2 == 0) {
        return (bin[size / 2 - 1] + bin[size / 2]) / 2.0;
    } else {
        return bin[size / 2];
    }
}

void printBins(vector<vector<int>>& bins) {
    for (int i = 0; i < bins.size(); i++) {
        cout << "Bin " << i + 1 << ": ";
        for (int val : bins[i]) {
            cout << val << " ";
        }
        cout << endl;
    }
}

void binByDepth(vector<int>& data, int numBins) {
    vector<vector<int>> bins;
    int binSize = data.size() / numBins;
    int remainder = data.size() % numBins;

    cout << "Bin by Depth:\n";
    int index = 0;
    for (int i = 0; i < numBins; i++) {
        int currentBinSize = binSize + (i < remainder ? 1 : 0);
        vector<int> bin(data.begin() + index, data.begin() + index + currentBinSize);
        bins.push_back(bin);
        index += currentBinSize;
    }
    printBins(bins);
}

void binByMean(vector<int>& data, int numBins) {
    vector<vector<int>> bins;
    int binSize = data.size() / numBins;
    int remainder = data.size() % numBins;

    cout << "\nBin by Mean:\n";
    int index = 0;
    for (int i = 0; i < numBins; i++) {
        int currentBinSize = binSize + (i < remainder ? 1 : 0);

        vector<int> bin(data.begin() + index, data.begin() + index + currentBinSize);

        int mean = accumulate(bin.begin(), bin.end(), 0) / bin.size();
        
        for (int& val : bin) val = mean;
        
        bins.push_back(bin);
        index += currentBinSize;
    }
    printBins(bins);
}

void binByMedian(vector<int>& data, int numBins) {
    vector<vector<int>> bins;
    int binSize = data.size() / numBins;
    int remainder = data.size() % numBins;

    cout << "\nBin by Median:\n";
    int index = 0;
    for (int i = 0; i < numBins; i++) {
        int currentBinSize = binSize + (i < remainder ? 1 : 0);
        vector<int> bin(data.begin() + index, data.begin() + index + currentBinSize);

        double median = calculateMedian(bin);
        
        for (int& val : bin) val = median;
        
        bins.push_back(bin);
        index += currentBinSize;
    }
    printBins(bins);
}

void binByBoundaries(vector<int>& data, int numBins) {
    vector<vector<int>> bins(numBins);
    int minVal = *min_element(data.begin(), data.end());
    int maxVal = *max_element(data.begin(), data.end());
    double binWidth = static_cast<double>(maxVal - minVal) / numBins;

    cout << "\nBin by Boundaries:\n";
    for (int val : data) {
        int binIndex = static_cast<int>((val - minVal) / binWidth);
        if (binIndex >= numBins) {
            binIndex = numBins - 1;
        }
        bins[binIndex].push_back(val);
    }

    for(int bin=0;bin<bins.size();bin++){
        int minBin = *min_element(bins[bin].begin(),bins[bin].end());
        int maxBin = *max_element(bins[bin].begin(),bins[bin].end());

        for(int it=0;it<bins[bin].size();it++){
            int eachBin=bins[bin][it];
            int distFromMin=abs(eachBin-minBin);
            int distFromMax=abs(eachBin-maxBin);

            if(distFromMax<distFromMin){
                bins[bin][it] = maxBin;
            }
            else{
                bins[bin][it] = minBin;
            }
        }
    }

    printBins(bins);
}

int main() {
    vector<int> data;
    string filename = "data.csv"; 
    readCSV(filename, data);
    
    if (data.empty()) {
        cerr << "No data read from the file." << endl;
        return 1; 
    }

    int numBins;
    cout<<"Enter number of bins: ";
    cin>>numBins; 

  
    sort(data.begin(), data.end());

    binByDepth(data, numBins);
    binByMean(data, numBins);
    binByMedian(data, numBins);
    binByBoundaries(data, numBins);

    return 0;
}
