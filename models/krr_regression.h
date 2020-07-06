#pragma once

#include "data.h"
#include <dlib/svm.h>

class KRRRegression {
    using sample_type = dlib::matrix<double, FEATURE_COUNT, 1>;
    using kernel_type = dlib::radial_basis_kernel<sample_type>;

    std::vector<sample_type> samples;
    std::vector<double> labels;

    dlib::decision_function<kernel_type> tester;

    static constexpr bool scale_log = true;

    void load_data(const Data& data, std::size_t solver_id) {
        for (const auto& sample: data) {
            samples.push_back(dlib::mat(get_features(sample)));
            labels.push_back(get_results(sample)[solver_id]);
            if (scale_log) {
                labels.back() = std::log2(labels.back() + 1);
            }
        }
    }

    void train() {
        dlib::krr_trainer<kernel_type> trainer;
        
        double gamma = 1.0 / dlib::compute_mean_squared_distance(dlib::randomly_subsample(samples, 2000));
        trainer.set_kernel(kernel_type(gamma));
        tester = trainer.train(samples, labels);
    }

public:
    friend void serialize(const KRRRegression& krrr, std::ostream& out);
    friend void deserialize(KRRRegression& krrr, std::istream& in);

    KRRRegression() = default;
    KRRRegression(const Data& data, std::size_t solver_id) {
        load_data(data, solver_id);
        train();
    }

    double operator()(const Features& f) const {
        if (scale_log) {
            return std::exp2(tester(dlib::mat(f))) - 1;
        } else {
            return tester(dlib::mat(f));
        }
    }
};

inline void serialize(const KRRRegression& krrr, std::ostream& out) {
    dlib::serialize(krrr.tester, out);
}
inline void deserialize(KRRRegression& krrr, std::istream& in) {
    dlib::deserialize(krrr.tester, in);
}
