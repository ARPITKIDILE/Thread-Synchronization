/*prg.c*/
/*  header file inclusion */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

/* matrix size and number of threads */
#define M 8
#define N 4

/* array declaration */
int A[M][M], B[M][M];

/* shared data */
/* global variables */
long unsigned int count=0;
int Z[M][M];

/* mutex and condition variable intialization */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

/* boolean number generator  with 50% probability of 0 and 1*/

int rand50 ()
{
/* rand() function will generate odd or even number with equal probability. If rand() generates odd number, the function willreturn 1 else it will return 0. */
  return rand () & 1;
}

/* number of 1 counter */
long unsigned int NoOf1 (int x, int y)
{
  int i, j;

  for (i = x; i < y; i++)
  {
      for (j = 0; j < M; j++)
    {
      if (A[i][j] == 1)
        {
          count = count + 1;
        }
    }

  }
  return count;
}

/* thread function */
void *threadsearch (void *args)
{
  pthread_mutex_lock (&lock);
  int i, j;
  long int number;
  static int x = 0;
  static int y = M / N;
  static int e = 2;
  printf ("worker threads\n");
  number = NoOf1 (x, y);
  printf ("the no of 1's : %lu\n", number);
  x = x + M / N;
  y = y + M / N;

  pthread_mutex_unlock (&lock);
}

/* multiplication function */
void multiplication (int x, int y, int e)
{

  int i, j, k, t,l,base=2;
  int power=1;
  for(l=1; l<=e; l++)
    {
        power = power * base;
    }
  for (t = 0; t <power; t++)
    {
      for (i = x; i < y; i++)
      {
      	 for (j = 0; j < M; j++)
        {
          B[i][j] = 0;
          for (k = 0; k < M; k++)
        {
          B[i][j] = B[i][j] + A[i][k] * A[k][j];
        }          
        if (B[i][j] > 0)
        {
          B[i][j] = 1;
        }      
        else
        {
          B[i][j] = 0;
        }          
  	   A[i][j] = B[i][j];
          Z[i][j] = B[i][j];
        }
    }
   }
  for (i = x; i < y; i++)
    {
      for (j = 0; j < M; j++)
      {
      printf ("%d\t", B[i][j]);
      }
      printf ("\n");
    }
}
/* thread function */
void *transitiveClosure(void *input)
{
  pthread_mutex_lock (&lock);
  int i, j;
  int a = *(int *) input;
  static int x = 0;
  static int y = M / N;
  printf ("worker threads2\n");
  multiplication (x, y, a);
  x = x + M / N;
  y = y + M / N;
  printf ("x=%d y=%d\n", x, y);
  pthread_mutex_unlock (&lock);
}

/* main function */
int main ()
{
  int i, j, done, ch, e;
  pthread_t thread[N], thread2[N];
  printf ("\nBOOLEAN MATRIX\n");
  for (i = 0; i < M; i++)
    {
      for (j = 0; j < M; j++)
      {
      A[i][j] = rand50 ();
      }
    }
  for (i = 0; i < M; i++)
    {
      for (j = 0; j < M; j++)
      {
      printf ("%d\t", A[i][j]);
      }
      printf ("\n");
    }
  printf ("\n");

    do
    {
      printf ("\nEnter the operation to be performed\n1.No of 1's\t 2.Transitive closure\t0.Terminate\n");
      scanf ("%d", &ch);
      printf ("\n");
    switch (ch)
    {
    case 1:
      {
        for (i = 0; i < N; i++)
          {
            pthread_create (&thread[i], NULL, threadsearch,(void *) NULL);
          }

        for (i = 0; i < N; i++)
          {
            pthread_join (thread[i], NULL);
          }

        printf("\nTOTAL NO. OF 1=%lu\n",count);
        break;
      }
    case 2:
      {
        printf ("Enter the value of e\n");
        scanf ("%d", &e);
        for (i = 0; i < N; i++)
        {
        pthread_create (&thread[i], NULL, transitiveClosure, (void *) &e);
          }

        for (i = 0; i < N; i++)
          {
            pthread_join (thread[i], NULL);
          }
          printf("\nThe transitive closure of matrix is\n");
          for (i = 0; i < M; i++)
          {
                for (j = 0; j < M; j++)
          {
         printf ("%d\t", B[i][j]);
          }
                  printf ("\n");
              }

        break;

      }
     }
    }while (ch != 0);

      pthread_mutex_destroy (&lock);
      return 0;
}
