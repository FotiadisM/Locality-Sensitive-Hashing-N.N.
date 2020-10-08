#include <iostream>
#include "../include/data.h"

using namespace std;

int Data::InitMnistDataSet(std::ifstream &inputFile)
{
    int32_t a = 0;
    uint8_t b = 0;

    //  magic number
    inputFile.read((char *)(&a), sizeof(a));
    if (!inputFile)
    {
        cerr << "Input file io error" << endl;
        return -1;
    }
    cout << __builtin_bswap32(a) << endl;

    // number of images
    inputFile.read((char *)(&a), sizeof(a));
    if (!inputFile)
    {
        cerr << "Input file io error" << endl;
        return -1;
    }
    cout << __builtin_bswap32(a) << endl;
    // from big endian to low endian
    this->n = __builtin_bswap32(a);

    inputFile.read((char *)(&a), sizeof(a));
    if (!inputFile)
    {
        cerr << "Input file io error" << endl;
        return -1;
    }
    cout << __builtin_bswap32(a) << endl;

    inputFile.read((char *)(&a), sizeof(a));
    if (!inputFile)
    {
        cerr << "Input file io error" << endl;
        return -1;
    }
    cout << __builtin_bswap32(a) << endl;

    this->d = 28 * 28;

    // if ((this->data = new int *[this->n]) == nullptr)
    // {
    //     perror("new");
    //     return -1;
    // }

    for (int i = 0; i < this->n; i++)
    {
        vector<int> image(this->d);
        // try
        // {
        //     this->data[i] = new int[this->d];
        // }
        // catch (const bad_alloc &e)
        // {
        // }
        for (int j = 0; j < this->d; j++)
        {
            inputFile.read((char *)(&b), sizeof(b));
            if (!inputFile)
            {
                cerr << "read() from input file failed" << endl;
                return -1;
            }
            image.push_back(int(b));
        }
        this->data.push_back(image);
    }

    return 0;
}
