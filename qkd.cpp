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
const char* img_file = "image";
SDL_Window* window;

using SysClock     = typename std::chrono::system_clock;
using Duration     = typename std::chrono::milliseconds;
using RNG_Engine   = typename std::default_random_engine;
using Metrics      = double;
using IdRepr       = unsigned int;
using Distribution = std::uniform_int_distribution<IdRepr>;

using Network =
    QKD_Network<SysClock, Duration, RNG_Engine, Distribution, Metrics, IdRepr>;

SDL_Window* WndInit()
{
    int sdlinit = SDL_Init( SDL_INIT_TIMER|SDL_INIT_VIDEO );
    if ( sdlinit < 0 )
    {
        std::cerr << "SDL error: %s" << SDL_GetError() << '\n';
        return nullptr;
    }

    int imginit = IMG_Init( IMG_INIT_PNG );
    if ( imginit == 0 )  // возвращает флаги
    {
        std::cerr << "SDL_image error: %s" << IMG_GetError() << '\n';
        return nullptr;
    }

    SDL_Window* wnd = SDL_CreateWindow( "qkd_demo",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        790, 590, 0 );
    if ( wnd == nullptr )
    {
        std::cerr << "SDL error: %s" << SDL_GetError() << '\n';
        throw std::runtime_error { "SDL_Window creation error" };
    }
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

    window = WndInit();
    removeInCWD( gv_file );
    removeInCWD( img_file );

    // module init
    auto* p_top = new QKD_Topology<Network>    {};
    auto* p_pth = new QKD_Pathfinder<Network>  {};
    auto* p_rqg = new QKD_RequestGen<Network>  { 10000ms };
    auto* p_kgm = new QKD_KeyGenModel<Network> {};

    // network init
    Network net { p_top, p_pth, p_rqg, p_kgm, 10 };

    auto n1 = net.addNode();
    auto n2 = net.addNode();
    auto n3 = net.addNode();
    auto n4 = net.addNode();
    auto n5 = net.addNode();

    [[maybe_unused]] auto l1 = net.addLink( n1, n2, 0 );
    [[maybe_unused]] auto l2 = net.addLink( n2, n3, 0 );
    [[maybe_unused]] auto l3 = net.addLink( n2, n4, 0 );

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
                removeInCWD( gv_file );
                removeInCWD( img_file );
                return EXIT_SUCCESS;
            }
        }
        #ifndef THREAD_BASED
            SleepEstimately(300, 700);
            if ( static_cast<double>(std::rand())/RAND_MAX > 0.8 )
                net.genQuantumKeys();

            SleepEstimately(300, 700);
            if ( static_cast<double>(std::rand())/RAND_MAX > 0.3 )
            {
                net.pushRequest();
                auto& req = net.popRequest();
                net.processRequest<DijkstraSP<Network>>( req );
            }
        #endif  // THREAD_BASED
    }
    return EXIT_SUCCESS;
}

#else  // #if defined(__linux__) || defined(_WIN64)

int main() {
    std::cerr << "Unsupported platform";
    return EXIT_FAILURE;
}

#endif  // #if defined(__linux__) || defined(_WIN64)
