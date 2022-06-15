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

//namespace bl = boost::log;


int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create("org.gtkmm.QKDsim");
    return app->make_window_and_run<SimulationWindow>(argc, argv, "QKD Network Simulation", 400, 800);
}