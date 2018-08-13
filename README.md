# EuclidEmulator
This repository contains the main source code of the EuclidEmulator, a fast and accurate tool to estimate the non-linear correction to the matter power spectrum.

Authors:   M. Knabenhans & J. Stadel<br/>
Date:      May 2018<br/>
Reference: Knabenhans et al., 2018, arXiv pre-print (submitted)<br/>

If you use EuclidEmulator in any way for a publication, please cite this paper.

STAY TUNED: A python wrapper for EuclidEmulator is currently being developped. It will facilitate the use of EuclidEmulator a lot. 

## Quick start
To compile the code run "make". An executable called "ee" will be created. To run the executable type 

./ee <&#969;<sub>b</sub>> <&#969;<sub>m</sub>> <n<sub>s</sub>> \<h\> <w<sub>0</sub>> <&#963;<sub>8</sub>> \<z\>

where the seven parameter correspond to the five standard &#923;CDM parameters, the DE equation of state parameter w<sub>0</sub> and redshift z. The first column of the produced output corresponds to the k-mode values at which the boost factor (given in the second column) is measured.

WE URGE EVERYONE TO READ AT LEAST THE SECTION ABOUT PITFALLS (see below).

## Description
EuclidEmulator is a tool for rapid and accurate estimation of the
non-linear correction ("boost") to the dark matter power spectrum.
It is based on a spectral decomposition technique (called "sparse
polynomial chaos expansion") and estimates the boost at an accuracy
level of a fraction of a percent in the entire allowed redshift and
wavenumber range.

An estimate of a full non-linear power spectrum is obtained by
multiplying the emulated boost with a linear power spectrum computed
by a Boltzmann code (e.g. CAMB or CLASS). As a result, the linear
regime of the non-linear power spectrum is quasi perfect and effects
not captured by Newtonian gravity of dark matter (such as super-horizon
damping or baryonic clustering) are accounted for to first order on
all scales.

For more details we refer to publication.

## Parameter ranges
For the emulation of the boost, the cosmological parameters have to be
within the following ranges:

0.0217 <= &#969;<sub>b</sub>    <= 0.0233<br/>
0.1326 <= &#969;<sub>m</sub>    <= 0.1526<br/>
0.9345 <= n<sub>s</sub>     <= 0.9965<br/>
0.6251 <= h       <= 0.7211<br/>
-1.250 <= w<sub>0</sub>     <= -0.750<br/>
0.7684 <= &#963;<sub>8</sub> <= 0.8614<br/>

The redshift has to be 10<sup>-15</sup> <= z <= 5.

Input values outside this range will produce an error. Notice that these parameter ranges do *not* define the same parameter space as the one used to construct the emulator (reported in Knabenhans et al. 2018) but a smaller one. The reason for this is that emulation near the boundaries of the construction parameter space might not lead to accurate results.

Notice that also z=0 will lead to an error (but setting z to a very small value like 1e-15 does the job). This will be fixed in the future.

## Code structure
### Main emulator code
The emulator source code consists of four files:

`ee.dat` (!!! NEVER EVER CHANGE THIS FILE !!!)<br/>
`EuclidEmulator.c`<br/>
`cosmo.c`<br/>
`cosmo.h`<br/>

In `ee.dat`, all the building blocks of the emulator are stored. This file
forms the core of the emulator code. The numbers in ee.dat are the results
of the study described in Knabenhans et al. (2018). It includes the mean
boost factor, the principal components, the normalized Legendre polynomials
and the corresponding coefficients. MODIFYING THIS FILE WILL RESULT IN WRONG
EMULATION RESULTS.

The code in `EuclidEmulator.c` is just the front end that reads in, assembles and evaluates
the functions and coefficients stored in ee.dat. From this file, the executable
"ee" is built via the Makefile.

The code stored in `cosmo.c` and `cosmo.h, respectively, is used for the inter-
polation in redshift space (in order to allow the user to ask for any real-
valued redshift z <= 5).

### Auxiliary scripts and files
In addition, the following files are in the EuclidEmulator repository:

`EuclidRef_Class.ini`<br/>
`GetPnonlin.py`<br/>
`example.sh`<br/>

`EuclidRef_Class.ini` is a CLASS parameter file specifying the relevant parameters for the Euclid reference cosmology. REMARK: &#937;<sub>rad</sub> is uniquely determined by the CMB temperature T<sub>CMB</sub>. Since &#937;<sub>rad</sub> was fixed for all cosmologies in the experimental design (cf. reference paper by Knabenhans et al., 2018; submitted), it must not be changed. Notice further, that the parameter A_s has to be chosen in accordance to the Emulator input parameter &#963;<sub>8</sub>. 

The python script `GetPnonlin.py` reads in both, the linear power spectrum (produced by CAMB) and the boost factor, from the respective data files. It then interpolates the linear power spectrum (using cubic splines in log space) in order to evaluate the interpolated function at the k-points given in the boost factor file. The non-linear power spectrum is then computed therefrom and plotted. The result is shown in `ExamplePlot.pdf`. This plot shows one of the curves displayed in Knabenhans et al. (2018), Fig. 9.

The bash script `example.sh` executes the two scripts described above: first, EuclidEmulator evaluates a boost factor for the Euclid reference cosmology at z=0.5. Second, CLASS is called to produce the corresponding linear power spectrum (together with a non-linear fit according to Takahashi's halo model, cf. Takahashi et al. 2012). Next, `GetPnonlin.py` is executed to combine the results.

## User guide

=============================== I M P O R T A N T ===============================<br/>
!!! PLEASE READ THE SECTION ABOUT PITFALLS BELOW IN ORDER TO ENSURE CORRECT RESULTS !!!
============================================================================<br/>

1. Prerequisites:<br/>
   * GNU Scientific Library (GSL)
   * Python2.7 together with numpy, scipy and matplotlib (for post-processing only)
   * CLASS (for post-processing only)
   
2. Building the code:<br/>
   Type "make". An executable called "ee" will be created.

   Make sure not to move the files cosmo.c, cosmo.h and ee.dat.
   They have to be in the same directory as EuclidEmulator.c and
   the executable ee.

3. Usage:<br/>
   ./ee <&#969;<sub>b</sub>> <&#969;<sub>m</sub>> <n<sub>s</sub>> \<h\> <w<sub>0</sub>> <&#963;<sub>8</sub>> \<z\>

   This will print the resulting boost factor to standard output. To store
   it in a file, just use output redirection, i.e. append " > BoostFile.dat"
   to the command above.

4. Computation of the full non-linear power spectrum:<br/>
   1. Produce a linear power spectrum with a Boltzmann code.
   2. Interpolate the linear power spectrum (we suggest a cubic spline interpolation in log space.
   3. Evaluate the interpolated liner power spectrum at the k modes of the emulated boost.
   4. Multiply the boost with the resulting linear power spectrum.

The script `example.sh` includes commands to perform step iv (assuming you have CLASS installed already and your CLASS file explanatory.ini has never been changed). Please change the path to the directory where your CLASS executable is located. Executing `example.sh` will sequentially compute the boost spectrum using EuclidEmulator, compute the corresponding linear power spectrum with CLASS, and finally call `GetPnonlin.py` to compute the non-linear power spectrum.

## Pitfalls

When you want to emulate a full non-linear power spectrum, you really have to make sure that you specify the exact same cosmology for EuclidEmulator to produce the boost factor and for the Boltzmann code you use to predict the linear power spectrum. Often, the parametrization of cosmologies used in the Boltzmann solvers is different than the one used by EuclidEmulator. Make sure the different parametrizations define the exact same cosmology!

Known differences are:

1.  CAMB and CLASS use om_cdm (the cold dark matter density &#969;<sub>cdm</sub>) instead of om_m (the total matter density
    &#969;<sub>m</sub>). Make sure that the following relation is satisfied: 
    <div align="center">&#969<sub>b</sub> + &#969<sub>cdm</sub> = &#969<sub>m</sub> </div>
    If you want to compute the non-linear power spectrum using a EuclidEmulated-boost for a real data set (like e.g. 
    Planck2015), you may find that the reported values doe not obey the above relation. This is most likely due to the fact
    that there is a small contribution due to neutrinos which is taken into account in that data set but not so in this 
    version of our emulator. Here's what you need to do:</br>

    1. To produce the boost factor with EuclidEmulator use &#969;<sub>b</sub> and &#969;<sub>m</sub> as reported in the data set.
    2. To produce the corresponding linear power spectrum use &#969;<sub>b</sub> and set the &#969;<sub>cdm</sub> parameter equal to &#969;<sub>m</sub>-&#969;<sub>b</sub>. Doing so you add the neutrino component to the CDM contribution which is the best that can be done with the current version of EuclidEmulator. Stay tuned as version 2 will allow for neutrinos to be taken into accound.
 
2.  CAMB and CLASS do usually not accept sigma_8 as a parameter for normalization of the power spectrum but rather use A_s. In
    order to convert these two parameters into each other in the context of using EuclidEmulator, you have to use the same
    conversion as is used in the EuclidEmulator code. Convert the parameters using the following proportionality:<br/>
    <div align="center"> A<sub>s</sub>/(2.215 * 10^(-9)) = (&#963;<sub>8</sub>/0.8496)^2