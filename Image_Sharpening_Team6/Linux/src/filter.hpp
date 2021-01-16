#pragma once

#include <fstream>
#include <utility>
#include <vector>
#include <chrono>
#include <iostream>

using namespace std;

// ============================================================================

template <typename T1, typename T2, typename T3>
struct triplet
{
    T1 first; T2 second; T3 third;

     triplet() = default;
    ~triplet() = default;

    // ------------------------------------------------------------------------

    template <typename U>
    triplet(U&& value):
        first(forward<U>(value)),
        second(forward<U>(value)),
        third(forward<U>(value)) {}

    template <typename U1, typename U2, typename U3>
    triplet(U1&& _first, U2&& _second, U3&& _third):
        first(forward<U1>(_first)),
        second(forward<U2>(_second)),
        third(forward<U3>(_third)) {}

    template <typename U1, typename U2, typename U3>
    triplet(triplet<U1, U2, U3>&& other):
        first(forward<U1>(other.first)),
        second(forward<U2>(other.second)),
        third(forward<U3>(other.third)) {}

    // ------------------------------------------------------------------------

    template <typename U1, typename U2, typename U3>
    triplet& operator=(triplet<U1, U2, U3>&& other)
    {
        first  = move(other.first);
        second = move(other.second);
        third  = move(other.third);
        return *this;
    }

    template <typename U1, typename U2, typename U3>
    triplet& operator=(const triplet<U1, U2, U3>& other)
    {
        first  = static_cast<T1>(other.first);
        second = static_cast<T2>(other.second);
        third  = static_cast<T3>(other.third);
        return *this;
    }

    // ------------------------------------------------------------------------

    template <typename U1, typename U2, typename U3>
    triplet operator+(const triplet<U1, U2, U3>& other) const
    {
        return
        {
            first  + other.first,
            second + other.second,
            third  + other.third
        };
    }

    template <typename U1, typename U2, typename U3>
    triplet& operator+=(const triplet<U1, U2, U3>& other)
    {
        first  += other.first;
        second += other.second;
        third  += other.third;
        return *this;
    }

    template <typename T>
    triplet<T, T, T> operator*(const T& other) const
    {
        return {other * first, other * second, other * third};
    }
};

// ============================================================================

using itrip_t = triplet<uint8_t, uint8_t, uint8_t>;
using ftrip_t = triplet<float, float, float>;

using ivec_t = vector<itrip_t>;
using fvec_t = vector<float>;

using image_t  = vector<ivec_t>;
using filter_t = vector<fvec_t>;

// ============================================================================

const auto filter = filter_t
({
    {        0.0f, -2.0f / 3.0f,         0.0f},
    {-2.0f / 3.0f, 11.0f / 3.0f, -2.0f / 3.0f},
    {        0.0f, -2.0f / 3.0f,         0.0f}
});

// ============================================================================

inline auto parse_input(const string& file)
{
    auto N = 0u, M = 0u;
    auto image = image_t();

    auto input = ifstream();
    input.open(file, ios_base::binary);

    auto aux = string();
    getline(input, aux);
    getline(input, aux);

    input >> aux; M = stoi(aux);
    input >> aux; N = stoi(aux);
    input >> aux;

    image.resize(N + 2u);
    for (auto& line: image)
        line.resize(M + 2u);

    getline(input, aux);

    for (auto i = 1u; i <= N; ++i)
    {
        auto buffer = vector<char>(3u * M);
        input.read(buffer.data(), buffer.size());

        for (auto j = 3u; j <= buffer.size(); j += 3u)
            image[i][j / 3u] =
            {
                move(buffer[j - 3u]),
                move(buffer[j - 2u]),
                move(buffer[j - 1u])
            };
    }

    return image;
}

// ============================================================================

inline void write_image(const string& file, const image_t& image)
{
    auto output = ofstream();
    auto N = image.size() - 2u;
    auto M = image.front().size() - 2u;

    output.open(file, ios_base::binary);
    output << "P6\n# \n" << M << ' ' << N << "\n255\n";

    for (auto i = 1u; i <= N; ++i)
        for (auto j = 1u; j <= M; ++j)
            output << image[i][j].first
                   << image[i][j].second
                   << image[i][j].third;
}

// ============================================================================

inline void sharpen(image_t& image)
{
    auto N = image.size();
    auto M = image.front().size();
    auto result = image_t(N, ivec_t(M));

    for (auto i = 1u; i < N - 1u; ++i)
        for (auto j = 1u; j < M - 1u; ++j)
        {
            auto aux = ftrip_t(0.0f);

            for (auto x: {-1, 0, 1})
                for (auto y: {-1, 0, 1})
                    aux += image[i + x][j + y] * filter[1 - x][1 - y];

            if (aux.first  < 0) aux.first  = 0;
            if (aux.second < 0) aux.second = 0;
            if (aux.third  < 0) aux.third  = 0;

            if (aux.first  > 255) aux.first  = 255;
            if (aux.second > 255) aux.second = 255;
            if (aux.third  > 255) aux.third  = 255;

            result[i][j] = aux;
        }

    image = move(result);
}

// ============================================================================