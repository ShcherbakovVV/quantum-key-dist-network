#ifndef SIMULATION_WINDOW_HPP
#define SIMULATION_WINDOW_HPP

//#include <condition_variable>
#include "Global.hpp"
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <variant>

#include <boost/random/beta_distribution.hpp>

#include "AdjacencyMatrix.hpp"
#include "DijkstraShortestPath.hpp"
#include "FIFO.hpp"
#include "KeyGenerationModel.hpp"
#include "LIFO.hpp"
#include "Global.hpp"
#include "Pathfinder.hpp"
#include "PriorityQueue.hpp"
#include "RandomQueue.hpp"
#include "RequestGenerator.hpp"
#include "QKD_Network.hpp"
#include "QueueManager.hpp"
#include "Topology.hpp"
#include "gtkmm/combobox.h"
#include "gtkmm/grid.h"

#include <gtkmm.h>


namespace br = boost::random;


int stoi10(const std::string&, size_t*);

template<typename T>
using StrTo = T (*)(const std::string&, size_t*);


class ErrorDialog : public Gtk::MessageDialog
{
public:
    void on_close_clicked([[maybe_unused]] int response_id) { close(); }

    ErrorDialog(Gtk::Window&, const char*, const char*);
};


struct SimulationParams
{
    Duration sim_time, sim_step;
    Glib::ustring queue_type;
    int queue_cap;
    Duration req_lifetime;
    double arr_lambda, srv_lambda;
};


class SimulationWindow : public Gtk::Window
{
private:
    bool stopped = false;

    Glib::Dispatcher perc_disp;
    Glib::Dispatcher end_disp;
    Glib::Dispatcher stop_disp;

    SimulationParams sim_params;
    std::jthread simulation;

    std::shared_ptr<QKD_Network> qkd_network 
        = std::make_shared<QKD_Network>();

    std::shared_ptr<RequestGenerator> req_generator {};
    std::shared_ptr<QueueManager> queue_manager {};

    std::shared_ptr<RNG<br::beta_distribution<Metrics>>> key_generation 
        = std::make_shared<RNG<br::beta_distribution<Metrics>>>();
    std::shared_ptr<KeyGenerationModel> key_gen_model 
        = std::make_shared<KeyGenerationModel>(key_generation);

    std::shared_ptr<KeyAmountMetricsPolicy> key_policy 
        = std::make_shared<KeyAmountMetricsPolicy>();
    std::shared_ptr<DijkstraShortestPath> algorithm 
        = std::make_shared<DijkstraShortestPath>();
    std::shared_ptr<Pathfinder> pathfinder 
        = std::make_shared<Pathfinder>(algorithm, key_policy);

    std::shared_ptr<AdjacencyMatrix> adj_matrix 
        = std::make_shared<AdjacencyMatrix>();
    std::shared_ptr<Topology> topology 
        = std::make_shared<Topology>(adj_matrix);

    RNG<std::exponential_distribution<double>> arrivals;
    RNG<std::exponential_distribution<double>> service;

    Gtk::Box wnd_box {Gtk::ORIENTATION_VERTICAL};

    Gtk::Frame sim_frame {"Simulation settings"};
    Gtk::Grid sim_grid {};
    Gtk::Label sim_time_label {"Simulation time", Gtk::ALIGN_START};
    Gtk::Grid sim_time_entry_grid {};
    Gtk::Entry sim_time_entry {};
    Gtk::ComboBoxText sim_time_entry_combo {};
    Gtk::Label sim_step_label {"Simulation step", Gtk::ALIGN_START};
    Gtk::Grid sim_step_entry_grid {};
    Gtk::Entry sim_step_entry {};
    Gtk::ComboBoxText sim_step_entry_combo {};

    Gtk::Frame queue_frame {"Queueing settings"};
    Gtk::Grid queue_grid {};
    Gtk::Label queue_type_label {"Queue service order", Gtk::ALIGN_START};
    Gtk::ComboBoxText queue_type_combo {};
    Gtk::Label queue_cap_label {"Request queue capacity, requests", Gtk::ALIGN_START};
    Gtk::Entry queue_cap_entry {};
    Gtk::Label req_life_label {"Request lifetime", Gtk::ALIGN_START};
    Gtk::Grid req_life_entry_grid {};
    Gtk::Entry req_life_entry {};
    Gtk::ComboBoxText req_life_entry_combo {};
    Gtk::Label req_arr_label {"Request arrival parameter", Gtk::ALIGN_START};
    Gtk::Entry req_arr_entry {};
    Gtk::Label req_srv_label {"Request service parameter", Gtk::ALIGN_START};
    Gtk::Entry req_srv_entry {};

    Gtk::Frame progress_frame {"Simulation progress"};
    Gtk::Box progress_box {Gtk::ORIENTATION_VERTICAL};
    Gtk::ProgressBar progressbar {};
    Gtk::TextView textview {};
    Gtk::Box buttons_box {Gtk::ORIENTATION_HORIZONTAL};

    Gtk::Button run_button {};
    Gtk::Box run_button_box {Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Image run_button_icon {};
    Gtk::Label run_button_label {"Run"};

    Gtk::Button stop_button {};
    Gtk::Box stop_button_box {Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Image stop_button_icon {};
    Gtk::Label stop_button_label {"Stop"};

    void simulation_setup();
    void queueing_setup();
    void progress_setup();

    template<typename T>
        T _entry_helper(const Gtk::Entry&, StrTo<T>);
    
    double double_from_entry(const Gtk::Entry&);
    int int_from_entry(const Gtk::Entry&);

    void on_set_percentage();
    void on_end_simulation();
    void on_stop_simulation();
    void on_run_simulation();

    void network_setup();
    void network_simulation();

public:
    SimulationWindow (const char*, int, int);

    void percentage_notify();
    void simulation_end_notify();
    void simulation_stop_notify();

    bool is_stopped() const { return stopped; }
};


template<typename T>
T SimulationWindow::_entry_helper(const Gtk::Entry& entry, StrTo<T> to_T)
{
    auto text = entry.get_buffer()->get_text();
    auto text_len = text.length();
    std::string str {text.data()};

    size_t proc_len = 0;
    T ret = to_T(str, &proc_len);
    if (proc_len < text_len)
        throw std::invalid_argument {"invalid argument"};
    return ret;
}

#endif  // SIMULATION_WINDOW_HPP
