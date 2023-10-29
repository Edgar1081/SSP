#include <sstream>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <memory>

class Io {
private:
    std::stringstream text;
    int target;
    int size;
    std::shared_ptr<int[]> instance;

public:
    Io(const char* filename) {
        std::fstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open the file: " << filename << std::endl;
            exit(1);
        }

        std::string line;
        while (std::getline(file, line)) {
            text << line;
        }

        int length = 0;
        std::string token;
        while (getline(text, token, ',')) {
            length++;
        }

        instance = std::shared_ptr<int[]>(new int[length - 1], std::default_delete<int[]>());

        int* rawArray = instance.get();
        text.clear();
        text.seekg(0, std::ios::beg);

        int i = 0;
        while (std::getline(text, token, ',')) {
            int sumando = std::stoi(token);
            if (i == length - 1) {
                target = sumando;
            } else {
                rawArray[i] = sumando;
                i++;
            }
        }

        size = length - 1;
        file.close();
    }

    int get_size() {
        return size;
    }

    int get_target() {
        return target;
    }

    std::shared_ptr<int[]>& get_array() {
        return instance;
    }

    ~Io() {
    }
};
