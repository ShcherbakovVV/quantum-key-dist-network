#include <cassert>
#include <cstdlib>

#include "PropertyTable.hpp"

int main()
{
    int exception_count = 0;

    #undef prop_table
    #define prop_table (PropertyTable::instance())
    
    auto node0 = prop_table.add_entry(DescType::NODE);
    auto node1 = prop_table.add_entry(DescType::NODE);
    auto link2 = prop_table.add_entry(DescType::LINK);
    Descriptor non_existent = 3;
    
    assert((node0 == 0));
    assert((node1 == 1));
    assert((link2 == 2));
    assert((prop_table.size().size_desc == 3 && prop_table.size().size_prop == 0));
    
    assert((prop_table(node0) == DESC_TYPE_NODE));
    assert((prop_table(node1) == DESC_TYPE_NODE));
    assert((prop_table(link2) == DESC_TYPE_LINK));
    assert((prop_table.entry_type(node0) == DescType::NODE));
    assert((prop_table.entry_type(node1) == DescType::NODE));
    assert((prop_table.entry_type(link2) == DescType::LINK));
    assert(prop_table.is_node(node0));
    assert(!prop_table.is_link(node0));
    assert(prop_table.is_node(node1));
    assert(!prop_table.is_link(node1));
    assert(!prop_table.is_node(link2));
    assert(prop_table.is_link(link2));
    try {
        prop_table(non_existent);
    } catch (...) {
        ++exception_count;  // 1
    }
    
    prop_table.add_property("property1");
    prop_table.add_property("property2");
    prop_table.add_property("property3");
    assert((prop_table.size().size_desc == 3 && prop_table.size().size_prop == 3));
    
    assert((prop_table(node0, "property1") == ""));
    assert((prop_table(node1, "property1") == ""));
    assert((prop_table(link2, "property1") == ""));
    assert((prop_table(node0, "property2") == ""));
    assert((prop_table(node1, "property2") == ""));
    assert((prop_table(link2, "property2") == ""));
    assert((prop_table(node0, "property3") == ""));
    assert((prop_table(node1, "property3") == ""));
    assert((prop_table(link2, "property3") == ""));
    try {
        prop_table(non_existent, "property3");
    } catch (...) {
        ++exception_count;  // 2
    }
    try {
        prop_table(link2, "non-existent property");
    } catch (...) {
        ++exception_count;  // 3
    }
    try {
        prop_table(non_existent, "non-existent property");
    } catch (...) {
        ++exception_count;  // 4
    }
    
    prop_table(node0, "property1") = "true";
    prop_table(node1, "property1") = "false";
    prop_table(link2, "property1") = "true";
    prop_table(node0, "property2") = "myvalue1";
    prop_table(node1, "property2") = "myvalue2";
    prop_table(link2, "property2") = "myvalue3";
    prop_table(node0, "property3") = "100";
    prop_table(node1, "property3") = "500.1";
    prop_table(link2, "property3") = "-0.1";
    
    assert((prop_table(node0, "property1") == "true"));
    assert((prop_table(node1, "property1") == "false"));
    assert((prop_table(link2, "property1") == "true"));
    assert((prop_table(node0, "property2") == "myvalue1"));
    assert((prop_table(node1, "property2") == "myvalue2"));
    assert((prop_table(link2, "property2") == "myvalue3"));
    assert((prop_table(node0, "property3") == "100"));
    assert((prop_table(node1, "property3") == "500.1"));
    assert((prop_table(link2, "property3") == "-0.1"));
    
    assert((prop_table[node0]["property1"] == "true"));
    assert((prop_table[node0]["property2"] == "myvalue1"));
    assert((prop_table[node0]["property3"] == "100"));
    assert((prop_table[node1]["property1"] == "false"));
    assert((prop_table[node1]["property2"] == "myvalue2"));
    assert((prop_table[node1]["property3"] == "500.1"));
    assert((prop_table[link2]["property1"] == "true"));
    assert((prop_table[link2]["property2"] == "myvalue3"));
    assert((prop_table[link2]["property3"] == "-0.1"));
    try {
        prop_table[node1]["non-existent property"];
    } catch (...) {
        ++exception_count;  // 5
    }
    try {
        prop_table[non_existent]["property2"];
    } catch (...) {
        ++exception_count;  // 6
    }
    try {
        prop_table[non_existent]["non-existent property"];
    } catch (...) {
        ++exception_count;  // 7
    }
    
    prop_table.delete_entry(node1);
    assert((prop_table.size().size_desc == 2 && prop_table.size().size_prop == 3));
    try {
        prop_table(node1, "property2");
    } catch (...) {
        ++exception_count;  // 8
    }
    try {
        prop_table[node1]["property2"];
    } catch (...) {
        ++exception_count;  // 9
    }
    
    prop_table(node0, "property1") = "";
    prop_table[node0]["property1"];

    if (exception_count == 9)
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}
