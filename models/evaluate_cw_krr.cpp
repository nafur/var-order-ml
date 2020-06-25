#include "loader.h"
#include "classification_wrapper.h"
#include "evaluate_classifier.h"
#include "krr_regression.h"

#include <fstream>

int main(int argc, char* argv[]) {
    auto files = parse_evaluate_commandline(argc, argv);
    Data data = load_data(files.second);

    Data training_data = data;
    filter_data(training_data);

    ClassificationWrapper<KRRRegression> cw_krr;
    dlib::deserialize(files.first) >> cw_krr;

    evaluate_classifier(cw_krr, training_data, data);

    return 0;
}