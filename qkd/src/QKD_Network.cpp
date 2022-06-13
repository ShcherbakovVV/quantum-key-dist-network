#include "QKD_Network.hpp"


// PROTECTED FUNCTIONS
std::string QKD_Network::_to_graphviz(std::optional<Path> path_opt) const
{
    const auto& mod = get_module(ModuleType::TOPOLOGY);
    const auto& topology = dynamic_cast<const Topology&>(mod);

    std::string gv = "graph G {\nbgcolor=\"transparent\"\n";

    for (size_t i = 0; i != PROP_TABLE.size().size_desc; ++i)
    {
        auto desc = static_cast<Descriptor>(i);
        if (PROP_TABLE.is_node(desc))
        {
            auto node = desc;
            gv += "\"" + PROP_TABLE(node, "label") + "\"[pos=\""
                       + PROP_TABLE(node, "x pos") + ","
                       + PROP_TABLE(node, "y pos") + "!\""
                       + "fontname = Arial,fontsize=28";

            if (PROP_TABLE(node, "type") == "target")
                gv += ",shape=box3d";
            else if (PROP_TABLE(node, "type") == "aux")
                gv += ",shape=rect";

            if (path_opt && path_opt.value().in_path(node))
            {
                gv += ",penwidth=5";
                if (node == path_opt.value().start() || node == path_opt.value().dest())
                    gv += ",style=filled,fillcolor=gray50";
            }
            gv += "];\n";
        }
        else if (PROP_TABLE.is_link(desc))
        {
            auto link = desc;

            auto adj_nodes = topology.access().adj_nodes(link);
            const auto& kamount = PROP_TABLE(link, "key amount");
            //const auto& krate = PROP_TABLE(link, "key rate, bps");

            gv += "\"" + PROP_TABLE(adj_nodes.first, "label") + "\"--\"" 
                       + PROP_TABLE(adj_nodes.second, "label") 
                       + "\"[label=\"" + kamount + " КК\""; // + krate + " бит/с\"";

            if (path_opt && path_opt.value().in_path(link))
                gv += ",penwidth=5";

            gv += ",labeljust=l,fontname = Arial,fontsize=24];\n";
        }
    }
    gv += '}';
    return gv;
}
// END OF PROTECTED FUNCTIONS


// PUBLIC FUNCTIONS
QKD_Network::QKD_Network ()
{
    STATISTICS.add_attribute("serviced requests", "0");
    STATISTICS.add_attribute("overall requests", "0");
    STATISTICS.add_attribute("serviced/overall request ratio", "0");
    STATISTICS.add_attribute("completion ratio", "0");
    STATISTICS.ignore_attribute("completion ratio");
}
