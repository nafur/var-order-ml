#pragma once

#include "data.h"

template<typename T>
void evaluate_classifier(const T& classifier, const Data& data) {
    std::size_t count = get_results_count(data);
    std::vector<double> runtimes(count, 0.0);
    double rt_class = 0;
    double rt_vbs = 0;
    for (const auto& sample: data) {
        std::size_t selection = classifier(get_features(sample));
        const Results& res = get_results(sample);
        double min = RESULT_INF;
        for (std::size_t i = 0; i < res.size(); ++i) {
            runtimes[i] += res[i];
            if (res[i] < min) {
                min = res[i];
            }
        }
        rt_class += res[selection];
        rt_vbs += min;
    }
    for (std::size_t i = 0; i < runtimes.size(); ++i) {
        std::cout << "\t" << i << "\t-> " << runtimes[i] << std::endl;
    }
    std::cout << "\tML\t-> " << rt_class << std::endl;
    std::cout << "\tVBS\t-> " << rt_vbs << std::endl;
}

template<typename T>
void evaluate_classifier(const T& classifier, const Data& training_data, const Data& data) {
    std::cout << "Accumulative runtimes on training data" << std::endl;
    evaluate_classifier(classifier, training_data);
    std::cout << "Accumulative runtimes on all data" << std::endl;
    evaluate_classifier(classifier, data);
}