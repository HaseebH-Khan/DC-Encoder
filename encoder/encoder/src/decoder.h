#pragma once

#include <iostream> 
#include <vector>

using namespace std;

bool verify_decoded(vector<int>& decoded_data, vector<int>& bin_data) {
    if (decoded_data.size() != bin_data.size()) {
        cout << "Error: decoded data size: " << decoded_data.size() << " is not equal to binary data size: " << bin_data.size() << endl;
        return false;
    }
    for (unsigned int i = 0; i < decoded_data.size(); i++) {
        if (decoded_data[i] != bin_data[i]) {
            cout << "Error: decoded data is not equal to binary data" << endl;
            return false;
        }
    }
    return true;
}

void nrzl_decode(vector<int>& encoded_data, vector<int>& decoded_data) {
    for (unsigned int i = 0; i < encoded_data.size(); i++) {
        decoded_data.push_back(encoded_data[i]);
    }
}

void nrzi_decode(vector<int>& encoded_data, vector<int>& decoded_data) {
    cout << "Choose initial value of previous bit (0 or 1): ";
    int prev;
    cin >> prev;
    if (prev != 0 && prev != 1) {
        cout << "Invalid bit" << endl;
        exit(1);
    }
    int data;
    for (unsigned int i = 0; i < encoded_data.size(); i++) {
        data = (prev + encoded_data[i]) & 1;
        prev = encoded_data[i];
        decoded_data.push_back(data);
    }
}

void manchester_decode(vector<int>& encoded_data, vector<int>& decoded_data) {
    unsigned int sz = encoded_data.size();
    if ((sz & 1) != 0) {
        cout << "Invalid data" << endl;
        exit(1);
    }
    for (unsigned int i = 0; i < sz; i += 2) {
        if (encoded_data[i] == 0 && encoded_data[i + 1] == 1) {
            decoded_data.push_back(0);
        }
        else if (encoded_data[i] == 1 && encoded_data[i + 1] == 0) {
            decoded_data.push_back(1);
        }
        else {
            cout << "Invalid bit" << endl;
            exit(1);
        }
    }
}

void differential_manchester_decode(vector<int>& encoded_data, vector<int>& decoded_data) {
    unsigned int sz = encoded_data.size();
    if ((sz & 1) != 0) {
        cout << "Invalid data" << endl;
        exit(1);
    }
    int prev;
    cout << "Choose initial value of previous bit (0 or 1): ";
    cin >> prev;
    if (prev != 0 && prev != 1) {
        cout << "Invalid bit" << endl;
        exit(1);
    }
    for (unsigned int i = 0; i < sz; i += 2) {
        if (encoded_data[i] == encoded_data[i + 1]) {
            cout << "Invalid bit" << endl;
            exit(1);
        }
        prev = encoded_data[i] ^ prev;
        decoded_data.push_back(prev);
        prev = encoded_data[i];
    }
}

void b8zs_unscrambler(vector<int>& encoded_data) {
    unsigned int sz = encoded_data.size();
    int prev_polarity = 1; //
    for (unsigned int i = 0; i < sz; i++) {
        // if less than 8 bits are left then ignore
        if (sz - i < 8) break;
        // [000+-0+-] or [000-+0-+]    ==>     [00000000] 
        else if
            (encoded_data[i] == 0 && encoded_data[i + 1] == 0 && encoded_data[i + 2] == 0
                && encoded_data[i + 3] == 1 && encoded_data[i + 4] == -1 && encoded_data[i + 5] == 0
                && encoded_data[i + 6] == 1 && encoded_data[i + 7] == -1 && prev_polarity == -1) {
            for (unsigned int j = i; j < i + 8; j++) {
                encoded_data[j] = 0;
            }
            i += 7;
        }
        else if
            (encoded_data[i] == 0 && encoded_data[i + 1] == 0 && encoded_data[i + 2] == 0
                && encoded_data[i + 3] == -1 && encoded_data[i + 4] == 1 && encoded_data[i + 5] == 0
                && encoded_data[i + 6] == -1 && encoded_data[i + 7] == 1 && prev_polarity == 1) {
            for (unsigned int j = i; j < i + 8; j++) {
                encoded_data[j] = 0;
            }
            i += 7;
        }
        else if (encoded_data[i] == 1 || encoded_data[i] == -1) {
            prev_polarity = encoded_data[i];
        }
    }
}

void hdb3_unscrambler(vector<int>& encoded_data) {
    unsigned int sz = encoded_data.size();
    int prev_polarity = 1; //
    for (unsigned int i = 0; i < sz; i++) {
        // if less than 4 bits are left then ignore
        if (sz - i < 4) break;
        // [000-] or [000+] or [+00+] or [-00-]    ==>     [0000]
        else if
            (encoded_data[i] == 0 && encoded_data[i + 1] == 0 && encoded_data[i + 2] == 0
                && encoded_data[i + 3] == -1 && prev_polarity == 1) {
            for (unsigned int j = i; j < i + 4; j++) {
                encoded_data[j] = 0;
            }
            prev_polarity = -prev_polarity;
            i += 3;
        }
        else if
            (encoded_data[i] == 0 && encoded_data[i + 1] == 0 && encoded_data[i + 2] == 0
                && encoded_data[i + 3] == 1 && prev_polarity == -1) {
            for (unsigned int j = i; j < i + 4; j++) {
                encoded_data[j] = 0;
            }
            prev_polarity = -prev_polarity;
            i += 3;
        }
        else if
            (encoded_data[i] == 1 && encoded_data[i + 1] == 0 && encoded_data[i + 2] == 0
                && encoded_data[i + 3] == 1 && prev_polarity == -1) {
            for (unsigned int j = i; j < i + 4; j++) {
                encoded_data[j] = 0;
            }
            prev_polarity = -prev_polarity;
            i += 3;
        }
        else if
            (encoded_data[i] == -1 && encoded_data[i + 1] == 0 && encoded_data[i + 2] == 0
                && encoded_data[i + 3] == -1 && prev_polarity == 1) {
            for (unsigned int j = i; j < i + 4; j++) {
                encoded_data[j] = 0;
            }
            prev_polarity = -prev_polarity;
            i += 3;
        }
    }
}
void ami_decode(vector<int>& encoded_data, vector<int>& decoded_data) {
    cout << "Have you scrambled the data? (y/n): ";
    char ch;
    cin >> ch;
    if (ch == 'y') {
        cout << "Choose scrambler type \n 1. b8zs \n 2. hdb3 \n";
        int type;
        cin >> type;
        if (type == 1) {
            b8zs_unscrambler(encoded_data);
        }
        else if (type == 2) {
            hdb3_unscrambler(encoded_data);
        }
        else {
            cout << "Invalid option" << endl;
            exit(1);
        }
    }
    for (unsigned int i = 0; i < encoded_data.size(); i++) {
        if (encoded_data[i] == 0) {
            decoded_data.push_back(0);
        }
        else {
            decoded_data.push_back(1);
        }
    }
}

vector<int> decode(vector<int>& encoded_data, int encoder_choice) {
    vector<int> decoded_data;
    switch (encoder_choice)
    {
    case 1:
        nrzl_decode(encoded_data, decoded_data);
        break;
    case 2:
        nrzi_decode(encoded_data, decoded_data);
        break;
    case 3:
        manchester_decode(encoded_data, decoded_data);
        break;
    case 4:
        differential_manchester_decode(encoded_data, decoded_data);
        break;
    case 5:
        ami_decode(encoded_data, decoded_data);
        break;
    default:
        break;
    }
    return decoded_data;
}