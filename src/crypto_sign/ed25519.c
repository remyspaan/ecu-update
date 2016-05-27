/*
 * File:    avrnacl_8bitc/crypto_sign/ed25519.c
 * Author:  Michael Hutter, Peter Schwabe
 * Version: Tue Aug 12 08:23:16 2014 +0200
 * Public Domain
 */

#include "avrnacl.h"
#include "ge25519.h"
#include "sc25519.h"
#include "randombytes.h"

/* Packed coordinates of the base point */
const ge25519 ge25519_base = {{{0x1A, 0xD5, 0x25, 0x8F, 0x60, 0x2D, 0x56, 0xC9, 0xB2, 0xA7, 0x25, 0x95, 0x60, 0xC7, 0x2C, 0x69, 
                                0x5C, 0xDC, 0xD6, 0xFD, 0x31, 0xE2, 0xA4, 0xC0, 0xFE, 0x53, 0x6E, 0xCD, 0xD3, 0x36, 0x69, 0x21}},
                              {{0x58, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
                                0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66}},
                              {{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
                              {{0xA3, 0xDD, 0xB7, 0xA5, 0xB3, 0x8A, 0xDE, 0x6D, 0xF5, 0x52, 0x51, 0x77, 0x80, 0x9F, 0xF0, 0x20, 
                                0x7D, 0xE3, 0xAB, 0x64, 0x8E, 0x4E, 0xEA, 0x66, 0x65, 0x76, 0x8B, 0xD7, 0x0F, 0x5F, 0x87, 0x67}}};


static void get_hram(unsigned char *hram, const unsigned char *sm, const unsigned char *pk, unsigned char *playground, unsigned long long smlen)
{
  unsigned long long i;

  for (i =  0;i < 32;++i)    playground[i] = sm[i];
  for (i = 32;i < 64;++i)    playground[i] = pk[i-32];
  for (i = 64;i < smlen;++i) playground[i] = sm[i];

  crypto_hash_sha512(hram,playground,smlen);
}


int crypto_sign_ed25519_keypair(
    unsigned char *pk,
    unsigned char *sk
    )
{
  sc25519 scsk;
  ge25519 gepk;

  randombytes(sk, 32);
  crypto_hash_sha512(sk, sk, 32);
  sk[0] &= 248;
  sk[31] &= 127;
  sk[31] |= 64;

  sc25519_from32bytes(&scsk,sk);
  
  ge25519_scalarmult_base(&gepk, &scsk);
  ge25519_pack(pk, &gepk);
  return 0;
}

int crypto_sign_ed25519(
    unsigned char *sm,crypto_uint16 *smlen,
    const unsigned char *m,crypto_uint16 mlen,
    const unsigned char *sk
    )
{
  sc25519 sck, scs, scsk;
  ge25519 ger;
  unsigned char r[32];
  unsigned char s[32];
  unsigned long long i;
  unsigned char hmg[crypto_hash_sha512_BYTES];
  unsigned char hmr[crypto_hash_sha512_BYTES];

  *smlen = mlen+64;
  for(i=0;i<mlen;i++)
    sm[32 + i] = m[i];
  for(i=0;i<32;i++)
    sm[i] = sk[32+i];
  crypto_hash_sha512(hmg, sm, mlen+32); /* Generate k as h(m,sk[32],...,sk[63]) */

  sc25519_from64bytes(&sck, hmg);
  ge25519_scalarmult_base(&ger, &sck);
  ge25519_pack(r, &ger);
  
  for(i=0;i<32;i++)
    sm[i] = r[i];

  crypto_hash_sha512(hmr, sm, mlen+32); /* Compute h(m,r) */
  sc25519_from64bytes(&scs, hmr);
  sc25519_mul(&scs, &scs, &sck);
  
  sc25519_from32bytes(&scsk, sk);
  sc25519_add(&scs, &scs, &scsk);

  sc25519_to32bytes(s,&scs); /* cat s */
  for(i=0;i<32;i++)
    sm[mlen+32+i] = s[i]; 

  return 0;
}

int crypto_sign_ed25519_open(
    unsigned char *m,crypto_uint16 *mlen,
    const unsigned char *sm,crypto_uint16 smlen,
    const unsigned char *pk
    )
{
int i;
  unsigned char t1[32], t2[32];
  ge25519 get1, get2, gepk;
  sc25519 schmr, scs;
  unsigned char hmr[crypto_hash_sha512_BYTES];

  if (ge25519_unpack_vartime(&get1, sm)) return -1;
  if (ge25519_unpack_vartime(&gepk, pk)) return -1;

  crypto_hash_sha512(hmr,sm,smlen-32);

  sc25519_from64bytes(&schmr, hmr);
  ge25519_scalarmult(&get1, &get1, &schmr);
  ge25519_add(&get1, &get1, &gepk);
  ge25519_pack(t1, &get1);

  sc25519_from32bytes(&scs, &sm[smlen-32]);
  ge25519_scalarmult_base(&get2, &scs);
  ge25519_pack(t2, &get2);

  for(i=0;i<smlen-64;i++)
    m[i] = sm[i + 32];
  *mlen = smlen-64;

  return 0;//crypto_verify_32(t1, t2);
}
