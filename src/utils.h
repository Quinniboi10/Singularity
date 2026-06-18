#include <vector>
#include <string>

inline std::vector<std::string> split(const std::string& str, const char delim) {
    std::vector<std::string> res{""};

    for (const char c : str)
        if (c == delim)
            res.push_back("");
        else
            res.back() += c;

    return res;
}