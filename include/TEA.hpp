#ifndef GUARD_TEA_ENCRYPTION_FROM_PPP
#define GUARD_TEA_ENCRYPTION_FROM_PPP

#include <cstdint>
#include <fstream>
#include <iomanip>

namespace tea {

constexpr int nchar = 2 * sizeof(uint32_t); // 64 bits
constexpr int kchar = 2 * nchar;            // 128 bits

//------------------------------------------------------------------------------
// basic operations
namespace {
inline void encipher(const uint32_t* const v, uint32_t* const w, const uint32_t* const k) {
    static_assert(sizeof(uint32_t) == 4, "size of uint32_t wrong for TEA");
    uint32_t y = v[0];
    uint32_t z = v[1];
    uint32_t sum = 0;
    const uint32_t delta = 0x9E3779B9;
    for (uint32_t n = 32; n-- > 0;) {
        y += (z << 4 ^ z >> 5) + z ^ sum + k[sum & 3];
        sum += delta;
        z += (y << 4 ^ y >> 5) + y ^ sum + k[sum >> 11 & 3];
    }
    w[0] = y;
    w[1] = z;
}

inline void decipher(const uint32_t* const v, uint32_t* const w, const uint32_t* const k) {
    static_assert(sizeof(uint32_t) == 4, "size of uint32_t wrong for TEA");
    uint32_t y = v[0];
    uint32_t z = v[1];
    uint32_t sum = 0xC6EF3720;
    const uint32_t delta = 0x9E3779B9;
    for (uint32_t n = 32; n-- > 0;) {
        z -= (y << 4 ^ y >> 5) + y ^ sum + k[sum >> 11 & 3];
        sum -= delta;
        y -= (z << 4 ^ z >> 5) + z ^ sum + k[sum & 3];
    }
    w[0] = y;
    w[1] = z;
}
} // namespace

//------------------------------------------------------------------------------
// more advanced functions for whole files

inline void encrypt_file(std::ifstream& inf, std::ofstream& outf, std::string key) {
    while (key.size() < kchar) {
        key += '0'; // pad key
    }
    const uint32_t* k = reinterpret_cast<const uint32_t*>(key.data());
    uint32_t outptr[2];
    char inbuf[nchar];
    uint32_t* inptr = reinterpret_cast<uint32_t*>(inbuf);
    int counter = 0;
    while (inf.get(inbuf[counter])) {
        outf << std::hex; // use hexadecimal output
        if (++counter == nchar) {
            encipher(inptr, outptr, k);
            // pad with leading zeros;
            outf << std::setw(8) << std::setfill('0') << outptr[0] << ' ' << std::setw(8)
                 << std::setfill('0') << outptr[1] << ' ';
            counter = 0;
        }
    }
    if (counter) { // pad
        while (counter != nchar)
            inbuf[counter++] = '0';
        encipher(inptr, outptr, k);
        outf << outptr[0] << ' ' << outptr[1] << ' ';
    }
}

inline void decrypt_file(std::ifstream& inf, std::ofstream& outf, std::string key) {
    while (key.size() < kchar) {
        key += '0'; // pad key
    }
    const uint32_t* k = reinterpret_cast<const uint32_t*>(key.data());
    uint32_t inptr[2];
    char outbuf[nchar + 1];
    outbuf[nchar] = 0; // terminator
    uint32_t* outptr = reinterpret_cast<uint32_t*>(outbuf);
    inf.setf(std::ios_base::hex, std::ios_base::basefield); // use hexadecimal input

    while (inf >> inptr[0] >> inptr[1]) {
        decipher(inptr, outptr, k);
        outf << outbuf;
    }
}

} // namespace tea

#endif
