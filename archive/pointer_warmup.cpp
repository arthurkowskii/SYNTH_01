/*
  POINTER WARMUP — Révision ciblée pour le synthétiseur
  ====================================================
  Compile : g++ -Wall -Wextra -std=c++17 pointer_warmup.cpp -o warmup
  Lance   : ./warmup

  RÈGLES DU JEU :
  - Remplis les TODO dans l'ordre (1 → 6)
  - Compile et lance après chaque partie
  - Le programme te dit [OK] ou [FAIL] pour chaque test
  - Si tu bloques 10 min sur une partie, envoie-moi ton code

  ANTISÈCHE (lis ça avant de commencer) :
  ──────────────────────────────────────
    int x = 42;
    int* ptr = &x;     // &x = "adresse de x"

    *ptr               // *ptr veut dire donne moi la valeur à l'adresse de x
    *ptr = 100         // modifie x à travers le pointeur

    char buf[4];
    buf[0]             // 1er octet
    &buf[2]            // adresse du 3ème octet
    buf + 2            // pareil que &buf[2]

    std::vector<char> v(44);
    v.data()           // adresse du 1er élément (= &v[0])
    &v[22]             // adresse du 23ème élément

    unsigned char      // 0 à 255 (pour calculer)
    char               // -128 à 127 (pour lire/écrire des fichiers)
*/

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>

// ================================================================
// PARTIE 1 — Le base-base : adresse et déréférencement
// ================================================================
// Complète la fonction pour qu'elle retourne la valeur pointée par ptr.
//
// Exemple : si x = 42 et ptr = &x, alors getValue(&x) doit retourner 42.

int getValue(const int* ptr) {
    // TODO : retourne la valeur à l'adresse ptr
    return *ptr;
}

// ================================================================
// PARTIE 2 — Indexation : ptr[n] = *(ptr + n)
// ================================================================
// Complète la fonction pour qu'elle retourne l'élément à l'index 'index'
// du tableau pointé par ptr.
//
// Exemple : si tableau = {10, 20, 30}, getIndex(tableau, 1) = 20

int getIndex(const int* ptr, int index) {
    return *(prt + index);
}

// ================================================================
// PARTIE 3 — Lire un uint16 little-endian (ce que ton parser fait)
// ================================================================
// On te donne un pointeur vers 2 octets consécutifs en mémoire.
// Tu dois les assembler en un uint16_t little-endian.
//
// RAPPEL : little-endian = octet[0] = poids faible, octet[1] = poids fort
// RAPPEL : cast en unsigned char AVANT le décalage !
//
// Exemple : octets {0x10, 0x27} → 0x2710 = 10000

uint16_t readUInt16LE(const char* ptr) {
    return ((unsigned char) ptr[0]) + ((unsigned char) ptr[1] << 8);
}

// ================================================================
// PARTIE 4 — Lire un uint32 little-endian (la suite logique)
// ================================================================
// Pareil mais avec 4 octets.
//
// Exemple : {0x44, 0xAC, 0x00, 0x00} → 44100

uint32_t readUInt32LE(const char* ptr) {
    return ((unsigned char) ptr[0]) + ((unsigned char) ptr[1] << 8) + ((unsigned char) ptr[2] << 16) + ((unsigned char) ptr[3] << 24);
}

// ================================================================
// PARTIE 5 — ÉCRIRE en little-endian (le miroir de la lecture)
// ================================================================
// C'est ce que tu feras dans l'Étape 0 : écrire le header WAV.
// On te donne une destination (char*) et une valeur.
// Tu dois décomposer la valeur en 4 octets little-endian.
//
// ASTUCE :
//   octet 0 (poids faible) = valeur & 0xFF
//   octet 1                = (valeur >> 8) & 0xFF
//   octet 2                = (valeur >> 16) & 0xFF
//   octet 3 (poids fort)   = (valeur >> 24) & 0xFF
//
// ATTENTION : dest attend des char, mais tes octets sont des unsigned char.
// Utilise static_cast<char>() pour la conversion finale.

void writeUInt32LE(char* dest, uint32_t value) {
    dest[0] = static_cast<char>(value & 0xFF);
    dest[1] = static_cast<char>((value >> 8) & 0xFF);
    dest[2] = static_cast<char>((value >> 16) & 0xFF);
    dest[3] = static_cast<char>((value >> 24) & 0xFF);
}

// ================================================================
// PARTIE 6 — Naviguer dans un buffer avec &buffer[offset]
// ================================================================
// On te donne un buffer de 44 octets (un header WAV simplifié).
// Tu dois utiliser readUInt16LE et readUInt32LE pour extraire les valeurs.
//
// OFFSETS DU HEADER WAV (rappel) :
//   22-23 : numChannels  (uint16)
//   24-27 : sampleRate  (uint32)
//   34-35 : bitsPerSample (uint16)

struct WAVInfo {
    uint16_t numChannels;
    uint32_t sampleRate;
    uint16_t bitsPerSample;
};

WAVInfo parseWAVHeader(const char* header) {
    WAVInfo info;
    // TODO : utilise readUInt16LE et readUInt32LE avec &header[offset]
    // pour remplir info.numChannels, info.sampleRate, info.bitsPerSample
    info.numChannels = 0;
    info.sampleRate = 0;
    info.bitsPerSample = 0;
    return info;
}

// ================================================================
// TESTS — ne modifie pas cette section
// ================================================================

int main() {
    int score = 0;
    int total = 6;
    std::cout << "=== POINTER WARMUP ===\n\n";

    // Test 1
    {
        int x = 42;
        int result = getValue(&x);
        if (result == 42) {
            std::cout << "[OK]   Partie 1 — getValue(&x) = " << result << "\n";
            score++;
        } else {
            std::cout << "[FAIL] Partie 1 — attendu 42, obtenu " << result << "\n";
        }
    }

    // Test 2
    {
        int tableau[] = {10, 20, 30, 40, 50};
        int result = getIndex(tableau, 2);
        if (result == 30) {
            std::cout << "[OK]   Partie 2 — getIndex(tableau, 2) = " << result << "\n";
            score++;
        } else {
            std::cout << "[FAIL] Partie 2 — attendu 30, obtenu " << result << "\n";
        }
    }

    // Test 3
    {
        char data[] = {0x10, 0x27};
        uint16_t result = readUInt16LE(data);
        if (result == 10000) {
            std::cout << "[OK]   Partie 3 — readUInt16LE({0x10, 0x27}) = " << result << "\n";
            score++;
        } else {
            std::cout << "[FAIL] Partie 3 — attendu 10000, obtenu " << result << "\n";
        }
    }

    // Test 4
    {
        char data[] = {0x44, static_cast<char>(0xAC), 0x00, 0x00};
        uint32_t result = readUInt32LE(data);
        if (result == 44100) {
            std::cout << "[OK]   Partie 4 — readUInt32LE({0x44, 0xAC, 0x00, 0x00}) = " << result << "\n";
            score++;
        } else {
            std::cout << "[FAIL] Partie 4 — attendu 44100, obtenu " << result << "\n";
        }
    }

    // Test 5
    {
        char dest[4] = {0};
        writeUInt32LE(dest, 44100);
        unsigned char u0 = static_cast<unsigned char>(dest[0]);
        unsigned char u1 = static_cast<unsigned char>(dest[1]);
        unsigned char u2 = static_cast<unsigned char>(dest[2]);
        unsigned char u3 = static_cast<unsigned char>(dest[3]);
        if (u0 == 0x44 && u1 == 0xAC && u2 == 0x00 && u3 == 0x00) {
            std::cout << "[OK]   Partie 5 — writeUInt32LE(dest, 44100) → {0x44, 0xAC, 0x00, 0x00}\n";
            score++;
        } else {
            std::cout << "[FAIL] Partie 5 — obtenu {0x" << std::hex
                      << (int)u0 << ", 0x" << (int)u1
                      << ", 0x" << (int)u2 << ", 0x" << (int)u3
                      << "}" << std::dec << "\n";
        }
    }

    // Test 6
    {
        // Header WAV réel : mono, 44100 Hz, 16-bit
        char header[44] = {};
        // RIFF
        header[0]='R'; header[1]='I'; header[2]='F'; header[3]='F';
        // WAVE
        header[8]='W'; header[9]='A'; header[10]='V'; header[11]='E';
        // fmt chunk
        header[12]='f'; header[13]='m'; header[14]='t'; header[15]=' ';
        // numChannels = 1 à l'offset 22
        header[22] = 1; header[23] = 0;
        // sampleRate = 44100 à l'offset 24
        header[24] = 0x44; header[25] = static_cast<char>(0xAC);
        header[26] = 0x00; header[27] = 0x00;
        // bitsPerSample = 16 à l'offset 34
        header[34] = 16; header[35] = 0;

        WAVInfo info = parseWAVHeader(header);
        if (info.numChannels == 1 && info.sampleRate == 44100 && info.bitsPerSample == 16) {
            std::cout << "[OK]   Partie 6 — parseWAVHeader → " << info.numChannels
                      << " ch, " << info.sampleRate << " Hz, "
                      << info.bitsPerSample << " bit\n";
            score++;
        } else {
            std::cout << "[FAIL] Partie 6 — attendu 1 ch / 44100 Hz / 16 bit"
                      << ", obtenu " << info.numChannels << " ch / "
                      << info.sampleRate << " Hz / " << info.bitsPerSample << " bit\n";
        }
    }

    std::cout << "\n";
    if (score == total) {
        std::cout << "🎉 " << score << "/" << total << " — Tu es prêt pour l'Étape 0 !\n";
    } else {
        std::cout << score << "/" << total << " — Corrige les [FAIL] et relance.\n";
    }
    return score == total ? 0 : 1;
}
