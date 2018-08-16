"""
ee_lens.py

EuclidEmulator submodule for computation of cosmological lensing quantities.

REMARK:      The geometry of the Universe is fixed to be flat (i.e.
             Omega_curvature = 1) and the radiation energy density
             is set to Om_rad = 4.183709411969527e-5/(h*h). These
             values were assumed in the construction process of 
             EuclidEmulator and hence must be used whenever one is
             working with it.
"""

# This file is part of EuclidEmulator 
# Copyright (c) 2018 Mischa Knabenhans
#
# EuclidEmulator is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# at your option) any later version.
#
# EuclidEmulator is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import numpy as np
from scipy.integrate import romb
from scipy.interpolate import CubicSpline
import _internal._ee_cosmoconv as cc
import _internal._ee_aux as aux

def lens_efficiency(sourcedist, dcomov, dcomov_lim, prec=12):
    """
    Signature:    lens_efficiency(sourcedist, dcomov, dcomov_lim)
    
    Description:  Computes the lens efficiency function q (see e.g. equation 
                  (24)the review article by Martin Kilbinger "Cosmology with
                  cosmic shear observations: a review", July 21, 2015, 
                  arXiv:1411.0115v2), given a source distribution function n and
                  two comoving distances.
                 
    Input types:  sourcedist - np.ndarray
                  dcomov - float (or int) or np.ndarray
                  dcomov_lim - float
    
    Output types: float (or np.ndarray if type(dcomov)=np.ndarray)
    """    
    
    # interpolate the source distribution function
    nfunc = CubicSpline(np.log10(sourcedist['chi']), np.log10(sourcedist['n']))
    
    result = []
    
    if isinstance(dcomov, np.ndarray):

        for d in dcomov:
            chi = np.linspace(d, dcomov_lim, 2**prec+1)
            n = 10**nfunc(np.log10(chi))
            integrand = n * (1-d/chi)
            result.append(romb(integrand, chi[1]-chi[0]))
        
        return np.asarray(result)
    
    elif isinstance(dcomov, float) or isinstance(dcomov,int):
        chi = np.linspace(dcomov, dcomov_lim, 2**prec+1)
        n = 10**nfunc(np.log10(chi))
        integrand = n * (1-dcomov/chi)
        
        return romb(integrand, chi[1]-chi[0])
    
    else:
        raise(TypeError, "The second argument 'dcomov' must be either a float,\
                          an integer or a np.ndarray.\n")

class GalaxyRedshiftDist(object):
    # Author: Rongchuan Zhao
    def __init__(self, alpha = 2.0, beta = 1.5, z_mean = 0.9):
        self._alpha = alpha
        self._beta = beta
        self._z_mean = z_mean

    def __call__(self, z_mean):
        return GalaxyRedshiftDist(z_mean = z_mean,
                                  alpha = self._alpha, beta = self._beta
                                 ).gala_probdist_func

    def _z_0(self):
        return self._z_mean/1.412

    @aux.normalize()
    def _gala_probdist(self, z):
        z_0 = self._z_0()
        return (z / z_0)**self._alpha*np.exp(-(z/z_0)**self._beta)

    @property
    def gala_probdist_func(self):
        return aux.Function(self._gala_probdist) 