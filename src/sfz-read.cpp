#include "sfz-read.hpp"
#include <iostream>

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
    struct header_tag : seq< one<'<'>, identifier, one<'>'> > {};
    
    struct header : seq<header_tag, star< seq<pad<sor< comment, kvpair >,space> > > > {};

    struct anything : sor<comment, header> {};
    struct file : until<eof, anything> {};

    // The state
    struct state
    {
    };

    // The action
    template< typename Rule >
    struct action : nothing< Rule >
    {
    };

    template<>
    struct action< comment >
    {
        template< typename Input >
        static void apply( const Input& in, state& out )
            {
                std::cout << "COMMENT " << in.string() << std::endl;
            }
    };

    template<>
    struct action< header >
    {
        template< typename Input >
        static void apply( const Input& in, state& out )
            {
                std::cout << "HEADER " << in.string() << std::endl;
            }
    };
    
    template<>
    struct action< header_tag >
    {
        template< typename Input >
        static void apply( const Input& in, state& out )
            {
                //std::cout << "HEADER HEADER" << in.string() << std::endl;
            }
    };

    template<>
    struct action< kvpair >
    {
        template< typename Input >
        static void apply( const Input& in, state& out )
            {
                //std::cout << "KVPAIR " << in.string() << std::endl;
            }
    };

    template<>
    struct action< kvkeyeq >
    {
        template< typename Input >
        static void apply( const Input& in, state& out )
            {
                //std::cout << "KVKEYEQ " << in.string() << std::endl;
            }
    };

    template<>
    struct action< kvvalue >
    {
        template< typename Input >
        static void apply( const Input& in, state& out )
            {
                //std::cout << "KVVALUE " << in.string() << std::endl;
            }
    };

    template<>
    struct action< kvvalue_digit >
    {
        template< typename Input >
        static void apply( const Input& in, state& out )
            {
                std::cout << "KVDIGIT " << in.string() << std::endl;
            }
    };

    template<>
    struct action< kvvalue_string >
    {
        template< typename Input >
        static void apply( const Input& in, state& out )
            {
                std::cout << "KVSTRING " << in.string() << std::endl;
            }
    };
}
    
void Document::parse(std::string &contents)
{
    rawText = contents;

    memory_input<> cmem( contents, "" );
    readParser::state result;
    tao::TAO_PEGTL_NAMESPACE::parse< readParser::file, readParser::action >( cmem, result );
    std::cout << "Parse complete" << std::endl;
}
}
