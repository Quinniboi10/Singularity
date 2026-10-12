#include <unordered_map>
#include <algorithm>
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

// Returns the index of the given target, -1 if not found
inline int find(const auto& iterable, const auto& target) {
    const auto idx = std::ranges::find(iterable, target);
    if (idx == std::end(iterable))
        return -1;
    else
        return std::distance(std::begin(iterable), idx);
}

inline std::string str_lower(const std::string& str) {
    std::string res = "";
    for (const char c : str) {
        if (c >= 'A' && c <= 'Z') {
            res += c - 'A' + 'a';
        }
        else {
            res += c;
        }
    }

    return res;
}

// Takes a string of '<key> <value> <key> <value> ...' and returns a map between the two
inline std::unordered_map<std::string, std::string> parse_command_pairs(const std::string& str) {
    std::unordered_map<std::string, std::string> map{};

    const std::vector<std::string> tokens = split(str, ' ');

    for (int i = 0; i < tokens.size() - 1; i += 2) {
        map[tokens[i]] = tokens[i + 1];
    }

    return map;
}

// Formats a number with commas
template<typename T>
inline std::string format_num(const T v) {
    auto s = std::to_string(v);

    int n = s.length() - 3;
    if (v < 0)
        n--;
    while (n > 0) {
        s.insert(n, ",");
        n -= 3;
    }

    return s;
}