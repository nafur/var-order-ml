#include "classification_wrapper.h"
#include "evaluate_classifier.h"
#include "loader.h"

#include "krr_regression.h"
#include "krr_classifier.h"
#include "mlp_classifier.h"
#include "svr_regression.h"

#ifndef CLASSIFIER
#error "No CLASSIFIER was specified, call with -DCLASSIFIER=\"...\""
#endif

int main(int argc, char* argv[]) {
    auto files = parse_evaluate_commandline(argc, argv);
    Data data = load_data(files.first);

    Data training_data = data;
    filter_data(training_data);

    CLASSIFIER obj;
    dlib::deserialize(files.second) >> obj;

    evaluate_classifier(obj, training_data, data);

    return 0;
}