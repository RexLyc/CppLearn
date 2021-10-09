#pragma once
#include<iterator>
#include<random>
#include<chrono>
namespace lyc_algorithm {
    // 传统单向扫描
    template<typename iter>
    iter partition(iter begin, iter end, iter pivot) {
        std::iter_swap(begin, pivot);
        iter i, j;
        for (i = begin, j = begin + 1; j != end; ++j) {
            if (*j < *begin) {
                ++i;
                std::iter_swap(i, j);
            }
        }
        std::iter_swap(i, begin);
        return i;
    }

    // 调试用的，留给你啦
    template<typename iter>
    void iterPrint(iter begin, iter end) {
        for (auto it = begin; it != end; ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    // randomized - select
    template<typename iter>
    iter randomizedSelect(iter curBegin, iter curEnd, size_t i) {
        // 使用了C++11中简单好用的mt19937随机数，用chrono库获取时间作为种子
        static std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
        static std::mt19937_64 rand(seed);
        if (curBegin == curEnd) {
            return curBegin;
        }
        // 随机数范围是0到区间最后一位，构造函数就是给出这个闭区间。
        std::uniform_int_distribution<std::size_t> dist(0, std::distance(curBegin, curEnd) - 1);
        auto pIndex = curBegin;
        std::advance(pIndex, dist(rand));
        //std::cout << std::distance(curBegin, pIndex) << std::endl;
        pIndex = partition(curBegin, curEnd, pIndex);
        //std::cout << std::distance(curBegin, pIndex) << std::endl;
        //iterPrint(curBegin, curEnd);
        if (std::distance(curBegin, pIndex) == i) {
            return pIndex;
        }
        else if (std::distance(curBegin, pIndex) < i) {
            return randomizedSelect(pIndex + 1, curEnd, i - std::distance(curBegin, pIndex) - 1);
        }
        else {
            return randomizedSelect(curBegin, pIndex, i);
        }
    }

    // partition和iterPrint在上面，自取
// 使用插入排序获得中位数
    template<typename iter>
    iter GetMedian(iter begin, iter end) {
        // insert sort
        iter a, b;
        for (std::size_t i = 1; i != std::distance(begin, end); ++i) {
            for (std::size_t j = i; j != 0; --j) {
                a = b = begin;
                std::advance(a, j - 1);
                std::advance(b, j);
                if (*a > *b) {
                    std::iter_swap(a, b);
                }
                else {
                    break;
                }
            }
        }
        //iterPrint(begin, end);
        std::advance(begin, (std::distance(begin, end) - 1) / 2); // 默认取较小的中位数
        //std::cout << "choose local median: " << *begin << std::endl;
        return begin;
    }

    template<typename iter>
    iter BFPRT(iter curBegin, iter curEnd, std::size_t i) {
        typedef std::iterator_traits<iter>::value_type T;
        if (std::distance(curBegin, curEnd) == 1) {
            return curBegin;
        }
        std::vector<T> medians;
        medians.reserve((std::distance(curBegin, curEnd) + 4) / 5); // 保证不完整的一组也考虑进来
        iter it = curBegin;
        iter temp = it;
        while (std::distance(it, curEnd) >= 5) {
            std::advance(temp, 5);
            medians.push_back(*GetMedian(it, temp));
            it = temp;
        }
        if (it != curEnd) { // 这就是那不完整的一组
            medians.push_back(*GetMedian(it, curEnd));
        }
        iter pivot = BFPRT(medians.begin(), medians.end(), medians.size() / 2);
        pivot = std::find(curBegin, curEnd, *pivot); //找到原来容器中的位置
        pivot = partition(curBegin, curEnd, pivot); //找到分片之后的位置（这里暂时没想到更好的办法）
        //iterPrint(curBegin, curEnd);
        if (std::distance(curBegin, pivot) == i) {
            return pivot;
        }
        else if (std::distance(curBegin, pivot) < i) {
            return BFPRT(pivot + 1, curEnd, i - std::distance(curBegin, pivot) - 1);
        }
        else {
            return BFPRT(curBegin, pivot, i);
        }
    }
}