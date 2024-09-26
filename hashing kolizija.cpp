#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <chrono>

// Hexas
std::string toHexString(uint64_t value, int width = 16) {
    std::stringstream stream;
    stream << std::hex << std::setw(width) << std::setfill('0') << value;
    return stream.str();
}

// "Maišymo" funkcija
std::string hashavimoGeneratorius(const std::string &input) {
    uint64_t vardas = 74117108105;
    uint64_t pavarde = 10111611683;
    
    uint64_t hashavimas1 = 0x20043112;
    uint64_t hashavimas2 = 0x19760223;
    uint64_t hashavimas3 = 0x12345678;
    uint64_t hashavimas4 = 0x9abcdef0;

    uint64_t pirminis1 = 149837;
    uint64_t pirminis2 = 458611;
    uint64_t pirminis3 = 823541;
    uint64_t pirminis4 = 1299709;

    for (char c : input) {
        hashavimas1 ^= static_cast<uint64_t>(c);
        hashavimas1 *= pirminis1;
        hashavimas1 ^= (hashavimas1 >> 69);
        
        hashavimas2 ^= static_cast<uint64_t>(c);
        hashavimas2 *= pirminis2;
        hashavimas2 ^= (hashavimas2 << 96);
        
        hashavimas3 ^= static_cast<uint64_t>(c);
        hashavimas3 *= pirminis3;
        hashavimas3 ^= (hashavimas3 >> 47);
        
        hashavimas4 ^= static_cast<uint64_t>(c);
        hashavimas4 *= pirminis4;
        hashavimas4 ^= (hashavimas4 << 33);
    }

    hashavimas1 ^= (hashavimas2 >> 20);
    hashavimas2 ^= (hashavimas1 << 26);
    
    hashavimas3 ^= (hashavimas4 >> 15);
    hashavimas4 ^= (hashavimas3 << 31);

    hashavimas1 *= vardas;
    hashavimas2 *= pavarde;
    hashavimas3 *= pavarde;
    hashavimas4 *= vardas;

    return toHexString(hashavimas1) + toHexString(hashavimas2) + toHexString(hashavimas3) + toHexString(hashavimas4);
}

int main() {
    std::ifstream myfile("testas/random_strings.txt");
    if (!myfile.is_open()) {
        std::cout << "Klaida: nepavyko atidaryti failo" << std::endl;
        return 0;
    }

    std::string line;
    int matchingHashesCount = 0;
    int totalLines = 0;

    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(myfile, line)) {
        totalLines++;
        std::istringstream iss(line);
        std::string string1, string2;

        if (!(iss >> string1 >> string2)) {
            std::cout << "Klaida: Netinkama eilutės struktūra" << std::endl;
            continue;
        }

        std::string hash1 = hashavimoGeneratorius(string1);
        std::string hash2 = hashavimoGeneratorius(string2);
        if (hash1 == hash2) {
            matchingHashesCount++;
        }
        if (totalLines % 10000 == 0) {
            std::cout << "Apdorotos eilutės: " << totalLines << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Iš viso eilučių: " << totalLines << std::endl;
    std::cout << "Sutampančių hashų skaičius: " << matchingHashesCount << std::endl;
    std::cout << "Programos veikimo laikas: " << elapsed.count() << " sekundes" << std::endl;

    myfile.close();

    return 0;
}
