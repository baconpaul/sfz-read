#include "sfz-read.hpp"
#include <iostream>
#include <cstdlib>
#include <sstream> 

#define TAO_PEGTL_NAMESPACE svz_read_pegtl

#include "tao/pegtl.hpp"

using namespace tao::TAO_PEGTL_NAMESPACE;


namespace SFZRead
{
namespace readParser
{
    // The grammar
    struct kvkey : identifier {};
    struct kvkeyeq : seq<kvkey, one<'='>> {};
    struct header_tag;

    struct kvvalue_string : until<sor<at<header_tag>,at<kvkeyeq>,at<eolf>>> {}; // I know this is wrong
    struct kvvalue_digit : seq< opt< one< '+', '-' > >, plus<digit>, opt< seq< string< '.' >, plus<digit> > > > {};

    struct kvvalue : sor<seq<kvvalue_digit, star<space>>, kvvalue_string> {};
    struct kvpair : if_must< kvkeyeq, kvvalue > {};
    struct comment : if_must< string<'/', '/'>, until<eolf>> {};
    struct header_type : identifier {};
    struct header_tag : seq< one<'<'>, header_type, one<'>'> > {};
    
    struct header : seq<header_tag, star< seq<pad<sor< comment, kvpair >,space> > > > {};

    struct anything : sor<comment, header> {};
    struct file : until<eof, anything> {};

    // The state
    struct sfzstate
    {
        KVPair currentPair;
        DocumentHeader currentHeader;
        std::vector<DocumentHeader> headers;
    };

    // The action
    template< typename Rule >
    struct action : nothing< Rule >
    {
    };

    template<>
    struct action< header_type >
    {
        template< typename Input >
        static void apply( const Input& in, sfzstate& out )
            {
                out.currentHeader.typeString = in.string();
            }
    };

    
    template<>
    struct action< header >
    {
        template< typename Input >
        static void apply( const Input& in, sfzstate& out )
            {
                out.headers.push_back(out.currentHeader);
                out.currentHeader = DocumentHeader();
            }
    };

    
    
    template<>
    struct action< kvpair >
    {
        template< typename Input >
        static void apply( const Input& in, sfzstate& out )
            {
                //std::cout << "KVPAIR " << in.string() << std::endl;
                out.currentHeader.opcodes.push_back(out.currentPair);
                out.currentPair = KVPair();
            }
    };


    template<>
    struct action< kvkey >
    {
        template< typename Input >
        static void apply( const Input& in, sfzstate& out )
            {
                out.currentPair.key = in.string();
            }
    };
    
    template<>
    struct action< kvvalue_digit >
    {
        template< typename Input >
        static void apply( const Input& in, sfzstate& out )
            {
                out.currentPair.type = KVPair::Double;
                out.currentPair.val = in.string();
                out.currentPair.fval = std::atof(in.string().c_str());
            }
    };

    template<>
    struct action< kvvalue_string >
    {
        template< typename Input >
        static void apply( const Input& in, sfzstate& out )
            {
                out.currentPair.type = KVPair::String;
                out.currentPair.val = in.string();
            }
    };
}
    
void Document::parse(std::string &contents)
{
    rawText = contents;

    memory_input<> cmem( contents, "" );
    readParser::sfzstate result;
    tao::TAO_PEGTL_NAMESPACE::parse< readParser::file, readParser::action >( cmem, result );
    std::cout << "Parse complete" << std::endl;
    headers = result.headers;
}

std::string Document::toString()
{
    std::stringstream oss;
    oss << "SFZ Document\n";
    for( auto h : headers )
        oss << h.toString();
    return oss.str();
}
std::string DocumentHeader::toString()
{
    std::stringstream oss;
    oss << "<" << typeString << ">\n";
    for( auto p : opcodes )
        oss << "    " << p.toString() << "\n";
    return oss.str();
}
std::string KVPair::toString()
{
    std::stringstream oss;
    oss << key << "=" ;
    if( type == String )
        oss << val << " (str)";
    else
        oss << fval << " (dbl)";
    return oss.str();
}
}
