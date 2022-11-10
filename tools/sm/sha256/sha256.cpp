#include "sha256.hpp"
#include "utils.hpp"
#include "scalar.hpp"
#include "timer.hpp"

void SHA256String (const string &s, string &hash)
{
    string ba;
    string2ba(s, ba);
    SHA256((uint8_t *)ba.c_str(), ba.size(), hash);
}

void SHA256 (const uint8_t * pData, uint64_t dataSize, string &hash)
{
    // Initialize hash values:
    // (first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):
    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;

    // Initialize array of round constants:
    // (first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):
    uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

    // Padding:
    // original message of length L bits
    // padded message: <original message of length L> 1 bit <K zero bits> <L as 64 bit integer>
    // (the number of bits will be a multiple of 512)

    // padded data = pData[dataSize] + 0x80 + paddedZeros*0x00 + dataSize[8]
    uint64_t paddedSizeInBitsMin = dataSize*8 + 1 + 64;
    uint64_t paddedSizeInBits = ((paddedSizeInBitsMin / 512) + 1)*512;
    uint64_t paddedSize = paddedSizeInBits / 8;
    uint64_t paddedZeros = (paddedSizeInBits - paddedSizeInBitsMin)/8;

    // Create the padding data buffer
    uint8_t padding[64] = {0};
    u642bytes(dataSize*8, &padding[56], true);
    uint64_t onePosition = 64 - 8 - paddedZeros - 1;
    padding[onePosition] = 0x80;
    uint64_t dataPosition = (dataSize/64)*64;
    for (uint64_t i=0; i<dataSize%64; i++)
    {
        padding[i] = pData[dataPosition+i];
    }

    // Process the message in successive 512-bit chunks: break message into 512-bit chunks
    for (uint64_t chunk=0; chunk<paddedSize/64; chunk++)
    {
        // create a 64-entry message schedule array w[0..63] of 32-bit words
        uint32_t w[64] = {0};

        // copy chunk into first 16 words w[0..15] of the message schedule array
        
        // determinte the buffer to copy data from
        const uint8_t *pChunkBytes;
        if (chunk == ((paddedSize/64) - 1))
        {
            pChunkBytes = padding;
        }
        else
        {
            pChunkBytes = pData + chunk*64;
        }

        // copy the 64 bytes (512 bits) into w[0..15]
        for (uint64_t i=0; i<16; i++)
        {
            bytes2u32(pChunkBytes + 4*i, w[i], true);
        }

        // Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
        for (uint64_t i=16; i<64; i++)
        {
            uint32_t s0 = rotateRight32(w[i-15], 7) ^ rotateRight32(w[i-15], 18) ^ ( w[i-15] >> 3 );
            uint32_t s1 = rotateRight32(w[i-2], 17) ^ rotateRight32(w[i-2], 19) ^ ( w[i-2] >> 10 );
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }

        // Initialize working variables to current hash value
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        uint32_t f = h5;
        uint32_t g = h6;
        uint32_t h = h7;

        // Compression function main loop
        for (uint64_t i=0; i<64; i++)
        {
            uint32_t S1 = rotateRight32(e, 6) ^ rotateRight32(e, 11) ^ rotateRight32(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = h + S1 + ch + k[i] + w[i];
            uint32_t S0 = rotateRight32(a, 2) ^ rotateRight32(a, 13) ^ rotateRight32(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;
    
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }
        // Add the compressed chunk to the current hash value
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
        h5 = h5 + f;
        h6 = h6 + g;
        h7 = h7 + h;
    }

    mpz_class hashScalar;
    hashScalar = h0;
    hashScalar = hashScalar << 32;
    hashScalar += h1;
    hashScalar = hashScalar << 32;
    hashScalar += h2;
    hashScalar = hashScalar << 32;
    hashScalar += h3;
    hashScalar = hashScalar << 32;
    hashScalar += h4;
    hashScalar = hashScalar << 32;
    hashScalar += h5;
    hashScalar = hashScalar << 32;
    hashScalar += h6;
    hashScalar = hashScalar << 32;
    hashScalar += h7;

    hash = "0x" + hashScalar.get_str(16);
}

vector<vector<string>> sha256TestVectors = {
    {"", "0xe3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
    {"0x", "0xe3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
    {"0x00", "0x6e340b9cffb37a989ca544e6bb780a2c78901d3fb33738768511a30617afa01d"},
    {"0x0000", "0x96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7"},
    {"0x01", "0x4bf5122f344554c53bde2ebb8cd2b7e3d1600ad631c385a5d7cce23c7785459a"},
    {"0x0102030405060708090a0b0c0d0e0f", "0x36030fcc7e566294905b49a720eb45bf962209d2ee1c9b73e2b7bc7ae8830376"},
    {"0x000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f", "0xb1d1a68efaee9083e1e43459e6ef9620320ba3eff096b2a96ef77956472c0e33"},
    {"0x000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0fffffff", "0xe603b922cc427b9b171e8c6fd23fbfbcd775913b4ec9242411e1d0cb77d1ef06"},
    };

void SHA256Test (Goldilocks &fr, Config &config)
{
    TimerStart(SHA256_TEST);

    for (uint64_t i=0; i<sha256TestVectors.size(); i++)
    {
        string input = sha256TestVectors[i][0];
        string expectedHash = sha256TestVectors[i][1];
        string hash;
        SHA256String(input, hash);
        if (hash != expectedHash)
        {
            cerr << "Error: SHA256Test() failed, hash of " << input << " is " << hash << " instead of " << expectedHash << endl;
            exitProcess();
        }
    }

    TimerStopAndLog(SHA256_TEST);

    cout << "SHA256Test() done" << endl;

}