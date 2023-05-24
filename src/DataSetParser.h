#ifndef DATASETPARSER_H
#define DATASETPARSER_H

#include <sstream>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <limits>
#include <boost/algorithm/string/replace.hpp>

enum class DataSets: uint8_t
{
    HORSE = 0
};

enum class ParameterType : uint8_t
{
    LINEAR = 0,
    SYMBOLIC = 1,
    TARGET = 2,
    IGNORED = 3
};

template<DataSets T>
struct DataSetsParameters {};

template<>
struct DataSetsParameters<DataSets::HORSE>
{
    static constexpr char* path = "/home/kot/Pobrane/horse-colic.data";
    static const inline std::vector<ParameterType> fieldMap = {ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::LINEAR,
                                                      ParameterType::LINEAR,
                                                      ParameterType::LINEAR,
                                                      ParameterType::LINEAR,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::LINEAR,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::LINEAR,
                                                      ParameterType::LINEAR,
                                                      ParameterType::SYMBOLIC,
                                                      ParameterType::LINEAR,
                                                      ParameterType::SYMBOLIC};
};

template<DataSets T>
struct Statistics
{
    struct Minmax
    {
        std::vector<float> minOfFields;
        std::vector<float> maxOfFields;

        Minmax(unsigned size) : minOfFields(size), maxOfFields(size)
        {
            std::fill(minOfFields.begin(), minOfFields.end(), std::numeric_limits<float>::max());
            std::fill(maxOfFields.begin(), maxOfFields.end(), std::numeric_limits<float>::min());
        }
    };

    static std::vector<std::vector<unsigned>> getNumberOfOccurrences(const std::vector<float>& vec, const Minmax& minmax)
    {
        std::vector<std::vector<unsigned>> stat(DataSetsParameters<T>::fieldMap.size());
        std::vector<int> symbolicFieldsList;
        for (unsigned j = 0; j < DataSetsParameters<T>::fieldMap.size(); ++j)
        {
            if (DataSetsParameters<T>::fieldMap[j] == ParameterType::SYMBOLIC)
            {
                symbolicFieldsList.push_back(j);
                stat[j] = std::vector<unsigned>(static_cast<int>(minmax.maxOfFields[j]) + 1);
            }
        }

        for (unsigned i = 0; i < vec.size(); i += DataSetsParameters<T>::fieldMap.size())
        {
            for (unsigned j = 0; j < symbolicFieldsList.size(); ++j)
            {
                const int fieldIdx = symbolicFieldsList[j];
                const int value = static_cast<int>(vec[i + fieldIdx]);
                ++stat[fieldIdx][value];
            }
        }
        return stat;
    }

    static Minmax getMinimumAndMaximumValues(const std::vector<float>& vec)
    {
        Minmax output(DataSetsParameters<T>::fieldMap.size());
        for (unsigned i = 0; i < vec.size(); i += DataSetsParameters<T>::fieldMap.size())
        {
            for (unsigned j = 0; j < DataSetsParameters<T>::fieldMap.size(); ++j)
            {
                output.minOfFields[j] = std::min(output.minOfFields[j], vec[i + j]);
                output.maxOfFields[j] = std::max(output.maxOfFields[j], vec[i + j]);
            }
        }
        return output;
    }
};

template<DataSets T>
class DataSetParser
{
public:
    static std::shared_ptr<std::vector<float>> readFile()
    {
        std::shared_ptr<std::vector<float>> out = std::make_shared<std::vector<float>>(0);

        std::ifstream file(DataSetsParameters<T>::path);
        std::string value;
        while (std::getline(file, value))
        {
            std::replace(value.begin(), value.end(), '?', '0');
            std::replace(value.begin(), value.end(), '.', ','); // bardzo wazne locale!
            std::stringstream ss(value);

            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> vstrings(begin, end);
            if (vstrings.size() < DataSetsParameters<T>::fieldMap.size())
            {
                std::cerr << "Invalid data file!" << std::endl;
                throw;
            }
            for (unsigned i = 0; i < std::min(vstrings.size(), DataSetsParameters<T>::fieldMap.size()); ++i)
            {
                out->push_back(std::stof(vstrings[i]));
            }
        }

        return out;
    }
};

#endif // DATASETPARSER_H
