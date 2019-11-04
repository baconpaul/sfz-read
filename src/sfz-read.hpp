#ifndef _SFZ_READ_HPP_INCLUDE
#define _SFZ_READ_HPP_INCLUDE

#include <vector>
#include <string>

namespace SFZRead {
    struct KVPair {
        std::string key;
        // FIXME make this have a double later too
        std::string val;
    };
    
    struct DocumentHeader {
        typedef enum {
            CONTROL,
            GLOBAL,
            MASTER,
            GROUP,
            REGION
        } Type;

        Type type;
        std::vector<KVPair> opcodes;
    };

    struct Document {
        std::string rawText;
        std::vector<DocumentHeader> sections;
        void parse( std::string &contents );
    };
    
};

#endif
