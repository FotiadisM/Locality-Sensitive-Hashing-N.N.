#include <cstdlib>
#include <cmath>
#include <cstring>
#include <random>
#include <chrono>
#include <algorithm>

#include "../include/kmeansplusplus.h"

using namespace std;

// classic
kmeansplusplus::kmeansplusplus(const int &mClusters, Data &mData)
    : nClusters(mClusters), data(mData)
{
    this->method = _Classic;
}

// lsh
kmeansplusplus::kmeansplusplus(const int &clusters, const int &lsh_k, const int &L, Data &data)
    : nClusters(clusters), lsh_k(lsh_k), L(L), data(data)
{
    this->method = _LSH;
}

// hypercube
kmeansplusplus::kmeansplusplus(const int &clusters, const int &cube_k, const int &M, const int &probes, Data &data)
    : nClusters(clusters), cube_k(cube_k), M(M), probes(probes), data(data)
{
    this->method = _Hypercube;
}

// complete
kmeansplusplus::kmeansplusplus(const int &clusters, const int &lsh_k, const int &L, const int &cube_k, const int &M, const int &probes, Data &data)
    : nClusters(clusters), lsh_k(lsh_k), L(L), cube_k(cube_k), M(M), probes(probes), data(data)
{
    this->method = _Complete;
}

kmeansplusplus::~kmeansplusplus() {}

int kmeansplusplus::Run()
{
    this->initCentroids();

    int totalChange = this->minChange + 1;

    while (totalChange > this->minChange)
    {
        vector<vector<int>> clusters;

        switch (this->method)
        {
        case _Classic:
            clusters = this->LloydsClastering();
            break;
        case _LSH:
            clusters = this->LloydsClastering();
            break;
        case _Hypercube:
            clusters = this->LloydsClastering();
            break;
        case _Complete:
            clusters = this->LloydsClastering();
            break;
        }

        totalChange = 0;

        for (int i = 0; i < this->nClusters; i++)
        {
            int clusterChange = 0;

            for (int j = 0; j < this->data.d; j++)
            {
                int mean = 0;

                for (int &index : clusters[i])
                {
                    mean += int(this->data.data[index][j]);
                }

                mean /= clusters[i].size();
                clusterChange += abs(int(this->centroids[i][j]) - mean);

                this->centroids[i][j] = uint8_t(mean);
            }

            totalChange += clusterChange;
        }

        cout << "total " << totalChange << endl;
    }

    return 0;
}

void kmeansplusplus::initCentroids()
{
    default_random_engine re(chrono::system_clock::now().time_since_epoch().count());

    // picking first centroid at random
    this->centroids.push_back(this->data.data[rand() % this->data.n]);

    for (int i = 1; i < this->nClusters; i++)
    {
        double maxDi = 0;
        vector<double> P(this->data.n + 1); // plus one for P[0] = 0

        for (auto &point : this->data.data)
        {
            double Di = double(this->minDistancefromCentroids(point));

            if (Di > maxDi)
                maxDi = Di;
        }

        P[0] = 0;
        for (int j = 1; j <= this->data.n; j++)
        {
            double Di = this->minDistancefromCentroids(this->data.data[j]);

            P[j] = pow(Di / maxDi, 2) + P[j - 1];
        }

        uniform_real_distribution<double> unif(0.0, P[this->data.n]);
        double x = unif(re);

        this->centroids.push_back(this->data.data[this->findNextCentroid(P, x) - 1]);
    }
}

uint32_t kmeansplusplus::minDistancefromCentroids(const vector<uint8_t> &point)
{
    vector<int> D;

    for (auto &centroid : this->centroids)
    {
        D.push_back(this->data.ManhattanDistance(point, centroid));
    }

    return (D.size() == 0) ? 0 : *min_element(begin(D), end(D));
}

int kmeansplusplus::findNextCentroid(const vector<double> &P, const double x)
{
    for (int i = 1; i <= this->data.n; i++)
    {
        if (x <= P[i])
        {
            return i;
        }
    }

    return -1;
}

vector<vector<int>> kmeansplusplus::LloydsClastering()
{
    vector<vector<int>> clusters(this->nClusters); // holds all data points for every centroid

    for (int i = 0; i < this->data.n; i++)
    {
        clusters[this->minCentroid(this->data.data[i])].push_back(i);
    }

    return clusters;
}

int kmeansplusplus::minCentroid(const vector<uint8_t> &point)
{
    int index = -1, minDistance = INT32_MAX;

    for (int i = 0; i < this->nClusters; i++)
    {
        int d = this->data.ManhattanDistance(this->centroids[i], point);
        if (d < minDistance)
        {
            minDistance = d;
            index = i;
        }
    }

    return index;
}