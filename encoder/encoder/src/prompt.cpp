#include <iostream>
#include <vector>

#include "randomBits.h"
#include "lineEncoders.h" 
#include "scrambler.h"
#include "decoder.h"
#include "graphical.h"
#include "palindrome.h"

using namespace std;

int main() {
    vector<int> bin_data;
    int generator_choice;
    cout << "Enter the index of the data generator of your chioce: " << endl;
    cout << "1. Random" << endl;
    cout << "2. Fixed Random" << endl;
    cin >> generator_choice;

    switch (generator_choice)
    {
    case 1:
        bin_data = random();
        break;
    case 2:
        bin_data = fixed_random();
        break;
    default:
        break;
    }

    int encoder_choice;
    cout << "Enter the index of the encoder of your chioce: " << endl;
    cout << "1. NRZ-L" << endl;
    cout << "2. NRZ-I" << endl;
    cout << "3. Manchester" << endl;
    cout << "4. Differential Manchester" << endl;
    cout << "5. AMI" << endl;
    cin >> encoder_choice;

    vector<int> encoded_data;
    switch (encoder_choice)
    {
    case 1:
        encoded_data = nrzl_encoder(bin_data);
        break;
    case 2:
        encoded_data = nrzi_encoder(bin_data);
        break;
    case 3:
        encoded_data = manchester_encoder(bin_data);
        break;
    case 4:
        encoded_data = differential_manchester_encoder(bin_data);
        break;
    case 5:
        cout << "Do you want to scramble the data? (y/n)" << endl;
        char s_choice;
        cin >> s_choice;
        encoded_data = ami_encoder(bin_data);
        if (s_choice == 'y') {
            cout << "Choose the scrambler: " << endl;
            cout << "1. B8ZS" << endl;
            cout << "2. HDB3" << endl;
            int scrm_choice;
            cin >> scrm_choice;
            switch (scrm_choice)
            {
            case 1:
                encoded_data = b8zs(encoded_data);
                break;
            case 2:
                encoded_data = hdb3(encoded_data);
                break;
            }
        }
        break;
    default:
        break;
    }

    cout << "Binary data generated: " << endl;
    for (unsigned int i = 0; i < bin_data.size(); i++) {
        cout << bin_data[i] << " ";
    } cout << endl;

    cout << "Binary data after encoding: " << endl;
    for (unsigned int i = 0; i < encoded_data.size(); i++) {
        cout << encoded_data[i] << " ";
    } cout << endl;

    vector<int> decoded_data = decode(encoded_data, encoder_choice);
    cout << "Decoded data: " << endl;
    for (unsigned int i = 0; i < decoded_data.size(); i++) {
        cout << decoded_data[i] << " ";
    } cout << endl;

    int ret;
    ret = graph(bin_data, encoded_data, decoded_data, encoder_choice);

    verify_decoded(decoded_data, bin_data) ? cout << "Decoded data is correct" << endl : cout << "Decoded data is incorrect" << endl;
    LongestPalindromicSubsequence(bin_data);
    return ret;
}