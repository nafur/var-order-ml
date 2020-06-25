#include "classification_wrapper.h"
#include "loader.h"

#include "krr_regression.h"
#include "krr_classifier.h"
#include "mlp_classifier.h"
#include "svr_regression.h"

#ifndef CLASSIFIER
#error "No CLASSIFIER was specified, call with -DCLASSIFIER=\"...\""
#endif

int main(int argc, char* argv[]) {
    auto files = parse_train_commandline(argc, argv);
    Data data = load_data(files.second);

    Data training_data = data;
    filter_data(training_data);

    CLASSIFIER obj(training_data);
    dlib::serialize(files.second) << obj;

    return 0;
}