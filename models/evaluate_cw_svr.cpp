#include "loader.h"
#include "classification_wrapper.h"
#include "evaluate_classifier.h"
#include "svr_regression.h"

#include <fstream>

int main(int argc, char* argv[]) {
    auto files = parse_evaluate_commandline(argc, argv);
    Data data = load_data(files.second);

    Data training_data = data;
    filter_data(training_data);

    ClassificationWrapper<SVRRegression> cw_svr;
    dlib::deserialize(files.first) >> cw_svr;

    evaluate_classifier(cw_svr, training_data, data);

    return 0;
}