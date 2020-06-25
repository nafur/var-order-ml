#pragma once

#include <limits>

#include <dlib/serialize.h>

#include "data.h"

template<typename T>
class ClassificationWrapper {
    std::vector<T> regressions;
public:
    template<typename TT>
    friend void serialize(const ClassificationWrapper<TT>& cw, std::ostream& out);
    template<typename TT>
    friend void deserialize(ClassificationWrapper<TT>& cw, std::istream& in);

    ClassificationWrapper() = default;
    ClassificationWrapper(const Data& data) {
        std::size_t count = get_results_count(data);
        for (std::size_t i = 0; i < count; ++i) {
            std::cout << "Training regression for " << i << std::endl;
            regressions.emplace_back(data, i);
        }
    }

    std::size_t operator()(const Features& f) const {
        double min = RESULT_INF;
        std::size_t res = 0;
        for (std::size_t i = 0; i < regressions.size(); ++i) {
            double cur = regressions[i](f);
            if (cur < min) {
                min = cur;
                res = i;
            }
        }
        return res;
    }
};

template<typename T>
void serialize(const ClassificationWrapper<T>& cw, std::ostream& out) {
    dlib::serialize(cw.regressions, out);
}
template<typename T>
void deserialize(ClassificationWrapper<T>& cw, std::istream& in) {
    dlib::deserialize(cw.regressions, in);
}
