#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __APPLE__
    #include <sys/uio.h>
#else
    #include <sys/io.h>
#endif

#include <sys/mman.h>
#include <assert.h>
#include <gsl/gsl_sf_legendre.h>
#include "cosmo.h"

#define EUCLID_EMULATOR_VERSION_MAJOR 1
#define EUCLID_EMULATOR_VERSION_MINOR 1

// Struct for return type of EucEmu()
struct FID{
    double *handle;
    int size;
};

// Actual emulator function
//void EucEmu(double *CosmoParams, double z);
//void EucEmu(double *CosmoParams, double z, double **kVals, int *nkVals, double **Boost, int *nBoost);
void EucEmu(double *CosmoParams, double *Redshifts, int len_z, double **kVals, int *nkVals, double **Boost, int *nBoost);
