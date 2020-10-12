#pragma once

#include <cmath>
#include <vector>

#include "./data.h"

class LSH
{
private:
    // stores (m^i)modM from i=0 to i = d-1 so as to no recompute
    std::vector<uint32_t> md;

    void calculate_s(std::vector<std::vector<int>> &S);
    uint32_t calculate_h(const std::vector<uint8_t> &x, const std::vector<int> &s);
    int calculate_a(const uint8_t &xi, const int &si);

public:
    int k, L, N;
    Data &data;
    uint32_t w, m, M;

    LSH(int k, int L, int N, Data &data, uint32_t w = 52, uint32_t m = uint32_t(pow(2, 30) - 5));
    ~LSH();

    int Run();
};