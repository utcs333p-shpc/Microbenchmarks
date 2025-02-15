/* This is a one-off microbenchmark for attempts to dissect
 * Zhaoxin's KX-6640MA (LuJiaZui) architecture
 */
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cpuid.h>
#include <pthread.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

// make mingw happy for cross compiling
#ifdef __MINGW32__
#define aligned_alloc(align, size) _aligned_malloc(size, align)
#endif

extern uint64_t noptest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t noptest1b(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t clktest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t clkmovtest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t addtest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t addnoptest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t addmovtest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t leatest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t leamultest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t rortest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t shltest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t rorbtstest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixrormultest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixrorshltest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t btstest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t btsmultest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t addmultest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t jmpmultest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t jmptest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t ntjmptest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixadd256int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixadd256int11(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixadd256fpint(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mix256fp(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mix256fp11(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latadd512int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latadd256int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latadd128int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latadd256fp(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latmul128int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latmul256int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latmul512int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latmulq512int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latmuldq512int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latmul256fp(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latadd128fp(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latmul128fp(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latfma512(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latfma256(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latfma128(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t add128int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t add256int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t add512int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mul512int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t muldq512int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mul128int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t add128fp(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mul128fp(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t fma512(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixfma256fma512(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mix21fma256fma512(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t fma256(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t fma128(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixfmafadd256(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixfmaadd256(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixfmaadd512(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixfma512add256(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixfmaand256(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixfmaandmem256(uint64_t iterations, float *arr) __attribute((sysv_abi));
extern uint64_t mixfmaaddmem256(uint64_t iterations, float *arr) __attribute((sysv_abi));
extern uint64_t nemesfpumix21(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t nemesfpu512mix21(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mul256fp(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t add256fp(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latmul64(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t latmul16(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mul16(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mul64(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t load128(uint64_t iterations, int *arr) __attribute((sysv_abi));
extern uint64_t spacedload128(uint64_t iterations, int *arr) __attribute((sysv_abi));
extern uint64_t loadscalar(uint64_t iterations, int *arr) __attribute((sysv_abi));
extern uint64_t load256(uint64_t iterations, float *arr) __attribute((sysv_abi));
extern uint64_t load512(uint64_t iterations, float *arr) __attribute((sysv_abi));
extern uint64_t store128(uint64_t iterations, int *arr, int *sink) __attribute((sysv_abi));
extern uint64_t store256(uint64_t iterations, float *arr, float *sink) __attribute((sysv_abi));
extern uint64_t store512(uint64_t iterations, float *arr, float *sink) __attribute((sysv_abi));
extern uint64_t mixaddmul128int(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixmul16mul64(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mixmul16mul64_21(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t pdeptest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t pdepmultest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t pexttest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t indepmovtest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t vecindepmovtest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t depmovtest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t vecdepmovtest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t xorzerotest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t vecxorzerotest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t movzerotest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t subzerotest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t vecsubzerotest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t depinctest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t depdectest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t depaddimmtest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t spacedstorescalar(uint64_t iterations, int *arr) __attribute((sysv_abi));
extern uint64_t aesenc128(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t aesdec128(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t aesencfadd128(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t aesencadd128(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t aesencfma128(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t aesencmul128(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t mix256faddintadd(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t movqtoxmmtest(uint64_t iterations) __attribute((sysv_abi));

extern uint64_t fma4_256(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t fma4_128(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t fdivtest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t fdivlattest(uint64_t iterations) __attribute((sysv_abi));
extern uint64_t fmuldenormtest(uint64_t iterations) __attribute((sysv_abi)); 
extern uint64_t fmuldenormlattest(uint64_t iterations) __attribute((sysv_abi)); 

float fpTestArr[8] __attribute__ ((aligned (64))) = { 0.2, 1.5, 2.7, 3.14, 5.16, 6.3, 7.7, 9.45 };
float fpSinkArr[8] __attribute__ ((aligned (64))) = { 2.1, 3.2, 4.3, 5.4, 6.2, 7.8, 8.3, 9.4 };
int *intTestArr;
int intSinkArr[8] __attribute__ ((aligned (64))) = { 2, 3, 4, 5, 6, 7, 8, 9 };

uint64_t load128wrapper(uint64_t iterations) __attribute((sysv_abi));
uint64_t loadscalarwrapper(uint64_t iterations) __attribute((sysv_abi));
uint64_t spacedload128wrapper(uint64_t iterations) __attribute((sysv_abi));
uint64_t spacedstorescalarwrapper(uint64_t iterations) __attribute((sysv_abi));
uint64_t load256wrapper(uint64_t iterations) __attribute((sysv_abi));
uint64_t load512wrapper(uint64_t iterations) __attribute((sysv_abi));
uint64_t store128wrapper(uint64_t iterations) __attribute((sysv_abi));
uint64_t store256wrapper(uint64_t iterations) __attribute((sysv_abi));
uint64_t store512wrapper(uint64_t iterations) __attribute((sysv_abi));
uint64_t mixfmaandmem256wrapper(uint64_t iterations)  __attribute((sysv_abi));
uint64_t mixfmaaddmem256wrapper(uint64_t iterations)  __attribute((sysv_abi));

float measureFunction(uint64_t iterations, float clockSpeedGhz, __attribute((sysv_abi)) uint64_t (*testfunc)(uint64_t));

int threads = 0;

int main(int argc, char *argv[]) {
  struct timeval startTv, endTv;
  struct timezone startTz, endTz;
  uint64_t iterations = 1500000000;
  uint64_t iterationsHigh = iterations * 5;
  uint64_t time_diff_ms;
  float latency, opsPerNs, clockSpeedGhz;
  uint64_t intTestArrLength = 1024;
  int avxSupported = 0, avx2Supported = 0, bmi2Supported = 0, avx512Supported = 0;
  int fmaSupported = 0, fma4Supported = 0;
  char *testName = NULL;

  if (argc > 1) {
      for (int argIdx = 1; argIdx < argc; argIdx++) {
          if (*(argv[argIdx]) == '-') {
              char *arg = argv[argIdx] + 1;
              if (strncmp(arg, "threads", 7) == 0) {
                  argIdx++;
                  threads = atoi(argv[argIdx]);
                  fprintf(stderr, "Multithreading mode, %d threads\n", threads);
              } else if (strncmp(arg, "iter", 4) == 0) {
                  argIdx++;
                  int iterMul = atoi(argv[argIdx]);
                  iterations *= iterMul;
                  iterationsHigh *= iterMul;
                  fprintf(stderr, "Scaled iterations by %d\n", iterMul);
              } else if (strncmp(arg, "test", 4) == 0) {
                  argIdx++;
                  testName = argv[argIdx];
                  fprintf(stderr, "Only running test %s\n", testName);
              }
          }
      }
  }

  intTestArr = aligned_alloc(64, sizeof(int) * intTestArrLength);
  for (uint64_t i = 0; i < intTestArrLength; i++) {
    intTestArr[i] = i;
  }

  if (__builtin_cpu_supports("avx")) {
    fprintf(stderr, "avx supported\n");
    avxSupported = 1;
  }

  if (__builtin_cpu_supports("avx2")) {
    fprintf(stderr, "avx2 supported\n");
    avx2Supported = 1;
  }

  if (__builtin_cpu_supports("bmi2")) {
    fprintf(stderr, "bmi2 supported\n");
    bmi2Supported = 1;
  }

  if (__builtin_cpu_supports("fma")) {
      fprintf(stderr, "fma3 supported\n");
      fmaSupported = 1;
  }

  if (__builtin_cpu_supports("fma4")) {
      fprintf(stderr, "fma4 supported\n");
      fma4Supported = 1;
  }

  uint32_t cpuidEax, cpuidEbx, cpuidEcx, cpuidEdx;
  __cpuid_count(7, 0, cpuidEax, cpuidEbx, cpuidEcx, cpuidEdx);
  if (cpuidEbx & (1UL << 16)) {
      fprintf(stderr, "AVX512 supported\n");
      avx512Supported = 1;
  }

  // figure out clock speed
  gettimeofday(&startTv, &startTz);
  clktest(iterationsHigh);
  gettimeofday(&endTv, &endTz);
  time_diff_ms = 1000 * (endTv.tv_sec - startTv.tv_sec) + ((endTv.tv_usec - startTv.tv_usec) / 1000);
  latency = 1e6 * (float)time_diff_ms / (float)iterationsHigh;
  // clk speed should be 1/latency, assuming we got one add per clk, roughly
  clockSpeedGhz = 1/latency;

  printf("Estimated clock speed: %.2f GHz\n", clockSpeedGhz);

  // avx-512 testing
  if (avx512Supported) {
    if (testName == NULL || argc > 1 && strncmp(argv[1], "fma512", 6) == 0)
      printf("512-bit FMA per clk: %.2f\n", measureFunction(iterations, clockSpeedGhz, fma512));
    if (testName == NULL || argc > 1 && strncmp(argv[1], "latfma512", 9) == 0)
      printf("512-bit FMA latency: %.2f clocks\n", 1 / measureFunction(iterations, clockSpeedGhz, latfma512));
    if (testName == NULL || argc > 1 && strncmp(argv[1], "load512", 7) == 0)    
      printf("512-bit loads per clk: %.2f\n", measureFunction(iterations, clockSpeedGhz, load512wrapper));
    if (testName == NULL || argc > 1 && strncmp(argv[1], "store512", 7) == 0)   
      printf("512-bit stores per clk: %.2f\n", measureFunction(iterations, clockSpeedGhz, store512wrapper));

  }

  if (fmaSupported) {
      if (avx2Supported && (testName == NULL || argc > 1 && strncmp(argv[1], "fma256", 6) == 0))
          printf("256-bit FMA per clk: %.2f\n", measureFunction(iterations, clockSpeedGhz, fma256));
      if (avx2Supported && (testName == NULL || argc > 1 && strncmp(argv[1], "latfma256", 9) == 0))
          printf("256-bit FMA latency: %.2f clocks\n", 1 / measureFunction(iterations, clockSpeedGhz, latfma256));
  }

  if (avxSupported && (testName == NULL || argc > 1 && strncmp(argv[1], "load256", 7) == 0))
    printf("256-bit loads per clk: %.2f\n", measureFunction(iterations, clockSpeedGhz, load256wrapper));
  if (avxSupported && (testName == NULL || argc > 1 && strncmp(argv[1], "store256", 7) == 0))
    printf("256-bit stores per clk: %.2f\n", measureFunction(iterations, clockSpeedGhz, store256wrapper));
  return 0;
}

struct TestThreadData {
    uint64_t iterations;
    uint64_t (*testfunc)(uint64_t);
};

void *TestThread(void *param) {
    struct TestThreadData *testData = (struct TestThreadData *)param;
    testData->testfunc(testData->iterations);
    return NULL;
}

float measureFunction(uint64_t iterations, float clockSpeedGhz,  __attribute((sysv_abi)) uint64_t (*testfunc)(uint64_t)) {
  struct timeval startTv, endTv;
  struct timezone startTz, endTz;
  uint64_t time_diff_ms, retval;
  float latency, opsPerNs;

  gettimeofday(&startTv, &startTz);
  if (threads == 0) retval = testfunc(iterations);
  else {
      pthread_t *testThreads = (pthread_t *)malloc(threads * sizeof(pthread_t));
      struct TestThreadData *testData = (struct TestThreadData*)malloc(threads * sizeof(struct TestThreadData));
      for (int threadIdx = 0; threadIdx < threads; threadIdx++) {
          testData[threadIdx].iterations = iterations;
          testData[threadIdx].testfunc = testfunc;
          pthread_create(testThreads + threadIdx, NULL, TestThread, testData + threadIdx);
      }

      for (int threadIdx = 0; threadIdx < threads; threadIdx++) {
          pthread_join(testThreads[threadIdx], NULL);
      }

      free(testThreads);
      free(testData);
  }
  gettimeofday(&endTv, &endTz);
  time_diff_ms = 1000 * (endTv.tv_sec - startTv.tv_sec) + ((endTv.tv_usec - startTv.tv_usec) / 1000);
  latency = 1e6 * (float)time_diff_ms / (float)iterations;
  opsPerNs = 1/latency;
  //printf("%f adds/ns, %f adds/clk?\n", opsPerNs, opsPerNs / clockSpeedGhz);
  //printf("return value: %lu\n", retval);
  return opsPerNs / clockSpeedGhz;
}

__attribute((sysv_abi)) uint64_t load128wrapper(uint64_t iterations) {
  return load128(iterations, intTestArr);
}

__attribute((sysv_abi)) uint64_t spacedload128wrapper(uint64_t iterations) {
  return spacedload128(iterations, intTestArr);
}

__attribute((sysv_abi)) uint64_t spacedstorescalarwrapper(uint64_t iterations) {
  return spacedstorescalar(iterations, intTestArr);
}

__attribute((sysv_abi)) uint64_t load256wrapper(uint64_t iterations) {
  return load256(iterations, fpTestArr);
}

__attribute((sysv_abi)) uint64_t loadscalarwrapper(uint64_t iterations) {
  return loadscalar(iterations, intTestArr);
}

__attribute((sysv_abi)) uint64_t load512wrapper(uint64_t iterations) {
  return load512(iterations, fpTestArr);
}

__attribute((sysv_abi)) uint64_t spacedload256wrapper(uint64_t iterations) {
  return spacedload128(iterations, intTestArr);
}

__attribute((sysv_abi)) uint64_t store128wrapper(uint64_t iterations) {
  return store128(iterations, intTestArr, intSinkArr);
}

__attribute((sysv_abi)) uint64_t store256wrapper(uint64_t iterations) {
  return store256(iterations, fpTestArr, fpSinkArr);
}

__attribute((sysv_abi)) uint64_t store512wrapper(uint64_t iterations) {
  return store512(iterations, fpTestArr, fpSinkArr);
}

__attribute((sysv_abi)) uint64_t mixfmaandmem256wrapper(uint64_t iterations) {
  return mixfmaandmem256(iterations, fpTestArr);
}

__attribute((sysv_abi)) uint64_t mixfmaaddmem256wrapper(uint64_t iterations) {
  return mixfmaaddmem256(iterations, fpTestArr);
}
