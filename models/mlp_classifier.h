#pragma once

#include "data.h"
#include <dlib/mlp.h>

class MLPClassifier {
    using sample_type = dlib::matrix<double, FEATURE_COUNT, 1>;
    using output_type = dlib::matrix<double, 3, 1>;
    using kernel_type = dlib::mlp::kernel_1a;

    std::vector<sample_type> samples;
    std::vector<output_type> labels;

    kernel_type classifier;

    void load_data(const Data& data) {
        for (const auto& sample: data) {
            std::size_t label = get_best_result(sample);
            output_type out({0,0,0});
            out(2 - label,0) = 1;
            samples.emplace_back(dlib::mat(get_features(sample)));
            labels.push_back(out);
        }
    }

    void train() {
        for (std::size_t i = 0; i < samples.size(); ++i) {
            classifier.train(samples[i], labels[i]);
        }
    }

public:
    friend void serialize(const MLPClassifier& mlpc, std::ostream& out);
    friend void deserialize(MLPClassifier& mlpc, std::istream& in);

    MLPClassifier(): classifier(FEATURE_COUNT, 2*FEATURE_COUNT, FEATURE_COUNT, 3) {}
    MLPClassifier(const Data& data): MLPClassifier() {
        load_data(data);
        train();
    }
    std::size_t operator()(const Features& f) const {
        output_type res = classifier(dlib::mat(f));
        if (res(0) > std::max(res(1), res(2))) return 0;
        if (res(1) > res(2)) return 1;
        return 2;
    }
};

inline void serialize(const MLPClassifier& mlpc, std::ostream& out) {
    dlib::serialize(mlpc.classifier, out);
}
inline void deserialize(MLPClassifier& mlpc, std::istream& in) {
    dlib::deserialize(mlpc.classifier, in);
}
