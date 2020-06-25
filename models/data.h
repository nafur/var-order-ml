#pragma once

#include <tuple>
#include <vector>

#define FEATURE_COUNT 21
#define RESULT_INF 1000

using Features = std::vector<double>;
using Results = std::vector<double>;
using Sample = std::tuple<std::string, Features, Results>;
using Data = std::vector<Sample>;

const std::string& get_name(const Sample& s) {
    return std::get<0>(s);
}
const Features& get_features(const Sample& s) {
    return std::get<1>(s);
}
const Results& get_results(const Sample& s) {
    return std::get<2>(s);
}

std::size_t get_best_result(const Sample& s) {
    const Results& res = get_results(s);
    std::size_t min = 0;
    for (std::size_t i = 1; i < res.size(); ++i) {
        if (res[i] < res[min]) {
            min = i;
        }
    }
    return min;
}

std::size_t get_results_count(const Data& data) {
    assert(!data.empty());
    return get_results(data[0]).size();
}

