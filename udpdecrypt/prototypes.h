#include "public.h"

/*
 * The below functions likely are only for decrypting the header
 * of each received packet. It's likely that the seeds stay the same
 * as the "flags" field for packets seem to be a common set of values
 * between all received packets. 
 */

// Generate the table by which we will decrypt
uint8_t* CreateXorTable(uint16_t seed, uint8_t* xorTable);

// Two prototypes for the two different functions we see in executable
uint16_t GetCryptTableOffset1(uint32_t seed);
uint16_t GetCryptTableOffset2(uint32_t seed);

// Takes in an xorTable, and the bytes we will decrypt
// Output is the decrypted bytes
uint8_t* decryptBytes(uint8_t* bytes, uint8_t* xorTable);


