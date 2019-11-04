#ifndef _SFZ_READ_HPP_INCLUDE
#define _SFZ_READ_HPP_INCLUDE

#include <vector>
#include <string>

namespace SFZRead {
    struct KVPair {
        std::string key;

        typedef enum {
            String,
            Double,
        } Type;

        Type type = String;
        std::string val;
        double fval;
        
        std::string toString();
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
        std::string typeString;
        std::vector<KVPair> opcodes;
        std::string toString();
    };

    struct Document {
        std::string rawText;
        std::vector<DocumentHeader> headers;
        void parse( std::string &contents );
        std::string toString();
    };
    
};

#endif
