#pragma once

#include "data.h"
#include <dlib/svm.h>

class KRRClassifier {
    using sample_type = dlib::matrix<double, FEATURE_COUNT, 1>;
    using kernel_type = dlib::radial_basis_kernel<sample_type>;

    std::vector<sample_type> samples;
    std::vector<double> labels;

    dlib::decision_function<kernel_type> classifier;

    void load_data(const Data& data) {
        for (const auto& sample: data) {
            double label = -1;
            {
                const Results& res = get_results(sample);
                double ref = RESULT_INF;
                for (std::size_t i = 0; i < res.size(); ++i) {
                    if (ref > res[i]) {
                        ref = res[i];
                        label = i;
                    }
                }
            }
            samples.emplace_back(dlib::mat(get_features(sample)));
            labels.push_back(label);
        }
    }

    void normalize_data() {
        dlib::vector_normalizer<sample_type> normalizer;
        normalizer.train(samples);
        for (auto& s: samples) {
            s = normalizer(s);
        }
    }

    void train(double gamma) {
        dlib::krr_trainer<kernel_type> trainer;
        trainer.use_classification_loss_for_loo_cv();
        trainer.set_kernel(kernel_type(gamma));
        std::vector<double> loo_values;
        classifier = trainer.train(samples, labels, loo_values);
        double accuracy = dlib::mean_sign_agreement(labels, loo_values);
        std::cout << "gamma " << gamma << " -> " << accuracy << std::endl;
    }

public:
    friend void serialize(const KRRClassifier& krrc, std::ostream& out);
    friend void deserialize(KRRClassifier& krrc, std::istream& in);

    KRRClassifier() = default;
    KRRClassifier(const Data& data) {
        load_data(data);
        normalize_data();
        // TODO: Check why all gammas are the same.
        train(0.001);
    }
    std::size_t operator()(const Features& f) const {
        double res = classifier(dlib::mat(f));
        assert(res >= 0);
        return std::round(res);
    }
};

inline void serialize(const KRRClassifier& krrc, std::ostream& out) {
    dlib::serialize(krrc.classifier, out);
}
inline void deserialize(KRRClassifier& krrc, std::istream& in) {
    dlib::deserialize(krrc.classifier, in);
}
