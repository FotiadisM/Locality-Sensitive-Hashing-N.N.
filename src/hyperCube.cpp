#include <iostream>
#include <cmath>
#include <bits/stdc++.h>

#include "../include/hyperCube.h"

using namespace std;

/*
 use 2 unordered sets instead of map as they dont add time complexity for searching
 since 2 O(1) == O(1)
 while using less space since they only store the keys and not the values associated with them
*/

f::f() {}

f::~f() {}

int f::calculate_f(std::string key)
{
    int num;

    if (this->setOfZeros.find(key) != this->setOfZeros.end())
        return 0;
    else if (this->setOfOnes.find(key) != this->setOfOnes.end())
        return 1;
    else
    {
        num = rand() % 2;
        if (num == 0)
        {
            this->setOfZeros.insert(key);
            return 0;
        }
        else
        {
            this->setOfOnes.insert(key);
            return 1;
        }
    }

    return -1;
}

HyperCube::HyperCube(int k, int M, int probes, Data &data, uint32_t w, uint32_t m)
    : k(k), M(M), probes(probes), data(data)
{
    this->ht = new hashTable(pow(2, k), k, data.d, w, m, pow(2, 32 / this->k));

    // initialize array of f functions
    for (int i = 0; i < k; i++)
    {
        f *f_instance = new f();
        this->fTable.push_back(f_instance);
    }

    hashData();
}

HyperCube::~HyperCube()
{
    for (int i = 0; i < k; i++)
    {
        delete this->fTable[i];
    }
}

int HyperCube::Run(const vector<uint8_t> &query, ofstream &outputFile, const int &N)
{

    vector<pair<int, int>> result = exec_query(query, N);

    // outputFile << "Query: " << q.second;


    // std::string s="";
    // for (int i = 0; i < this->k; i++)
    //     s = s + std::to_string(this->fTable[i]->calculate_f(std::to_string(this->ht->calculate_h(query, this->ht->S[i]))));

    //  cout << get<1>(this->ht->table[std::stoi(s, nullptr, 2)][1]) << endl;

    int i = 0;
    vector<pair<int, vector<uint8_t>>> exactNeighboor = this->data.RangeSearch2(query, N);

    for (auto& approximateNeighboor : result)
    {
        outputFile << "Nearest neighbor " << i << " : " << approximateNeighboor.second << endl;
        
        outputFile << "distanceHypercube: " << approximateNeighboor.first << endl;

        outputFile << "distanceTrue: " << exactNeighboor[i].first << endl << endl;
        // exactNeighboor++;
        // *exactNeighboor->second

        i++;
    }

        outputFile << "R-near neighbors:" << endl;

    // for (auto &rs : this->data.RangeSearch(query, 20000))
    // {
    //     std::string s="";
    //     for (int i = 0; i < this->k; i++)
    //         s = s + std::to_string(this->fTable[i]->calculate_f(std::to_string(this->ht->calculate_h(rs, this->ht->S[i]))));

    //     outputFile << "image number " << get<1>(this->ht->table[std::stoi(s, nullptr, 2)][1]) << endl;
    //     // this->ht
    // }

    return 0;
}

void HyperCube::hashData()
{
    for (int j = 0; j < this->data.n; j++)
    {
        std::string s = "";

        for (int i = 0; i < this->k; i++)
            s += std::to_string(this->fTable[i]->calculate_f(std::to_string(this->ht->calculate_h(this->data.data[j], this->ht->S[i]))));

        this->hyperCubeInsert(s, j, this->data.data[j]);
    }
}

void HyperCube::hyperCubeInsert(const std::string &s, int index, std::vector<uint8_t> &point)
{
    // cout << "String is : " << s << endl;
    // cout << "postion of string is : " << index << endl;
    this->ht->insertItem(std::stoi(s, nullptr, 2), index, point);
}

vector<pair<int, int>> HyperCube::exec_query(const std::vector<uint8_t> &query, const int &N)
{
    vector<pair<int, vector<uint8_t>>> possible_neighbors;
    list<string> bucketList;
    list<string>::iterator it;
    std::string s = "";
    int counter = 0;
    int m;

    for (int i = 0; i < this->k; i++)
        s = s + std::to_string(this->fTable[i]->calculate_f(std::to_string(this->ht->calculate_h(query, this->ht->S[i]))));

    bucketList = HammingDist(s, probes);
    bucketList.push_front(s);

    it = bucketList.begin();
    while (counter < M && it != bucketList.end())
    {
        m = std::stoi(*it, nullptr, 2);
        for (auto &image : this->ht->getItems(m))
        {
            possible_neighbors.emplace_back(image.first, image.second);
            counter++;
            if (counter >= M)
                break;
        }
        ++it;
    }

    return this->data.GetClosestNeighbors(query, possible_neighbors, N);
}

std::string HyperCube::toBinary(int n, int size)
{
    std::string r;

    while (n != 0)
    {
        r = (n % 2 == 0 ? "0" : "1") + r;
        n /= 2;
    }

    while (int(r.size()) < size)
        r = "0" + r;

    return r;
}

int HyperCube::hamming(std::string str1, std::string str2)
{
    int i = 0, count = 0;

    while (str1[i] != '\0')
    {
        if (str1[i] != str2[i])
            count++;
        i++;
    }

    return count;
}

list<string> HyperCube::HammingDist(const std::string s, int probes)
{
    list<string> l;
    std::string curr;
    int level = 1;
    int counter = 0;
    int dist;

    while (1)
    {
        for (int i = 0; i < pow(2, s.size()); i++)
        {
            curr = toBinary(i, s.size());
            dist = hamming(s, curr);
            if (dist == level)
            {
                l.push_back(curr);
                counter++;
            }

            if (counter >= probes)
                return l;
        }
        level++;
    }

    return l;
}