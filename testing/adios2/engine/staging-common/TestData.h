/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 */
#ifndef TESTING_ADIOS2_ENGINE_COMMON_TESTDATA_H_
#define TESTING_ADIOS2_ENGINE_COMMON_TESTDATA_H_

#include <cstdint>

#include <array>
#include <limits>
#include <string>

#ifdef WIN32
#define NOMINMAX
#endif

// Number of rows

std::size_t Nx = 10;

std::string data_S1 = "Testing ADIOS2 String type";
std::array<std::string, 1> data_S1array = {{"one"}};
std::array<std::string, 3> data_S3 = {{"one", "two", "three"}};

std::vector<int8_t> data_I8;
std::vector<int16_t> data_I16;
std::vector<int32_t> data_I32;
std::vector<int64_t> data_I64;
std::array<int8_t, 10> data_U8;
std::array<int16_t, 10> data_U16;
std::array<int32_t, 10> data_U32;
std::array<int64_t, 10> data_U64;
std::vector<float> data_R32;
std::vector<double> data_R64;
std::vector<std::complex<float>> data_C32;
std::vector<std::complex<double>> data_C64;
double data_R64_2d[10][2];
double data_R64_2d_rev[2][10];

std::vector<int8_t> in_I8;
std::vector<int16_t> in_I16;
std::vector<int32_t> in_I32;
std::vector<int64_t> in_I64;
std::vector<float> in_R32;
std::vector<double> in_R64;
std::vector<std::complex<float>> in_C32;
std::vector<std::complex<double>> in_C64;
std::vector<double> in_R64_2d;
std::vector<double> in_R64_2d_rev;

int8_t in_scalar_I8;
int16_t in_scalar_I16;
int32_t in_scalar_I32;
int64_t in_scalar_I64;
float in_scalar_R32;
double in_scalar_R64;
std::complex<float> in_scalar_C32;
std::complex<double> in_scalar_C64;

double data_scalar_R64;

void generateSimpleForwardData(std::vector<double> &data_forward, int step,
                               int start, int count, int total_size)
{
    int64_t j = 100 * step + start;

    data_forward.clear();
    for (int i = 0; i < count; i++)
    {
        data_forward.push_back((double)j + i);
    }
}

void generateSimpleReverseData(std::vector<double> &data_reverse, int step,
                               int start, int count, int total_size)
{
    int64_t j = 100 * step + total_size - start;

    data_reverse.clear();
    for (int i = 0; i < count; i++)
    {
        data_reverse.push_back((double)j - i);
    }
}

int validateSimpleForwardData(std::vector<double> &data_forward, int step,
                              int64_t start, int64_t count, int64_t total_size)
{
    int ret = 0;
    int64_t j = 100 * step + start;

    std::cout << "Calling validate Simple data forward with step " << step
              << ", start = " << start << ", count = " << count << std::endl;
    for (int i = 0; i < count; i++)
    {
        if (data_forward[i] != (double)j + i)
        {
            std::cout << "Expected data_forward[" << i << "] to be "
                      << (double)j + i << " got " << data_forward[i]
                      << std::endl;
            ret = 1;
        }
    }
    return ret;
}

int validateSimpleReverseData(std::vector<double> &data_reverse, int step,
                              int64_t start, int64_t count, int64_t total_size)
{
    int ret = 0;
    int64_t j = 100 * step + total_size - start;

    for (int i = 0; i < count; i++)
    {
        if (data_reverse[i] != (double)j - i)
        {
            std::cout << "Expected data_reverse[" << i << "] to be "
                      << (double)j - i << " got " << data_reverse[i]
                      << std::endl;
            ret = 1;
        }
    }
    return ret;
}

void generateCommonTestData(int step, int rank, int size, int Nx, int r64_Nx)
{
    int64_t j = rank * Nx * 10 + step;
    int64_t r64_j = j;

    data_I8.resize(Nx);
    data_I16.resize(Nx);
    data_I32.resize(Nx);
    data_I64.resize(Nx);
    data_R32.resize(Nx);
    data_R64.resize(r64_Nx);
    data_C32.resize(Nx);
    data_C64.resize(Nx);

    if (r64_Nx != Nx)
    {
        /* for rank 1 (which has the data of rank 0 in this case, use a
         * different j */
        r64_j = (rank - 1) * Nx * 10 + step;
    }
    data_scalar_R64 = (step + 1) * 1.5;
    for (int i = 0; i < Nx; i++)
    {
        data_I8[i] = (int8_t)(j + 10 * i);
        data_I16[i] = (int16_t)(j + 10 * i);
        data_I32[i] = (int32_t)(j + 10 * i);
        data_I64[i] = (int64_t)(j + 10 * i);
        data_R32[i] = (float)j + 10 * i;
        if (r64_Nx > i)
            data_R64[i] = (double)r64_j + 10 * i;
        data_C32[i].imag((float)j + 10 * i);
        data_C32[i].real((float)-(j + 10 * i));
        data_C64[i].imag((double)j + 10 * i);
        data_C64[i].real((double)-(j + 10 * i));
        data_R64_2d[i][0] = (double)j + 10 * i;
        data_R64_2d[i][1] = (double)10000 + j + 10 * i;
        data_R64_2d_rev[0][i] = (double)j + 10 * i;
        data_R64_2d_rev[1][i] = (double)10000 + j + 10 * i;
    }
    for (int i = Nx; i < r64_Nx; i++)
    {
        data_R64[i] = (double)r64_j + 10 * i;
    }
}

int validateCommonTestData(int start, int length, size_t step,
                           int missing_end_data)
{
    int failures = 0;
    if (in_scalar_R64 != 1.5 * (step + 1))
    {
        std::cout << "Expected " << 1.5 * (step + 1) << ", got "
                  << in_scalar_R64 << " for in_scalar_R64, timestep " << step
                  << std::endl;
        failures++;
    }
    for (int i = 0; i < length; i++)
    {
        if (in_I8[i] != (int8_t)((i + start) * 10 + step))
        {
            std::cout << "Expected 0x" << std::hex
                      << (int8_t)((i + start) * 10 + step) << ", got 0x"
                      << std::hex << in_I8[i] << " for in_I8[" << i
                      << "](global[" << i + start << "])" << std::endl;
            failures++;
        }
        if (in_I16[i] != (int16_t)((i + start) * 10 + step))
        {
            std::cout << "Expected 0x" << std::hex
                      << (int16_t)((i + start) * 10 + step) << ", got 0x"
                      << std::hex << in_I16[i] << " for in_I16[" << i
                      << "](global[" << i + start << "])" << std::endl;
            failures++;
        }
        if (in_I32[i] != (int32_t)((i + start) * 10 + step))
        {
            std::cout << "Expected 0x" << std::hex
                      << (int32_t)((i + start) * 10 + step) << ", got 0x"
                      << std::hex << in_I32[i] << " for in_I32[" << i
                      << "](global[" << i + start << "])" << std::endl;
            failures++;
        }
        if (in_I64[i] != (int64_t)((i + start) * 10 + step))
        {
            std::cout << "Expected 0x" << std::hex
                      << (int64_t)((i + start) * 10 + step) << ", got 0x"
                      << std::hex << in_I64[i] << " for in_I64[" << i
                      << "](global[" << i + start << "])" << std::endl;
            failures++;
        }

        if (in_R32[i] != (float)((i + start) * 10 + step))
        {
            std::cout << "Expected " << (float)((i + start) * 10 + step)
                      << ", got " << in_R32[i] << " for in_R32[" << i
                      << "](global[" << i + start << "])" << std::endl;
            failures++;
        }
        if (in_R64[i] != (double)((i + start) * 10 + step))
        {
            std::cout << "Expected " << (double)((i + start) * 10 + step)
                      << ", got " << in_R64[i] << " for in_R64[" << i
                      << "](global[" << i + start << "])" << std::endl;
            failures++;
        }
        if (!missing_end_data)
        {
            if ((in_C32[i].imag() != (float)((i + start) * 10 + step)) ||
                (in_C32[i].real() != -(float)((i + start) * 10 + step)))
            {
                std::cout << "Expected [" << (float)((i + start) * 10 + step)
                          << ", " << -(float)((i + start) * 10 + step)
                          << "], got " << in_C32[i] << " for in_C32[" << i
                          << "](global[" << i + start << "])" << std::endl;
                failures++;
            }
            if ((in_C64[i].imag() != (double)((i + start) * 10 + step)) ||
                (in_C64[i].real() != (-(double)((i + start) * 10 + step))))
            {
                std::cout << "Expected [" << (double)((i + start) * 10 + step)
                          << ", " << -(double)((i + start) * 10 + step)
                          << "], got " << in_C64[i] << " for in_C64[" << i
                          << "](global[" << i + start << "])" << std::endl;
                failures++;
            }
            if (in_R64_2d[2 * i] != (double)((i + start) * 10 + step))
            {
                std::cout << "Expected " << (double)((i + start) * 10 + step)
                          << ", got " << in_R64_2d[i] << " for in_R64_2d[" << i
                          << "][0](global[" << i + start << "][0])"
                          << std::endl;
                failures++;
            }
            if (in_R64_2d[2 * i + 1] !=
                (double)(10000 + (i + start) * 10 + step))
            {
                std::cout << "Expected "
                          << (double)(10000 + (i + start) * 10 + step)
                          << ", got " << in_R64_2d[i] << " for in_R64_2d[" << i
                          << "][1](global[" << i + start << "][1])"
                          << std::endl;
                failures++;
            }
            if (in_R64_2d_rev[i] != (double)((i + start) * 10 + step))
            {
                std::cout << "Expected " << (double)((i + start) * 10 + step)
                          << ", got " << in_R64_2d_rev[i]
                          << " for in_R64_2d_rev[0][" << i << "](global[0]["
                          << i + start << "])" << std::endl;
                failures++;
            }
            if (in_R64_2d_rev[i + length] !=
                (double)(10000 + (i + start) * 10 + step))
            {
                std::cout << "Expected "
                          << (double)(10000 + (i + start) * 10 + step)
                          << ", got " << in_R64_2d_rev[i + length]
                          << " for in_R64_2d_rev[1][" << i << "](global[1]["
                          << i + start << "])" << std::endl;
                failures++;
            }
        }
    }
    return failures;
}

#endif // TESTING_ADIOS2_ENGINE_COMMON_TESTDATA_H_
