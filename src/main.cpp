#include <iostream>
#include "HorsesDataSetParser.h"
int main()
{
    std::string path("/home/kot/Pobrane/horse-colic.data");
    std::shared_ptr<std::vector<Horse>> horses = HorsesDataSetParser::readFile(path);
	std::cout << "DUPA" << std::endl;
    return 0;
}
	
