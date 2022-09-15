#include <condition_variable>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <thread>

#include <gtkmm/application.h>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/random/beta_distribution.hpp>

#include "SimulationWindow.hpp"
#include "QKD_Network.hpp"

int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create("org.gtkmm.QKDsim");
    SimulationWindow wnd {"QKD Network Simulation", 400, 400};
    wnd.show_all();
    return app->run(wnd, argc, argv);
}
