
#include <cstdio>
#include <chrono>
#include <thread>
#include "parser.h"

int main() {
    std::chrono::milliseconds timespan(1000);

    typedef std::tuple<double, double, double> CSV_format;
    typedef std::function<CSV_format(const std::vector<std::string> &)> formatterT;
    printf("Hello");
    std::this_thread::sleep_for(timespan);
    enum RGB { Red = 1, Green, Blue };

    const std::string COLOR_MAP_PATH = "./example.csv";

    printf("Hello");
    std::this_thread::sleep_for(timespan);

    // Load the color map
    //
    auto colorMap = read_csv<CSV_format>(
        COLOR_MAP_PATH,
        ",",
        [](const std::vector<std::string> &values) {
        return CSV_format {
                // Here is the formatter lambda that convert each value from string to what you want
                std::strtod(values[Red].c_str(), nullptr),
                std::strtod(values[Green].c_str(), nullptr),
                std::strtod(values[Blue].c_str(), nullptr)
        };
    });
}
