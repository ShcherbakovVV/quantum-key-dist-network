#include "SimulationWindow.hpp"
#include "glibmm/ustring.h"
#include "gtkmm/enums.h"


int stoi10(const std::string& str, size_t* pos = nullptr)
{
    return std::stoi(str, pos, 10);
}


// ErrorDialog
// PUBLIC FUNCTIONS
ErrorDialog::ErrorDialog (Gtk::Window& parent, const char* message, const char* secondary)
:
    Gtk::MessageDialog {parent, message, true, Gtk::MessageType::ERROR, Gtk::ButtonsType::CLOSE, true}
{
    set_title("Error");
    set_secondary_text(secondary);
    set_icon_name("dialog-error");
    signal_response().connect(sigc::mem_fun(*this, &ErrorDialog::on_close_clicked));
}
// END OF PUBLIC FUNCTIONS


// SimulationWindow
// PRIVATE FUNCTIONS
void SimulationWindow::simulation_setup()
{
    wnd_box.append(sim_frame);
    sim_frame.set_child(sim_grid);
    sim_frame.set_label_align(Gtk::Align::CENTER);
    sim_grid.set_column_spacing(10);
    sim_grid.set_column_homogeneous(true);

    sim_time_entry_combo.append("ms");
    sim_time_entry_combo.append("s");
    sim_time_entry_combo.append("min");
    sim_time_entry_combo.append("h");
    sim_time_entry_combo.append("d");
    sim_time_entry_combo.set_active(0);

    sim_time_entry.set_hexpand(true);
    sim_time_entry_grid.attach(sim_time_entry, 0, 0);
    sim_time_entry_grid.attach(sim_time_entry_combo, 1, 0);

    sim_step_entry_combo.append("ms");
    sim_step_entry_combo.append("s");
    sim_step_entry_combo.append("min");
    sim_step_entry_combo.append("h");
    sim_step_entry_combo.append("d");
    sim_step_entry_combo.set_active(0);

    sim_step_entry.set_hexpand(true);
    sim_step_entry_grid.attach(sim_step_entry, 0, 0);
    sim_step_entry_grid.attach(sim_step_entry_combo, 1, 0);

    sim_time_label.set_margin(10);
    sim_time_label.set_margin_bottom(0);
    sim_time_entry.set_margin(10);
    sim_time_entry.set_margin_bottom(0);
    sim_time_entry.set_margin_end(5);
    sim_time_entry_combo.set_margin(10);
    sim_time_entry_combo.set_margin_bottom(0);
    sim_time_entry_combo.set_margin_start(5);
    sim_step_label.set_margin(10);
    sim_step_entry.set_margin(10);
    sim_step_entry.set_margin_end(5);
    sim_step_entry_combo.set_margin(10);
    sim_step_entry_combo.set_margin_bottom(10);
    sim_step_entry_combo.set_margin_start(5);
    sim_grid.attach(sim_time_label, 0, 0);
    sim_grid.attach(sim_time_entry_grid, 1, 0);
    sim_grid.attach(sim_step_label, 0, 1);
    sim_grid.attach(sim_step_entry_grid, 1, 1);
}


void SimulationWindow::queueing_setup()
{
    wnd_box.append(queue_frame);
    queue_frame.set_child(queue_grid);
    queue_frame.set_label_align(Gtk::Align::CENTER);
    queue_grid.set_column_spacing(10);
    queue_grid.set_column_homogeneous(true);

    queue_type_combo.append("FIFO");
    queue_type_combo.append("LIFO");
    queue_type_combo.append("Priority Queue");
    queue_type_combo.append("Random");

    req_life_entry_combo.append("ms");
    req_life_entry_combo.append("s");
    req_life_entry_combo.append("min");
    req_life_entry_combo.append("h");
    req_life_entry_combo.append("d");
    req_life_entry_combo.set_active(0);

    req_life_entry.set_hexpand();
    req_life_entry_grid.attach(req_life_entry, 0, 0);
    req_life_entry_grid.attach(req_life_entry_combo, 1, 0);

    queue_type_label.set_margin(10);
    queue_type_label.set_margin_bottom(0);
    queue_type_combo.set_margin(10);
    queue_type_combo.set_margin_bottom(0);
    queue_cap_label.set_margin(10);
    queue_cap_label.set_margin_bottom(0);
    queue_cap_entry.set_margin(10);
    queue_cap_entry.set_margin_bottom(0);
    req_life_label.set_margin(10);
    req_life_entry.set_margin(10);
    req_life_entry_combo.set_margin(10);
    req_life_entry_combo.set_margin_start(0);
    req_arr_label.set_margin(10);
    req_arr_label.set_margin_top(0);
    req_arr_label.set_margin_bottom(0);
    req_arr_entry.set_margin(10);
    req_arr_entry.set_margin_top(0);
    req_arr_entry.set_margin_bottom(0);
    req_srv_label.set_margin(10);
    req_srv_entry.set_margin(10);
    queue_grid.attach(queue_type_label, 0, 0);
    queue_grid.attach(queue_type_combo, 1, 0);
    queue_grid.attach(queue_cap_label, 0, 1);
    queue_grid.attach(queue_cap_entry, 1, 1);
    queue_grid.attach(req_life_label, 0, 2);
    queue_grid.attach(req_life_entry_grid, 1, 2);
    queue_grid.attach(req_arr_label, 0, 3);
    queue_grid.attach(req_arr_entry, 1, 3);
    queue_grid.attach(req_srv_label, 0, 4);
    queue_grid.attach(req_srv_entry, 1, 4);
}


void SimulationWindow::progress_setup()
{
    wnd_box.append(progress_frame);
    progress_frame.set_label_align(Gtk::Align::CENTER);
    progress_frame.set_child(progress_box);
    progress_box.set_spacing(5);
    progress_box.set_valign(Gtk::Align::FILL);

    progress_box.append(progressbar);
    progressbar.set_show_text(true);
    progressbar.set_text("0%");
    progressbar.set_margin(10);

    progress_box.append(textview);
    textview.set_editable(false);
    textview.set_vexpand(true);
    textview.set_margin(10);

    progress_box.append(buttons_box);
    buttons_box.set_spacing(5);
    buttons_box.set_homogeneous(true);
    buttons_box.append(run_button);
    buttons_box.append(stop_button);

    run_button.set_margin(10);
    run_button.set_child(run_button_box);
    run_button.signal_clicked().connect(sigc::mem_fun(*this, &SimulationWindow::on_run_simulation));
    run_button_box.set_halign(Gtk::Align::CENTER);
    run_button_box.set_spacing(5);
    run_button_icon.set_from_icon_name("media-playback-start");
    run_button_box.append(run_button_icon);
    run_button_box.append(run_button_label);

    stop_button.set_margin(10);
    stop_button.set_child(stop_button_box);
    stop_button.set_sensitive(false);
    stop_button.signal_clicked().connect(sigc::mem_fun(*this, &SimulationWindow::on_stop_simulation));
    stop_button_box.set_halign(Gtk::Align::CENTER);
    stop_button_box.set_spacing(5);
    stop_button_icon.set_from_icon_name("media-playback-stop");
    stop_button_box.append(stop_button_icon);
    stop_button_box.append(stop_button_label);
}


double SimulationWindow::double_from_entry(const Gtk::Entry& entry)
{
    return _entry_helper<double>(entry, std::stod);
}

int SimulationWindow::int_from_entry(const Gtk::Entry& entry)
{
    return _entry_helper<int>(entry, stoi10);
}


void SimulationWindow::on_set_percentage()
{
    auto ratio = STATISTICS.attribute_as<double>("completion ratio");
    int perc = static_cast<int>(ratio * 100);
    char perc_str[5];
    std::snprintf(perc_str, 5, "%i%%", perc);
    progressbar.set_text(perc_str);
    progressbar.set_fraction(ratio);
}


void SimulationWindow::on_run_simulation()
{
    stopped = false;

    run_button.set_sensitive(false);
    stop_button.set_sensitive(true);

    auto buf = textview.get_buffer();
    buf->erase(buf->begin(), buf->end());

    auto sim_time_fac = sim_time_entry_combo.get_active_text();
    auto sim_step_fac = sim_step_entry_combo.get_active_text();
    auto req_life_fac = req_life_entry_combo.get_active_text();
    try {
        auto get_factor = [](auto dur) -> double
        {
            double ret = 1;
            if (std::string {dur.data()} == "ms")       ret = 1;
            else if (std::string {dur.data()} == "s")   ret = 1000;
            else if (std::string {dur.data()} == "min") ret = 1000 * 60;
            else if (std::string {dur.data()} == "h")   ret = 1000 * 60 * 60;
            else if (std::string {dur.data()} == "d")   ret = 1000 * 60 * 60 * 24;
            return ret;
        };

        sim_params.sim_time = static_cast<Duration>(double_from_entry(sim_time_entry) * get_factor(sim_time_fac));
        sim_params.sim_step = static_cast<Duration>(double_from_entry(sim_step_entry) * get_factor(sim_step_fac));
        sim_params.queue_type = queue_type_combo.get_active_text();
        sim_params.queue_cap = int_from_entry(queue_cap_entry);
        sim_params.req_lifetime = static_cast<Duration>(double_from_entry(req_life_entry) * get_factor(req_life_fac));
        sim_params.arr_lambda = double_from_entry(req_arr_entry);
        sim_params.srv_lambda = double_from_entry(req_srv_entry);

        if (sim_params.sim_time <= 0
            || sim_params.sim_step <= 0
            || sim_params.queue_cap <= 0
            || sim_params.queue_type.empty())
        {
            throw std::invalid_argument {"invalid argument"};
        }

    } catch (...) {
        auto err_wnd = Gtk::make_managed<ErrorDialog>(*this, "Error", "Incorrect simulation parameters!");
        err_wnd->show();
        run_button.set_sensitive(true);
        stop_button.set_sensitive(false);
        return;
    }
    textview.get_buffer()->insert_at_cursor("Running simulation...\n");
    textview.get_buffer()->insert_at_cursor("Simulation parameters:\n");
    textview.get_buffer()->insert_at_cursor("    simulation time: ");
    textview.get_buffer()->insert_at_cursor(sim_time_entry.get_buffer()->get_text() + " ");
    textview.get_buffer()->insert_at_cursor(sim_time_fac + "\n");

    textview.get_buffer()->insert_at_cursor("    simulation step: ");
    textview.get_buffer()->insert_at_cursor(sim_step_entry.get_buffer()->get_text() + " ");
    textview.get_buffer()->insert_at_cursor(sim_step_fac + "\n");

    textview.get_buffer()->insert_at_cursor("    request queue type: ");
    textview.get_buffer()->insert_at_cursor(queue_type_combo.get_active_text() + "\n");

    textview.get_buffer()->insert_at_cursor("    request queue capacity, requests: ");
    textview.get_buffer()->insert_at_cursor(queue_cap_entry.get_buffer()->get_text() + "\n");

    textview.get_buffer()->insert_at_cursor("    request lifetime: ");
    textview.get_buffer()->insert_at_cursor(req_life_entry.get_buffer()->get_text() + " ");
    textview.get_buffer()->insert_at_cursor(req_life_fac + "\n");

    textview.get_buffer()->insert_at_cursor("    arrival lambda: ");
    textview.get_buffer()->insert_at_cursor(req_arr_entry.get_buffer()->get_text() + "\n");

    textview.get_buffer()->insert_at_cursor("    service lambda: ");
    textview.get_buffer()->insert_at_cursor(req_srv_entry.get_buffer()->get_text() + "\n");

    network_setup();
    network_simulation();
}


void SimulationWindow::on_end_simulation()
{
    stop_button.set_sensitive(false);
    run_button.set_sensitive(true);

    textview.get_buffer()->insert_at_cursor("Simulation finished\n");
    textview.get_buffer()->insert_at_cursor("Simulation statistics:\n");
    textview.get_buffer()->insert_at_cursor(STATISTICS.to_str(4) + "\n");
}


void SimulationWindow::on_stop_simulation()
{
    stopped = true;

    progressbar.set_text("0%");
    progressbar.set_fraction(0);

    stop_button.set_sensitive(false);
    run_button.set_sensitive(true);
}


void SimulationWindow::network_setup()
{
    qkd_network = std::make_shared<QKD_Network>();

    DescriptorCounter::reset();
    PROP_TABLE.clear();
    STATISTICS.reset();

    key_generation = std::make_shared<RNG<br::beta_distribution<Metrics>>>();
    key_gen_model = std::make_shared<KeyGenerationModel>(key_generation);

    key_policy = std::make_shared<KeyAmountMetricsPolicy>();
    algorithm = std::make_shared<DijkstraShortestPath>();
    pathfinder = std::make_shared<Pathfinder>(algorithm, key_policy);

    adj_matrix = std::make_shared<AdjacencyMatrix>();
    topology = std::make_shared<Topology>(adj_matrix);

    if (sim_params.queue_type == "FIFO")
    {
        auto req_builder = std::make_shared<TimedRequestBuilder>(sim_params.req_lifetime);
        req_generator = std::make_shared<RequestGenerator>(req_builder);

        auto req_queue = std::make_shared<LimitedTimedFIFO>(sim_params.queue_cap);
        queue_manager = std::make_shared<QueueManager>(req_queue);
    }
    else if (sim_params.queue_type == "LIFO")
    {
        auto req_builder = std::make_shared<TimedRequestBuilder>(sim_params.req_lifetime);
        req_generator = std::make_shared<RequestGenerator>(req_builder);

        auto req_queue = std::make_shared<LimitedTimedLIFO>(sim_params.queue_cap);
        queue_manager = std::make_shared<QueueManager>(req_queue);
    }
    else if (sim_params.queue_type == "Priority Queue")
    {
        auto req_builder = std::make_shared<TimedPriorityRequestBuilder>(sim_params.req_lifetime, 1, 3);
        req_generator = std::make_shared<RequestGenerator>(req_builder);

        auto req_queue = std::make_shared<LimitedTimedPriorityQueue>(sim_params.queue_cap);
        queue_manager = std::make_shared<QueueManager>(req_queue);
    }
    else if (sim_params.queue_type == "Random")
    {
        auto req_builder = std::make_shared<TimedRequestBuilder>(sim_params.req_lifetime);
        req_generator = std::make_shared<RequestGenerator>(req_builder);

        auto req_queue = std::make_shared<LimitedTimedRandomQueue>(sim_params.queue_cap);
        queue_manager = std::make_shared<QueueManager>(req_queue);
    }

    qkd_network->connect_module(ModuleType::KEY_GEN_MODEL, key_gen_model);
    qkd_network->connect_module(ModuleType::PATHFINDER, pathfinder);
    qkd_network->connect_module(ModuleType::REQ_GENERATOR, req_generator);
    qkd_network->connect_module(ModuleType::QUEUE_MANAGER, queue_manager);
    qkd_network->connect_module(ModuleType::TOPOLOGY, topology);

    auto nevs_smol   = topology->add_node({{"type", "target"}, {"label", "Невская Ратуша - Смольный"}, {"x pos", "6"}, {"y pos", "10.5"}});   // 1
    auto ats_smol    = topology->add_node({{"type", "aux"},    {"label", "ОВ АТС Смольного"},          {"x pos", "6"}, {"y pos", "12"}});   // 1
    auto ov_ttk      = topology->add_node({{"type", "aux"},    {"label", "ОВ ТТК"},                    {"x pos", "6"}, {"y pos", "13.5"}});   // 1
    auto okt_zd      = topology->add_node({{"type", "target"}, {"label", "Октябрьская ЖД"},            {"x pos", "6"}, {"y pos", "15"}});  // 2
    auto ivc_rzd_spb = topology->add_node({{"type", "aux"},    {"label", "ИВЦ СпБ РЖД"},               {"x pos", "0"}, {"y pos", "15"}});  // 3
    auto tosno       = topology->add_node({{"type", "aux"},    {"label", "Тосно"},                     {"x pos", "1"}, {"y pos", "13.5"}});  // 4
    auto chudovo     = topology->add_node({{"type", "aux"},    {"label", "Чудово"},                    {"x pos", "2"}, {"y pos", "12"}});  // 5
    auto mvishera    = topology->add_node({{"type", "aux"},    {"label", "Малая Вишера"},              {"x pos", "3"}, {"y pos", "10.5"}});   // 6
    auto torbino     = topology->add_node({{"type", "aux"},    {"label", "Торбино"},                   {"x pos", "4"}, {"y pos", "9"}});   // 7
    auto uglovka     = topology->add_node({{"type", "aux"},    {"label", "Угловка"},                   {"x pos", "5"}, {"y pos", "7.5"}});   // 8
    auto bologoe     = topology->add_node({{"type", "target"}, {"label", "Бологое"},                   {"x pos", "6"}, {"y pos", "6"}});   // 9
    auto udomlya     = topology->add_node({{"type", "aux"},    {"label", "Удомля"},                    {"x pos", "10"}, {"y pos", "6"}});   // 10
    auto kalininsky  = topology->add_node({{"type", "target"}, {"label", "ЦОД <<Калининский>>"},       {"x pos", "14"}, {"y pos", "6"}});  // 11
    auto vvolochek   = topology->add_node({{"type", "aux"},    {"label", "Вышний Волочек"},            {"x pos", "7"}, {"y pos", "4.5"}});   // 12
    auto spirovo     = topology->add_node({{"type", "aux"},    {"label", "Спирово"},                   {"x pos", "8"}, {"y pos", "3"}});   // 13
    auto likhoslavl  = topology->add_node({{"type", "aux"},    {"label", "Лихославль"},                {"x pos", "9"}, {"y pos", "1.5"}});   // 14
    auto tver        = topology->add_node({{"type", "aux"},    {"label", "Тверь"},                     {"x pos", "10"}, {"y pos", "0"}});   // 15
    auto zavidovo    = topology->add_node({{"type", "aux"},    {"label", "Завидово"},                  {"x pos", "11"}, {"y pos", "-1.5"}});   // 16
    auto klin        = topology->add_node({{"type", "aux"},    {"label", "Клин"},                      {"x pos", "12"}, {"y pos", "-3"}});   // 16
    auto kryukovo    = topology->add_node({{"type", "aux"},    {"label", "Крюково"},                   {"x pos", "13"}, {"y pos", "-4.5"}});   // 17
    auto gvc_rzd     = topology->add_node({{"type", "aux"},    {"label", "ГВЦ РЖД"},                   {"x pos", "14"}, {"y pos", "-6"}});   // 18
    auto m9          = topology->add_node({{"type", "target"}, {"label", "ЦОД <<М10>>"},               {"x pos", "18"}, {"y pos", "-3"}});   // 19
    auto c_rzd       = topology->add_node({{"type", "target"}, {"label", "Ц РЖД"},                     {"x pos", "18"}, {"y pos", "-4.5"}});   // 20
    auto mivc_rzd    = topology->add_node({{"type", "target"}, {"label", "МИВЦ РЖД"},                  {"x pos", "18"}, {"y pos", "-6"}});   // 21

    topology->add_link(nevs_smol,   ats_smol,    {{"distance", "1.4"}});
    topology->add_link(ats_smol,    ov_ttk,      {{"distance", "8.6"}});
    topology->add_link(ov_ttk,      ivc_rzd_spb, {{"distance", "3.3"}});
    topology->add_link(okt_zd,      ivc_rzd_spb, {{"distance", "3"}});
    topology->add_link(ivc_rzd_spb, tosno,       {{"distance", "55"}});
    topology->add_link(tosno,       chudovo,     {{"distance", "86.7"}});
    topology->add_link(chudovo,     mvishera,    {{"distance", "72.3"}});
    topology->add_link(mvishera,    torbino,     {{"distance", "59"}});
    topology->add_link(torbino,     uglovka,     {{"distance", "71.6"}});
    topology->add_link(uglovka,     bologoe,     {{"distance", "59.1"}});
    topology->add_link(kalininsky,  udomlya,     {{"distance", "4"}});
    topology->add_link(udomlya,     bologoe,     {{"distance", "78.4"}});
    topology->add_link(bologoe,     vvolochek,   {{"distance", "54.9"}});
    topology->add_link(vvolochek,   spirovo,     {{"distance", "38.7"}});
    topology->add_link(spirovo,     likhoslavl,  {{"distance", "53"}});
    topology->add_link(likhoslavl,  tver,        {{"distance", "53.2"}});
    topology->add_link(tver,        zavidovo,    {{"distance", "55.5"}});
    topology->add_link(zavidovo,    klin,        {{"distance", "24.9"}});
    topology->add_link(klin,        kryukovo,    {{"distance", "52.2"}});
    topology->add_link(kryukovo,    gvc_rzd,     {{"distance", "45.6"}});
    topology->add_link(gvc_rzd,     m9,          {{"distance", "17.1"}});
    topology->add_link(gvc_rzd,     c_rzd,       {{"distance", "0.5"}});
    topology->add_link(gvc_rzd,     mivc_rzd,    {{"distance", "1.3"}});

    arrivals.reset_params(sim_params.arr_lambda);
    service.reset_params(sim_params.srv_lambda);
}


void SimulationWindow::network_simulation()
{
    simulation = std::jthread {[&]{ qkd_network->simulation(*this, 
                                                            arrivals, service, 
                                                            sim_params.sim_step, 
                                                            sim_params.sim_time); } };                                                 
    simulation.detach();
    //qkd_network.simulation(*this, arrivals, service, sim_params.sim_step, sim_params.sim_time);
}
// END OF PRIVATE FUNCTIONS


// PUBLIC FUNCTIONS
SimulationWindow::SimulationWindow (const char* title, int w, int h)
{
    set_title(title);
    set_default_size(w, h);
    set_resizable(false);

    set_child(wnd_box);
    wnd_box.set_spacing(5);
    wnd_box.set_margin(5);

    perc_disp.connect(sigc::mem_fun(*this, &SimulationWindow::on_set_percentage));
    end_disp.connect(sigc::mem_fun(*this, &SimulationWindow::on_end_simulation));
    stop_disp.connect(sigc::mem_fun(*this, &SimulationWindow::on_stop_simulation));

    simulation_setup();
    queueing_setup();
    progress_setup();
}


void SimulationWindow::percentage_notify()
{
    perc_disp.emit();
}


void SimulationWindow::simulation_end_notify()
{
    end_disp.emit();
}


void SimulationWindow::simulation_stop_notify()
{
    stop_disp.emit();
}
// END OF PUBLIC FUNCTIONS