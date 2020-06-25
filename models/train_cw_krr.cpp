#include "loader.h"
#include "classification_wrapper.h"
#include "evaluate_classifier.h"
#include "krr_regression.h"

#include <fstream>

int main(int argc, char* argv[]) {
    auto files = parse_train_commandline(argc, argv);
    Data data = load_data(files.first);

    Data training_data = data;
    filter_data(training_data);

    ClassificationWrapper<KRRRegression> cw_krr(training_data);
    dlib::serialize(files.second) << cw_krr;

    return 0;
}