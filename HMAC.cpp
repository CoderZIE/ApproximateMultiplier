#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


template <typename T>
void reverseVec(vector<T>& vec) {
    reverse(vec.begin(), vec.end());
}

// Function definitions

void reverseStr(string& str)
{
    int n = str.length();
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

int nor_gate(int a, int b) {
    if (a == 0 & b == 0) {
        return 1;
    } else {
        return 0;
    }
}

int not_gate(int a) {
    return (a == 0) ? 1 : 0;
}

int and_gate(int a, int b) {
    return (a == 1 & b == 1) ? 1 : 0;
}

int nand_gate(int a, int b) {
    return (a == 1 & b == 1) ? 0 : 1;
}

int xor_gate(int a, int b) {
    return (a != b) ? 1 : 0;
}

int or_gate(int a, int b) {
    return (a == 1 | b == 1) ? 1 : 0;
}

int mux(int sel, int data0, int data1) {
    if (sel == 0) {
        return data0;
    } else if (sel == 1) {
        return data1;
    } else {
        return -1;
    }
}

pair<int, int> full_adder(int A, int B, int Cin) {
    int S = xor_gate(xor_gate(A, B), Cin);
    int Cout = or_gate(or_gate(and_gate(A, B), and_gate(B, Cin)), and_gate(Cin, A));
    return make_pair(S, Cout);
}

pair<int, pair<int, int>> accurateCompressor(int a, int b, int c, int d, int cin) {
    pair<int, int> tempSumCout1 = full_adder(a, b, cin);
    pair<int, int> tempSumCout2 = full_adder(tempSumCout1.first, c, d);

    int sum = tempSumCout2.first;
    int carry_out = tempSumCout2.second;
    int borrow_out = tempSumCout1.second;

    return make_pair(sum, make_pair(carry_out, borrow_out));
}

pair<int, int> approximateCompressor(int a, int b, int c, int d) {
    int temp1 = xor_gate(a, b);
    int temp2 = nand_gate(a, b);
    int temp3 = xor_gate(c, d);
    int temp4 = nand_gate(c, d);

    int temp5 = nand_gate(temp1, temp3);
    int temp6 = not_gate(temp1);
    int temp7 = and_gate(temp2, temp4);

    int sum = mux(temp3, temp1, temp6);
    int carry = nand_gate(temp7, temp5);

    return make_pair(sum, carry);
}

string adder_16bit(string a, string b) {
    int carry = 0;
    string result = "";

    for (int i = 15; i >= 0; --i) {
        int bit_a = a[i] - '0';
        int bit_b = b[i] - '0';

        pair<int, int> adderResult = full_adder(bit_a, bit_b, carry);

        result = char(adderResult.first + '0') + result;
        carry = adderResult.second;
    }

    return result;
}

pair<int, int> half_adder(int a, int b) {
    int sum_out = xor_gate(a, b);
    int carry_out = and_gate(a, b);
    return make_pair(sum_out, carry_out);
}

string approximate_multiplier(string a, string b) {
    // Reversing the strings
    reverseStr(a);
    reverseStr(b);

    if (a.length() != 8 || b.length() != 8) {
        return "Inputs should be 8-bit binary values";
    }

    vector<vector<int>> partial_products(8, vector<int>(8, 0));

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            partial_products[i][j] = and_gate(a[i] - '0', b[j] - '0');
        }
    }

    partial_products[7][0] = not_gate(partial_products[7][0]);
    partial_products[7][1] = not_gate(partial_products[7][1]);
    partial_products[7][2] = not_gate(partial_products[7][2]);
    partial_products[7][3] = not_gate(partial_products[7][3]);
    partial_products[7][4] = not_gate(partial_products[7][4]);
    partial_products[7][5] = not_gate(partial_products[7][5]);
    partial_products[7][6] = not_gate(partial_products[7][6]);

    partial_products[0][7] = not_gate(partial_products[0][7]);
    partial_products[1][7] = not_gate(partial_products[1][7]);
    partial_products[2][7] = not_gate(partial_products[2][7]);
    partial_products[3][7] = not_gate(partial_products[3][7]);
    partial_products[4][7] = not_gate(partial_products[4][7]);
    partial_products[5][7] = not_gate(partial_products[5][7]);
    partial_products[6][7] = not_gate(partial_products[6][7]);


    for (int i = 0; i < 7; i++) {
        int temp = partial_products[7][i];
        partial_products[7][i] = partial_products[i][7];
        partial_products[i][7] = temp;
    }


    // Stage 1 compression
    pair<int, int> s1c1 = full_adder(partial_products[4][0], partial_products[3][1], 0);
    pair<int, int> s2c2 = approximateCompressor(partial_products[5][0], partial_products[4][1], partial_products[3][2], partial_products[2][3]);
    pair<int, int> s3c3 = approximateCompressor(partial_products[6][0], partial_products[5][1], partial_products[4][2], partial_products[3][3]);
    pair<int, int> s4c4 = full_adder(partial_products[2][4], partial_products[1][5], 0);
    pair<int, int> s5c5 = approximateCompressor(partial_products[7][0], partial_products[6][1], partial_products[5][2], partial_products[4][3]);
    pair<int, pair<int, int>>  s6c6c1 = accurateCompressor(partial_products[3][4], partial_products[2][5], partial_products[1][6], partial_products[0][7], 0);
    pair<int, int> s7c7 = approximateCompressor(1, partial_products[7][1], partial_products[6][2], partial_products[5][3]);
    pair<int, pair<int, int>>  s8c8c2 = accurateCompressor(partial_products[4][4], partial_products[3][5], partial_products[2][6], partial_products[1][7], s5c5.second);
    pair<int, int> s9c9 = approximateCompressor(partial_products[7][2], partial_products[6][3], partial_products[5][4], partial_products[4][5]);
    pair<int, int> s10c10 = full_adder(partial_products[3][6], partial_products[2][7], s7c7.second);
    pair<int, int> s11c11 = approximateCompressor(partial_products[7][3], partial_products[6][4], partial_products[5][5], partial_products[4][6]);
    pair<int, int> s12c12 = full_adder(partial_products[7][4], partial_products[6][5], 0);

    // Stage 2 compression
    pair<int, int> s13c13 = full_adder(partial_products[2][0], partial_products[1][1], 0);
    pair<int, int> s14c14 = approximateCompressor(partial_products[3][0], partial_products[2][1], partial_products[1][2], partial_products[0][3]);
    pair<int, int> s15c15 = approximateCompressor(s1c1.first, partial_products[2][2], partial_products[1][3], partial_products[0][4]);
    pair<int, int> s16c16 = approximateCompressor(s2c2.first, s1c1.second, partial_products[1][4], partial_products[0][5]);
    pair<int, int> s17c17 = approximateCompressor(s3c3.first, s2c2.second, s4c4.first, partial_products[0][6]);
    pair<int, int> s18c18 = approximateCompressor(s6c6c1.first, s4c4.second, s5c5.first, s3c3.second);
    pair<int, int> s19c19 = approximateCompressor(s6c6c1.second.second, s6c6c1.second.first, s7c7.first, s8c8c2.first);
    pair<int, pair<int, int>>  s20c20c3 = accurateCompressor(s8c8c2.second.first, s9c9.first, s10c10.first, s8c8c2.second.second, 0);
    pair<int, pair<int, int>>  s21c21c4 = accurateCompressor(s11c11.first,s9c9.second, s10c10.second, partial_products[3][7], s20c20c3.second.second);
    pair<int, pair<int, int>>  s22c22c5 = accurateCompressor(s12c12.first, s11c11.second, partial_products[5][6], partial_products[4][7], s21c21c4.second.second);
    pair<int, pair<int, int>>  s23c23c6 = accurateCompressor(partial_products[7][5], s12c12.second, partial_products[6][6], partial_products[5][7], s22c22c5.second.second);
    pair<int, int> s24c24 = full_adder(partial_products[7][6], partial_products[6][7], s23c23c6.second.second);

    // Stage 3 (16 bit adder)
    vector<int> adderA = {0, partial_products[0][1], partial_products[0][2], s13c13.second, s14c14.second, s15c15.second, s16c16.second, s17c17.second, s18c18.second, s19c19.second, s20c20c3.second.first, s21c21c4.second.first, s22c22c5.second.first, s23c23c6.second.first, s24c24.second, 0};
    vector<int> adderB = {partial_products[0][0], partial_products[1][0], s13c13.first, s14c14.first, s15c15.first, s16c16.first, s17c17.first, s18c18.first, s19c19.first, s20c20c3.first, s21c21c4.first, s22c22c5.first, s23c23c6.first, s24c24.first, partial_products[7][7], 1};


    reverseVec(adderA);
    reverseVec(adderB);


    string strA(adderA.begin(), adderA.end());
    string strB(adderB.begin(), adderB.end());

    string result = adder_16bit(strA, strB);

    return result;
}

int main() {
    // Example usage

    // Defining 8-bit binary numbers
    string binary_number_a = "00010010";
    string binary_number_b = "00010010";

    // Using the approximate multiplier
    string result = approximate_multiplier(binary_number_a, binary_number_b);

    // Displaying the result
    cout << "Result: " << result << endl;

    return 0;
}
