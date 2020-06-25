#include "loader.h"
#include "classification_wrapper.h"
#include "evaluate_classifier.h"
#include "svr_regression.h"

#include <fstream>

int main(int argc, char* argv[]) {
    auto files = parse_train_commandline(argc, argv);
    Data data = load_data(files.first);

    Data training_data = data;
    filter_data(training_data);

    ClassificationWrapper<SVRRegression> cw_svr(training_data);
    dlib::serialize(files.second) << cw_svr;

    return 0;
}