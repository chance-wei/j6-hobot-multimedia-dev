/*
 * Copyright 2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef TE600_ENGINE_H
# define TE600_ENGINE_H

#include "logging.h"

extern const char *te600_engine_id;

#ifdef DEBUG
#define Log_Debug(...) 			\
	do{							\
    	pr_debug(__VA_ARGS__);	\
   		usleep(30000);			\
	}while (0);
#else
#define Log_Debug
#endif

#define Log_Error(...) 			\
	do{							\
    	pr_err(__VA_ARGS__);	\
   		usleep(30000);			\
	}while (0);

# if defined(__GNUC__) && __GNUC__ >= 4 && \
     (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
#  pragma GCC diagnostic ignored "-Wvariadic-macros"
# endif

# ifdef ALG_DEBUG
#  define ALG_DGB(x, ...) fprintf(stderr, "ALG_DBG: " x, __VA_ARGS__)
#  define ALG_INFO(x, ...) fprintf(stderr, "ALG_INFO: " x, __VA_ARGS__)
#  define ALG_WARN(x, ...) fprintf(stderr, "ALG_WARN: " x, __VA_ARGS__)
# else
#  define ALG_DGB(x, ...)
#  define ALG_INFO(x, ...)
#  define ALG_WARN(x, ...)
# endif

# define ALG_ERR(x, ...) fprintf(stderr, "ALG_ERR: " x, __VA_ARGS__)
# define ALG_PERR(x, ...) \
                do { \
                    fprintf(stderr, "ALG_PERR: " x, __VA_ARGS__); \
                    perror(NULL); \
                } while(0)
# define ALG_PWARN(x, ...) \
                do { \
                    fprintf(stderr, "ALG_PERR: " x, __VA_ARGS__); \
                    perror(NULL); \
                } while(0)

typedef struct evp_cipher_handles_st {
    int key_size;
    EVP_CIPHER *_hidden;
} evp_cipher_handles;

typedef struct evp_dgst_handles_st {
	EVP_MD *_hidden;
} evp_dgst_handles;

/*
 * MAGIC Number to identify correct initialisation
 * of afalg_ctx.
 */
# define MAGIC_INIT_NUM 0x1890671

struct te600_ctx_st {
	struct kcapi_handle *handle;
	int init_done;
};

typedef struct te600_ctx_st te600_ctx;

void ENGINE_load_te600(void);

#endif
