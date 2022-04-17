////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/bit/KeyValue.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/Utils.hpp>
#include <yq/type/ByteArray.hpp>

using namespace yq;

void    print(const KVTree& tree, unsigned int depth=0)
{
    for(auto& a : tree.subs){
        for(unsigned int i=0;i<depth;++i)
            std::cout << "    ";
        std::cout << a.key << '\t' << a.data << '\n';
        print(a, depth+1);
    }
}

int main(int argc, char* argv[])
{
    const char* zfile   = nullptr;
    bool do_body        = false;
    for(int i=1; i<argc; ++i){
        if((argv[i][0] == '-') && (argv[i][1] == '-')){
            if(is_similar(argv[i], "--body"))
                do_body = true;
        } else {
            if(!zfile)
                zfile   = argv[i];
        }
    }
    
    if(!zfile){
        std::cerr << "Usage: " << argv[0] << " [--body] [file]\n";
        return -1;
    }
    
    
    log_to_std_error();
    KVTree          tree;
    std::string     body;
    ByteArray       bytes   = file_bytes(zfile);
    if(!tree.parse(bytes, (do_body?&body:nullptr), true, zfile))
        return -1;
    print(tree);
    return 0;
}
