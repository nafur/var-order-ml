#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

#include "data.h"

#include "json.hpp"
using json = nlohmann::json;

void print_train_usage_and_exit(int argc, char* argv[]) {
    std::cout << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
    std::exit(1);
}

std::pair<std::string,std::string> parse_train_commandline(int argc, char* argv[]) {
    if (argc != 3) print_train_usage_and_exit(argc, argv);
    return { argv[1], argv[2] };
}

void print_evaluate_usage_and_exit(int argc, char* argv[]) {
    std::cout << "Usage: " << argv[0] << " <data input file> <model file>" << std::endl;
    std::exit(1);
}

std::pair<std::string,std::string> parse_evaluate_commandline(int argc, char* argv[]) {
    if (argc != 3) print_evaluate_usage_and_exit(argc, argv);
    return { argv[1], argv[2] };
}

json load_json_data(const std::string& filename) {
    std::ifstream i(filename);
    json res;
    i >> res;
    return res;
}

Data load_data(const std::string& filename) {
    json j = load_json_data(filename);
    std::vector<std::string> solvers;
    {
        std::set<std::string> solverset;
        for (const auto& file: j.items()) {
            for (const auto& s: file.value()["results"].items()) {
                solverset.insert(s.key());
            }
        }
        for (const auto& s: solverset) {
            solvers.emplace_back(s);
        }
    }
    std::cout << "Found solvers:" << std::endl;
    for (std::size_t i = 0; i < solvers.size(); ++i) {
        std::cout << " " << i << ": " << solvers[i];
    }
    std::cout << std::endl;
    Data res;
    for (const auto& file: j.items()) {
        Features features;
        {
            for (const auto& f: file.value()["features"]) {
                features.emplace_back(f.get<double>());
            }
            assert(features.size() == FEATURE_COUNT);
        }
        Results results;
        {
            const json& r = file.value()["results"];
            for (const auto& s: solvers) {
                if (r.find(s) == r.end()) {
                    results.emplace_back(RESULT_INF);
                } else {
                    results.emplace_back(r[s].get<double>());
                }
            }
        }
        res.emplace_back(file.key(), std::move(features), std::move(results));
    }
    std::cout << "Loaded " << res.size() << " samples" << std::endl;
    return res;
}

void filter_data(Data& data) {
    auto it = std::remove_if(data.begin(), data.end(), [](const Sample& s){
        const Results& r = get_results(s);
        auto mm = std::minmax_element(r.begin(), r.end());
        double min = *mm.first;
        double max = *mm.second;
        if (max <= 0.01) return true;
        if (r.size() < 2) return false;
        if (min*1.5 >= max) return true;
        return false;
    });
    data.erase(it, data.end());

    std::cout << "Remaining after filter: " << data.size() << std::endl;
}