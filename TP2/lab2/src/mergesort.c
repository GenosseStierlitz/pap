#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include<omp.h>

#include <x86intrin.h>

#define NBEXPERIMENTS   7

static long long unsigned int experiments [NBEXPERIMENTS] ;

static   unsigned int N ;

typedef  int  *array_int ;

static array_int X ;

void init_array (array_int T)
{
  register int i ;

  for (i = 0 ; i < N ; i++)
    {
      T [i] = N - i ;
    }
}

void print_array (array_int T)
{
  register int i ;

  for (i = 0 ; i < N ; i++)
    {
      printf ("%d ", T[i]) ;
    }
  printf ("\n") ;
}

int is_sorted (array_int T)
{
  register int i ;

print_array(T);

  for (i = 1 ; i < N ; i++)
    {
        /* test designed specifically for our usecase */
        if (T[i-1] +1  != T [i] )
            return 0 ;
    }
  return 1 ;
}

long long unsigned int average (long long unsigned int *exps)
{
  unsigned int i ;
  long long unsigned int s = 0 ;

  for (i = 2; i < (NBEXPERIMENTS-2); i++)
    {
      s = s + exps [i] ;
    }

  return s / (NBEXPERIMENTS-2) ;
}


void merge(int T[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrs */
    int L[n1], R[n2];

    /* Copy data to temp arrs L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = T[l + i];
    for (j = 0; j < n2; j++)
        R[j] = T[m + 1+ j];

    /* Merge the temp arrs back into T[l..r]*/
    i = 0; // Initial index of first subarr
    j = 0; // Initial index of second subarr
    k = l; // Initial index of merged subarr
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            T[k] = L[i];
            i++;
        }
        else
        {
            T[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        T[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        T[k] = R[j];
        j++;
        k++;
    }
}



void merge_sort (int *T, const int size)
{
    /* TODO: sequential version of the merge sort algorithm */
    int l, r;
    l=0;
    r=size-1;
     if (l < r)
     {

         int m = l+(r-l)/2;

         // the first and second halves will be sorted here
         merge_sort(T, size/2);
         merge_sort(T+size/2, size-(size/2));

         merge(T, l, m, r);
     }
}


void parallel_merge_sort (int *T, const int size)
{
    /* TODO: sequential version of the merge sort algorithm */
}


int main (int argc, char **argv)
{
  unsigned long long int start, end, residu ;
  unsigned long long int av ;
  unsigned int exp ;

    if (argc != 2)
    {
      fprintf (stderr, "mergesort N \n") ;
      exit (-1) ;
    }

  N = 1 << (atoi(argv[1])) ;
  X = (int *) malloc (N * sizeof(int)) ;

  printf("--> Sorting an array of size %u\n",N);

  start = _rdtsc () ;
  end   = _rdtsc () ;
  residu = end - start ;



  printf("sequential sorting ...\n");


    for (exp = 0 ; exp < NBEXPERIMENTS; exp++)
    {
      init_array (X) ;

      print_array (X) ;
      start = _rdtsc () ;

               merge_sort (X, N) ;

      end = _rdtsc () ;
      experiments [exp] = end - start ;

      if (! is_sorted (X))
	{
            fprintf(stderr, "ERROR: the array is not properly sorted\n") ;
            exit (-1) ;
	}
    }

  av = average (experiments) ;
  printf ("\n merge sort serial\t\t %Ld cycles\n\n", av-residu) ;

  printf("parallel sorting ...\n");

  for (exp = 0 ; exp < NBEXPERIMENTS; exp++)
    {
      init_array (X) ;

      start = _rdtsc () ;

           parallel_merge_sort (X, N) ;

      end = _rdtsc () ;
      experiments [exp] = end - start ;

      if (! is_sorted (X))
	{
            fprintf(stderr, "ERROR: the array is not properly sorted\n") ;
            print_array(X);
            exit (-1) ;
	}
    }

  av = average (experiments) ;
  printf ("\n merge sort parallel with tasks\t %Ld cycles\n\n", av-residu) ;


}
