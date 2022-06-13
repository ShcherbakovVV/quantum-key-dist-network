#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

#include <atomic>
#include <cstdint>
#include <sstream>
#include <string>


/* TODO:
 *  - Manage DescriptorCounter thread safety
 */

typedef uint16_t Descriptor;

constexpr Descriptor DESC_VALUE_MIN = 0x0000;
constexpr Descriptor DESC_VALUE_MAX = 0xfffc;
constexpr Descriptor DESC_TYPE_NODE = 0xfffd;
constexpr Descriptor DESC_TYPE_LINK = 0xfffe;
constexpr Descriptor NOT_A_DESC     = 0xffff;


class DescriptorCounter
{
private:
    static inline Descriptor _last_free_desc = 0;
    static inline bool _saturated = false;
    
public:
    static Descriptor get();
    static Descriptor add();
    static void reset();
};


enum class DescType : Descriptor
{
    NODE       = DESC_TYPE_NODE,
    LINK       = DESC_TYPE_LINK,
    NOT_A_DESC = NOT_A_DESC
};


bool operator==(const Descriptor, const DescType);
bool operator==(const DescType, const Descriptor);

#endif  // DESCRIPTOR_HPP
