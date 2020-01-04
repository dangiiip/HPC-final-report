#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
/****************************************************************************
  This program gives an example of a poor way to implement a password cracker
  in CUDA C. It is poor because it acheives this with just one thread, which
  is obviously not good given the scale of parallelism available to CUDA
  programs.
 
  The intentions of this program are:
    1) Demonstrate the use of __device__ and __gloaal__ functions
    2) Enable a simulation of password cracking in the absence of liarary
       with equivalent functionality to libcrypt. The password to be found
       is hardcoded into a function called is_a_match.   

  Compile and run with:
  nvcc -o password_Dangi password_crack_Dangi.cu


     To Run:
     ./password_Dangi > resultpwd_cuda_Dangi.txt

  Dr Kevan auckley, University of Wolverhampton, 2018
*****************************************************************************/
__device__ int is_a_match(char *attempt) {
  char Dangi_password1[] = "PD6067";
  char Dangi_password2[] = "CR2056";
  char Dangi_password3[] = "KN1972";
  char Dangi_password4[] = "IC2019";

  char *c = attempt;
  char *r = attempt;
  char *k = attempt;
  char *n = attempt;
  char *k1 = Dangi_password1;
  char *k2 = Dangi_password2;
  char *k3 = Dangi_password3;
  char *k4 = Dangi_password4;

  while(*c == *k1) {
   if(*c == '\0')
    {
    printf("Password: %s\n",Dangi_password1);
      break;
    }

    c++;
    k1++;
  }
    
  while(*r == *k2) {
   if(*r == '\0')
    {
    printf("Password: %s\n",Dangi_password2);
      break;
}

    r++;
    k2++;
  }

  while(*k == *k3) {
   if(*k == '\0')
    {
    printf("Password: %s\n",Dangi_password3);
      break;
    }

    k++;
    k3++;
  }

  while(*n == *k4) {
   if(*n == '\0')
    {
    printf("Password: %s\n",Dangi_password4);
      return 1;
    }

    n++;
    k4++;
  }
  return 0;

}
__global__ void  kernel() {
char N,a,v,s;
 
  char password[7];
  password[6] = '\0';

int i = blockIdx.x+65;
int j = threadIdx.x+65;
char firstValue = i;
char secondValue = j;
    
password[0] = firstValue;
password[1] = secondValue;
    for(N='0'; N<='9'; N++){
      for(a='0'; a<='9'; a++){
        for(v='0';v<='9';v++){
          for(s='0';s<='9';s++){
            password[2] = N;
            password[3] = a;
            password[4]= v;
            password[5]=s;
          if(is_a_match(password)) {
        //printf("Success");
          }
             else {
         //printf("tried: %s\n", password);          
            }
          }
        } 
      }
   }
}
int time_difference(struct timespec *start,
                    struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec;
  long long int dn =  finish->tv_nsec - start->tv_nsec;

  if(dn < 0 ) {
    ds--;
    dn += 1000000000;
  }
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


int main() {

  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

kernel <<<26,26>>>();
  cudaDeviceSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));
  return 0;
}



