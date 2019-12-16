#include <bits/stdc++.h>

std::string buff;

int main(int argc, char *argv[]) {
    std::ifstream ifs(argv[1]);
    std::ofstream ofs(argv[2]);

    while (ifs >> buff) {
        for (auto c : buff) {
            if (c == ',') ofs << '\t';
            else ofs << c;
        }
        ofs << std::endl;
    }
    return 0;
}