#include "sfz-read.hpp"
#include <iostream>

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
}
