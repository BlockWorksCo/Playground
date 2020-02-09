/**
 * Configuration for the wolfSSL library.
 */
#ifndef _WOLFSSL_CONFIG_H_
#define _WOLFSSL_CONFIG_H_

/* Define to 1 if you have the `alarm' function. */
#define HAVE_ALARM 0

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 0

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 0

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 0

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 0

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 0

/* Define to 1 if you have the `inet_ntoa' function. */
#define HAVE_INET_NTOA 0

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 0

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
 to 0 otherwise. */
#define HAVE_MALLOC 0

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 0

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 0

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 0

/* Define if you have POSIX threads libraries and header files. */
#define HAVE_PTHREAD 0

/* Define to 1 if your system has a GNU libc compatible `realloc' function,
 and to 0 otherwise. */
#define HAVE_REALLOC 0

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 0

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 0

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 0

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 0

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 0

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 0

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 0

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 0

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 0

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 0

/* Define to 1 or 0, depending whether the compiler supports simple visibility
 declarations. */
#define HAVE_VISIBILITY 0

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 0

/* We don't have a C library system time implementation. */
#define XTIME(t) (0)
#define XGMTIME have_no_gmtime

/* A certificate is always considered valid since it's only checked during
 * login before there is a proper knowledge about the clock
 */
#define HAVE_VALIDATE_DATE
#define XVALIDATE_DATE(d, f, t) (1)
#define NO_TIME_H


#define NO_WRITEV 1

#define NO_RABBIT 1
#define NO_DES 1
#define NO_DES3 1
#define NO_RC4 1
#define NO_WOLFSSL_DIR 1
#define NO_FILESYSTEM 1

#ifdef CONFIG_DTLS_PSK
#define WOLFSSL_STATIC_PSK 1
#else
#define NO_PSK 1
#endif

#ifndef CONFIG_DTLS_PKI
#define NO_CERTS 1
#endif

#define NO_RSA 1
#define NO_OLD_TLS 1
#define NO_DH 1

#ifdef CONFIG_DTLS_PKI
/* Support the elliptic curves and TLS extensions in DTLS */
#define HAVE_SUPPORTED_CURVES 1
#define HAVE_TLS_EXTENSIONS 1
#define HAVE_ECC 1
#define ECC_TIMING_RESISTANT 1

// Support of trusted_ca_keys extensions is a patched functionality
// (no native support in wolfSSL library)
#define HAVE_TLSX_TRUSTED_CA_KEYS 1

// Only use the curves that we actually use, secp256r1, which is enabled by
// default and must be turned of with NO_ECC256 if that one isn't wanted.
#define ECC_USER_CURVES

// CSR and new key generation
#define WOLFSSL_KEY_GEN 1

#endif // CONFIG_DTLS_PKI

#define HAVE_AESCCM 1

#if defined(CONFIG_DEVICE_SECURE) || defined(CONFIG_DEVICE_DLMS) || defined(CONFIG_UNITTESTS)

#define WOLFSSL_AES_DIRECT 1

#endif

#if defined(CONFIG_DEVICE_FORTEC) || defined(CONFIG_DEVICE_DLMS) || defined(CONFIG_UNITTESTS)

#define HAVE_AESGCM 1

#endif

// Enable/disable encrypted authentication (NULL cipher suite)
#ifndef CONFIG_DTLS_AUTH_ENCRYPT
#define HAVE_NULL_CIPHER 1
#endif

#ifdef CONFIG_DTLS_PERSISTENT_CACHE
#define PERSIST_SESSION_CACHE 1
#endif

#if defined(CONFIG_SMIP) || defined(CONFIG_OS_POSIX)
// Small stack support results in hundreds of 2KiB and 4KiB mallocs in
// integer.c, which the SMIP port is too slow to handle gracefully.
#define NO_WOLFSSL_SMALL_STACK
#else
#define WOLFSSL_LOW_MEMORY
#endif

#define NO_HANDSHAKE_DONE_CB 1
#define WOLFSSL_MAX_SUITE_SZ 16
#define USER_TICKS 1
#define SMALL_SESSION_CACHE 1
#define WOLFSSL_DTLS 1
#define WOLFSSL_USER_IO 1
#define SINGLE_THREADED 1
#define WOLFSSL_AES_COUNTER 1
#define NO_PWDBASED 1

#ifndef CONFIG_TARGET_HOST
#define NO_ERROR_STRINGS 1
#endif

#define CUSTOM_RAND_GENERATE hwrngGetSeed

#define NO_DEV_RANDOM 1

// Don't define RNG, use new WC_RNG instead
#define NO_OLD_RNGNAME 1

#if !defined(CONFIG_HOST_TOOLS) && !defined(CONFIG_UNITTESTS)
#define NO_WOLFSSL_SERVER 1
#endif

#ifdef CONFIG_STM32
// Not building with STM32F2_HASH, because there is no locking in the SHA and
// MD5 code. It's non-trivial and scary to implement it, because the whole
// hash subsystem would have to stay locked while there is an Md5 context
// alive, which is potentially several seconds (e.g. checksumming a file).
#define WOLFSSL_STM32F2 1
#define NO_STM32_HASH 1

// The following is defined again by wolfssl/wolfcrypt/settings.h, so undefine
// them here to avoid errors
#undef NO_DEV_RANDOM
#undef NO_WOLFSSL_DIR
#undef NO_OLD_RNGNAME
#endif

// Enable logging from WolfSSL
//#define DEBUG_WOLFSSL 1

#endif /* _WOLFSSL_CONFIG_H_ */
