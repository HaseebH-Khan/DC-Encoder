#pragma once

#include <iostream>
#include <vector>

using namespace std;
//0 is - and 1 is + except for ami where 0 is neutral, 1 is + and -1 is -

vector<int> nrzl_encoder(vector<int>& bin_data) {
    vector<int> encoded_data;
    for (unsigned int i = 0; i < bin_data.size(); i++) {
        encoded_data.push_back(bin_data[i]);
    }
    return encoded_data;
}

vector<int> nrzi_encoder(vector<int>& bin_data) {
    vector<int> encoded_data;
    cout << "Choose initial value of previous bit (0 or 1): ";
    int prev;
    cin >> prev;
    for (unsigned int i = 0; i < bin_data.size(); i++) {
        if (bin_data[i] == 1) {
            prev = !prev;
        }
        encoded_data.push_back(prev);
    }
    return encoded_data;
}

vector<int> manchester_encoder(vector<int>& bin_data) {
    //time period halves
    vector<int> encoded_data;
    for (unsigned int i = 0; i < bin_data.size(); i++) {
        if (bin_data[i] == 0) {
            encoded_data.push_back(0);
            encoded_data.push_back(1);
        }
        else {
            encoded_data.push_back(1);
            encoded_data.push_back(0);
        }
    }
    return encoded_data;
}

vector<int> differential_manchester_encoder(vector<int>& bin_data) {
    //time period halves
    int prev = 0;
    vector<int> encoded_data;
    for (unsigned int i = 0; i < bin_data.size(); i++) {
        if (bin_data[i] == 0) {
            encoded_data.push_back(prev);
            encoded_data.push_back(!prev);
        }
        else {
            encoded_data.push_back(!prev);
            encoded_data.push_back(prev);
            prev = !prev;
        }
    }
    return encoded_data;
}

// ami encoder
vector<int> ami_encoder(vector<int>& bin_data) {
    //3 voltage levels
    //0 : neutral
    //1 : alternate +/-
    vector<int> encoded_data;
    int prev_polarity = 1; //assumed to be 1
    for (unsigned int i = 0; i < bin_data.size(); i++) {
        if (bin_data[i] == 0) {
            encoded_data.push_back(0);
        }
        else {
            (prev_polarity == 1) ? encoded_data.push_back(1) : encoded_data.push_back(-1);
            prev_polarity = -prev_polarity;
        }
    }
    return encoded_data;
}