//* This file is for continuum dislocation density field-based theory
//* Zhangchen Fan
//* Harbin Institute of Technology, Shenzhen
//* Centre for Micro-mechanics Modelling and Characterisation
//* 6 Jan 2024

// ruanqc 2025-08/09 modified for 2d patterning from CrystalPlasticityBussoUpdate.C 
// V251031, temp version 251105

#include "CrystalPlasticityBussoUpdate_patternMat.h"
#include "libmesh/int_range.h"

registerMooseObject("SolidMechanicsApp", CrystalPlasticityBussoUpdate_patternMat);

InputParameters
CrystalPlasticityBussoUpdate_patternMat::validParams()
{
  InputParameters params = CrystalPlasticityDislocationUpdateBase::validParams();
  params.addClassDescription("Busso's version of crystal plasticity.");

  params.addParam<Real>("r", 8.314, "Latent hardening coefficient");
  params.addParam<Real>("temperature", 22.0, "temperature in Celsius");
  params.addParam<Real>("p", 0.2, "exponent for slip rate in busso's flow rule");
  params.addParam<Real>("q", 1.2, "exponent for slip rate in busso's flow rule");
  params.addParam<Real>("f0", 2.77e-19, "thermal activation energy");
  params.addParam<Real>("gdot0", 1.0e6, "initial slip rate");
  params.addParam<Real>("tau_0", 20.0, "initial lattice friction strength of the material");
  params.addParam<Real>("burgers", 0.257e-6, "magnitude of burgers vector");
  params.addParam<Real>("shear_modulus", 45.0e3, "shear_modulus");
  params.addParam<Real>("boltzmann", 1.38065e-23, "The Boltzmann Constant");
  params.addParam<Real>("scaling_Cb", 1.0, "The scaling parameter for the backstress");
  params.addParam<Real>("scaling_Cd", 0.3, "The scaling parameter for the diffusionstress");
  params.addParam<Real>("tauF_on", 0.0, "ON-1.0, OFF-0.0 (tauF= St*(1 -+ tauF_on*rhoG/rhoT) on or off)");

  params.addParam<Real>("dlamb", 0.3, "initial slip rate");
  params.addParam<Real>("w1", 1.5, "cross-hardening constants, adopted from Cheong2004");
  params.addParam<Real>("w2", 1.2, "cross-hardening constants, adopted from Cheong2004");

  params.addCoupledVar(
      "edge_dislo_den_pos_1", 0.0, "Positive edge dislocation density: slip system 1");
  params.addCoupledVar(
      "edge_dislo_den_neg_1", 0.0, "Negative edge dislocation density: slip system 1");
  params.addCoupledVar(
      "edge_dislo_den_pos_2", 0.0, "Positive edge dislocation density: slip system 2");
  params.addCoupledVar(
      "edge_dislo_den_neg_2", 0.0, "Negative edge dislocation density: slip system 2");
  params.addCoupledVar(
      "edge_dislo_den_pos_3", 0.0, "Positive edge dislocation density: slip system 3");
  params.addCoupledVar(
      "edge_dislo_den_neg_3", 0.0, "Negative edge dislocation density: slip system 3");
  params.addCoupledVar(
      "edge_dislo_den_pos_4", 0.0, "Positive edge dislocation density: slip system 4");
  params.addCoupledVar(
      "edge_dislo_den_neg_4", 0.0, "Negative edge dislocation density: slip system 4");
  params.addCoupledVar(
      "edge_dislo_den_pos_5", 0.0, "Positive edge dislocation density: slip system 5");
  params.addCoupledVar(
      "edge_dislo_den_neg_5", 0.0, "Negative edge dislocation density: slip system 5");
  params.addCoupledVar(
      "edge_dislo_den_pos_6", 0.0, "Positive edge dislocation density: slip system 6");
  params.addCoupledVar(
      "edge_dislo_den_neg_6", 0.0, "Negative edge dislocation density: slip system 6");
  params.addCoupledVar(
      "edge_dislo_den_pos_7", 0.0, "Positive edge dislocation density: slip system 7");
  params.addCoupledVar(
      "edge_dislo_den_neg_7", 0.0, "Negative edge dislocation density: slip system 7");
  params.addCoupledVar(
      "edge_dislo_den_pos_8", 0.0, "Positive edge dislocation density: slip system 8");
  params.addCoupledVar(
      "edge_dislo_den_neg_8", 0.0, "Negative edge dislocation density: slip system 8");
  params.addCoupledVar(
      "edge_dislo_den_pos_9", 0.0, "Positive edge dislocation density: slip system 9");
  params.addCoupledVar(
      "edge_dislo_den_neg_9", 0.0, "Negative edge dislocation density: slip system 9");
  params.addCoupledVar(
      "edge_dislo_den_pos_10", 0.0, "Positive edge dislocation density: slip system 10");
  params.addCoupledVar(
      "edge_dislo_den_neg_10", 0.0, "Negative edge dislocation density: slip system 10");
  params.addCoupledVar(
      "edge_dislo_den_pos_11", 0.0, "Positive edge dislocation density: slip system 11");
  params.addCoupledVar(
      "edge_dislo_den_neg_11", 0.0, "Negative edge dislocation density: slip system 11");
  params.addCoupledVar(
      "edge_dislo_den_pos_12", 0.0, "Positive edge dislocation density: slip system 12");
  params.addCoupledVar(
      "edge_dislo_den_neg_12", 0.0, "Negative edge dislocation density: slip system 12");

  return params;
}

CrystalPlasticityBussoUpdate_patternMat::CrystalPlasticityBussoUpdate_patternMat(const InputParameters & parameters)
  : CrystalPlasticityDislocationUpdateBase(parameters),
    // Constitutive values
    _r(getParam<Real>("r")),
    _temperature(getParam<Real>("temperature")),
    _p(getParam<Real>("p")),
    _q(getParam<Real>("q")),
    _f0(getParam<Real>("f0")),
    _gdot0(getParam<Real>("gdot0")),
    _tau_0(getParam<Real>("tau_0")),
    _burgers(getParam<Real>("burgers")),
    _shear_modulus(getParam<Real>("shear_modulus")),
    _boltzmann(getParam<Real>("boltzmann")),
    _scaling_Cb(getParam<Real>("scaling_Cb")),
    _tauF_on(getParam<Real>("tauF_on")),
    _scaling_Cd(getParam<Real>("scaling_Cd")),
    _dlamb(getParam<Real>("dlamb")),
    _w1(getParam<Real>("w1")),
    _w2(getParam<Real>("w2")),

    _backstress(_number_slip_systems),
    _tau_d(_number_slip_systems),

    _edge_dislo_den_pos_1(coupledValue("edge_dislo_den_pos_1")),

    _edge_dislo_den_pos_grad_1(coupledGradient("edge_dislo_den_pos_1")), // Coupled rhoep gradient

    _edge_dislo_den_neg_1(coupledValue("edge_dislo_den_neg_1")),

    _edge_dislo_den_neg_grad_1(coupledGradient("edge_dislo_den_neg_1")), // Coupled rhoen gradient

    _edge_dislo_den_pos_2(coupledValue("edge_dislo_den_pos_2")),

    _edge_dislo_den_pos_grad_2(coupledGradient("edge_dislo_den_pos_2")), // Coupled rhoep gradient

    _edge_dislo_den_neg_2(coupledValue("edge_dislo_den_neg_2")),

    _edge_dislo_den_neg_grad_2(coupledGradient("edge_dislo_den_neg_2")), // Coupled rhoen gradient

    _edge_slip_direction(
        declareProperty<std::vector<Real>>("edge_slip_direction")), // Edge slip directions
    _screw_slip_direction(
        declareProperty<std::vector<Real>>("screw_slip_direction")), // Screw slip direction

    _deformation_gradient(getMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _plastic_deformation_gradient(
        getMaterialProperty<RankTwoTensor>("plastic_deformation_gradient")),


    _dislo_velocity(declareProperty<std::vector<Real>>("dislo_velocity")), // used for Kernals updated, ruanqc added info

    // ruanqc 2025-08/09 for patterning
    _dislo_velocity_p(declareProperty<std::vector<Real>>("dislo_velocity_p")), // Dislocation velocity_p
    _dislo_velocity_m(declareProperty<std::vector<Real>>("dislo_velocity_m")), // Dislocation velocity_m

    // ruanqc 2025-08/09 for patterning 
    _slip_increment_p(declareProperty<std::vector<Real>>("slip_increment_p")),
    _slip_increment_m(declareProperty<std::vector<Real>>("slip_increment_m")),

    _rss_qc(declareProperty<std::vector<Real>>("rss_qc")),  //ruanqc 250922 added to checkout resolved shear stress    

    _accumulated_equivalent_plastic_strain(
        declareProperty<Real>(_base_name + "accumulated_equivalent_plastic_strain")),
    _accumulated_equivalent_plastic_strain_old(
        getMaterialPropertyOld<Real>(_base_name + "accumulated_equivalent_plastic_strain"))

{
}

void
CrystalPlasticityBussoUpdate_patternMat::initQpStatefulProperties()
{
  CrystalPlasticityDislocationUpdateBase::initQpStatefulProperties();

  std::vector<Real> rho_edge_pos(_number_slip_systems);
  std::vector<Real> rho_edge_neg(_number_slip_systems);

  rho_edge_pos[0] = _edge_dislo_den_pos_1[_qp];
  rho_edge_pos[1] = _edge_dislo_den_pos_2[_qp];
  // rho_edge_pos[2] = _edge_dislo_den_pos_3[_qp];
  // rho_edge_pos[3] = _edge_dislo_den_pos_4[_qp];
  // rho_edge_pos[4] = _edge_dislo_den_pos_5[_qp];
  // rho_edge_pos[5] = _edge_dislo_den_pos_6[_qp];
  // rho_edge_pos[6] = _edge_dislo_den_pos_7[_qp];
  // rho_edge_pos[7] = _edge_dislo_den_pos_8[_qp];
  // rho_edge_pos[8] = _edge_dislo_den_pos_9[_qp];
  // rho_edge_pos[9] = _edge_dislo_den_pos_10[_qp];
  // rho_edge_pos[10] = _edge_dislo_den_pos_11[_qp];
  // rho_edge_pos[11] = _edge_dislo_den_pos_12[_qp];

  rho_edge_neg[0] = _edge_dislo_den_neg_1[_qp];
  rho_edge_neg[1] = _edge_dislo_den_neg_2[_qp];
  // rho_edge_neg[2] = _edge_dislo_den_neg_3[_qp];
  // rho_edge_neg[3] = _edge_dislo_den_neg_4[_qp];
  // rho_edge_neg[4] = _edge_dislo_den_neg_5[_qp];
  // rho_edge_neg[5] = _edge_dislo_den_neg_6[_qp];
  // rho_edge_neg[6] = _edge_dislo_den_neg_7[_qp];
  // rho_edge_neg[7] = _edge_dislo_den_neg_8[_qp];
  // rho_edge_neg[8] = _edge_dislo_den_neg_9[_qp];
  // rho_edge_neg[9] = _edge_dislo_den_neg_10[_qp];
  // rho_edge_neg[10] = _edge_dislo_den_neg_11[_qp];
  // rho_edge_neg[11] = _edge_dislo_den_neg_12[_qp];

  // Set initial slip resistance
  for (const auto i : make_range(_number_slip_systems))
  {
    Real initial_hardening_total_dislocation_density = 0.0;
    for (const auto j : make_range(_number_slip_systems))
    {
      // mooseWarning("x0-",i,j);
      if (i == j) // self vs. latent hardening
        initial_hardening_total_dislocation_density +=
            (_w1 + 1.0 - _w2) * (rho_edge_pos[j] + rho_edge_neg[j]);
      // (_w1 + 1.0 - _w2) * (_edge_dislocation_density[_qp][j] +
      // _screw_dislocation_density[_qp][j]); // self hardening
      else
        initial_hardening_total_dislocation_density += _w1 * (rho_edge_pos[j] + rho_edge_neg[j]);
      // _w1   * (_edge_dislocation_density[_qp][j] + _screw_dislocation_density[_qp][j]); // latent
      // hardening
    }
    _slip_resistance[_qp][i] =
        _dlamb * _shear_modulus * _burgers * std::sqrt(initial_hardening_total_dislocation_density);
  }

  _edge_slip_direction[_qp].resize(LIBMESH_DIM * _number_slip_systems);
  _screw_slip_direction[_qp].resize(LIBMESH_DIM * _number_slip_systems);
}

void
CrystalPlasticityBussoUpdate_patternMat::calculateSchmidTensor(
    const unsigned int & number_slip_systems,
    const std::vector<RealVectorValue> & plane_normal_vector,
    const std::vector<RealVectorValue> & direction_vector,
    std::vector<RankTwoTensor> & schmid_tensor,
    const RankTwoTensor & crysrot)
{
  std::vector<RealVectorValue> local_direction_vector, local_plane_normal;
  local_direction_vector.resize(number_slip_systems);
  local_plane_normal.resize(number_slip_systems);
  // mooseWarning("number_slip_systems", number_slip_systems);

  // Temporary directions and normals to calculate
  // screw dislocation slip direction
  RealVectorValue temp_mo;
  RealVectorValue temp_no;
  RealVectorValue temp_screw_mo;

  // Update slip direction with crystal orientation
  for (const auto i : make_range(_number_slip_systems))
  {
    local_direction_vector[i].zero();
    local_plane_normal[i].zero();

    for (const auto j : make_range(LIBMESH_DIM))
      for (const auto k : make_range(LIBMESH_DIM))
      {
        local_direction_vector[i](j) =
            local_direction_vector[i](j) + crysrot(j, k) * direction_vector[i](k);

        local_plane_normal[i](j) =
            local_plane_normal[i](j) + crysrot(j, k) * plane_normal_vector[i](k);
      }

    // Calculate Schmid tensor
    for (const auto j : make_range(LIBMESH_DIM))
      for (const auto k : make_range(LIBMESH_DIM))
      {
        schmid_tensor[i](j, k) = local_direction_vector[i](j) * local_plane_normal[i](k);
      }
  }

  _edge_slip_direction[_qp].resize(LIBMESH_DIM * _number_slip_systems);
  _screw_slip_direction[_qp].resize(LIBMESH_DIM * _number_slip_systems);

  // Store slip direction (already normalized)
  // for edge and screw dislocations
  // to couple with dislocation transport
  for (const auto i : make_range(_number_slip_systems))
  {
    for (const auto j : make_range(LIBMESH_DIM))
    {
      temp_mo(j) = local_direction_vector[i](j);
      temp_no(j) = local_plane_normal[i](j);
    }

    temp_screw_mo = temp_mo.cross(temp_no);

    for (const auto j : make_range(LIBMESH_DIM))
    {
      _edge_slip_direction[_qp][i * LIBMESH_DIM + j] = local_direction_vector[i](j);
      _screw_slip_direction[_qp][i * LIBMESH_DIM + j] = temp_screw_mo(j);
    }
  }
}

void
CrystalPlasticityBussoUpdate_patternMat::setInitialConstitutiveVariableValues()
{
  // No need for this subroutine
}

void
CrystalPlasticityBussoUpdate_patternMat::setSubstepConstitutiveVariableValues()
{
  // No need for this subroutine
}

bool
CrystalPlasticityBussoUpdate_patternMat::calculateSlipRate()
{

  std::vector<Real> local_edge_slip_direction, local_screw_slip_direction;
  local_edge_slip_direction.resize(LIBMESH_DIM);
  local_screw_slip_direction.resize(LIBMESH_DIM);

  // ruanqc 2025-08/09 modified for 2d patterning
  // without this, segmentation fault, often due to
  // an out-of-bounds access, uninitialized pointer/reference, or invalid vector size.
  _slip_increment_p[_qp].resize(_number_slip_systems);
  _slip_increment_m[_qp].resize(_number_slip_systems);
  
  _rss_qc[_qp].resize(_number_slip_systems); //ruanqc 250922 added to checkout resolved shear stress

  calculateSlipResistance();

  std::vector<Real> rho_edge_pos(_number_slip_systems);
  std::vector<Real> rho_edge_neg(_number_slip_systems);
  std::vector<Real> rho_screw_pos(_number_slip_systems);
  std::vector<Real> rho_screw_neg(_number_slip_systems);

  std::vector<Real> rho_edge_pos_grad_x(_number_slip_systems);
  std::vector<Real> rho_edge_neg_grad_x(_number_slip_systems);
  std::vector<Real> rho_edge_pos_grad_y(_number_slip_systems);
  std::vector<Real> rho_edge_neg_grad_y(_number_slip_systems);
  std::vector<Real> rho_edge_pos_grad_z(_number_slip_systems);
  std::vector<Real> rho_edge_neg_grad_z(_number_slip_systems);

  // Assign dislocation density vectors
  rho_edge_pos[0] = _edge_dislo_den_pos_1[_qp];
  rho_edge_pos[1] = _edge_dislo_den_pos_2[_qp];
  // rho_edge_pos[2] = _edge_dislo_den_pos_3[_qp];
  // rho_edge_pos[3] = _edge_dislo_den_pos_4[_qp];
  // rho_edge_pos[4] = _edge_dislo_den_pos_5[_qp];
  // rho_edge_pos[5] = _edge_dislo_den_pos_6[_qp];
  // rho_edge_pos[6] = _edge_dislo_den_pos_7[_qp];
  // rho_edge_pos[7] = _edge_dislo_den_pos_8[_qp];
  // rho_edge_pos[8] = _edge_dislo_den_pos_9[_qp];
  // rho_edge_pos[9] = _edge_dislo_den_pos_10[_qp];
  // rho_edge_pos[10] = _edge_dislo_den_pos_11[_qp];
  // rho_edge_pos[11] = _edge_dislo_den_pos_12[_qp];

  rho_edge_neg[0] = _edge_dislo_den_neg_1[_qp];
  rho_edge_neg[1] = _edge_dislo_den_neg_2[_qp];
  // rho_edge_neg[2] = _edge_dislo_den_neg_3[_qp];
  // rho_edge_neg[3] = _edge_dislo_den_neg_4[_qp];
  // rho_edge_neg[4] = _edge_dislo_den_neg_5[_qp];
  // rho_edge_neg[5] = _edge_dislo_den_neg_6[_qp];
  // rho_edge_neg[6] = _edge_dislo_den_neg_7[_qp];
  // rho_edge_neg[7] = _edge_dislo_den_neg_8[_qp];
  // rho_edge_neg[8] = _edge_dislo_den_neg_9[_qp];
  // rho_edge_neg[9] = _edge_dislo_den_neg_10[_qp];
  // rho_edge_neg[10] = _edge_dislo_den_neg_11[_qp];
  // rho_edge_neg[11] = _edge_dislo_den_neg_12[_qp];

  // Assigin dislocation density gradient vectors
  rho_edge_pos_grad_x[0] = _edge_dislo_den_pos_grad_1[_qp](0);
  rho_edge_pos_grad_x[1] = _edge_dislo_den_pos_grad_2[_qp](0);

  rho_edge_neg_grad_x[0] = _edge_dislo_den_neg_grad_1[_qp](0);
  rho_edge_neg_grad_x[1] = _edge_dislo_den_neg_grad_2[_qp](0);

  rho_edge_pos_grad_y[0] = _edge_dislo_den_pos_grad_1[_qp](1);
  rho_edge_pos_grad_y[1] = _edge_dislo_den_pos_grad_2[_qp](1);

  rho_edge_neg_grad_y[0] = _edge_dislo_den_neg_grad_1[_qp](1);
  rho_edge_neg_grad_y[1] = _edge_dislo_den_neg_grad_2[_qp](1);

  rho_edge_pos_grad_z[0] = _edge_dislo_den_pos_grad_1[_qp](2);
  rho_edge_pos_grad_z[1] = _edge_dislo_den_pos_grad_2[_qp](2);

  rho_edge_neg_grad_z[0] = _edge_dislo_den_neg_grad_1[_qp](2);
  rho_edge_neg_grad_z[1] = _edge_dislo_den_neg_grad_2[_qp](2);

  Real theta = _temperature + 273.15;
  Real RhoTotSlip;
  for (const auto i : make_range(_number_slip_systems))
  {
    if (rho_edge_pos[i] <= _zero_tol)
      rho_edge_pos[i] = 0.0;

    if (rho_edge_neg[i] <= _zero_tol)
      rho_edge_neg[i] = 0.0;

    RhoTotSlip = rho_edge_pos[i] + rho_edge_neg[i];

    if (_edge_slip_direction[_qp][i * LIBMESH_DIM] < 1.e-10)
      local_edge_slip_direction[0] = 0.0;
    else
      local_edge_slip_direction[0] = 1.0 / _edge_slip_direction[_qp][i * LIBMESH_DIM];

    if (_edge_slip_direction[_qp][i * LIBMESH_DIM + 1] < 1.e-10)
      local_edge_slip_direction[1] = 0.0;
    else
      local_edge_slip_direction[1] = 1.0 / _edge_slip_direction[_qp][i * LIBMESH_DIM + 1];

    if (_edge_slip_direction[_qp][i * LIBMESH_DIM + 2] < 1.e-10)
      local_edge_slip_direction[2] = 0.0;
    else
      local_edge_slip_direction[2] = 1.0 / _edge_slip_direction[_qp][i * LIBMESH_DIM + 2];

    if (_screw_slip_direction[_qp][i * LIBMESH_DIM] < 1.e-10)
      local_screw_slip_direction[0] = 0.0;
    else
      local_screw_slip_direction[0] = 1.0 / _screw_slip_direction[_qp][i * LIBMESH_DIM];

    if (_screw_slip_direction[_qp][i * LIBMESH_DIM + 1] < 1.e-10)
      local_screw_slip_direction[1] = 0.0;
    else
      local_screw_slip_direction[1] = 1.0 / _screw_slip_direction[_qp][i * LIBMESH_DIM + 1];

    if (_screw_slip_direction[_qp][i * LIBMESH_DIM + 2] < 1.e-10)
      local_screw_slip_direction[2] = 0.0;
    else
      local_screw_slip_direction[2] = 1.0 / _screw_slip_direction[_qp][i * LIBMESH_DIM + 2];


    _backstress(i) = _scaling_Cb * _burgers * _shear_modulus *
                     (rho_edge_pos_grad_x[i] * local_edge_slip_direction[0] -
                      rho_edge_neg_grad_x[i] * local_edge_slip_direction[0] +
                      rho_edge_pos_grad_y[i] * local_edge_slip_direction[1] -
                      rho_edge_neg_grad_y[i] * local_edge_slip_direction[1]) /
                     std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol); //ruanqc avoid zero division

    _tau_d(i)      = _scaling_Cd * _burgers * _shear_modulus *
                     (rho_edge_pos_grad_x[i] * local_edge_slip_direction[0] +
                      rho_edge_neg_grad_x[i] * local_edge_slip_direction[0] +
                      rho_edge_pos_grad_y[i] * local_edge_slip_direction[1] +
                      rho_edge_neg_grad_y[i] * local_edge_slip_direction[1]) /
                     std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol);  //ruanqc avoid zero division
                     
    // Real driving_force = std::abs(_tau[_qp][i] - _backstress(i) ) - _slip_resistance[_qp][i];     
    
    _rss_qc[_qp][i] = _tau[_qp][i]; //ruanqc 250922 added to checkout resolved shear stress


    Real driving_force_p = std::abs(_tau[_qp][i] - _backstress(i) - _tau_d(i)) - _slip_resistance[_qp][i] * 
                                                            (1.0 - _tauF_on * (rho_edge_pos[i] - rho_edge_neg[i]) / 
                                                            std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol)  //ruanqc avoid zero division  
                                                            );    
                      
    Real driving_force_m = std::abs(_tau[_qp][i] - _backstress(i) + _tau_d(i)) - _slip_resistance[_qp][i] * 
                                                            (1.0 + _tauF_on * (rho_edge_pos[i] - rho_edge_neg[i]) / 
                                                            std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol)   //ruanqc avoid zero division
                                                            );                          
                      
    if (driving_force_p < _zero_tol)
    {
      _slip_increment_p[_qp][i] = 0.0;
    }
    else
    {
      _slip_increment_p[_qp][i] =
          _gdot0 *
          std::exp(-_f0 / _boltzmann / theta *
                   std::pow((1.0 - std::pow((driving_force_p / _tau_0), _p)), _q)) *
          std::copysign(1.0, _tau[_qp][i] - _backstress(i) -  _tau_d(i));
    }

    if (driving_force_m < _zero_tol)
    {
      _slip_increment_m[_qp][i] = 0.0;
    }
    else
    {
      _slip_increment_m[_qp][i] =
          _gdot0 *
          std::exp(-_f0 / _boltzmann / theta *
                   std::pow((1.0 - std::pow((driving_force_m / _tau_0), _p)), _q)) *
          std::copysign(1.0, _tau[_qp][i] - _backstress(i) +  _tau_d(i));
    }

    // total contribution from positive and negative dislocations to slip increment in Lp
    _slip_increment[_qp][i] = ( _slip_increment_p[_qp][i] +  _slip_increment_m[_qp][i]) ;

    if (std::abs(_slip_increment[_qp][i]) * _substep_dt > _slip_incr_tol)
    {
      if (_print_convergence_message)
        mooseWarning("Maximum allowable slip increment exceeded ",
                     std::abs(_slip_increment[_qp][i]) * _substep_dt);

      return false;
    }

  }

//  mooseWarning("gamma_dot1=",_slip_increment[_qp][0]);
  // mooseWarning("gamma_dot2=",_slip_increment[_qp][1]);

  calculateDislocationVelocity();

  return true;
}

void
CrystalPlasticityBussoUpdate_patternMat::calculateSlipResistance()
{

  std::vector<Real> rho_edge_pos(_number_slip_systems);
  std::vector<Real> rho_edge_neg(_number_slip_systems);

  _slip_resistance[_qp].resize(_number_slip_systems);

  rho_edge_pos[0] = _edge_dislo_den_pos_1[_qp];
  rho_edge_pos[1] = _edge_dislo_den_pos_2[_qp];
  // rho_edge_pos[2] = _rho_edge_pos_3[_qp];
  // rho_edge_pos[3] = _rho_edge_pos_4[_qp];
  // rho_edge_pos[4] = _rho_edge_pos_5[_qp];
  // rho_edge_pos[5] = _rho_edge_pos_6[_qp];
  // rho_edge_pos[6] = _rho_edge_pos_7[_qp];
  // rho_edge_pos[7] = _rho_edge_pos_8[_qp];
  // rho_edge_pos[8] = _rho_edge_pos_9[_qp];
  // rho_edge_pos[9] = _edge_dislo_den_pos_10[_qp];
  // rho_edge_pos[10] = _edge_dislo_den_pos_11[_qp];
  // rho_edge_pos[11] = _edge_dislo_den_pos_12[_qp];

  rho_edge_neg[0] = _edge_dislo_den_neg_1[_qp];
  rho_edge_neg[1] = _edge_dislo_den_neg_2[_qp];
  // rho_edge_neg[2] = _rho_edge_neg_3[_qp];
  // rho_edge_neg[3] = _rho_edge_neg_4[_qp];
  // rho_edge_neg[4] = _rho_edge_neg_5[_qp];
  // rho_edge_neg[5] = _rho_edge_neg_6[_qp];
  // rho_edge_neg[6] = _rho_edge_neg_7[_qp];
  // rho_edge_neg[7] = _rho_edge_neg_8[_qp];
  // rho_edge_neg[8] = _rho_edge_neg_9[_qp];
  // rho_edge_neg[9] = _edge_dislo_den_neg_10[_qp];
  // rho_edge_neg[10] = _edge_dislo_den_neg_11[_qp];
  // rho_edge_neg[11] = _edge_dislo_den_neg_12[_qp];

  for (const auto i : make_range(_number_slip_systems))
  {
    if (rho_edge_pos[i] <= _zero_tol)
      rho_edge_pos[i] = 0.0;

    if (rho_edge_neg[i] <= _zero_tol)
      rho_edge_neg[i] = 0.0;

    Real hardening_total_dislocation_density = 0.0;
    for (const auto j : make_range(_number_slip_systems))
    {
      if (i == j) // self vs. latent hardening
        hardening_total_dislocation_density +=
            (_w1 + 1.0 - _w2) * (rho_edge_pos[j] + rho_edge_neg[j]); // self hardening
      else
        hardening_total_dislocation_density +=
            _w1 * (rho_edge_pos[j] + rho_edge_neg[j]); // latent hardening
    }
    _slip_resistance[_qp][i] =
        _dlamb * _shear_modulus * _burgers * std::sqrt(hardening_total_dislocation_density);
  }
}

void
CrystalPlasticityBussoUpdate_patternMat::calculateDislocationVelocity()
{
  std::vector<Real> rho_edge_pos(_number_slip_systems);
  std::vector<Real> rho_edge_neg(_number_slip_systems);

  
  _dislo_velocity[_qp].resize(_number_slip_systems);   // used for Kernals updated, ruanqc added info

  // // ruanqc 2025-08/09 modified for 2d patterning 
  _dislo_velocity_p[_qp].resize(_number_slip_systems);
  _dislo_velocity_m[_qp].resize(_number_slip_systems);

  Real total_dislocation_density = 0.0; // total dislocation density in the current slip system

  rho_edge_pos[0] = _edge_dislo_den_pos_1[_qp];
  rho_edge_pos[1] = _edge_dislo_den_pos_2[_qp];
  // rho_edge_pos[2] = _rho_edge_pos_3[_qp];
  // rho_edge_pos[3] = _rho_edge_pos_4[_qp];
  // rho_edge_pos[4] = _rho_edge_pos_5[_qp];
  // rho_edge_pos[5] = _rho_edge_pos_6[_qp];
  // rho_edge_pos[6] = _rho_edge_pos_7[_qp];
  // rho_edge_pos[7] = _rho_edge_pos_8[_qp];
  // rho_edge_pos[8] = _rho_edge_pos_9[_qp];
  // rho_edge_pos[9] = _edge_dislo_den_pos_10[_qp];
  // rho_edge_pos[10] = _edge_dislo_den_pos_11[_qp];
  // rho_edge_pos[11] = _edge_dislo_den_pos_12[_qp];

  rho_edge_neg[0] = _edge_dislo_den_neg_1[_qp];
  rho_edge_neg[1] = _edge_dislo_den_neg_2[_qp];
  // rho_edge_neg[2] = _rho_edge_neg_3[_qp];
  // rho_edge_neg[3] = _rho_edge_neg_4[_qp];
  // rho_edge_neg[4] = _rho_edge_neg_5[_qp];
  // rho_edge_neg[5] = _rho_edge_neg_6[_qp];
  // rho_edge_neg[6] = _rho_edge_neg_7[_qp];
  // rho_edge_neg[7] = _rho_edge_neg_8[_qp];
  // rho_edge_neg[8] = _rho_edge_neg_9[_qp];
  // rho_edge_neg[9] = _edge_dislo_den_neg_10[_qp];
  // rho_edge_neg[10] = _edge_dislo_den_neg_11[_qp];
  // rho_edge_neg[11] = _edge_dislo_den_neg_12[_qp];

  for (const auto i : make_range(_number_slip_systems))
  {

    if (rho_edge_pos[i] <= _zero_tol)
      rho_edge_pos[i] = 0.0;

    if (rho_edge_neg[i] <= _zero_tol)
      rho_edge_neg[i] = 0.0;

    total_dislocation_density =
        rho_edge_pos[i] + rho_edge_neg[i]; // + rho_screw_pos[i] + rho_screw_neg[i];

    // // ruanqc commented and modified by adding two driving forces for + and - dislocations
    // Real driving_force   = std::abs(_tau[_qp][i] - _backstress(i) - _tau_d(i)) - _slip_resistance[_qp][i];

    Real driving_force_p = std::abs(_tau[_qp][i] - _backstress(i) - _tau_d(i)) - _slip_resistance[_qp][i] * 
                      (1.0 - _tauF_on * (rho_edge_pos[i] - rho_edge_neg[i]) / 
                                                              std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol)   //ruanqc avoid zero division
                      );    

    Real driving_force_m = std::abs(_tau[_qp][i] - _backstress(i) + _tau_d(i)) - _slip_resistance[_qp][i] * 
                      (1.0 + _tauF_on * (rho_edge_pos[i] - rho_edge_neg[i]) /
                                                              std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol)   //ruanqc avoid zero division
                      );                    
 
    

    if (driving_force_p > _zero_tol)
    {  
      _dislo_velocity_p[_qp][i] =  _slip_increment_p[_qp][i] / _burgers / 
                                                              std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol );    //zero_tol -ruanqc avoid zero division
    }
    else
    {  
      _dislo_velocity_p[_qp][i] = 0.0;
    }


    if (driving_force_m > _zero_tol)
    {  
      _dislo_velocity_m[_qp][i] = _slip_increment_m[_qp][i] / _burgers / 
                                                              std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol );         
    }
    else
    {  
      _dislo_velocity_m[_qp][i] = 0.0;
    }
  }
}  


void
CrystalPlasticityBussoUpdate_patternMat::calculateEquivalentSlipIncrement(
    RankTwoTensor & equivalent_slip_increment)
{
  CrystalPlasticityDislocationUpdateBase::calculateEquivalentSlipIncrement(
      equivalent_slip_increment);

  calculateAccumulatedEquivalentPlasticStrain();
}

void
CrystalPlasticityBussoUpdate_patternMat::calculateAccumulatedEquivalentPlasticStrain()
{
  RankTwoTensor plastic_strain_rate, term1, term2;
  RankTwoTensor elastic_deformation_gradient, inverse_elastic_deformation_gradient,
      inverse_plastic_deformation_gradient;

  inverse_plastic_deformation_gradient = _plastic_deformation_gradient[_qp].inverse();
  elastic_deformation_gradient = _deformation_gradient[_qp] * inverse_plastic_deformation_gradient;
  inverse_elastic_deformation_gradient = elastic_deformation_gradient.inverse();
  for (const auto i : make_range(_number_slip_systems))
  {
    term1 = _slip_increment[_qp][i] * elastic_deformation_gradient * _flow_direction[_qp][i] *
            inverse_elastic_deformation_gradient * _substep_dt;

    term2 = _slip_increment[_qp][i] * inverse_elastic_deformation_gradient.transpose() *
            _flow_direction[_qp][i].transpose() * elastic_deformation_gradient.transpose() *
            _substep_dt;
    plastic_strain_rate += 0.5 * (term1 + term2);
  }
  _accumulated_equivalent_plastic_strain[_qp] =
      _accumulated_equivalent_plastic_strain_old[_qp] +
      std::sqrt(2.0 / 3.0 * plastic_strain_rate.doubleContraction(plastic_strain_rate));
}

void
CrystalPlasticityBussoUpdate_patternMat::calculateConstitutiveSlipDerivative(std::vector<Real> & dslip_dtau)
{
  std::vector<Real> rho_edge_pos(_number_slip_systems);  //ruanqc added
  std::vector<Real> rho_edge_neg(_number_slip_systems);  //ruanqc added

  Real total_dislocation_density = 0.0; // ruanqc added total dislocation density in the current slip system

  for (const auto i : make_range(_number_slip_systems))  //ruanqc added
  {
    // assign the correct coupled variables per slip system
    if (i == 0)
    {
      rho_edge_pos[i] = _edge_dislo_den_pos_1[_qp]; // Coupled rhoep
      rho_edge_neg[i] = _edge_dislo_den_neg_1[_qp]; // Coupled rhoen
    }
    else if (i == 1)
    {
      rho_edge_pos[i] = _edge_dislo_den_pos_2[_qp]; // Coupled rhoep
      rho_edge_neg[i] = _edge_dislo_den_neg_2[_qp]; // Coupled rhoen
    }
    else
    {
      // Future slip systems: default to zero (or extend if more coupled vars are added)
      rho_edge_pos[i] = 0.0;
      rho_edge_neg[i] = 0.0;
    }

    if (rho_edge_pos[i] <= _zero_tol)
      rho_edge_pos[i] = 0.0;

    if (rho_edge_neg[i] <= _zero_tol)
      rho_edge_neg[i] = 0.0;

    total_dislocation_density =
        rho_edge_pos[i] + rho_edge_neg[i]; // + rho_screw_pos[i] + rho_screw_neg[i];

  }

  Real theta = _temperature + 273.15;
  for (const auto i : make_range(_number_slip_systems))
  {
    Real driving_force_p = std::abs(_tau[_qp][i] - _backstress(i) - _tau_d(i)) - _slip_resistance[_qp][i] * 
                      (1.0 - _tauF_on * (rho_edge_pos[i] - rho_edge_neg[i]) / 
                                        std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol) 
                      );    

    Real driving_force_m = std::abs(_tau[_qp][i] - _backstress(i) + _tau_d(i)) - _slip_resistance[_qp][i] * 
                      (1.0 + _tauF_on * (rho_edge_pos[i] - rho_edge_neg[i]) /
                                        std::max((rho_edge_pos[i] + rho_edge_neg[i]), _zero_tol) 
                      );                       



    Real u = 0.0;
    Real uprime_p = 0.0, vprime_p = 0.0;
    Real uprime_m = 0.0, vprime_m = 0.0;
    Real dslip_dtau_p = 0.0; //ruanqc added
    Real dslip_dtau_m = 0.0; //ruanqc added

    if (driving_force_p < _zero_tol)
      dslip_dtau_p = 0.0;
    else
    {
      u = driving_force_p / _tau_0;
      // clamp u to [0, 1-eps] to avoid negative base to fractional power
      const Real eps = 1e-12;
      if (u < 0.0)
        u = 0.0;
      if (u >= 1.0)
        u = 1.0 - eps;

      vprime_p = std::pow((1.0 - std::pow(u, _p)), _q - 1.0);
      uprime_p = std::pow(u, _p - 1.0);

      dslip_dtau_p = std::copysign(1.0, _tau[_qp][i] - _backstress(i) -  _tau_d(i)) *  
                      _gdot0 * std::exp(-_f0 / _boltzmann / theta * std::pow((1.0 - std::pow(u, _p)), _q)) *  //dot_gamma_{e+}
                      _p * _q * _f0 / _boltzmann / theta * vprime_p * uprime_p *
                      1.0 / _tau_0 * std::copysign(1.0, _tau[_qp][i]) * _substep_dt;
    }

    if (driving_force_m < _zero_tol)
      dslip_dtau_m = 0.0;
    else
    {
      u = driving_force_m / _tau_0;
      const Real eps = 1e-12;
      if (u < 0.0)
        u = 0.0;
      if (u >= 1.0)
        u = 1.0 - eps;

      vprime_m = std::pow((1.0 - std::pow(u, _p)), _q - 1.0);
      uprime_m = std::pow(u, _p - 1.0);

      dslip_dtau_m = std::copysign(1.0, _tau[_qp][i] - _backstress(i) +  _tau_d(i)) *  
                      _gdot0 * std::exp(-_f0 / _boltzmann / theta * std::pow((1.0 - std::pow(u, _p)), _q)) *  //dot_gamma_{e-}
                      _p * _q * _f0 / _boltzmann / theta * vprime_m * uprime_m *
                      1.0 / _tau_0 * std::copysign(1.0, _tau[_qp][i]) * _substep_dt;
    }

    // slip_increment derivative contribution from positive and negative dislocations
    dslip_dtau[i] =  ( dslip_dtau_p + dslip_dtau_m); //ruanqc added

    // mooseWarning("tau=",_tau[_qp][i]);
  }
}

bool
CrystalPlasticityBussoUpdate_patternMat::areConstitutiveStateVariablesConverged()
{
  return true;
}

void
CrystalPlasticityBussoUpdate_patternMat::updateSubstepConstitutiveVariableValues()
{
  // No need for this subroutine
}

void
CrystalPlasticityBussoUpdate_patternMat::cacheStateVariablesBeforeUpdate()
{
  // No need for this subroutine
}

void
CrystalPlasticityBussoUpdate_patternMat::calculateStateVariableEvolutionRateComponent()
{
  // No need for this subroutine
}

bool
CrystalPlasticityBussoUpdate_patternMat::updateStateVariables()
{
  return true;
}
