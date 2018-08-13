#include <stdio.h>
#include <stdlib.h>
#include <EuclidEmulator.h>

int main(int argc, char* argv[]){

    fprintf(stderr, "# EuclidEmulator Version %d.%d\n",
            EUCLID_EMULATOR_VERSION_MAJOR,
            EUCLID_EMULATOR_VERSION_MINOR);

    // Check number of input arguments: 8 arguments are expected:
    // 1 executable name + 6 cosmological parameter + 1 redshift value
    assert(argc>=8);

    // Convert input arguments into appropriate format
    double omega_b = atof(argv[1]);
    double omega_m = atof(argv[2]);
    double n_s = atof(argv[3]);
    double h = atof(argv[4]);
    double w_0 = atof(argv[5]);
    double sigma_8 = atof(argv[6]);
    double* zvec;

    double cosmo_params[6] = {omega_b, omega_m, n_s, h, w_0, sigma_8};
    
    int nz = argc-7; 
    zvec = malloc( nz*sizeof(double) );
    int counter;
    for(counter = 0; counter < nz; counter++){
        zvec[counter] = atof(argv[7+counter]);
        assert(zvec[counter]<=5.0); 
    }
 
    // The radiation density is fixed by the CMB temperature (which is 
    // fixed as well for our purposes).
    const double omega_rad = 4.183709411969527e-5; /* corresponds to 2.755 K Tcmb */


    fprintf( stderr, "#\n# Cosmology:\n");
    fprintf( stderr, "#\tdOmega0: %.8f\n", omega_m/(h*h));
    fprintf( stderr, "#\tdOmegaRad: %.8f\n", omega_rad/(h*h));
    fprintf( stderr, "#\tdOmegaDE: %.8f\n\n", 1.-omega_m/(h*h)-omega_rad/(h*h));

    // Prepare data containers for return values of emulator function
    double* k_values;      // will contain the k modes at which the 
                            // emulator is evaluated
    int length_k_values;    // will contain the number of k modes

    double* boost;         // will contain the boost factor values 
                            // for the k modes in k_values
    int length_boost;       // will contain the number of boost values

    /* REMARK 1:
    ** We declare these data containers and pass them to the EucEmu function
    ** This allows us to be ignorant about the length of the arrays returned
    ** by EucEmu. We pass a pointer to a pointer to the first element of the
    ** two arrays k_values and boost. This allows for modification of the 
    ** pointers pointing directly to the array entries (because the pointer
    ** passed to and returned by the function EucEmu is not altered). Inside
    ** EucEmu the intermediate pointer iterates over all array cells filling
    ** the array with the corresponding values.
    ** Similarly, as we assign a values to length_k_values and length_boost
    ** only inside EucEmu, we modify the content of these two variables inside
    ** the function. This is only allowed by passing by reference which is 
    ** exactly what is done.
    */

    /* REMARK 2: 
    ** length_k_values and length_boost are supposed to be equal. We return
    ** them from the EucEmu function for sanity checking
    */

    // Call the emulator
    EucEmu(&cosmo_params[0], &zvec[0], nz, &k_values, &length_k_values, &boost, &length_boost);

    assert(length_k_values == length_boost);
    
    int j;
    for(j=0; j<nz*length_k_values; j++){
        if(j%length_k_values==0) printf("Next redshift\n");
        printf("%f\t%f\n", k_values[j%length_k_values], boost[j]);        
    }

    fprintf(stderr, "Done!\n");

    return 0;
}