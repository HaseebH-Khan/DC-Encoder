#pragma once

#include <vector>

using namespace std;

vector<int> b8zs(vector<int>& v) {
    int cnt = 0;
    int curr_polarity = 1;
    for (unsigned int i = 0; i < v.size(); i++) {
        if (v[i] == 0) {
            cnt++;
            if (cnt == 8) {
                v[i - 4] = -curr_polarity; // V
                v[i - 3] = curr_polarity; // B
                v[i - 1] = -curr_polarity; // V
                v[i] = curr_polarity; // B
                cnt = 0;
            }
        }
        else {
            cnt = 0;
            if (v[i] == -1) {
                curr_polarity = -1;
            }
            else if (v[i] == 1) {
                curr_polarity = 1;
            }
        }
    }
    return v;
}

vector<int> hdb3(vector<int>& v) {
    int cnt = 0; // number of consecutive zeros
    int curr_polarity = 1; //assumed to be 1
    bool even_parity = true; // parity of occurrences of non-zero values
    for (unsigned int i = 0; i < v.size(); i++) {
        if (v[i] == 0) {
            cnt++;
            if (cnt == 4) {
                if (even_parity) {
                    v[i - 3] = -curr_polarity; // B
                    v[i] = -curr_polarity; // V
                    curr_polarity = -curr_polarity;
                }
                else {
                    v[i] = curr_polarity; // V
                    even_parity = true;   // parity adjusted
                }
                cnt = 0;
            }
        }
        else {
            cnt = 0;
            even_parity = !even_parity;
            if (v[i] == -1) {
                curr_polarity = -1;
            }
            else if (v[i] == 1) {
                curr_polarity = 1;
            }
        }
    }
    return v;
}