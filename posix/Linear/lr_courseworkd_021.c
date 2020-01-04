#include <stdio.h>
#include <math.h>
#include <pthread.h>

/******************************************************************************
 * This program takes an initial estimate of m and c and finds the associated
 * rms error. It is then as a base to generate and evaluate 8 new estimates,
 * which are steps in different directions in m-c space. The best estimate is
 * then used as the base for another iteration of "generate and evaluate". This
 * continues until none of the new estimates are better than the base. This is
 * a gradient search for a minimum in mc-space.
 *
 * To compile:
 *   cc -o lr_courseworkd_021 lr_courseworkd_021.c -lm -pthread
 *
 * To run:
 *   ./lr_courseworke_021
 *
 * Dr Kevan Buckley, University of Wolverhampton, 2018
 *****************************************************************************/
 int i;
  double bm = 1.3;
  double bc = 10;
  double be;
  double dm[8];
  double dc[8];
  double e[8];
  double step = 0.01;
  double best_error = 999999999;
  int best_error_i;
  int minimum_found = 0;

  double om[] = {0,1,1, 1, 0,-1,-1,-1};
  double oc[] = {1,1,0,-1,-1,-1, 0, 1}; 

typedef struct point_t {
  double z;
  double f;
} point_t;

int n_data = 1000;
point_t data[];

double residual_error(double z, double f, double m, double c) {
  double e = (m * z) + c - f;
  return e * e;
}

double rms_error(double m, double c) {
  int i;
  double mean;
  double error_sum = 0;
  
  for(i=0; i<n_data; i++) {
    error_sum += residual_error(data[i].z, data[i].f, m, c);  
  }
  
  mean = error_sum / n_data;
  
  return sqrt(mean);
}

// Calculate the difference between two times

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
{
long long int ds =   finish->tv_sec - start->tv_sec; 
long long int dn =   finish->tv_nsec - start->tv_nsec; 
if(dn < 0 ) 
{
ds--;
dn += 1000000000; 
} 

*difference = ds * 1000000000 + dn;

return !(*difference > 0);
}

void *regression(void *args)
{
      int *r = args;
      int i = *r;
      printf("\n Thread no. %d", i);
      dm[i] = bm + (om[i] * step);
      dc[i] = bc + (oc[i] * step);    
   
      e[i] = rms_error(dm[i], dc[i]);
      if(e[i] < best_error) {
        best_error = e[i];
        best_error_i = i;
      
	}
}

int main() {
 pthread_t p_threads[8];

  struct timespec start, finish;   
  long long int time_elapsed;
  
  be = rms_error(bm, bc);

  clock_gettime(CLOCK_MONOTONIC, &start);

  

  while(!minimum_found) {
    for(i=0;i<8;i++) {
      pthread_create(&p_threads[i], NULL, (void*)regression, &i);
	
	pthread_join(p_threads[i], NULL);
    }
      

    printf("best m,c is %lf,%lf with error %lf in direction %d\n", 
      dm[best_error_i], dc[best_error_i], best_error, best_error_i);
    if(best_error < be) {
      be = best_error;
      bm = dm[best_error_i];
      bc = dc[best_error_i];
    } else {
      minimum_found = 1;
    }
  }
  printf("minimum m,c is %lf,%lf with error %lf\n", bm, bc, be);

  clock_gettime(CLOCK_MONOTONIC, &finish);
   time_difference(&start, &finish, &time_elapsed);
   printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
                                         (time_elapsed/1.0e9)); 

  return 0;
}
point_t data[] = {
  {73.36,102.82},{66.06,118.01},{82.76,132.16},{65.89,90.31},
  {72.17,103.11},{73.69,111.48},{77.36,112.67},{71.17,102.00},
  {79.02,119.87},{79.18,117.60},{68.11,101.62},{12.26,49.42},
  {37.47,65.48},{88.56,132.59},{82.09,119.57},{ 8.05,20.24},
  {50.29,85.08},{50.10,86.40},{27.43,38.50},{64.60,105.00},
  {23.45,44.54},{ 5.62,28.10},{36.72,74.88},{67.84,127.36},
  {45.69,54.43},{ 5.50,49.81},{26.39,50.67},{ 7.93,31.84},
  {97.77,142.30},{52.02,92.98},{23.17,57.12},{39.68,66.32},
  {85.14,127.72},{48.65,78.48},{ 9.81,44.52},{26.31,66.68},
  {24.51,52.53},{21.22,53.17},{64.74,112.23},{97.21,144.28},
  {20.93,37.25},{97.32,125.44},{ 4.07, 9.60},{89.43,153.50},
  { 3.02, 8.75},{33.27,51.92},{65.04,87.96},{92.91,134.17},
  {44.35,55.17},{79.34,101.47},{13.48,52.08},{54.10,90.03},
  {96.28,133.60},{49.96,67.53},{49.15,78.44},{20.10,51.23},
  {29.80,84.89},{94.12,143.10},{29.50,48.25},{15.02,26.26},
  {19.23,65.86},{60.55,101.77},{20.35,36.11},{76.77,105.08},
  { 1.51, 2.44},{39.56,65.53},{58.31,95.07},{24.68,44.45},
  {75.70,118.02},{73.23,98.11},{73.25,103.83},{42.73,71.01},
  {58.34,94.50},{38.20,77.22},{51.48,72.97},{40.47,60.03},
  {47.46,81.55},{ 4.20,40.39},{70.02,109.63},{87.89,127.68},
  {12.71,56.00},{ 4.68,17.64},{80.10,123.60},{25.52,65.22},
  {77.99,106.45},{24.79,67.24},{68.15,111.07},{96.66,129.97},
  {46.15,69.20},{ 0.05,18.56},{99.40,126.45},{43.51,48.33},
  {61.42,112.23},{94.45,131.21},{14.40,14.40},{11.08,33.78},
  {31.12,72.62},{85.25,127.99},{47.50,65.42},{35.13,60.48},
  {93.19,130.01},{87.56,132.17},{99.62,156.43},{37.29,53.50},
  {84.52,128.35},{38.44,69.97},{29.92,49.58},{66.92,98.94},
  {88.16,129.28},{74.42,134.04},{23.29,32.81},{76.28,128.97},
  { 7.86,28.02},{53.12,89.02},{86.07,120.97},{95.27,143.74},
  {90.99,126.63},{83.24,113.46},{25.62,39.20},{12.74,41.73},
  {74.90,112.16},{62.01,84.85},{79.60,115.12},{44.20,75.19},
  {13.89,49.88},{58.17,82.05},{33.49,60.49},{82.06,142.57},
  {57.99,81.95},{37.88,71.52},{84.26,113.63},{ 0.34,17.10},
  { 7.36,39.82},{53.61,80.96},{76.94,113.90},{58.50,104.85},
  {71.31,110.11},{27.32,47.06},{ 3.97,40.07},{ 2.46,23.53},
  {18.57,34.51},{85.18,134.45},{86.55,122.47},{84.65,139.69},
  {31.02,51.27},{34.92,57.43},{75.79,112.54},{66.07,105.57},
  {19.73,34.89},{17.05,37.42},{ 7.14,37.03},{47.62,82.24},
  {61.83,89.38},{14.93,59.84},{32.97,52.00},{ 1.93,38.87},
  {71.73,98.98},{77.19,115.10},{35.46,63.95},{ 8.15,34.40},
  {21.43,36.31},{88.15,123.82},{78.82,123.96},{62.89,80.88},
  {25.09,51.84},{15.08,56.73},{ 3.55, 7.59},{45.83,90.68},
  {99.57,137.90},{29.03,64.03},{52.39,86.36},{69.19,107.46},
  {34.44,58.79},{53.62,86.32},{21.32,41.52},{78.70,126.13},
  {37.37,42.70},{69.75,114.09},{13.12,26.77},{27.83,64.35},
  {70.64,103.45},{ 7.87,27.86},{99.81,131.28},{14.27,48.27},
  {67.11,108.12},{51.94,87.09},{42.79,61.96},{72.75,105.87},
  {25.52,56.75},{61.06,99.84},{55.35,82.00},{24.36,55.29},
  {25.92,50.67},{38.23,56.34},{71.57,109.31},{45.00,78.78},
  {90.13,115.60},{60.58,110.38},{98.59,132.94},{13.48,39.94},
  {74.51,103.03},{81.40,121.00},{12.21,39.24},{62.10,102.23},
  {67.12,107.28},{ 3.98,22.94},{35.53,69.36},{59.52,88.50},
  {79.37,113.27},{37.78,79.22},{94.34,140.45},{23.68,43.81},
  {65.63,106.65},{63.58,90.77},{11.01,23.22},{84.58,117.77},
  {38.13,64.59},{81.53,130.57},{59.28,91.84},{90.70,130.04},
  {33.28,56.89},{54.48,84.35},{45.73,84.33},{13.34,39.57},
  {95.76,139.75},{46.86,66.68},{29.51,58.21},{19.98,31.60},
  {48.71,93.33},{40.08,57.40},{16.44,42.14},{97.41,141.53},
  {50.94,81.23},{62.02,101.97},{41.78,82.13},{83.34,117.71},
  {62.83,106.73},{10.31,44.64},{82.00,118.07},{59.07,81.51},
  {42.21,63.30},{16.01,35.85},{25.89,50.78},{ 9.32,32.12},
  { 4.84,45.36},{90.66,123.62},{98.07,155.17},{60.65,110.18},
  {47.38,78.81},{21.96,49.62},{32.57,72.22},{50.65,89.25},
  {72.61,92.25},{42.16,72.36},{68.47,100.29},{59.06,83.01},
  {57.59,92.94},{11.61,42.90},{80.40,123.91},{30.00,61.66},
  {89.39,133.90},{25.25,51.12},{55.95,76.05},{13.11,28.47},
  {62.65,92.37},{63.68,91.48},{32.59,62.38},{80.95,134.50},
  {48.89,89.70},{80.92,102.62},{ 4.69,44.09},{23.27,53.20},
  {75.96,108.66},{46.45,80.13},{10.06,29.89},{30.24,50.93},
  {65.62,82.45},{71.75,100.13},{81.95,120.01},{35.69,54.53},
  {23.58,44.75},{98.54,139.98},{93.37,129.29},{17.07,38.95},
  {11.90,32.31},{18.76,41.75},{42.37,75.18},{94.91,128.84},
  {16.80,32.49},{67.47,112.89},{66.47,116.19},{37.16,40.88},
  {80.53,101.62},{10.32,19.98},{ 3.80,21.73},{73.27,105.70},
  {78.63,112.46},{77.27,126.23},{58.11,103.81},{40.55,70.29},
  {17.09,46.69},{99.59,136.39},{33.69,71.35},{25.47,62.80},
  {86.34,117.67},{78.95,124.61},{37.03,60.67},{59.43,91.46},
  {71.02,103.64},{22.94,38.43},{ 1.35,26.16},{67.76,74.59},
  {46.30,69.11},{24.62,70.01},{79.46,134.58},{ 2.51,28.74},
  {41.27,51.27},{69.95,96.97},{45.69,48.05},{78.54,115.65},
  {56.44,103.00},{12.17,38.25},{46.38,73.48},{84.10,112.86},
  {85.79,118.81},{51.49,82.33},{42.56,67.96},{15.83,37.96},
  {62.05,114.32},{98.73,164.21},{33.32,62.70},{72.00,105.60},
  {83.63,132.81},{99.60,131.86},{27.36,64.32},{55.23,88.19},
  {25.24,59.65},{61.35,88.90},{84.38,136.84},{ 3.29,29.77},
  { 7.60,33.46},{35.69,68.45},{72.18,115.15},{74.96,116.61},
  {67.29,112.17},{93.14,127.57},{38.33,61.08},{37.85,78.52},
  {50.37,73.41},{63.67,106.98},{ 9.11,39.99},{64.63,95.67},
  {22.52,41.58},{44.79,87.35},{57.05,93.10},{ 9.97,40.10},
  {34.87,50.45},{ 1.12,10.15},{49.76,99.49},{85.50,120.77},
  { 1.93,39.27},{68.44,110.73},{13.40,32.05},{34.07,56.12},
  {51.31,79.19},{29.60,55.42},{32.70,57.33},{86.50,104.39},
  {55.60,81.12},{43.80,73.24},{75.47,103.32},{42.63,61.06},
  {55.36,88.79},{18.07,54.10},{45.44,78.38},{62.47,73.65},
  {32.46,56.16},{ 8.35,47.34},{73.17,99.07},{10.41,35.99},
  {58.55,91.87},{44.20,69.65},{83.75,112.69},{86.50,143.89},
  {89.40,139.15},{28.44,47.16},{82.91,132.92},{61.17,109.64},
  {96.91,141.97},{ 9.68,37.20},{ 4.21,26.59},{88.03,143.78},
  {58.83,87.06},{25.08,46.90},{98.78,153.77},{55.99,70.21},
  { 1.10,19.36},{43.60,74.82},{35.70,82.70},{95.96,145.87},
  {90.61,119.30},{15.17,40.94},{42.08,58.42},{28.82,62.43},
  {93.95,134.46},{97.05,155.02},{87.82,129.50},{50.86,89.56},
  {21.59,43.65},{82.12,124.39},{62.16,84.58},{23.33,43.69},
  {98.58,125.82},{95.79,125.24},{68.98,98.79},{64.58,100.15},
  {11.55,38.50},{17.34,27.53},{85.86,114.23},{34.88,23.88},
  {37.60,54.95},{76.96,96.88},{41.37,62.39},{69.13,127.05},
  {91.47,116.46},{87.63,132.71},{34.26,68.02},{68.46,104.21},
  {61.30,99.65},{83.90,119.99},{23.58,41.47},{16.10,34.63},
  {49.22,75.10},{61.51,95.83},{73.89,111.14},{ 2.30,25.38},
  {21.66,50.29},{68.66,116.52},{21.08,35.55},{51.98,88.61},
  {37.65,58.50},{87.23,147.60},{66.20,96.70},{ 8.26,40.80},
  {93.34,151.55},{58.89,97.82},{84.19,116.27},{ 1.12,23.84},
  {86.38,89.15},{62.55,103.79},{73.05,99.38},{30.95,30.08},
  {96.00,145.96},{ 8.88,27.36},{35.91,54.17},{78.87,117.01},
  {30.72,65.67},{59.94,99.63},{94.92,132.73},{43.93,74.79},
  {80.58,103.06},{61.06,106.13},{16.51,41.73},{ 1.47, 7.87},
  {14.17,21.41},{40.99,66.07},{36.02,59.95},{14.33,35.31},
  {91.35,132.89},{ 0.16,29.96},{59.31,82.50},{ 7.70,30.65},
  {75.68,106.11},{29.12,57.24},{53.31,73.66},{55.42,98.09},
  {63.28,86.78},{63.76,119.81},{65.70,110.95},{79.63,120.48},
  {91.62,140.18},{10.84,55.03},{35.64,74.46},{42.92,97.53},
  {79.40,100.73},{28.15,54.02},{99.51,126.02},{62.25,101.89},
  {20.76,39.50},{10.03,43.97},{10.23,31.80},{65.64,104.41},
  {99.97,147.91},{52.09,102.00},{29.71,43.92},{ 4.03,24.96},
  {68.28,99.97},{11.92,31.02},{24.39,54.70},{21.78,43.28},
  {29.68,49.30},{56.77,86.72},{22.90,43.75},{93.63,125.56},
  {42.37,71.52},{ 6.55,20.37},{82.96,114.20},{22.13,36.44},
  {44.26,80.87},{ 7.47,29.15},{47.84,60.31},{ 6.79,33.94},
  {40.36,78.12},{94.12,123.75},{89.04,132.74},{36.50,58.33},
  {37.23,76.54},{11.54,43.31},{26.05,57.22},{86.85,130.49},
  { 5.24,36.17},{21.11,49.39},{ 6.08,31.64},{87.28,102.21},
  {39.13,72.90},{46.00,73.78},{81.21,112.27},{ 2.72,24.94},
  {18.56,39.86},{41.82,67.80},{64.02,88.90},{ 4.38,23.77},
  {23.58,35.78},{75.22,126.15},{18.46,47.91},{59.09,86.91},
  {41.20,68.29},{13.12,37.81},{42.29,74.76},{51.27,68.80},
  {64.75,116.16},{49.37,81.76},{93.89,123.38},{51.71,81.62},
  {77.78,121.73},{24.66,43.98},{29.63,63.78},{95.59,135.81},
  {46.73,91.61},{27.69,57.99},{84.74,135.78},{65.86,102.23},
  { 7.06,26.16},{61.17,104.28},{20.38,29.98},{57.47,90.06},
  {94.71,136.45},{90.03,124.77},{59.06,87.93},{44.61,75.89},
  {33.44,73.03},{37.68,56.66},{ 6.52,30.27},{35.13,59.15},
  { 3.47,13.38},{68.90,105.98},{54.03,81.28},{84.32,110.23},
  {51.99,74.26},{43.11,99.62},{72.26,126.65},{41.00,71.47},
  {46.45,69.83},{19.97,48.50},{ 9.51,22.97},{27.62,54.74},
  {98.61,157.02},{86.97,137.74},{42.16,70.26},{78.03,119.62},
  {20.45,34.66},{67.21,91.07},{ 8.97,35.25},{99.49,132.36},
  {85.16,116.22},{58.19,84.49},{37.70,68.95},{70.84,112.46},
  {57.39,101.09},{45.64,72.96},{44.39,75.08},{44.93,88.27},
  {16.30,35.81},{85.54,126.35},{30.12,63.73},{61.05,88.51},
  {69.15,103.29},{47.85,72.88},{13.34,38.69},{62.18,106.34},
  {69.63,102.48},{17.67,47.72},{58.46,89.32},{33.35,73.81},
  {23.30,47.03},{32.87,86.41},{18.74,27.93},{92.10,126.62},
  {27.50,41.93},{95.45,137.23},{11.96,40.54},{57.15,78.21},
  {40.08,71.36},{ 4.75,11.32},{27.21,68.49},{80.44,132.76},
  {27.00,64.27},{22.21,79.62},{91.86,136.38},{89.32,142.03},
  {42.95,87.42},{80.74,124.29},{14.47,34.21},{34.48,67.00},
  {81.41,149.44},{90.59,116.51},{80.49,92.80},{94.57,147.78},
  {80.21,115.32},{92.00,125.48},{64.42,94.43},{96.35,140.76},
  { 0.46,33.35},{41.29,80.44},{96.47,129.13},{44.62,80.21},
  {22.88,25.86},{25.14,54.75},{92.36,126.77},{99.99,137.55},
  {82.75,98.09},{52.46,62.03},{91.80,138.97},{26.46,63.47},
  {93.51,133.09},{14.56,55.23},{11.03,44.27},{35.09,64.06},
  {90.60,130.74},{54.54,93.24},{18.43,46.91},{99.21,140.47},
  {70.29,88.59},{54.22,87.55},{15.43,35.00},{15.26,34.13},
  {59.03,98.46},{ 2.27,35.20},{ 0.75,11.33},{21.57,31.19},
  {13.92,36.31},{18.52,31.37},{67.89,108.15},{82.92,132.69},
  {54.63,73.24},{ 1.68,46.57},{30.86,65.28},{38.85,88.49},
  {90.89,120.59},{67.00,77.89},{87.75,115.00},{27.87,55.65},
  {37.53,62.11},{19.55,27.67},{ 6.63,38.49},{23.86,45.95},
  {19.09,50.18},{43.38,75.23},{ 6.80,13.15},{29.31,54.48},
  {49.82,73.13},{ 7.85,37.58},{22.48,51.11},{75.54,116.63},
  {33.93,64.07},{12.85,41.72},{13.05,40.70},{30.44,37.80},
  { 8.19,11.89},{60.69,117.59},{69.30,109.77},{15.98,54.21},
  {69.71,100.02},{71.18,102.27},{84.95,134.70},{22.84,51.67},
  {76.53,126.67},{55.53,79.98},{30.95,63.50},{69.68,103.26},
  {38.72,72.91},{ 0.60,10.49},{58.42,90.41},{56.71,92.53},
  { 3.54,42.83},{12.59,31.55},{25.57,59.25},{68.34,109.47},
  { 2.54,31.62},{10.11,45.46},{26.45,61.98},{29.52,55.47},
  {24.09,51.15},{63.86,80.81},{74.20,133.53},{75.01,118.47},
  {65.56,93.25},{85.28,121.51},{71.80,98.64},{14.78,40.31},
  {71.58,104.81},{40.34,94.57},{41.26,62.68},{48.57,75.76},
  {81.23,132.80},{15.28,41.55},{99.54,154.44},{87.17,132.74},
  {40.46,88.69},{62.15,116.42},{28.74,44.98},{30.13,57.25},
  {61.89,100.32},{77.57,113.00},{14.25,18.09},{65.76,101.93},
  {29.85,45.39},{85.18,127.01},{70.49,101.53},{79.82,106.64},
  {43.96,71.71},{54.68,81.92},{97.64,138.40},{72.72,122.97},
  {25.52,22.33},{32.29,58.32},{ 0.96,20.17},{55.06,99.22},
  {25.84,53.73},{10.49,56.67},{30.16,62.62},{58.37,87.55},
  {40.21,70.89},{24.25,54.42},{36.40,50.95},{92.33,136.48},
  {15.43,42.87},{72.40,111.38},{56.00,85.90},{44.80,85.96},
  {14.41,31.65},{ 1.96,15.52},{93.24,126.98},{55.67,83.60},
  {51.62,88.14},{64.37,86.91},{29.84,59.92},{72.24,99.11},
  {81.49,113.74},{93.86,154.88},{85.65,132.06},{91.33,128.29},
  {58.78,107.60},{33.09,79.07},{52.35,66.51},{24.47,40.19},
  {40.82,75.63},{22.82,49.44},{77.43,124.91},{93.49,130.09},
  {93.15,134.18},{ 2.36,30.56},{ 2.60,13.99},{42.20,62.35},
  {96.31,126.36},{91.69,121.80},{41.35,80.21},{66.34,89.23},
  {83.45,109.55},{30.46,56.24},{54.28,95.58},{72.07,102.63},
  {71.49,104.41},{60.67,83.30},{19.14,53.48},{33.74,48.87},
  { 1.85,20.34},{30.82,66.88},{ 9.92,33.53},{21.63,32.69},
  {43.72,76.95},{97.13,131.87},{61.74,101.18},{26.20,70.09},
  {55.38,86.89},{46.82,68.76},{10.43,32.38},{63.35,113.08},
  {33.60,77.18},{94.58,143.78},{70.73,110.82},{72.76,105.71},
  {10.50,22.40},{46.73,60.17},{64.01,93.80},{80.47,125.45},
  {81.81,125.98},{80.03,136.86},{46.10,67.97},{67.17,108.05},
  {37.90,65.81},{19.03,57.35},{67.23,92.72},{47.93,80.14},
  {73.59,101.99},{ 3.40,32.74},{71.44,112.60},{33.09,51.87},
  { 2.08,27.85},{30.39,49.08},{ 1.18,16.21},{69.70,115.09},
  {40.93,76.23},{18.02,38.08},{50.28,77.58},{ 4.08,30.45},
  {69.69,125.56},{32.36,68.65},{98.73,136.81},{57.32,91.11},
  {84.37,127.82},{80.38,121.73},{82.17,109.60},{15.49,30.24},
  {78.67,123.99},{14.29,66.29},{26.61,51.60},{10.39,17.71},
  {66.50,104.75},{68.92,115.60},{78.67,113.40},{58.77,81.37},
  {19.57,50.07},{60.80,104.77},{34.97,63.06},{ 3.11,15.27},
  {49.44,78.52},{89.94,136.74},{86.96,111.62},{27.45,64.82},
  {72.19,121.39},{17.09,54.79},{43.38,71.91},{30.07,54.79},
  {96.42,134.12},{68.23,92.71},{22.85,63.25},{ 3.61,17.43},
  {43.09,70.08},{18.37,45.69},{39.90,81.71},{14.52,48.03},
  {96.08,131.54},{93.23,130.73},{32.33,69.52},{45.79,78.42},
  {77.47,110.68},{66.63,117.56},{39.67,83.71},{33.01,48.46},
  {58.61,99.09},{11.50,33.64},{78.49,117.62},{97.05,142.43},
  {73.32,124.52},{10.56,27.04},{69.33,102.91},{63.34,100.38},
  { 2.03,23.26},{35.49,64.64},{66.36,88.94},{32.46,56.88},
  {12.51,32.28},{79.14,108.70},{67.79,105.54},{79.54,116.04},
  {15.34,36.67},{79.76,121.74},{70.61,103.29},{10.66,43.81},
  {64.37,118.39},{69.99,97.77},{10.75,31.03},{ 7.47,30.55},
  {51.33,79.49},{71.50,120.45},{52.72,91.59},{72.03,123.62},
  {13.65,39.37},{59.36,82.79},{10.10,41.62},{69.96,101.38},
  {12.06,44.46},{97.62,127.00},{79.09,99.99},{19.73,50.21},
  {50.85,81.04},{40.13,61.30},{95.27,124.77},{30.90,62.42},
  { 6.30,17.52},{46.36,82.66},{72.92,108.28},{85.11,113.23},
  { 8.86,37.51},{50.52,80.50},{70.80,108.22},{26.49,48.10},
  {46.95,83.86},{95.89,132.49},{22.82,34.71},{79.62,126.04},
  {23.28,56.07},{42.27,81.15},{58.12,96.54},{88.50,133.06},
  {56.78,90.03},{83.69,111.21},{71.24,99.63},{31.86,48.70},
  {16.79,40.12},{90.49,130.08},{28.45,64.61},{47.24,94.87},
  {36.42,68.76},{15.81,40.48},{ 3.57,13.30},{42.53,84.95},
  {30.39,57.17},{57.45,80.32},{ 7.89,42.93},{89.15,121.87},
  { 3.94,28.55},{40.98,60.65},{ 1.76,23.92},{14.77,50.19},
  {52.47,81.59},{38.62,64.83},{21.49,66.77},{31.25,65.17},
  { 1.09,27.47},{57.10,90.20},{51.24,77.45},{ 8.02,27.75},
  {40.82,71.86},{95.88,133.37},{ 6.01,47.92},{86.57,116.76},
  {28.84,55.89},{94.02,151.44},{90.05,123.63},{35.57,68.17},
  {29.93,44.81},{18.71,28.51},{86.08,116.42},{48.49,78.97},
  { 4.98,25.48},{79.98,121.58},{ 1.49,16.92},{67.39,104.96},
  { 9.06,35.95},{59.89,85.66},{70.98,107.76},{50.83,77.40},
  {36.75,77.66},{29.82,62.49},{ 2.77,15.50},{46.65,73.77},
  {92.27,143.64},{35.35,57.38},{45.28,68.20},{72.73,108.20},
  {27.21,53.92},{58.84,92.29},{99.92,138.13},{77.64,113.61},
  {67.47,108.17},{10.95,20.86},{85.48,110.64},{11.00,45.11}
};
