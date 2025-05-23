#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    std::unordered_map<int, std::string> m;
    auto [it, success] = m.emplace(1, "123456");
    m.insert()
    m[1] = "one";
    m[2] = "two";
    m[3] = "three";
    for (auto& p : m) {
        std::cout << p.first << " " << p.second << std::endl;
    }
    return 0;
}