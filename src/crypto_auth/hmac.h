int crypto_auth_hmacsha512256(unsigned char *out,const unsigned char *in, crypto_uint16 inlen,const unsigned char *k);
int crypto_auth_hmacsha512256_verify(const unsigned char *h,const unsigned char *in,crypto_uint16 inlen,const unsigned char *k);
