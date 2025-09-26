//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// ruanqc -20250818 modified based on previous version
// DisloVelocity_DbSlip.C
// Used for 1/2D Patterning unCoupled with mechanics

#pragma once

#include "Material.h"
#include "LinearInterpolation.h"
#include "DerivativeMaterialInterface.h"

class DisloVelocity_DbSlip_patternMat : public DerivativeMaterialInterface<Material>
{
public:
  DisloVelocity_DbSlip_patternMat(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties() override;

  virtual void initQpStatefulProperties();

  const unsigned int _nss;

  std::vector<Real> _gssT;

  const Real _abstemp;

  const Real _boltzmann;

  const Real _burgers;

  const Real _f0;

  const Real _gdot0;

  const Real _dlamb;

  const Real _shear_modulus;

  const Real _p;

  const Real _q;

  const Real _tau0hat;

  const Real _taualpha;

  const Real _w1;

  const Real _w2;

  const Real _c_backs;

  const Real _c_diffu;
  
  const Real _tauF_on;

private:
  /// member variable to hold the computed diffusivity coefficient
  MaterialProperty<std::vector<Real>> & _dislo_velocity;
  // ruanqc added for positive and negative dislocation velocity
    MaterialProperty<std::vector<Real>> & _dislo_velocity_p;  
    MaterialProperty<std::vector<Real>> & _dislo_velocity_m;

  /// member variable to hold the computed convection velocity gradient term
  const MaterialProperty<std::vector<Real>> & _velocity_old;
  // ruanqc added for positive and negative dislocation velocity
    const MaterialProperty<std::vector<Real>> & _velocity_old_p;
    const MaterialProperty<std::vector<Real>> & _velocity_old_m;

  MaterialProperty<std::vector<Real>> & _slip_increment;
  // ruanqc added for slip rate
    MaterialProperty<std::vector<Real>> & _slip_increment_p;
    MaterialProperty<std::vector<Real>> & _slip_increment_m;

  const VariableValue & _edge_dislo_den_pos_1;

  const VariableGradient & _grad_edge_dislo_den_pos_1;

  const VariableValue & _edge_dislo_den_neg_1;

  const VariableGradient & _grad_edge_dislo_den_neg_1;

  const VariableValue & _edge_dislo_den_pos_2;

  const VariableGradient & _grad_edge_dislo_den_pos_2;

  const VariableValue & _edge_dislo_den_neg_2;

  const VariableGradient & _grad_edge_dislo_den_neg_2;

  // Real _initial_velocity;

  MaterialProperty<Real> & _slip_rate;
    // ruanqc added for positive and negative slip rate
    MaterialProperty<Real> & _slip_rate_p;
    MaterialProperty<Real> & _slip_rate_m;

  MaterialProperty<std::vector<Real>> & _slip_resistance;

  MaterialProperty<std::vector<Real>> & _total_dislo_den;

  MaterialProperty<Real> & _rhot;

  MaterialProperty<Real> & _tau_backstress;

  MaterialProperty<Real> & _tau_diffusion;

  const Real _scale;

  const Real _critical_val;
};
