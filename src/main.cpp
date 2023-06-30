#include <iostream>
#include "DataSetParser.h"

const DataSets SELECTED_SET = DataSets::HORSE;

int main()
{
   std::shared_ptr<std::vector<DataSetsParameters<DataSets::HORSE>::Types::DataType>> data = DataSetParser<DataSetsParameters<SELECTED_SET>>::readFile();
   for (unsigned i = 0; i < data->size(); ++i)
   {
       printTuple(data->at(i));
       std::cout << std::endl;
   }
   // for (unsigned i = 0; i < minmax.minOfFields.size(); ++i)
  // }
  //  std::vector<std::vector<unsigned>> numberOfOccurrences = Statistics<SELECTED_SET>::getNumberOfOccurrences(*data, minmax);
    
    return 0;
}
	
