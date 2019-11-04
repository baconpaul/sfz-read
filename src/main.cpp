#include "sfz-read.hpp"
#include <fstream>
#include <iostream>
#include <sstream> 

int main( int argc, char **argv )
{
    std::cout << "Main" << std::endl;
    std::string fn = "./default.sfz";
    if( argc >= 2 )
    {
        std::cout << "   a1=" << argv[ 1 ] << std::endl;
        std::cout << "   a2=" << argv[ 2 ] << std::endl;
        fn = std::string(argv[1]) + "/" + std::string(argv[2]);
    }
    std::cout << "Reading SFZ file " << fn << std::endl;

    std::ifstream inFile;
    inFile.open(fn.c_str()); //open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
    auto contents = strStream.str(); //str holds the content of the file

    std::cout << "Parsing contents" << std::endl;
    SFZRead::Document doc;
    doc.parse(contents);
}
