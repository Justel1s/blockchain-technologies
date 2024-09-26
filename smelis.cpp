#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <random>
#include <chrono>

// Hexas
std::string toHexString(uint64_t value, int width = 16) {
    std::stringstream stream;
    stream << std::hex << std::setw(width) << std::setfill('0') << value;
    return stream.str();
}

// "Maišymo" funkcija
std::string hashavimoGeneratorius(const std::string &input,  uint64_t smelis) {
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

    hashavimas1 ^= smelis;

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
    std::string input;
    int choice;
    std::cout << "1 - Ivesti ranka, 2 - Nuskaityti faila: ";
    std::cin >> choice;
    std::cin.ignore();

    // Pradedame matuoti laiką
    auto start = std::chrono::high_resolution_clock::now();

    // Ranka
    if (choice == 1) {
        std::cout << "Iveskite tekstą: ";
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << "Klaida: tusčia" << std::endl;
            return 0;
        }

    // Failas
    } else if (choice == 2) {
        std::ifstream myfile("testas/random_strings.txt");
        if (!myfile.is_open()) {
            std::cout << "Klaida: nepavyko atidaryti failo" << std::endl;
            return 0;
        }

        std::ostringstream buffer;
        buffer << myfile.rdbuf();
        input = buffer.str();
        std::cout << input << std::endl;

        myfile.close();
        if (input.empty()) {
            std::cout << "Klaida: tuscias failas" << std::endl;
            return 0;
        }
    } else {
        std::cout << "Klaida: blogas pasirinkimas" << std::endl;
    }
    // CSPRNG generatorius
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
    uint64_t smelis = dis(gen);

    // Spausdiname atsitiktinę vertę
    std::cout << "Smėlis: " << smelis << std::endl;
    // Maišome atsitiktinę vertę su įvestimi ir skaičiuojame hashą
    std::string hash = hashavimoGeneratorius(input, smelis);

    // Laiko matavimo pabaiga
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Hashas: " << hash << std::endl;
    std::cout << "Programos veikimo laikas: " << elapsed.count() << " sekundes" << std::endl;

    return 0;
}
