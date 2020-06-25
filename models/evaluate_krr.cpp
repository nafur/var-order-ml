#include "loader.h"
#include "evaluate_classifier.h"
#include "krr_classifier.h"

#include <fstream>

int main(int argc, char* argv[]) {
    auto files = parse_evaluate_commandline(argc, argv);
    Data data = load_data(files.second);

    Data training_data = data;
    filter_data(training_data);

    KRRClassifier krrc;
    dlib::deserialize(files.first) >> krrc;

    evaluate_classifier(krrc, training_data, data);

    return 0;
}