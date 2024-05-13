#ifndef _DSP_HPP_
#define _DSP_HPP_

#include <vector>
#include <algorithm>
#include <cmath>
#include <type_traits>
#include <iostream>
#include <complex>
#include <numeric>

#include "data_types.h"

namespace DSP
{
    template <typename T>
    typename std::enable_if<!std::is_same<T, bool>::value, void>::type
    DSP_ABS_MINMAX(const std::vector<T> &src, T &minVal, T &maxVal, size_t &minIndex, size_t &maxIndex)
    {
        static_assert(std::is_arithmetic<T>::value, "Template argument must be an arithmetic type.");

        if (src.empty())
            return;

        auto compAbs = [](const T &a, const T &b)
        { return std::abs(a) < std::abs(b); };

        auto minIt = std::min_element(src.begin(), src.end(), compAbs);
        auto maxIt = std::max_element(src.begin(), src.end(), compAbs);

        minVal = std::abs(*minIt);
        maxVal = std::abs(*maxIt);
        minIndex = std::distance(src.begin(), minIt);
        maxIndex = std::distance(src.begin(), maxIt);
    }

    template <typename T>
    void DSP_ABS_VEC(const std::vector<T> &src, std::vector<T> &result)
    {
        result.resize(src.size());
        std::transform(src.begin(), src.end(), result.begin(), [](const T &val)
                       { return std::abs(val); });
    }

    void DSP_ABS_VEC(const ARRAY_COMPLEX &src, ARRAY_D &result)
    {
        result.resize(src.size());
        std::transform(src.begin(), src.end(), result.begin(), [](const std::complex<double> &val)
                       { return std::abs(val); });
    }

    void DSP_ADD_C(COMPLEX src1, COMPLEX src2, COMPLEX result)
    {
        result = src1 + src2;
    }

    // 结构体用于同时存储平均值和标准差
    template <typename T>
    struct MeanSigma
    {
        T mean;
        T sigma;
    };

    // 辅助函数，计算平方
    template <typename T>
    T square(T x)
    {
        return x * x;
    }

    template <int N, class T>
    constexpr T nthPower(T x)
    {
        T ret = x;
        for (int i = 1; i < N; ++i)
        {
            ret *= x;
        }
        return ret;
    }

    template <class T, int N>
    struct SumDiffNthPower
    {
        explicit SumDiffNthPower(T x) : mean_(x) {}
        constexpr T operator()(T sum, T current)
        {
            return sum + nthPower<N>(current - mean_);
        }
        T mean_;
    };

    template <class T, int N, class Iter_T>
    T nthMoment(Iter_T first, Iter_T last, T mean)
    {
        const auto cnt = std::distance(first, last);
        return std::accumulate(first, last, T(), SumDiffNthPower<T, N>(mean)) /
               static_cast<typename std::iterator_traits<Iter_T>::value_type>(cnt - 1);
    }

    template <typename ForwardIt>
    constexpr typename std::iterator_traits<ForwardIt>::value_type mean(ForwardIt first, ForwardIt last)
    {
        using input_t = typename std::iterator_traits<ForwardIt>::value_type;
        const auto acc = std::accumulate(first, last, static_cast<input_t>(0));
        return acc / static_cast<input_t>(std::distance(first, last));
    }

    template <std::size_t N, typename ForwardIt>
    constexpr typename std::iterator_traits<ForwardIt>::value_type moment(ForwardIt first, ForwardIt last)
    {
        using input_t = typename std::iterator_traits<ForwardIt>::value_type;
        const auto m = mean(first, last);
        return nthMoment<input_t, N>(first, last, m);
    }

    template <std::size_t N, typename ForwardIt>
    constexpr typename std::iterator_traits<ForwardIt>::value_type moment(ForwardIt first, ForwardIt last,
                                                                          const typename std::iterator_traits<ForwardIt>::value_type mean)
    {
        using input_t = typename std::iterator_traits<ForwardIt>::value_type;
        return nthMoment<input_t, N>(first, last, mean);
    }

    template <typename ForwardIt>
    constexpr typename std::iterator_traits<ForwardIt>::value_type variance(ForwardIt first, ForwardIt last)
    {
        return moment<2>(first, last);
    }

    // 计算平均值和标准差的泛型函数
    template <typename InputIt>
    MeanSigma<typename std::iterator_traits<InputIt>::value_type> computeMeanSigma(InputIt first, InputIt last)
    {
        using T = typename std::iterator_traits<InputIt>::value_type;
        MeanSigma<T> result;

        // 计算平均值
        result.mean = mean(first, last);

        // 计算标准差
        T varianceSum = variance(first, last);
        result.sigma = std::sqrt(varianceSum); // 样本标准差

        return result;
    }

    // 适配器函数，用于std::vector
    template <typename T>
    void DSP_MEAN(std::vector<T> &src, DOUBLE *mean, DOUBLE *sigma)
    {
        MeanSigma<T> res = computeMeanSigma(src.begin(), src.end());
        *mean = res.mean;
        *sigma = res.sigma;
    }

    // 适配器函数，用于C风格数组
    template <typename T>
    void DSP_MEAN(T *src, DOUBLE *mean, DOUBLE *sigma, size_t size)
    {
        MeanSigma<T> res = computeMeanSigma(src, src + size);
        *mean = res.mean;
        *sigma = res.sigma;
    }

};

#endif //