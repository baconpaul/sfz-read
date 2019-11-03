#ifndef _SFZ_READ_HPP_INCLUDE
#define _SFZ_READ_HPP_INCLUDE

#include <vector>
#include <string>

namespace SFZRead {
    struct KVPair {
        std::string key;
        std::string val;
    };
    
    struct DocumentSection {
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
        std::vector<DocumentSection> sections;
    };
    
};

#endif
