#pragma once
#include <iostream>
#include <vector>
using namespace std;
int min(int a, int b)
{
    int res = a;
    if (b < a)
        res = b;
    return res;
}

//Manachar's Algorithm 
void LongestPalindromicSubsequence(vector<int>& binary)
{
    int n = binary.size();
    binary.resize(n * 2);
    if (n == 0)
        return;
    n = 2 * n + 1;
    vector<int> len; 
    len.resize(2*n);
    len.push_back(0);
    len.push_back(1);
    int center = 1; 
    int right = 2; 
    int curr_right = 0; 
    int curr_left; 
    int LPSlen = 0;
    int LPSpos = 0;
    int diff = -1;

    for (curr_right = 2; curr_right < n; curr_right++)
    {
        curr_left = 2 * center - curr_right;
        len.push_back(0);
        diff = right - curr_right;
        if (diff > 0)
            len[curr_right] = min(len[curr_left], diff);

        while (((curr_right + len[curr_right]) < n && (curr_right - len[curr_right]) > 0) &&
            (((curr_right + len[curr_right] + 1) % 2 == 0) ||
                (binary[(curr_right + len[curr_right] + 1) / 2] == binary[(curr_right - len[curr_right] - 1) / 2])))
        {
            len[curr_right]++;
        }

        if (len[curr_right] > LPSlen) 
        {
            LPSlen = len[curr_right];
            LPSpos = curr_right;
        }

        if (curr_right + len[curr_right] > right)
        {
            center = curr_right;
            right = curr_right + len[curr_right];
        }
        
    }
    int start = (LPSpos - LPSlen) / 2;
    int end = start + LPSlen;
    cout << "Longest Palindromic Subsequence: { ";
    for (int i = start; i < end; i++)
        cout << binary[i] << ", ";
    cout << " }" << endl;
}
