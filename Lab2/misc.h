#ifndef MISC_H
#define MISC_H

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define SBOOL(a) ((a) ? "true" : "false")

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define nothing -1
#define undefined -1
#define null 0
#define nullptr 0
#define nullstr ""
#define nullchr '\0'
#define true 1
#define false 0

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned long long ullong;
typedef long long llong;
typedef long double ldouble;

/*
 * Type <functionname>_s(...)
 * safe wrapper functions for alternative handling error exit
 */
void free_s(void *d);
void* malloc_s(int sz);
void* realloc_s(void *d, int sz);

int fork_s();
int pipe_s(int fd[2]);
int stat_s(const char *path, struct stat *pstat);

/*
 * Type* <typename>_s(int N)
 * safely allocates a list of N types
 */
uchar* uchar_s(int N);
uint* uint_s(int N);
ulong* ulong_s(int N);
ullong* ullong_s(int N);
llong* llong_s(int N);
char* char_s(int N);
int* int_s(int N);
long* long_s(int N);
double* double_s(int N);
float* float_s(int N);
ldouble* ldouble_s(int N);

/*
 * Type* <typename>2_s(Type* d, int N)
 * safely reallocates a list of types to N types 
 */
uchar* uchar2_s(uchar *d, int N);
uint* uint2_s(uint *d, int N);
ulong* ulong2_s(ulong *d, int N);
ullong* ullong2_s(ullong *d, int N);
llong* llong2_s(llong *d, int N);
char* char2_s(char *d, int N);
int* int2_s(int *d, int N);
long* long2_s(long *d, int N);
double* double2_s(double *d, int N);
float* float2_s(float *d, int N);
ldouble* ldouble2_s(ldouble *d, int N);

/*
 * Type** p<typename>_s(int N)
 * safely allocates a list of N pointer types
 */
uchar** puchar_s(int N);
uint** puint_s(int N);
ulong** pulong_s(int N);
ullong** pullong_s(int N);
llong** pllong_s(int N);
char** pchar_s(int N);
int** pint_s(int N);
long** plong_s(int N);
double** pdouble_s(int N);
float** pfloat_s(int N);
ldouble** pldouble_s(int N);

/*
 * Type** p<typename>2_s(Type** d, int N)
 * safely reallocates a list of pointer types to N pointer types 
 */
uchar** puchar2_s(uchar **d, int N);
uint** puint2_s(uint **d, int N);
ulong** pulong2_s(ulong **d, int N);
ullong** pullong2_s(ullong **d, int N);
llong** pllong2_s(llong **d, int N);
char** pchar2_s(char **d, int N);
int** pint2_s(int **d, int N);
long** plong2_s(long **d, int N);
double** pdouble2_s(double **d, int N);
float** pfloat2_s(float **d, int N);
ldouble** pldouble2_s(ldouble **d, int N);

/*
 * memory manipulation funcions given known byte size,
 * internaly changes the pointers to char* (bytes) and back 
 * to void pointer on return
 *
 * set : initializes the array to v bytes
 * unset : initializes the array to null bytes
 * rm : free pointer and return nullptr
 * mk : makes pointer to of byte size
 * rs : resizes a pointer to new byte size
 * cp : copies bytes size into other pointer
 * mv : moves bytes size into other pointer
 * eq : compares bytes size of pointers
 * len : computes size of null termiated bytes pointer
 * checksum : bytes data given size
 */
void* set(void *d, char v, int sz);
void* unset(void *d, int sz);
void* rm(void *d);
void* mk(int sz);
void* rs(void *d, int sz);
void* cp(void *lhs, const void *rhs, int sz);
void* mv(void *lhs, void **prhs, int sz);
void* cat(void *lhs, const void *rhs, int szl, int szr);
void* catd(void *lhs, const void *rhs, const void *dilm, int szl, int szr, int szd);
int eq(const void* lhs, const void *rhs, int sz);
int len(const void *d);
ullong checksum(const void *d, off_t sz);

/*
 * memory manipulation funcions given null terminated bytes 
 * pointer, internaly changes the pointers to char* (bytes)  
 * and back to void pointer on return
 *
 * same as previous but now stops at the first null termination
 */
void* set0(void *d, char v);
void* unset0(void *d);
void* mk0(int sz);
void* rs0(void *d, int sz);
void* cp0(void *lhs, const void *rhs);
void* mv0(void *lhs, void **prhs);
int eq0(const void* lhs, const void *rhs);
int eq02(const void* lhs, const void *rhs, int *pN);
ullong checksum0(const void *d);

/*
 * memory manipulation funcions given null terminated bytes 
 * pointer, internaly changes the pointers to char* (bytes)  
 * and back to void pointer on return
 *
 * same as previous but now ensures the size of the right hand
 * size using len(rhs) using resize.
 */
void* cp0len(void *lhs, const void *rhs);
void* mv0len(void *lhs, void **prhs);
void* cat0len(void *lhs, const void *rhs);
void* catd0len(void *lhs, const void *rhs, const void *dilm);

/*
 * same as previous but allows you to request the byte size
 * of the resulting bytes pointer.
 */
void* cp0len2(void *lhs, const void *rhs, int *pN);
void* mv0len2(void *lhs, void **prhs, int *pN);
void* cat0len2(void *lhs, const void *rhs, int *pN);
void* catd0len2(void *lhs, const void *rhs, const void *dilm, int *pN);

/* implementation of the functions */
#include "misc.incl"

#endif