/*
 * ce.c
 *
 *  Created on: 2015.12.21
 *      Author: feng.wang
 */

#include <security/sprd_ce_r1p0.h>
#include <security/sprd_crypt_r1p0.h>
#include <security/pk1.h>

#ifdef LTC_TEE_MODE
#include <kernel/panic.h>
#include <trace.h>
#define printf(...) DMSG(__VA_ARGS__)
#define fprintf(stream, ...) EMSG(__VA_ARGS__)
#define fflush(stream) (void)0
#define exit(...) panic()
#endif

#define SPRD_CRYPT_ARGCHK(x)   if (!(x)) return SPRD_CRYPT_ERR_ARG;
//#define CE_BLOCK_SIZE SPRD_DEFAULT_SRC_LEN

static struct sprd_ce_parms {
    bool status;
    struct sprd_ce_cfg cfg;
    union sprd_ce_mode mode;
    struct sprd_ce_data data;
    struct sprd_ce_key key;
    struct sprd_ce_iv iv;
    struct sprd_ce_flag flag;
} s_parms = {
	false, 
	{}, 
	{}, 
	{}, 
	{}, 
	{}, 
	{0, 0, 0, 1}};

struct mode_translation {
    int mode;
    int out_len;
} s_mode_translation[] = {
    { SPRD_CE_AES_ECB,  0 },
    { SPRD_CE_AES_CBC,  0 },
    { SPRD_CE_AES_CTR,  0 },
    { SPRD_CE_AES_XTS,  0 },

    { SPRD_CE_TDES_ECB,  0 },
    { SPRD_CE_TDES_CBC,  0 },

    { SPRD_CE_HASH_MD5,         16 },
    { SPRD_CE_HASH_SHA1,        20 },
    { SPRD_CE_HASH_SHA224,      28 },
    { SPRD_CE_HASH_SHA256,      32 },
    { SPRD_CE_HASH_SHA384,      48 },
    { SPRD_CE_HASH_SHA512,      64 },
    { SPRD_CE_HASH_SHA512_224,  28 },
    { SPRD_CE_HASH_SHA512_256,  32 },
    { SPRD_CE_HMAC_MD5,         16 },
    { SPRD_CE_HMAC_SHA1,        20 },
    { SPRD_CE_HMAC_SHA224,      28 },
    { SPRD_CE_HMAC_SHA256,      32 },
    { SPRD_CE_HMAC_SHA384,      48 },
    { SPRD_CE_HMAC_SHA512,      64 },
    { SPRD_CE_HMAC_SHA512_224,  28 },
    { SPRD_CE_HMAC_SHA512_256,  32 },

    {SPRD_CE_RSA_ENC_1024,          128 },
    {SPRD_CE_RSA_ENC_2048,          256 },
    {SPRD_CE_RSA_ENC_3072,          384 },
    {SPRD_CE_RSA_ENC_4096,          512 },
    {SPRD_CE_RSA_DEC_1024,          128 },
    {SPRD_CE_RSA_DEC_2048,          256 },
    {SPRD_CE_RSA_DEC_3072,          384 },
    {SPRD_CE_RSA_DEC_4096,          512 },
    {SPRD_CE_RSA_KEY_GEN_PRIME_512, 256 },
    {SPRD_CE_RSA_KEY_GEN_PRIME_1024,512 },
    {SPRD_CE_RSA_KEY_GEN_PRIME_1536,768 },
    {SPRD_CE_RSA_KEY_GEN_PRIME_2048,1024},

    {SPRD_CE_RNG_MIX,               4},
    {SPRD_CE_RNG_TRUE,              4},
    {SPRD_CE_RNG_PSEUDO,            4},

};

#define BIT(_n) ((unsigned int) 1 << (_n))

int sprd_crypt_do(
        unsigned int mode,
        const unsigned char *in,
        unsigned int in_len,
        unsigned char *out,
        unsigned int *out_len,
        const unsigned char *key,
        unsigned int key_len,
        const unsigned char *iv,
        unsigned int iv_len){

    int err = SPRD_CE_OK;
    unsigned int ce_mode;

    SPRD_CRYPT_ARGCHK(in != NULL);
    SPRD_CRYPT_ARGCHK(in_len != 0);
    SPRD_CRYPT_ARGCHK(out != NULL);
    SPRD_CRYPT_ARGCHK(out_len != NULL);
    sprd_ce_init();
    switch(mode) {
    case SPRD_CRYPT_AES_CBC:
        SPRD_CRYPT_ARGCHK(key != NULL);
        SPRD_CRYPT_ARGCHK(key_len == 32); //256bit
        SPRD_CRYPT_ARGCHK(iv != NULL);
        SPRD_CRYPT_ARGCHK(iv_len == 16); //128bit
        SPRD_CRYPT_ARGCHK(*out_len >= in_len);
        break;
    case SPRD_CRYPT_HASH_SHA256:
        SPRD_CRYPT_ARGCHK(*out_len >= 32);
        break;
    case SPRD_CRYPT_RSA_ENC_2048:
        SPRD_CRYPT_ARGCHK(key != NULL);
        SPRD_CRYPT_ARGCHK(key_len == 260); //2 keys should be in the key buffer
        SPRD_CRYPT_ARGCHK(*out_len >= in_len);
        break;
    }
    SPRD_CRYPT_ARGCHK(mode < sizeof(s_mode_translation)/sizeof(struct mode_translation));

    //init
    if(s_parms.status == true) {
        return SPRD_CE_NOP;
    }

    sec_memset(&s_parms, 0, sizeof(struct sprd_ce_parms));

    s_parms.cfg.src_outstanding_num = 0x7;
    s_parms.cfg.dst_outstanding_num = 0x7;
    s_parms.cfg.src_word_switch = 0x0;
    s_parms.cfg.dst_word_switch = 0x0;
    s_parms.cfg.src_byte_switch = 0x1;
    s_parms.cfg.dst_byte_switch = 0x1;
    s_parms.cfg.list_up_iv_sec_cnt = 0x0;
    s_parms.cfg.list_end = 0x0;
    s_parms.cfg.list_pause = 0x0;
    s_parms.cfg.gen_int_each_list = 0x0;
    s_parms.cfg.do_wait_bdone = 0x1;
    s_parms.cfg.key_in_session = 0x0;
    s_parms.cfg.key_in_efuse = 0x0;
    s_parms.cfg.key_in_ddr = 0x1;
    s_parms.cfg.std_end = 0x1;
    s_parms.cfg.link_mode = SPRD_CE_STD;

    s_parms.flag.ret = 0x00;
    s_parms.flag.status = SPRD_CE_END;
    s_parms.flag.start = 0x00;
    s_parms.flag.end = 0x00;

    sprd_ce_open();

    sprd_ce_set_cfg(&s_parms.cfg);

    s_parms.status = true;

    //set mode
    ce_mode = s_mode_translation[mode].mode;

    s_parms.mode.mode = ce_mode;

    switch(s_parms.mode.mode & SPRD_CE_MAIN_MODE_MASK) {
    case SPRD_CE_AES:
        s_parms.mode.aes_mode.cfg.xts_iv_rotate = 0;
        s_parms.mode.aes_mode.cfg.keylen = key_len == 16 ? 0x0 : (key_len == 24 ? 0x1 : (key_len == 32 ? 0x2 : 0x0));;
        s_parms.mode.aes_mode.cfg.mode = ce_mode & SPRD_CE_SUB_MODE_MASK;
        s_parms.mode.aes_mode.cfg.enc_dec_sel = SPRD_CE_DEC;
        s_parms.mode.aes_mode.cfg.en = 1;
        break;
    case SPRD_CE_HASH:
        s_parms.mode.hash_mode.cfg.mode = ce_mode & SPRD_CE_SUB_MODE_MASK;
        s_parms.mode.hash_mode.cfg.en = 0x1;
        break;
    case SPRD_CE_RSA:
        s_parms.mode.rsa_mode.cfg.random_mode_sel = 2;
        s_parms.mode.rsa_mode.cfg.random_sel = 0;
        s_parms.mode.rsa_mode.cfg.miller_num = 4;
        s_parms.mode.rsa_mode.cfg.mode = ce_mode & SPRD_CE_SUB_MODE_MASK;
        s_parms.mode.rsa_mode.cfg.en = 0x1;
        break;
    default:
        break;
    }

    if(sprd_ce_set_mode(&s_parms.mode) != SPRD_CE_OK) {
        goto failed;
    }

    //set data and key
    s_parms.key.key = (unsigned char *)key;
    s_parms.key.key_len = key_len;
    s_parms.key.key_max_len = key_len;
    s_parms.key.key_cacheable = false;

    s_parms.iv.iv = (unsigned char *)iv;
    s_parms.iv.iv_len = iv_len;
    s_parms.iv.iv_max_len = iv_len;

    s_parms.data.in = (unsigned char *)in;
    s_parms.data.in_len = in_len;
    s_parms.data.in_max_len = in_len;
    s_parms.data.in_cacheable = false;
    s_parms.data.out = out;
    s_parms.data.out_len = *out_len;
    s_parms.data.out_max_len = *out_len;
    s_parms.data.out_cacheable = false;

    switch(s_parms.mode.mode & SPRD_CE_MAIN_MODE_MASK) {
    case SPRD_CE_AES:
        if ((err = sprd_ce_set_key_phy(s_mode_translation[mode].mode, &s_parms.key)) != SPRD_CE_OK) {
            goto failed;
        }

        if ((err = sprd_ce_set_iv(s_mode_translation[mode].mode, &s_parms.iv)) != SPRD_CE_OK) {
            goto failed;
        }

        sprd_ce_set_cfg(&s_parms.cfg);

        if ((err = sprd_ce_set_data_phy(s_parms.mode.mode, &s_parms.data)) != SPRD_CE_OK) {
            goto failed;
        }
        break;

    case SPRD_CE_HASH:
        sprd_ce_set_cfg(&s_parms.cfg);

        s_parms.data.out_len = 32;
        if ((err = sprd_ce_set_data_phy(s_parms.mode.mode, &s_parms.data)) != SPRD_CE_OK) {
            goto failed;
        }
        break;

    case SPRD_CE_RSA:
        if ((err = sprd_ce_set_key_phy(s_mode_translation[mode].mode, &s_parms.key)) != SPRD_CE_OK) {
            goto failed;
        }

        sprd_ce_set_cfg(&s_parms.cfg);

        if ((err = sprd_ce_set_data_phy(s_parms.mode.mode, &s_parms.data)) != SPRD_CE_OK) {
            goto failed;
        }
        break;
    }

    //process
    sprd_ce_operate();
    s_parms.flag.status = sprd_ce_polling();
    if (s_parms.flag.status == SPRD_CE_END) {
        err = SPRD_CE_OK;
    } else {
        err = SPRD_CE_FAILED;
    }

failed:
    sprd_ce_clr_mode(&s_parms.mode);
    s_parms.status = false;
    sprd_ce_close();
    sprd_ce_destroy();
    return err;
}

void sha256_csum_wd(const unsigned char *input,unsigned int ilen,unsigned char *output,unsigned int chunk_sz) {
	unsigned int outlen = 32;
	sprd_crypt_do(
		SPRD_CRYPT_HASH_SHA256,
		input, ilen,
		output, &outlen,
		NULL, 0,
		NULL, 0);
}


int RSA_PubDec(unsigned char *pub_E, unsigned char *mod_N, int bitLen_N, unsigned char *from, unsigned char *to) {
	unsigned int outlen = 256;
	unsigned char key[260];
	int byteLen_N = bitLen_N >> 3;
	unsigned char text[byteLen_N];

	sec_memcpy(key, mod_N, 256);
	invert_char(pub_E,sizeof(unsigned int));
	sec_memcpy(key+256, pub_E, 4);
	sprd_crypt_do(
		SPRD_CRYPT_RSA_ENC_2048,
		from, 256,
		text, &outlen,
		key, 260,
		NULL, 0);
	return padding_check_PKCS1_type_1(to,byteLen_N,text+1,byteLen_N -1,byteLen_N);
}

int AES_CBC_Dec(unsigned char *ct, unsigned int ct_len, unsigned char *pt, unsigned int *pt_len,
		unsigned char *key, unsigned int key_len, unsigned char *iv, unsigned int iv_len) {

	return sprd_crypt_do(
		SPRD_CRYPT_AES_CBC,
		ct, ct_len,
		pt, pt_len,
		key, key_len,
		iv, iv_len);
}

#if 0
//#define TEST_AES
//#define TEST_SHA
#define TEST_RSA

#ifdef TEST_AES
int sprd_aes_cbc_test(void) {
    int i, j;
    unsigned int tmp_len = 0;
    unsigned char *key = (unsigned char *) 0x7F000000;
    unsigned char *iv = (unsigned char *) 0x7F010000;
    unsigned char *in = (unsigned char *) 0x7F020000;
    unsigned char *out = (unsigned char *) 0x7F030000;
    static const struct {
        unsigned int ct_len;
        const char key[128] __attribute__ ((aligned (8)));
        const char iv[128] __attribute__ ((aligned (8)));
        const char ct[128] __attribute__ ((aligned (8)));
        const char pt[128];
    } tests[] = {
        { 32,
          { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
            0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
            0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
            0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f },
          { 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
            0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f },
          { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f },
          { 0x7d, 0x8e, 0x57, 0x40, 0xd5, 0x48, 0x68, 0x42,
            0x85, 0x47, 0x5b, 0x0f, 0x1d, 0xcd, 0xf8, 0x26,
            0xf3, 0xbc, 0xe0, 0x5c, 0x6d, 0x55, 0x04, 0x6f,
            0xd4, 0xed, 0x87, 0x3d, 0xe6, 0xf1, 0xc7, 0xc1 },
        },
    };

    for (i = 0; i < (int)(sizeof(tests) / sizeof(tests[0])); i++) {

    	sec_memcpy(key, tests[i].key, 32);
    	sec_memcpy(iv, tests[i].iv, 16);
    	sec_memcpy(in, tests[i].ct, 32);
        tmp_len = tests[i].ct_len;

        sprd_crypt_do(
            SPRD_CRYPT_AES_CBC,
            in, tests[i].ct_len,
            out, &tmp_len,
            key, 32,//256bit
            iv, 16);

        if (sec_memcmp(out, tests[i].pt, tests[i].ct_len) != 0) {
            return -1;
        }
    }
    return 0;
}
#endif
#ifdef TEST_SHA
int  sprd_sha256_test(void)
{
    unsigned char *in = (unsigned char *) 0x7F020000;
    unsigned char *out = (unsigned char *) 0x7F030000;
    static const struct {
		const char *msg __attribute__ ((aligned (8)));
		unsigned int msg_len;
		unsigned char hash[32];
    } tests[] = {
        { "a", 1,
        { 0xca, 0x97, 0x81, 0x12, 0xca, 0x1b, 0xbd, 0xca,
          0xfa, 0xc2, 0x31, 0xb3, 0x9a, 0x23, 0xdc, 0x4d,
          0xa7, 0x86, 0xef, 0xf8, 0x14, 0x7c, 0x4e, 0x72,
          0xb9, 0x80, 0x77, 0x85, 0xaf, 0xee, 0x48, 0xbb } },
        { "abcdefghijklmnopqrstuvwxyz", 26,
        { 0x71, 0xc4, 0x80, 0xdf, 0x93, 0xd6, 0xae, 0x2f,
          0x1e, 0xfa, 0xd1, 0x44, 0x7c, 0x66, 0xc9, 0x52,
          0x5e, 0x31, 0x62, 0x18, 0xcf, 0x51, 0xfc, 0x8d,
          0x9e, 0xd8, 0x32, 0xf2, 0xda, 0xf1, 0x8b, 0x73 } },
        { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012", 55,
        { 0x59, 0x52, 0x1a, 0xa5, 0xa7, 0x2b, 0xfd, 0x08,
          0x7f, 0xc7, 0xb1, 0x80, 0xef, 0xff, 0x1e, 0x20,
          0xdc, 0x27, 0xa7, 0xd6, 0x23, 0x2c, 0xc1, 0xeb,
          0xb7, 0x33, 0x18, 0x3d, 0x02, 0xa8, 0xc0, 0x62 } },
        { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123", 56,
        { 0xf5, 0x0e, 0x77, 0x55, 0x9f, 0xcf, 0xb0, 0xbe,
          0x5b, 0x29, 0x8f, 0x78, 0x08, 0x57, 0xcf, 0x0c,
          0x5f, 0x06, 0xaf, 0x72, 0x58, 0x38, 0x43, 0x1f,
          0x7c, 0x9d, 0xa4, 0x8f, 0xf0, 0x24, 0xba, 0x30 } },
        { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890", 63,
        { 0x60, 0x24, 0x4f, 0x16, 0x18, 0x27, 0xbb, 0x1f,
          0xe6, 0x2a, 0xcc, 0xf0, 0xd4, 0xa5, 0x42, 0x16,
          0xdf, 0x21, 0x03, 0x14, 0x6b, 0x18, 0xe4, 0xce,
          0xe6, 0x10, 0xac, 0x97, 0x24, 0x6c, 0x0b, 0x0b } },
        { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678901", 64,
        { 0xe5, 0xbf, 0x9c, 0x62, 0xa4, 0x1d, 0x4b, 0x47,
          0xd1, 0x2d, 0x26, 0x8e, 0xe1, 0x5b, 0x09, 0xff,
          0x45, 0xf9, 0xd9, 0x2f, 0xdb, 0xb1, 0xd3, 0xf9,
          0x7c, 0x25, 0xd2, 0xc6, 0x4a, 0xef, 0xd8, 0x07,} },
        /*{ "12345678901234567890123456789012345678901234567890123456789012345678901234567890",//80
        { 0xf3, 0x71, 0xbc, 0x4a, 0x31, 0x1f, 0x2b, 0x00,
          0x9e, 0xef, 0x95, 0x2d, 0xd8, 0x3c, 0xa8, 0x0e,
          0x2b, 0x60, 0x02, 0x6c, 0x8e, 0x93, 0x55, 0x92,
          0xd0, 0xf9, 0xc3, 0x08, 0x45, 0x3c, 0x81, 0x3e,} },
        { "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwx",//128B
        { 0x6c, 0x05, 0xbe, 0x2c, 0x42, 0x68, 0x84, 0x3a,
          0xe4, 0x7e, 0x68, 0xe6, 0x11, 0x27, 0x7c, 0xe6,
          0x2c, 0x02, 0x15, 0x3f, 0x2f, 0x4d, 0x2e, 0x1e,
          0x2a, 0x1a, 0x4b, 0x44, 0xf7, 0x66, 0xcf, 0x74 } },
        { "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",//156B
        { 0xa8, 0x5e, 0x52, 0xd7, 0x7d, 0x9a, 0x6a, 0x6b,
          0x31, 0x71, 0x3d, 0x54, 0x7b, 0xc2, 0xe8, 0x25,
          0x65, 0x88, 0xc2, 0x1a, 0xec, 0x8e, 0x32, 0xdd,
          0xde, 0xdd, 0xa2, 0x53, 0xd5, 0x85, 0x0e, 0x21 } },
        { "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz0123456789",//192B
        { 0xaf, 0x9f, 0x0c, 0xb3, 0x80, 0x99, 0x44, 0xba,
          0x91, 0x4d, 0xd2, 0xd2, 0x87, 0x21, 0xc6, 0xf0,
          0x39, 0x56, 0x91, 0x1f, 0x44, 0x50, 0xe4, 0x81,
          0xcb, 0x18, 0xff, 0x9f, 0x92, 0xef, 0xdc, 0x65 } },*/
    };

    int i;
    unsigned int out_len = 32;

    for (i = 0; i < (int)(sizeof(tests) / sizeof(tests[0])); i++) {

    	sec_memcpy(in, tests[i].msg, tests[i].msg_len);

        sprd_crypt_do(
            SPRD_CRYPT_HASH_SHA256,
            in, tests[i].msg_len,
            out, &out_len,
            NULL, 0,
            NULL, 0);

        if (sec_memcmp(out, tests[i].hash, 32) != 0) {
        	*((unsigned int*)(0x440000004)) = i+1;
            return -1;
        }
    }
    return 0;

}
#endif
#ifdef TEST_RSA
int sprd_rsa_enc_test(void) {
    int i;
    unsigned char *key = (unsigned char *) 0x7F000000;
    unsigned char *in = (unsigned char *) 0x7F020000;
    unsigned char *out = (unsigned char *) 0x7F030000;
    unsigned int tmp_len;
    static const struct {
        unsigned int data_len;
        unsigned char key[512] __attribute__ ((aligned (8)));
        unsigned char din[256] __attribute__ ((aligned (8)));
        unsigned char dout[256];
    } tests[] =
    {
        256,
        {
            //N
            0xBB, 0xE6, 0x11, 0x28, 0x13, 0x40, 0xD8, 0x6D,
            0xE9, 0x54, 0x78, 0x82, 0x01, 0xA0, 0xDE, 0xCE,
            0x2D, 0x57, 0x69, 0x8E, 0x8A, 0x93, 0x26, 0x4F,
            0x72, 0x3F, 0xEB, 0xEF, 0xE1, 0xE9, 0xF9, 0x2E,
            0x6F, 0x43, 0x47, 0x0A, 0x4B, 0xA0, 0x83, 0xFC,
            0x8C, 0xC4, 0x93, 0xD9, 0xFC, 0x2E, 0xC4, 0xF9,
            0x7E, 0x67, 0xDC, 0x7C, 0xC6, 0xE5, 0xAE, 0xB4,
            0x1A, 0xB3, 0xDB, 0xD1, 0xE3, 0x9A, 0xB3, 0xB3,
            0xF2, 0xCB, 0x94, 0xBA, 0xCC, 0x30, 0x73, 0x62,
            0x46, 0x74, 0x44, 0xC1, 0x6B, 0x6B, 0x86, 0x11,
            0xB7, 0xD1, 0x7F, 0xA0, 0xD3, 0x53, 0x99, 0x7C,
            0x9A, 0x78, 0xA1, 0x04, 0x6C, 0x7D, 0x56, 0x09,
            0x08, 0x7C, 0x14, 0x61, 0xDC, 0xE5, 0x89, 0xB3,
            0x24, 0xB1, 0x73, 0x25, 0x7D, 0xE0, 0x5F, 0x43,
            0x36, 0x1C, 0x12, 0xDD, 0xB5, 0xE3, 0x8C, 0x6F,
            0x53, 0x4F, 0x94, 0xE1, 0xD3, 0xDC, 0x94, 0xBE,
            0xD8, 0x5D, 0xDD, 0x4A, 0x47, 0x13, 0xF6, 0xEE,
            0x5F, 0x7C, 0xA2, 0xE5, 0xBF, 0x92, 0xA6, 0xC9,
            0x08, 0x11, 0x5A, 0xFD, 0x97, 0x68, 0x60, 0x0A,
            0x72, 0xBA, 0xA0, 0x5E, 0x81, 0x27, 0xD5, 0x6C,
            0x19, 0x5D, 0xBA, 0x0F, 0x04, 0x64, 0x2B, 0x79,
            0x51, 0xA1, 0x36, 0x5F, 0x34, 0x43, 0xBF, 0xAB,
            0x31, 0xDB, 0x57, 0xBB, 0xA6, 0x94, 0x9F, 0x3D,
            0xBC, 0xE6, 0x69, 0xBD, 0xBC, 0x5D, 0xC4, 0xC0,
            0x0D, 0x20, 0xC8, 0xF2, 0x85, 0x67, 0xCE, 0x4F,
            0x68, 0xC7, 0x9D, 0x73, 0x95, 0x8B, 0x42, 0x2C,
            0x0D, 0x12, 0x98, 0xAD, 0x53, 0x5F, 0x21, 0xCD,
            0x9C, 0x01, 0x21, 0xDE, 0x7E, 0xB2, 0x32, 0xF1,
            0xC4, 0x66, 0xF5, 0x3D, 0x53, 0x6C, 0x24, 0xA0,
            0x86, 0x9B, 0xD4, 0x61, 0xB5, 0x63, 0xF6, 0x45,
            0x4E, 0xD4, 0x26, 0x86, 0x0C, 0xC9, 0x43, 0x73,
            0x9F, 0x18, 0xED, 0x20, 0xE8, 0xA3, 0xC0, 0xC9,

            //e
            0x01, 0x00, 0x01, 0x00,

            //D
            /*0xAE, 0x96, 0xFC, 0xB4, 0x0F, 0x07, 0xBE, 0xD2,
            0x89, 0xEE, 0xEA, 0x36, 0x6A, 0x8A, 0xEE, 0xDC,
            0x06, 0xB7, 0x65, 0x26, 0x84, 0x4E, 0x6C, 0x57,
            0x7F, 0xA2, 0x33, 0x79, 0x66, 0xA1, 0xFF, 0xDE,
            0x0C, 0x96, 0x27, 0xC9, 0x42, 0x32, 0x38, 0xD1,
            0x8E, 0x7F, 0x0D, 0x9E, 0x8B, 0xBB, 0x6D, 0x5D,
            0x21, 0xFE, 0x21, 0x9A, 0x73, 0x39, 0x71, 0x96,
            0x7A, 0x05, 0x4B, 0x28, 0x80, 0x62, 0xFB, 0xBD,
            0x25, 0xC9, 0xD2, 0x3D, 0xAB, 0x83, 0x0D, 0x83,
            0x70, 0x51, 0x92, 0x9D, 0x62, 0x51, 0xBB, 0x76,
            0x75, 0x25, 0x9D, 0x80, 0x73, 0x3C, 0x07, 0xAF,
            0x06, 0xDD, 0x35, 0x48, 0x26, 0x02, 0x63, 0xD1,
            0xEA, 0xE7, 0x58, 0x0D, 0x02, 0xB2, 0xE9, 0xBA,
            0xE2, 0x4A, 0x5B, 0xE2, 0xA7, 0x4E, 0xB7, 0x55,
            0x7D, 0x4E, 0xA2, 0x9D, 0x0D, 0xE8, 0x0C, 0x52,
            0x58, 0xC7, 0xE6, 0x5F, 0xDB, 0x7A, 0xE7, 0x5D,
            0xC1, 0x6D, 0x94, 0x2C, 0x4F, 0x03, 0x6D, 0x34,
            0x17, 0x6C, 0xD1, 0xD9, 0x17, 0xF9, 0x06, 0x60,
            0x20, 0xED, 0xF3, 0x1C, 0xD8, 0x5E, 0x01, 0xB8,
            0xBA, 0x15, 0xBF, 0x4F, 0x5A, 0xE9, 0xF1, 0xE5,
            0x4F, 0xD8, 0xE7, 0x51, 0x28, 0x9C, 0x9A, 0x82,
            0x83, 0x74, 0xD0, 0x44, 0x52, 0x1B, 0xEB, 0x65,
            0x05, 0x68, 0x02, 0x1B, 0x01, 0xD9, 0xC8, 0x16,
            0x2B, 0x10, 0xEF, 0x5E, 0x01, 0xFE, 0xF5, 0x47,
            0x5C, 0x8C, 0xC4, 0xCE, 0x43, 0x6A, 0x44, 0xA1,
            0x42, 0x9E, 0x28, 0x8A, 0x56, 0x47, 0x20, 0x5C,
            0x7F, 0x48, 0x7B, 0xB0, 0xC2, 0xF5, 0x48, 0xA4,
            0xC9, 0xCB, 0x71, 0x5F, 0x60, 0x48, 0x22, 0xC6,
            0x56, 0x55, 0xDA, 0x1E, 0x9F, 0x42, 0x11, 0x1A,
            0xA1, 0x03, 0xEC, 0x62, 0x69, 0xFF, 0xDE, 0xBB,
            0xC7, 0x04, 0xF4, 0xAC, 0xAE, 0x61, 0xE1, 0xB5,
            0x71, 0x4C, 0x08, 0x72, 0x4C, 0xDA, 0x96, 0x01,*/
        },

        {
            0xAE, 0x96, 0xFC, 0xB4, 0x0F, 0x07, 0xBE, 0xD2,
            0x89, 0xEE, 0xEA, 0x36, 0x6A, 0x8A, 0xEE, 0xDC,
            0x06, 0xB7, 0x65, 0x26, 0x84, 0x4E, 0x6C, 0x57,
            0x7F, 0xA2, 0x33, 0x79, 0x66, 0xA1, 0xFF, 0xDE,
            0x0C, 0x96, 0x27, 0xC9, 0x42, 0x32, 0x38, 0xD1,
            0x8E, 0x7F, 0x0D, 0x9E, 0x8B, 0xBB, 0x6D, 0x5D,
            0x21, 0xFE, 0x21, 0x9A, 0x73, 0x39, 0x71, 0x96,
            0x7A, 0x05, 0x4B, 0x28, 0x80, 0x62, 0xFB, 0xBD,
            0x25, 0xC9, 0xD2, 0x3D, 0xAB, 0x83, 0x0D, 0x83,
            0x70, 0x51, 0x92, 0x9D, 0x62, 0x51, 0xBB, 0x76,
            0x75, 0x25, 0x9D, 0x80, 0x73, 0x3C, 0x07, 0xAF,
            0x06, 0xDD, 0x35, 0x48, 0x26, 0x02, 0x63, 0xD1,
            0xEA, 0xE7, 0x58, 0x0D, 0x02, 0xB2, 0xE9, 0xBA,
            0xE2, 0x4A, 0x5B, 0xE2, 0xA7, 0x4E, 0xB7, 0x55,
            0x7D, 0x4E, 0xA2, 0x9D, 0x0D, 0xE8, 0x0C, 0x52,
            0x58, 0xC7, 0xE6, 0x5F, 0xDB, 0x7A, 0xE7, 0x5D,
            0xC1, 0x6D, 0x94, 0x2C, 0x4F, 0x03, 0x6D, 0x34,
            0x17, 0x6C, 0xD1, 0xD9, 0x17, 0xF9, 0x06, 0x60,
            0x20, 0xED, 0xF3, 0x1C, 0xD8, 0x5E, 0x01, 0xB8,
            0xBA, 0x15, 0xBF, 0x4F, 0x5A, 0xE9, 0xF1, 0xE5,
            0x4F, 0xD8, 0xE7, 0x51, 0x28, 0x9C, 0x9A, 0x82,
            0x83, 0x74, 0xD0, 0x44, 0x52, 0x1B, 0xEB, 0x65,
            0x05, 0x68, 0x02, 0x1B, 0x01, 0xD9, 0xC8, 0x16,
            0x2B, 0x10, 0xEF, 0x5E, 0x01, 0xFE, 0xF5, 0x47,
            0x5C, 0x8C, 0xC4, 0xCE, 0x43, 0x6A, 0x44, 0xA1,
            0x42, 0x9E, 0x28, 0x8A, 0x56, 0x47, 0x20, 0x5C,
            0x7F, 0x48, 0x7B, 0xB0, 0xC2, 0xF5, 0x48, 0xA4,
            0xC9, 0xCB, 0x71, 0x5F, 0x60, 0x48, 0x22, 0xC6,
            0x56, 0x55, 0xDA, 0x1E, 0x9F, 0x42, 0x11, 0x1A,
            0xA1, 0x03, 0xEC, 0x62, 0x69, 0xFF, 0xDE, 0xBB,
            0xC7, 0x04, 0xF4, 0xAC, 0xAE, 0x61, 0xE1, 0xB5,
            0x71, 0x4C, 0x08, 0x72, 0x4C, 0xDA, 0x96, 0x01,
        },

        {
            0x44, 0x63, 0xD0, 0x5B, 0xA8, 0xE1, 0x68, 0x18,
            0x24, 0x2E, 0x83, 0xBF, 0xAE, 0x99, 0xD7, 0x3F,
            0x70, 0x03, 0x4F, 0x63, 0x10, 0x8A, 0x37, 0x88,
            0x49, 0x4D, 0xF7, 0xDA, 0x08, 0x11, 0x5C, 0x0A,
            0x14, 0x63, 0x67, 0x4B, 0x15, 0x69, 0x3B, 0x1D,
            0x03, 0x9D, 0x9F, 0x49, 0x7C, 0x14, 0x09, 0xCD,
            0x9E, 0x10, 0x4C, 0x80, 0xC0, 0x92, 0x43, 0x67,
            0x4A, 0x61, 0xD2, 0x8F, 0xBE, 0xA2, 0xB4, 0xA9,
            0x31, 0x2D, 0xE1, 0x3B, 0xA8, 0xA9, 0x5B, 0x6E,
            0x40, 0xD8, 0x41, 0x94, 0xA3, 0x48, 0x03, 0x80,
            0x1F, 0x69, 0x75, 0x10, 0xF3, 0x8D, 0x13, 0xC8,
            0x75, 0x72, 0x7C, 0xEA, 0x32, 0xC2, 0x90, 0xEA,
            0x87, 0x25, 0xC8, 0xF0, 0xE3, 0xF9, 0x32, 0x45,
            0x28, 0x01, 0x3B, 0x97, 0x8F, 0xE9, 0x30, 0xDB,
            0x89, 0x4B, 0x57, 0x3D, 0x00, 0x8D, 0x0A, 0xB6,
            0x65, 0xD2, 0xF2, 0x30, 0x80, 0x12, 0x1E, 0xE5,
            0xBA, 0xE7, 0x50, 0x63, 0xEA, 0xC3, 0xCF, 0x83,
            0xA9, 0x39, 0xF4, 0x93, 0x14, 0x9C, 0xA0, 0xBD,
            0x22, 0xC6, 0xBD, 0x22, 0x3D, 0xBA, 0xBC, 0xDF,
            0x75, 0xA3, 0x63, 0x18, 0xDD, 0xFB, 0x56, 0x02,
            0xCF, 0x2A, 0x5C, 0xE5, 0x35, 0x54, 0x4D, 0x34,
            0x5E, 0xEC, 0x90, 0xAA, 0x88, 0x95, 0xD9, 0x8A,
            0xA6, 0x91, 0x17, 0x33, 0xEB, 0xA4, 0xF1, 0xAB,
            0xA9, 0x15, 0xC4, 0x36, 0x16, 0xBC, 0xBD, 0x5F,
            0xE1, 0x54, 0xD8, 0x68, 0x83, 0xFE, 0x44, 0x19,
            0x62, 0x29, 0xF4, 0x81, 0xA2, 0x04, 0xE0, 0xE5,
            0x86, 0x4B, 0x78, 0x66, 0x91, 0x19, 0x65, 0xE7,
            0x5B, 0x0F, 0xA5, 0x73, 0xB3, 0xFD, 0x76, 0x59,
            0xEC, 0x62, 0x9C, 0xA2, 0xBC, 0xB6, 0x0D, 0x00,
            0x24, 0xC0, 0xD8, 0x57, 0x91, 0x4E, 0x54, 0x8D,
            0xAD, 0x22, 0x65, 0x30, 0xD3, 0x6D, 0xEF, 0x98,
            0x77, 0x80, 0xDF, 0xBD, 0x6F, 0x09, 0x73, 0xAB,
        },

    };
    for (i = 0; i < (int)(sizeof(tests) / sizeof(tests[0])); i++) {

		sec_memcpy(out, tests[i].dout, tests[i].data_len);
		sec_memcpy(key, tests[i].key, 256+4);
        tmp_len = tests[i].data_len;

		sprd_crypt_do(
			SPRD_CRYPT_RSA_ENC_2048,
			out, tests[i].data_len,
			in, &tmp_len,
			key, 256+4,
			NULL, 0);

		if (sec_memcmp(in, tests[i].din, tests[i].data_len) != 0) {
			return -1;
		}
	}
	return 0;
}
#endif
int sprd_crypt_test(void) {
    int err = 0;
    //*((volatile unsigned int*)(0x44000000)) = 0xFF;
#ifdef TEST_AES
    err = sprd_aes_cbc_test();
    if (err != 0) {
    //*((volatile unsigned int*)(0x44000000)) = 0xAAAAAAAA;
        ddr_print_string("aes cbc failed!\r\n");
    	goto failed;
    }
    //*((unsigned int*)(0x44000004)) = 1;
    ddr_print_string("sprd AES is OK\r\n");
#endif
#ifdef TEST_SHA
    err = sprd_sha256_test();
    if (err != 0) {
        //*((volatile unsigned int*)(0x44000000)) = 0xBBBBBBBB;
        ddr_print_string("sha256 failed\r\n");
	goto failed;
    }
    //*((volatile unsigned int*)(0x44000008)) = 2;
    ddr_print_string("sprd SHA is ok\r\n");
#endif
#ifdef TEST_RSA
    err = sprd_rsa_enc_test();
    if (err != 0) {
    	//*((volatile unsigned int*)(0x44000000)) = 0xCCCCCCCC;
        ddr_print_string("rsa failed\r\n");
	goto failed;
    }
    //*((volatile unsigned int*)(0x4400000c)) = 3;
    ddr_print_string("sprd RSA is OK\r\n");
#endif
    while(1);
failed:
    return err;
}
#endif
