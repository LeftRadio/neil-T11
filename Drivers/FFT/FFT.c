/**
  ******************************************************************************
  * @file	 	fft.с"
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "fft.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float fft_In[(1 << NS_P) * 2];
float fft_Out[(1 << NS_P) * 2];

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static __inline void fft_calc(float *in, float *out, float* Coeffs);
static __inline int fft_binary_inversion(int i);		// функция бинарной инверсии индекса
static __inline void fft_shift(float *data);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  функция расчёта поворотных множителей для БПФ
  * @param  None
  * @retval результирующий массив поворотных множителей c[1 << p]
  */
//void fft_make(float *c)
//{
//	uint16_t n, i;
//	float w, f;
//	n = 1 << NS_P; // размер массива (число точек БПФ)
//
//	w = (2. * M_PI) / (float) n;
//	f = 0.;
//
//	for (i = 0; i < n; i++)
//	{
//		*c++ =  cosf(f);
//		*c++ = -sinf(f);
//		f += w;
//	}
//}


/**
  * @brief  функция расчёта поворотных множителей для ОБПФ
  * @param  None
  * @retval результирующий массив поворотных множителей c[1 << p]
  */
//void fft_make_reverse(float *c) // результирующий массив поворотных множителей c[1 << p]
//{
//	int n, i;
//	float w, f;
//	n = 1 << NS_P; // размер массива (число точек ОБПФ)
//	w = (2. * M_PI) / (float) n;
//	f = 0.;
//	for (i = 0; i < n; i++)
//	{
//		*c++ = cosf(f);
//		*c++ = sinf(f);
//		f += w;
//	}
//}



/**
  * @brief  :
  * @param  :
  * @retval : None
  */
void fft_realMag_uint8_calc(uint8_t *In, uint8_t *Out, const float* coeffs)
{
//	uint16_t _NS = (1 << NS_P);
//	volatile float fft_In[(2 * _NS) + 1];
//	volatile float fft_Out[(2 * _NS) + 1];
//	volatile uint16_t i;
//
//	for(i = 0; i < _NS * 2; i += 2)
//	{
//		fft_In[i] = (float)(*In);
//		fft_In[i+1] = 0;
//		In++;
//	}
//
//	fft_calc((float*)fft_In, (float*)fft_Out, (float*)coeffs);
//
//	for (i = 0; i <= _NS; i += 2)
//	{
//		*Out = (uint8_t)(sqrtf((fft_Out[i] * fft_Out[i]) + (fft_Out[i + 1] * fft_Out[i + 1])) / 64.0);
//		Out++;
//	}
}


/**
  * @brief  :
  * @param  :
  * @retval : None
  */
void fft_realMag_int16_calc(int16_t *In, int16_t *Out, const float* coeffs)
{
	uint16_t ns = (1 << NS_P);
	volatile uint16_t i;

	/* Hamming window, 0.54 - 0.46 * (cos(2 * pi * n) / N);  */
	i = 0;
	do
	{
		fft_In[i] = (float)(*In);		// * ( 0.54F - (0.46F * (cosf(2.0F * (float)M_PI * (float)i) / (ns * 2))) );
		fft_In[i + 1] = 0;
		In++;
		i += 2;
	} while( i < (ns * 2));

	fft_calc((float*)fft_In, (float*)fft_Out, (float*)coeffs);

	i = 0;
	do
	{
		*Out = (int16_t)(sqrtf((fft_Out[i] * fft_Out[i]) + (fft_Out[i + 1] * fft_Out[i + 1])) / (float)ns);		//   / 64.0F
		Out++;
		i += 2;
	} while( i < ns );
}


/**
  * @brief  :
  * @param  :
  * @retval : None
  */
void fft_realMag_float_calc(float *In, float *Out, const float* coeffs)
{
//	uint16_t _NS = (1 << NS_P);
//	volatile float fft_In[(2 * _NS) + 1];
//	volatile float fft_Out[(2 * _NS) + 1];
//	volatile uint16_t i;
//
//	/* In */
//	do
//	{
//		fft_In[i] = *(In + i);
//		fft_In[i+1] = 0;
//		i += 2;
//	} while(i < _NS * 2);
//
//	/* --- Perform fft calculs --- */
//	fft_calc((float*)fft_In, (float*)fft_Out, (float*)coeffs);
//
//	/* Out */
//	i = 0;
//	do
//	{
//		*(Out + i) = sqrtf((fft_Out[i] * fft_Out[i]) + (fft_Out[i + 1] * fft_Out[i + 1])) / 64.0F;
//		i++;
//	} while(i <= _NS);
}


/**
  * @brief  функция прямого БПФ
  * @param  входной массив
  * @retval выходной массив
  */
static __inline void fft_calc(float *in, float *out, float* Coeffs)
{
	int i;
	int n = 1 << NS_P;  // число точек БПФ
	int n2 = n >> 1; // n/2
	float re, im, re1, im1, re2, im2; // c, c1, c2
	float *p1, *p2;
	const float *p0;

	// копировать элементы массива `in` в массив `out` с битовой инверсией
	for (i = 0; i < n; i++)
	{
		int j = fft_binary_inversion(i) << 1;
		int k = i << 1;

		p0 = in  + j;
		p1 = out + k;
		*p1++ = *p0++; // out[i] = in[j]
		*p1   = *p0;   //

		p0 = in  + k;
		p1 = out + j;
		*p1++ = *p0++; // out[j] = in[i]
		*p1   = *p0;   //
	}

	// выполнение бабочек ("понеслась душа в рай" (C) Hokum)
	for (i = 0; i < NS_P; i++)
	{
		int m = 1 << (i + 1); // через сколько эл-тов бабочка * 2
		int r = m << 1;       // размер группы * 2
		int nom = 0;          // номер группы * r
		int k = 0;            // номер эл-та в группе * 2
		int y = 0;            // индекс W * 2
		int z = 0;
		int h = 1 << (NS_P - i); // шаг для W * 2
		int j;

		for (j = n2; j > 0; j--)
		{
			if (k >= m)
			{
				k = y = 0;
				nom += r;
				z = nom;
			}

			// c <= c[y]
			p0 = Coeffs + y;
			re = *p0++;
			im = *p0;

			// c2 <= out[z + m]
			p1  = out + (z + m);
			re2 = *p1++;
			im2 = *p1;

			// c1 <= c2 * c
			re1 = re2 * re - im2 * im;
			im1 = im2 * re + re2 * im;

			// c2 <= out[z]
			p2  = out + z;
			re2 = *p2++;
			im2 = *p2;

			// out[z]     <= c2 + c1
			// out[z + m] <= c2 - c1
			*p2-- = im2 + im1;
			*p1-- = im2 - im1;
			*p2   = re2 + re1;
			*p1   = re2 - re1;

			k += 2;
			z += 2;
			y += h;
		}
	}
}


/**
  * @brief  функция перестановки отсчётов спектра (что бы "0" в центре)
  * @param  входной массив
  * @retval массив после БПФ
  */
static __inline void fft_shift(float *data)
{
	int n = 1 << NS_P; // число точек БПФ
	float *buf = (float*) malloc(sizeof(float) * 2 * n);
	if (buf == (float*) NULL) return;
	else
	{
		memcpy(buf, data, sizeof(float) * 2 * n);
		memcpy(data + n, buf, sizeof(float) * n);
		memcpy(data, buf + n, sizeof(float) * n);
		free((void*) buf);
	}
}


/**
  * @brief  функция бинарной инверсии индекса
  * @param  исходный индекс [0, (1 << NS_P) - 1]
  * @retval инверсный индекс
  */
static __inline int fft_binary_inversion(int i) //
{
	int j = 0;
	uint8_t p = NS_P;

	while (p-- > 0)
	{
		j <<= 1;
		j |= i & 1;
		i >>= 1;
	}
	return j;
}






/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
