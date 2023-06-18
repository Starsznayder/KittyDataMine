#ifndef ParsersH
#define ParsersH

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cerrno>
#include <cstring>
#include <tuple>


using IrisTuple = std::tuple<float, float, float, float>;
using HabermanTuple = std::tuple<float, float, float>;
using GermanTuple = std::tuple<
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



std::vector<std::tuple<float, float, float>> read_example() {
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


std::tuple<std::vector<HabermanTuple>,
           std::vector<uint8_t>> read_haberman() {

    std::ifstream File("data/haberman.csv");

    std::vector<HabermanTuple> data_vec;
    std::vector<uint8_t> cls_vec;

    float cls,
          a,
          b,
          c;

    while (File >> cls >> a >> b >> c) {
        data_vec.push_back(HabermanTuple(a, b, c));
        cls_vec.push_back((uint8_t) cls);
    }
    return std::make_tuple(data_vec, cls_vec);
}


std::tuple<std::vector<IrisTuple>,
           std::vector<uint8_t>> read_iris(std::string path) {

    std::ifstream File(path);

    std::vector<IrisTuple> data_vec;
    std::vector<uint8_t> cls_vec;


    float cls;
    float d0, d1, d2, d3;

    while (File >> cls >> d0 >> d1 >> d2 >> d3) {
        data_vec.push_back(IrisTuple(d0, d1, d2, d3));
        cls_vec.push_back((uint8_t) cls);
    }
    return std::make_tuple(data_vec, cls_vec);
}


std::tuple<std::vector<GermanTuple>,
           std::vector<uint8_t>> read_german() {

    std::ifstream File("data/german.csv");

    std::vector<GermanTuple> data_vec;
    std::vector<uint8_t> cls_vec;

    float cls;
    float d0,
          d1, 
          d2,
          d3,
          d4,
          d5,
          d6,
          d7,
          d8,
          d9,
          d10,
          d11,
          d12,
          d13,
          d14,
          d15,
          d16,
          d17,
          d18,
          d19;

    while (File >> cls >> 
            d0 >>    
            d1 >>    
            d2 >>    
            d3 >>    
            d4 >>    
            d5 >>    
            d6 >>    
            d7 >>    
            d8 >>    
            d9 >>    
            d10 >> 
            d11 >> 
            d12 >> 
            d13 >> 
            d14 >> 
            d15 >> 
            d16 >> 
            d17 >> 
            d18 >> 
            d19
        ) {
        data_vec.push_back(GermanTuple(
    (uint8_t) d0,
    (float)   d1,
    (uint8_t) d2,
    (uint8_t) d3,
    (float)   d4,
    (uint8_t) d5,
    (uint8_t) d6,
    (float)   d7,
    (uint8_t) d8,
    (uint8_t) d9,
    (float)   d10,
    (uint8_t) d11,
    (float)   d12,
    (uint8_t) d13,
    (uint8_t) d14,
    (float)   d15,
    (uint8_t) d16,
    (float)   d17,
    (uint8_t) d18,
    (uint8_t) d19
        ));
        cls_vec.push_back((uint8_t) cls);
    }
    return std::make_tuple(data_vec, cls_vec);
}

#endif
