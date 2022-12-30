
/**********************************************************************************
 * Arquivo	: random.h                                                            *
 * Criacao  : 1997                                                                *
 * Autor    : Makoto Matsumoto and Takuji Nishimura                               *
 * Descricao: Definicao de funcoes para geracao de numeros aleatorios.            *
 **********************************************************************************/

#ifndef __random_h__
#define __random_h__


#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

void init_genrand(unsigned long s);
void init_by_array(unsigned long init_key[], int key_length);
unsigned long genrand_int32(void);
long genrand_int31(void);
double genrand_real1(void);
double genrand_real2(void);
double genrand_real3(void);
double genrand_res53(void);
int xrandomRange (unsigned short int min, unsigned   short int max);



#endif
