#include <iostream>

#if defined(__linux__) || defined(_WIN64)

#include <chrono>
#include <random>
#include <cstdlib>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "src/qkdnetwork.hpp"

const char* gv_file = "graph";
const char* img_file = "image.jpg";
const char* bg_file = "res/bg.jpg";
SDL_Window* window;

using SysClock     = typename std::chrono::system_clock;
using Duration     = typename std::chrono::milliseconds;
using RNG_Engine   = typename std::default_random_engine;
using Metrics      = double;
using IdRepr       = unsigned int;
using Distribution = std::uniform_int_distribution<IdRepr>;

using Network = QKD_Network<SysClock, Duration, RNG_Engine, Distribution, Metrics, IdRepr>;

void RemoveInCWD( std::string file )
{
    char cwd[PATH_MAX];
    #if defined(__linux__)
        getcwd( cwd, PATH_MAX );
        strcat( cwd, "/" );
        remove( strcat( cwd, file.c_str() ) );
    #elif defined(_WIN64)
        // WinAPI
    #endif
}

SDL_Window* WindowInit()
{
    int sdlinit = SDL_Init( SDL_INIT_TIMER|SDL_INIT_VIDEO );
    if ( sdlinit < 0 )
        throw std::runtime_error { std::string {"SDL error: "} + SDL_GetError() };

    int imginit = IMG_Init( IMG_INIT_PNG );
    if ( imginit == 0 )  // возвращает флаги
        throw std::runtime_error { std::string {"SDL_image error: "} + IMG_GetError() };

    SDL_Window* wnd = SDL_CreateWindow( "qkd_demo",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        1000, 700, 0 );
    if ( wnd == nullptr )
        throw std::runtime_error { "SDL_Window creation error" };
    return wnd;
}

void SleepEstimately( double min_msecs, double max_msecs )
{
    int msecs;
    do {
        msecs = static_cast<int>( max_msecs*std::rand()/RAND_MAX );
    } while ( msecs < min_msecs && msecs > max_msecs );
    std::this_thread::sleep_for( std::chrono::milliseconds { msecs } );
}

int main()
{
    boost::log::core::get()->set_filter
        ( boost::log::trivial::severity >= boost::log::trivial::info );

    window = WindowInit();
    RemoveInCWD( gv_file );
    RemoveInCWD( img_file );

    // module init
    auto* p_top = new QKD_Topology<Network>    {};
    auto* p_pth = new QKD_Pathfinder<Network>  {};
    auto* p_rqg = new QKD_RequestGen<Network>  { 10000ms };
    auto* p_kgm = new QKD_KeyGenModel<Network> {};

    // network init
    Network net { p_top, p_pth, p_rqg, p_kgm, 10 };

    auto ats_smol    = net.addNode( NodeClass::Target, "АТС Смольного" );
    auto okt_zd      = net.addNode( NodeClass::Target, "Октябрьская ЖД" );
    auto ivc_rzd_spb = net.addNode( NodeClass::Aux,    "ИВЦ СпБ РЖД" );
    auto tosno       = net.addNode( NodeClass::Aux,    "Тосно" );
    auto chudovo     = net.addNode( NodeClass::Aux,    "Чудово" );
    auto mvishera    = net.addNode( NodeClass::Aux,    "Малая Вишера" );
    auto torbino     = net.addNode( NodeClass::Aux,    "Торбино" );
    auto uglovka     = net.addNode( NodeClass::Aux,    "Угловка" );
    auto bologoe     = net.addNode( NodeClass::Aux,    "Бологое" );
    auto udomlya     = net.addNode( NodeClass::Target, "Удомля" );
    auto kalininsky  = net.addNode( NodeClass::Target, "ЦОД <<Калининский>>" );
    auto vvolochek   = net.addNode( NodeClass::Aux,    "Вышний Волочек" );
    auto spirovo     = net.addNode( NodeClass::Aux,    "Спирово" );
    auto likhoslavl  = net.addNode( NodeClass::Aux,    "Лихославль" );
    auto tver        = net.addNode( NodeClass::Aux,    "Тверь" );
    auto klin        = net.addNode( NodeClass::Aux,    "Клин" );
    auto zelenograd  = net.addNode( NodeClass::Aux,    "Зеленоград" );
    auto gvc_rzd     = net.addNode( NodeClass::Aux,    "ГВЦ РЖД" );
    auto m10         = net.addNode( NodeClass::Target, "ЦОД <<М10>>" );
    auto c_rzd       = net.addNode( NodeClass::Target, "Ц РЖД" );
    auto mivc_rzd    = net.addNode( NodeClass::Target, "МИВЦ РЖД" );

    net.addLink( ats_smol, ivc_rzd_spb, 0 );
    net.addLink( okt_zd, ivc_rzd_spb, 0 );
    net.addLink( ivc_rzd_spb, tosno, 0 );
    net.addLink( tosno, chudovo, 0 );
    net.addLink( chudovo, mvishera, 0 );
    net.addLink( mvishera, torbino, 0 );
    net.addLink( torbino, uglovka, 0 );
    net.addLink( uglovka, bologoe, 0 );
    net.addLink( kalininsky, udomlya, 0 );
    net.addLink( udomlya, bologoe, 0 );
    net.addLink( bologoe, vvolochek, 0 );
    net.addLink( vvolochek, spirovo, 0 );
    net.addLink( spirovo, likhoslavl, 0 );
    net.addLink( likhoslavl, tver, 0 );
    net.addLink( tver, klin, 0 );
    net.addLink( klin, zelenograd, 0 );
    net.addLink( zelenograd, gvc_rzd, 0 );
    net.addLink( gvc_rzd, m10, 0 );
    net.addLink( gvc_rzd, c_rzd, 0 );
    net.addLink( gvc_rzd, mivc_rzd, 0 );

    setNodePos( net.getNodeById( ats_smol ), 0, 9 );
    setNodePos( net.getNodeById( okt_zd ), 1, 10 );
    setNodePos( net.getNodeById( ivc_rzd_spb ), 0, 11 );
    setNodePos( net.getNodeById( tosno ), 3, 11 );
    setNodePos( net.getNodeById( chudovo ), 4, 10 );
    setNodePos( net.getNodeById( mvishera ), 5, 9 );
    setNodePos( net.getNodeById( torbino ), 6, 8 );
    setNodePos( net.getNodeById( uglovka ), 7, 7 );
    setNodePos( net.getNodeById( bologoe ), 8, 6 );
    setNodePos( net.getNodeById( udomlya ), 9, 9 );
    setNodePos( net.getNodeById( kalininsky ), 9, 10 );
    setNodePos( net.getNodeById( vvolochek ), 9, 5 );
    setNodePos( net.getNodeById( spirovo ), 10, 4 );
    setNodePos( net.getNodeById( likhoslavl ), 11, 3 );
    setNodePos( net.getNodeById( tver ), 12, 2 );
    setNodePos( net.getNodeById( klin ), 13, 1 );
    setNodePos( net.getNodeById( zelenograd ), 14, 0 );
    setNodePos( net.getNodeById( gvc_rzd ), 16, 0 );
    setNodePos( net.getNodeById( m10 ), 16, 4 );
    setNodePos( net.getNodeById( c_rzd ), 17, 1 );
    setNodePos( net.getNodeById( mivc_rzd ), 18, 0 );

    #ifdef THREAD_BASED
        std::thread kgen  = net.keyGenThread();
        std::thread rgen  = net.reqGenThread();
        std::thread rproc = net.reqProcThread<DijkstraSP<Network>>();
        kgen.join();
        rgen.join();
        rproc.join();
    #endif  // THREAD_BASED

    while (true)
    {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) )
        {
            if ( event.type == SDL_WINDOWEVENT  // закрытие окна
                 && event.window.event == SDL_WINDOWEVENT_CLOSE )
            {
                SDL_Quit();
                RemoveInCWD( gv_file );
                RemoveInCWD( img_file );
                return EXIT_SUCCESS;
            }
        }
        #ifndef THREAD_BASED
            if ( static_cast<double>(std::rand())/RAND_MAX > 0.86 )
                net.genQuantumKeys();
            SleepEstimately( 300, 700 );

            if ( static_cast<double>(std::rand())/RAND_MAX > 0.3 )
                net.pushRequest();
            SleepEstimately( 300, 700 );

            if ( static_cast<double>(std::rand())/RAND_MAX > 0.3 )
            {
                try {
                    auto req = net.popRequest();
                    net.processRequest<DijkstraSP<Network>>( req );
                } catch ( std::runtime_error& exc ) {  // no request in queue
                    continue;
                }
            }
            SleepEstimately( 300, 700 );
        #endif  // THREAD_BASED
    }
    return EXIT_SUCCESS;
}

#else  // #if defined(__linux__) || defined(_WIN64)

// int main() {
//     std::cerr << "Unsupported platform";
//     return EXIT_FAILURE;
// }
    #pragma message("Unsupported platform, aborting build")

#endif  // #if defined(__linux__) || defined(_WIN64)
