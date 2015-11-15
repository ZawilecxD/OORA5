#include<papi.h>
#include <stdbool.h>

int eventSet = PAPI_NULL;
bool papi_supported = true;
int papi_err;
long long values[4] = {0,};
long long L2_TCM_values[5] = {0,};
long long L3_TCM_values[5] = {0,};
long long L1_DCM_values[5] = {0,};
long long STL_ICY_values[5] = {0,};
long long REF_CYC_values[5] = {0,};

void papi_init(){
  long events[5] = {PAPI_L2_TCM, PAPI_L3_TCM, PAPI_L1_DCM, PAPI_STL_ICY};
  int i;

  if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
    fprintf(stderr, "PAPI unsupported.\n");
    papi_supported = false;
  } else if (PAPI_num_counters() < 4) {
    fprintf(stderr, "PAPI unsupported.\n");
    papi_supported = false;
  }

  if ((papi_err = PAPI_create_eventset(&eventSet)) != PAPI_OK) {
    fprintf(stderr, "Could not create event set: %s\n", PAPI_strerror(papi_err));
  }

  for (i=0; i<4; ++i) {
    if ((papi_err = PAPI_add_event(eventSet, events[i])) != PAPI_OK ) {
      fprintf(stderr, "Could not add event: %s\n", PAPI_strerror(papi_err));
    }
  }

}

void papi_start(){
  if (papi_supported) {
    if ((papi_err = PAPI_start(eventSet)) != PAPI_OK) {
      fprintf(stderr, "Error while starting counters: %s\n", PAPI_strerror(papi_err));
    }
  }
}

void papi_stop(int i){
  if ((papi_err = PAPI_stop(eventSet, values)) != PAPI_OK) {
    fprintf(stderr, "Error while getting values: %s\n", PAPI_strerror(papi_err));
  }

  L2_TCM_values[i] = values[0];
  L3_TCM_values[i] = values[1];
  L1_DCM_values[i] = values[2];
  STL_ICY_values[i] = values[3];
}

void print_papi_results(){
  printf("Performance counters, factorization stage: \n");
  printf("L2 TCM:\n");
  int i;
  for(i=0;i<5;i++){
    printf("%ld\n", L2_TCM_values[i]);
  }
  printf("L3 TCM\n");
  for(i=0;i<5;i++){
    printf("%ld\n", L3_TCM_values[i]);
  }
  printf("L1 DCM\n");
  for(i=0;i<5;i++){
    printf("%ld\n", L1_DCM_values[i]);
  }
  printf("STL ICY\n");
  for(i=0;i<5;i++){
    printf("%ld\n", STL_ICY_values[i]);
  }
}
