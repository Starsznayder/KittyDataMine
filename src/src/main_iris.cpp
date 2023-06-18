#ifndef ParserH
#define ParserH

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cerrno>
#include <cstring>
#include <tuple>

using IrisType = std::tuple<float, float, float, float>;
using HabermanType = std::tuple<float, float, float>;
using GermanType = std::tuple<
    uint8_t,
    float,
    uint8_t,
    uint8_t,
    float,
    uint8_t,
    uint8_t,
    float,
    uint8_t,
    uint8_t,
    float,
    uint8_t,
    float,
    uint8_t,
    uint8_t,
    float,
    uint8_t,
    float,
    uint8_t,
    uint8_t
>;

std::vector<std::tuple<float, float, float>> Readfile() {
    std::ifstream File("./example.csv");
    std::vector<std::tuple<float, float, float>> data;
    std::string name;
    float a;
    float b;
    float c;

    while (File >> a >> b >> c) {
        data.push_back(std::tuple<float, float, float>(a, b, c));
    }
    return data;
}

std::tuple<std::vector<HabermanType>,
           std::vector<uint8_t>> read_haberman() {

    std::ifstream File("data/haberman.csv");

    std::vector<HabermanType> data_vec;
    std::vector<uint8_t> cls_vec;


    uint8_t cls;
    float a;
    float b;
    float c;

    while (File >> cls >> a >> b >> c) {
        data_vec.push_back(HabermanType(a, b, c));
    }
    return std::make_tuple(data_vec, cls_vec);
}

int main() {
    auto df = read_haberman();
    auto X = std::get<0>(df); 
    auto y = std::get<1>(df);

    for (const auto& i : X) {
       std::cout << std::get<0>(i) << ", " << std::get<1>(i) << ", " << std::get<2>(i) << ", " << std::endl;
    }

    return 0;
}

#endif
