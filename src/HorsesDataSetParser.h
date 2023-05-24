#ifndef HORSESDATASETPARSER_H
#define HORSESDATASETPARSER_H

#include <regex>
#include <sstream>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <boost/algorithm/string/replace.hpp>

struct Horse
{
    enum class TemperatureOfExtremilities : unsigned
    {
        UNKNOWN = 0,
        NORMAL = 1,
        WARM = 2,
        COOL = 3,
        COLD = 4
    };

    enum class PeripherialPulse : unsigned
    {
        UNKNOWN = 0,
        NORMAL = 1,
        INCREASED = 2,
        REDUCED = 3,
        ABSENT = 4
    };

    enum class MucousMembranes : unsigned
    {
        UNKNOWN = 0,
        NORMAL = 1,
        BRIGHT = 2,
        PALE = 3,
        PALE_CYANOTIC = 4,
        RED = 5,
        DARK = 6
    };

    enum class CapilaryRefillTime : unsigned
    {
        UNKNOWN = 0,
        LOW = 1,
        HIGH = 2
    };

    enum class PainLevel : unsigned
    {
        UNKNOWN = 0,
        NO = 1,
        DEPRESSED = 2,
        MILD = 3,
        SEVERE  = 4,
        CONTINOUS = 5
    };

    enum class Persistalis : unsigned
    {
        UNKNOWN = 0,
        HYPERMOLITE = 1,
        NORMAL = 2,
        HYPOMOLITE = 3,
        ABSENT  = 4
    };

    enum class AbdominalDistension : unsigned
    {
        UNKNOWN = 0,
        NONE = 1,
        SLIGHT = 2,
        MODERATE = 3,
        SEVERE  = 4
    };

    enum class NasogastricTube : unsigned
    {
        UNKNOWN = 0,
        NONE = 1,
        SLIGHT = 2,
        SIGNIFICANT = 3,
    };

    enum class NasogastricReflux : unsigned
    {
        UNKNOWN = 0,
        NONE = 1,
        HIGH = 2,
        LOW = 3,
    };

    enum class RectalExamination : unsigned
    {
        UNKNOWN = 0,
        NORMAL = 1,
        INCREASED = 2,
        DECREASED = 3,
        ABSENT = 4
    };

    enum class Abdomen : unsigned
    {
        UNKNOWN = 0,
        NORMAL = 1,
        OTHER = 2,
        LARGE = 3,
        DISTENDED_SMALL = 4,
        DISTENDED_LARGE = 5,
    };

    enum class AbdominocentesisAppearance : unsigned
    {
        UNKNOWN = 0,
        CLEAR = 1,
        CLOUDY = 2,
        SEROSANGUINOUS = 3
    };

    enum class Outcome : unsigned
    {
        UNKNOWN = 0,
        LIVED = 1,
        DIED = 2,
        KILLED = 3
    };

    bool surgeryNeeded;
    bool isYoung;
    int ID;
    float rectTemp;
    int pulse;
    int respRate;
    TemperatureOfExtremilities extremTemp;
    PeripherialPulse peripherialPulse;
    MucousMembranes mucousMembranes;
    CapilaryRefillTime capilaryRefillTime;
    PainLevel painLevel;
    Persistalis persistalis;
    AbdominalDistension abdominalDistension;
    NasogastricTube nasogastricTube;
    NasogastricReflux nasogastricReflux;
    int nasogastricRefluxPH;
    RectalExamination rectalExamination;
    Abdomen abdomen;
    float cellVolume;
    float totalProtein;
    AbdominocentesisAppearance abdominocentesisAppearance;
    float abdomcentesisTotalProtein;
    Outcome outcome;

    Horse() {}

    Horse(std::string value)
    {
        std::cout << value << std::endl;
        std::replace(value.begin(), value.end(), '?', '0');
        std::replace(value.begin(), value.end(), '.', ',');
        //value = std::regex_replace(value, std::regex("."), ","); // bardzo wazne locale!
        std::stringstream ss(value);

        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);

        if (vstrings.size() < 23)
        {
            std::cout << "KOT!" << std::endl;
            throw;
        }

        surgeryNeeded = std::stoi(vstrings[0]) - 1;
        isYoung = std::stoi(vstrings[1]) - 1;
        ID = std::stoi(vstrings[2]);
        rectTemp = std::stof(vstrings[3]);
        pulse = std::stoi(vstrings[4]);
        respRate = std::stoi(vstrings[5]);
        extremTemp = static_cast<TemperatureOfExtremilities>(std::stoi(vstrings[6]));
        peripherialPulse = static_cast<PeripherialPulse>(std::stoi(vstrings[7]));
        mucousMembranes = static_cast<MucousMembranes>(std::stoi(vstrings[8]));
        capilaryRefillTime = static_cast<CapilaryRefillTime>(std::stoi(vstrings[9]));
        painLevel = static_cast<PainLevel>(std::stoi(vstrings[10]));
        persistalis = static_cast<Persistalis>(std::stoi(vstrings[11]));
        abdominalDistension = static_cast<AbdominalDistension>(std::stoi(vstrings[12]));
        nasogastricTube = static_cast<NasogastricTube>(std::stoi(vstrings[13]));
        nasogastricReflux = static_cast<NasogastricReflux>(std::stoi(vstrings[14]));
        nasogastricRefluxPH = std::stoi(vstrings[15]);
        rectalExamination = static_cast<RectalExamination>(std::stoi(vstrings[16]));
        abdomen = static_cast<Abdomen>(std::stoi(vstrings[17]));
        cellVolume = std::stof(vstrings[18]);
        totalProtein = std::stof(vstrings[19]);
        abdominocentesisAppearance = static_cast<AbdominocentesisAppearance>(std::stoi(vstrings[20]));
        abdomcentesisTotalProtein = std::stof(vstrings[21]);
        outcome = static_cast<Outcome>(std::stoi(vstrings[22]));
    }
};

class HorsesDataSetParser
{
public:
    static std::shared_ptr<std::vector<Horse>> readFile(const std::string& path)
    {
        std::shared_ptr<std::vector<Horse>> out = std::make_shared<std::vector<Horse>>(0);

        std::ifstream file(path);
        std::string str;
        while (std::getline(file, str))
        {
            out->push_back(Horse(str));
        }

        return out;
    }
};

#endif // HORSESDATASETPARSER_H
