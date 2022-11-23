#pragma once

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <process.h>
#include <vector>
/*
// Problem2: length of fixed subsequence will be n-2 because of header and tailer of inversed bit
*/

using namespace std;

// COURTESY: Robert Jenkins' 96 bit Mix Function
unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a = a - b;  a = a - c;  a = a ^ (c >> 13);
    b = b - c;  b = b - a;  b = b ^ (a << 8);
    c = c - a;  c = c - b;  c = c ^ (b >> 13);
    a = a - b;  a = a - c;  a = a ^ (c >> 12);
    b = b - c;  b = b - a;  b = b ^ (a << 16);
    c = c - a;  c = c - b;  c = c ^ (b >> 5);
    a = a - b;  a = a - c;  a = a ^ (c >> 3);
    b = b - c;  b = b - a;  b = b ^ (a << 10);
    c = c - a;  c = c - b;  c = c ^ (b >> 15);
    return c;
}

vector<int> random() {
    int n;
    cout << "Enter number of bits to be generated randomly: ";
    cin >> n;
    if (n < 2) {
        cout << "Number of bits must be greater than 1" << endl;
        exit(1);
    }
    vector<int> bin_data(n);

    unsigned long seed = mix(clock(), time(NULL), _getpid());
    srand(seed);
    for (int i = 0; i < n; i++) {
        bin_data[i] = rand() & 1;
    }
    return bin_data;
}

vector<int> fixed_random() {
    int n;
    cout << "Enter number of bits to be generated randomly: ";
    cin >> n;
    if (n < 2) {
        cout << "Number of bits must be greater than 1" << endl;
        exit(1);
    }
    vector<int> bin_data(n);

    int b;
    cout << "Choose bit to be generated in a contigous subarray (0 or 1): ";
    cin >> b;
    if (b != 0 && b != 1) {
        cout << "Invalid bit" << endl;
        exit(1);
    }

    int l;
    cout << "Enter length of subarray: ";
    cin >> l;
    if (l > n - 1 || l < 1) {
        cout << "Invalid length" << endl;
        exit(1);
    }
    //Adds a header and tailer of inversed bit to the subarray   
    vector<int> sub_seq(l + 2);
    sub_seq[0] = !b;
    sub_seq[l + 1] = !b;
    for (int i = 1; i <= l; i++) {
        sub_seq[i] = b;
    }
    int r; 
    r = rand() % (n - l - 2); 
    for (int i = 0; i < n; i++) {
        if (i >= r && i < r + l + 2) {
            bin_data[i] = sub_seq[i - r];
        }
        else {
            int x = rand() & 1;
            bin_data[i] = x;
        }
    }
    return bin_data;
}