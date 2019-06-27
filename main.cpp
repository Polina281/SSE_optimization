#include <iostream>
#include<cstdio>
#include <cstdlib> 
#include<cmath>
#include<ctime>
#include<malloc.h>
#include<xmmintrin.h>

using namespace std;

void computeArrayCpp(float* pX, float* pY, float* pZ, float* pResult, int nSize) //вычисляет
{
	int i;
	
	for ( i = 0; i<nSize; i++ ) 
	{
		float x = *pX;
		float y = *pY;
		float z = *pZ;
		
		*pResult = (float) 2.03 * (y*y*y) - (3.14*z) + (x / z);
		
		pX++;
		pY++;
		pZ++;
		pResult++;
	}
}

void computeArrayCppSSE(float* pX, float* pY, float* pZ, float* pResult, int nSize)
{
	int nLoop = nSize / (sizeof(__m128)/sizeof(float));
	
	__m128 m3;
	__m128 m1;
	__m128 m2;
	__m128 m4;
	__m128 m5;
	
	//Преобразуем указатели на float к типу __m128
	__m128* pSrcx = (__m128*) pX;
	__m128* pSrcy = (__m128*) pY;
	__m128* pSrcz = (__m128*) pZ;
	__m128* pDest = (__m128*) pResult;
	
	__m128 m203 = _mm_set_ps1(2.03f);
	__m128 m314 = _mm_set_ps1(-3.14f);

	for ( int i = 0; i < nLoop; i++ )
   {
	    m3 = _mm_mul_ps(*pSrcy, *pSrcy);
	    m3 = _mm_mul_ps(*pSrcy, m3);  // y в кубе
	    m1 = _mm_mul_ps(*pSrcz, m314); //-3,14*z
	    m2 = _mm_div_ps(*pSrcx, *pSrcz); //x/z
	    m4 = _mm_add_ps(m3, m1);
	    m5 = _mm_add_ps(m4, m2);
	    *pDest = _mm_mul_ps(m203, m5);
	    
		pSrcx++;
		pSrcy++;
		pSrcz++;
		pDest++;               
	}
}

void initRandomArray(float* a, int size)
{
	for(int i = 0; i < size; i++)
	{
		float x = (float)rand() / RAND_MAX;
		float notZeroX = (x == 0.) ? 0.1 : x;
		a[i] = notZeroX * 100;
	}
}

void printArray(float* a, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << a[i] << " ";
	}
	
	cout << endl;
}

int main(int argc, char** argv) {
	srand(static_cast<unsigned int>(clock()));
	
	setlocale(LC_ALL, "Russian");
	
	const int MAX_SIZE = 10000000;
	
	float *x = (float*)_aligned_malloc(sizeof(float)*MAX_SIZE, 16);
	float *y = (float*)_aligned_malloc(sizeof(float)*MAX_SIZE, 16);
	float *z = (float*)_aligned_malloc(sizeof(float)*MAX_SIZE, 16);	
	float *fxyz = (float*)_aligned_malloc(sizeof(float)*MAX_SIZE, 16);
	
	int startTime, endTime;

	//Получаем время начала работы
	startTime = clock();
	initRandomArray(x, MAX_SIZE);
	initRandomArray(y, MAX_SIZE);
	initRandomArray(z, MAX_SIZE);
	endTime = clock();
//	printArray(x, MAX_SIZE);
	printf("Инициализация массивов: %d мс\n", endTime - startTime);

	startTime = clock();		
	computeArrayCpp(x, y, z, fxyz, MAX_SIZE);
//	printArray(fxyz, MAX_SIZE);
	endTime = clock();
	printf("Вычисление средствами C++: %d мс\n", endTime - startTime);
	
	startTime = clock();
	computeArrayCppSSE(x, y, z, fxyz, MAX_SIZE);
//	printArray(fxyz, MAX_SIZE);
	endTime = clock();
	printf("Вычисление средствами SSE: %d мс\n", endTime - startTime);

	_aligned_free(x);
    _aligned_free(y);
    _aligned_free(z);
    _aligned_free(fxyz);
    
	return 0;
}
