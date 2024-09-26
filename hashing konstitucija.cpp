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
    std::ifstream file("testas/konstitucija.txt");
    if (!file.is_open()) {
        std::cout << "Klaida: nepavyko atidaryti failo" << std::endl;
        return 0;
    }

    // Skaitymas iš failo
    std::vector<std::string> eilutes;
    std::string line;
    while (std::getline(file, line)) {
        eilutes.push_back(line);
    }
    file.close();

    // Eilučių kiekiai (1, 2, 4, 8, 16, 32, ...)
    std::vector<int> kiekiu_sarasas = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    // Kartojimų skaičius
    const int kartojimai = 5;

    // Laikų saugojimas
    std::vector<double> laiko_rezultatai;

    for (int kiekis : kiekiu_sarasas) {
        double bendra_laikas = 0.0;

        for (int k = 0; k < kartojimai; k++) {
            std::string visos_eilutes;
            for (int i = 0; i < kiekis && i < eilutes.size(); i++) {
                visos_eilutes += eilutes[i];
            }

            // Laikmačio pradžia
            auto start = std::chrono::high_resolution_clock::now();

            // Hashavimo funkcija
            std::string hash = hashavimoGeneratorius(visos_eilutes);

            // Laikmačio pabaiga
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            bendra_laikas += elapsed.count();
        }

        // Suvidurkiname
        laiko_rezultatai.push_back(bendra_laikas / kartojimai);
    }

    // Išvedame rezultatus
    std::cout << "Eilučių kiekis ir suvidurkintas hashavimo laikas (sekundėmis):" << std::endl;
    for (size_t i = 0; i < kiekiu_sarasas.size(); ++i) {
        std::cout << "Eilučių kiekis: " << kiekiu_sarasas[i]
                  << ", Laikas: " << laiko_rezultatai[i] << " s" << std::endl;
    }

    return 0;
}
