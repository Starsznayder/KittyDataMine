#ifndef DATASETPARSER_H
#define DATASETPARSER_H

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <tuple>
#include <boost/algorithm/string/replace.hpp>

enum class DataSets: uint8_t
{
    HORSE = 0
};

template<DataSets T>
struct DataSetsParameters
{
    static constexpr char* path = "\0";
    using DataType = std::tuple<>;
};

template<>
struct DataSetsParameters<DataSets::HORSE>
{
    static constexpr char* path = "/home/kot/Pobrane/horse-colic.data";

    struct Types
    {
        using DataType = std::tuple<uint8_t, //ParameterType::SYMBOLIC,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    float, //ParameterType::LINEAR,
                                    float, //ParameterType::LINEAR,
                                    float, //ParameterType::LINEAR,
                                    float, //ParameterType::LINEAR,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    float, //ParameterType::LINEAR,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    int8_t, //ParameterType::SYMBOLIC,
                                    float, //ParameterType::LINEAR,
                                    float, //ParameterType::LINEAR,
                                    uint8_t, //ParameterType::SYMBOLIC,
                                    float, //ParameterType::LINEAR,
                                    uint8_t>; //ParameterType::SYMBOLIC>;
    };
};

template <typename... Args>
void printImpl(const Args&... tupleArgs) {
    size_t index = 0;
    auto printElem = [&index](const auto& x) {
        if (index++ > 0)
            std::cout << ", ";
        std::cout << x;
    };

    (printElem(tupleArgs), ...);
}

template<size_t I = 0, typename... Tp>
void setTuple(std::tuple<Tp...>& t, const std::vector<std::string>& vstrings, int& i)
{
    std::get<I>(t) = std::stof(vstrings[i++]);
    if constexpr(I+1 != sizeof...(Tp))
    {
        setTuple<I+1>(t, vstrings, i);
    }
}

template<size_t I = 0, typename... Tp>
void printTuple(std::tuple<Tp...>& t) {
    std::cout << static_cast<float>(std::get<I>(t)) << " ";
    if constexpr(I+1 != sizeof...(Tp))
    {
        printTuple<I+1>(t);
    }
}

template<typename T>
class DataSetParser
{
public:
    static std::shared_ptr<std::vector<typename T::Types::DataType>> readFile()
    {
        std::shared_ptr<std::vector<typename T::Types::DataType>> out = std::make_shared<std::vector<typename T::Types::DataType>>(0);
        static const size_t numberOfTupleMembers = std::tuple_size<typename T::Types::DataType>::value;

        std::ifstream file(T::path);
        std::string value;
        while (std::getline(file, value))
        {
            std::replace(value.begin(), value.end(), '?', '0');
            std::replace(value.begin(), value.end(), '.', ','); // bardzo wazne locale!
            std::stringstream ss(value);

            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> vstrings(begin, end);
            std::string* ptr = vstrings.data();
            if (vstrings.size() < numberOfTupleMembers)
            {
                std::cerr << "Invalid data file!" << std::endl;
                throw;
            }
            typename T::Types::DataType tmp;
            int i = 0;

            setTuple(tmp, vstrings, i);
            out->push_back(tmp);
        }

        return out;
    }
};

#endif // DATASETPARSER_H
