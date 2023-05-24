#include <iostream>
#include "DataSetParser.h"

const DataSets SELECTED_SET = DataSets::HORSE;

int main()
{
    std::shared_ptr<std::vector<float>> data = DataSetParser<SELECTED_SET>::readFile();
    Statistics<SELECTED_SET>::Minmax minmax = Statistics<SELECTED_SET>::getMinimumAndMaximumValues(*data);
    for (unsigned i = 0; i < minmax.minOfFields.size(); ++i)
    {
        std::cout << "minimum [" << i << "]: " << minmax.minOfFields[i] << std::endl;
        std::cout << "maximum [" << i << "]: " << minmax.maxOfFields[i] << std::endl;
    }
    std::vector<std::vector<unsigned>> numberOfOccurrences = Statistics<SELECTED_SET>::getNumberOfOccurrences(*data, minmax);
	std::cout << "DUPA" << std::endl;
    return 0;
}
	
