#include "loader.h"
#include "evaluate_classifier.h"
#include "krr_classifier.h"

#include <fstream>

int main(int argc, char* argv[]) {
    auto files = parse_train_commandline(argc, argv);
    Data data = load_data(files.first);

    Data training_data = data;
    filter_data(training_data);

    KRRClassifier krrc(data);
    dlib::serialize(files.second) << krrc;

    return 0;
}