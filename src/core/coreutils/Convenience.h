#pragma once

#include <functional>



#define TNT_NS1( x )            namespace x {
#define TNT_END_NS1( x )        }

#define TNT_NS2( x, y )         namespace x { namespace y {
#define TNT_END_NS2( y, x )     } }

#define TNT_NS3( x, y, z )      namespace x { namespace y { namespace z {
#define TNT_END_NS3( z, y, x )  } } }


#define TNT_INTERFACE struct

//To mark an argument as a out argument
#define TNT_OUT

//To mark an argument as a in-out argument
#define TNT_IN_OUT

namespace Tanyatu {

class ScopedOperation
{
public:
    ScopedOperation( std::function< void() > entry,
                     std::function< void() > exit )
        : m_exitTask( exit )
    {
        if( entry ) {
            entry();
        }
    }

    ~ScopedOperation()
    {
        if( m_exitTask ) {
            m_exitTask();
        }
    }


private:
    std::function< void() > m_exitTask;
};

} //end of namespace Tanyatu

#define AT_SCOPE_EXIT( x ) \
    Tanyatu::ScopedOperation __tnt_scpop( 0, [ & ]() { x; })

#define SCOPE_LIMIT( x, y ) \
    Tanyatu::ScopedOperation __tnt_scpop( [ & ]() { x; }, [ & ]() { y; })


#define TNT_SCOPE
