#pragma once

#include "data.h"
#include <dlib/svm.h>

class SVRRegression {
    using sample_type = dlib::matrix<double, FEATURE_COUNT, 1>;
    using kernel_type = dlib::radial_basis_kernel<sample_type>;

    std::vector<sample_type> samples;
    std::vector<double> labels;

    dlib::decision_function<kernel_type> tester;

    void load_data(const Data& data, std::size_t solver_id) {
        for (const auto& sample: data) {
            samples.push_back(dlib::mat(get_features(sample)));
            labels.push_back(get_results(sample)[solver_id]);
        }
    }

    void train() {
        dlib::svr_trainer<kernel_type> trainer;
        
        trainer.set_kernel(kernel_type(0.1));
        trainer.set_c(10);
        trainer.set_epsilon_insensitivity(0.001);
        tester = trainer.train(samples, labels);
    }

public:
    friend void serialize(const SVRRegression& svrr, std::ostream& out);
    friend void deserialize(SVRRegression& svrr, std::istream& in);

    SVRRegression() = default;
    SVRRegression(const Data& data, std::size_t solver_id) {
        load_data(data, solver_id);
        train();
    }

    double operator()(const Features& f) const {
        return tester(dlib::mat(f));
    }
};

inline void serialize(const SVRRegression& svrr, std::ostream& out) {
    dlib::serialize(svrr.tester, out);
}
inline void deserialize(SVRRegression& svrr, std::istream& in) {
    dlib::deserialize(svrr.tester, in);
}
