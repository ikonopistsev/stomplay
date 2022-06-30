#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include "stomplay/version.hpp"
//#include "stomplay/conv.hpp"
#include "stomplay/frame.hpp"

std::ostream& output(std::ostream& os)
{
    namespace ch = std::chrono;
    auto n = ch::system_clock::now();
    auto d = n.time_since_epoch();
    auto ms = ch::duration_cast<ch::milliseconds>(d).count() % 1000;
    std::time_t t{ch::duration_cast<ch::seconds>(d).count()};

    using namespace std::literals;
    return os << std::put_time(gmtime(&t), "%FT%T") 
        << '.' << std::setfill('0') << std::setw(3) << ms << "Z "sv;
}

inline std::ostream& cerr()
{
    return output(std::cerr);
}

inline std::ostream& cout()
{
    return output(std::cout);
}

using namespace std::literals;

using logon_type = stomplay::logon<std::string>;
// using buffer_type = std::vector<char>;
// using vlogon_type = stomplay::logon<buffer_type>;

// template<class T>
// void operator+=(buffer_type& lhs, const T& rhs)
// {
//     lhs.insert(lhs.end(), rhs.begin(), rhs.end());
// }

int main()
{   
    cout() << "stomplay v"sv << stomplay::version() << std::endl;

    logon_type logon("h"sv, "l"sv, "p"sv);
    //vlogon_type vlogone("h"sv, "l"sv, "p"sv);

    return 0;
}
