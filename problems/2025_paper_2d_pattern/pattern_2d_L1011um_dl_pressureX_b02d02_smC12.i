# ------------------------------------------------------------------------------------------------
# ruanqc-250820 modified to implement 2d patterning 
# based on Hardinlusion version c1m1_AL_H0002__coarse_reload_096per_t0369_rho7e7.i
# ------------------------------------------------------------------------------------------------

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 128
    ny = 128
    xmin = 0.0
    ymin = 0.0
    xmax = 1.011
    ymax = 1.011
    elem_type = 'QUAD4'
  []
[]


[Variables]
  [disp_x]
    order = FIRST
      family = LAGRANGE
  []
  [disp_y]
    order = FIRST
      family = LAGRANGE
  []
  [rho_edge_pos_1]
    order = FIRST
    family = LAGRANGE
    block = '0'
  []
  [rho_edge_neg_1]
    order = FIRST
    family = LAGRANGE
    block = '0'
  []
    [rho_edge_pos_2]
    order = FIRST
    family = LAGRANGE
    block = '0'
  []
  [rho_edge_neg_2]
    order = FIRST
    family = LAGRANGE
    block = '0'
  []
[]

[ICs]
  # [./rho_edge_pos_1]
  #   type = FunctionIC
  #   variable = 'rho_edge_pos_1'
  #   function = Gaussian_xy
  # []
  # [./rho_edge_neg_1]
  #   type = FunctionIC
  #   variable = 'rho_edge_neg_1'
  #   function = Gaussian_xy
  # []
  # [./rho_edge_pos_2]
  #   type = FunctionIC
  #   variable = 'rho_edge_pos_2'
  #   function = Gaussian_xy
  # []
  # [./rho_edge_neg_2]
  #   type = FunctionIC
  #   variable = 'rho_edge_neg_2'
  #   function = Gaussian_xy
  # []  

  [./rho_edge_pos_1]
    type = RandomIC
    variable = 'rho_edge_pos_1'
    min = ${fparse 4000.0 - 0.01*1.732*4000.0}
    max = ${fparse 4000.0 + 0.01*1.732*4000.0}
    seed = 42
  []
  [./rho_edge_neg_1]
    type = RandomIC
    variable = 'rho_edge_neg_1'
    min = ${fparse 4000.0 - 0.01*1.732*4000.0}
    max = ${fparse 4000.0 + 0.01*1.732*4000.0}
    seed = 42
  []
  [./rho_edge_pos_2]
    type = RandomIC
    variable = 'rho_edge_pos_2'
    min = ${fparse 4000.0 - 0.01*1.732*4000.0}
    max = ${fparse 4000.0 + 0.01*1.732*4000.0}
    seed = 42
  []
  [./rho_edge_neg_2]
    type = RandomIC
    variable = 'rho_edge_neg_2'
    min = ${fparse 4000.0 - 0.01*1.732*4000.0}
    max = ${fparse 4000.0 + 0.01*1.732*4000.0}
    seed = 42
  []  
[]

[Functions]
  [./Gaussian_x]
    type = ParsedFunction
    expression = 'if(abs(x - 1.011) < 0.0079, 0.001*16000.0 * exp(-((x - 1.011)^2 / (2.0 * 0.0079^2))) + 8000.0, 8000.0)'
  [../]

  [./Gaussian_y]
    type = ParsedFunction
    expression = 'if(abs(y - 1.011) < 0.0079, 0.001*16000.0 * exp(-((y - 1.011)^2 / (2.0 * 0.0079^2))) + 8000.0, 8000.0)'
  [../]

  [./Gaussian_xy]
    type = ParsedFunction
      expression = 'if((abs(x - 1.011) < 0.0079) * (abs(y - 1.011) < 0.0079), 0.001*16000.0 * exp(-((x - 1.011)^2 / (2.0 * 0.0079^2))) * exp(-((y - 1.011)^2 / (2.0 * 0.0079^2))) + 8000.0, 8000.0)'
  [../]

  [./disp_load]
    type = ParsedFunction
    expression = '0.05*2.022*t'
  [../]

  [./pressue_piecewise]
    type = PiecewiseLinear
    x = '0.0 1e-4  0.3'
    y = '0.0 1     1'
  [../]
[]

[BCs]
  [left_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'left'
    value = 0.0
  []
  [bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0.0
  []

  [right_x]
    type = Pressure
    variable = disp_x
    boundary = 'right'
    factor = -0.966 #-0.683
    function = pressue_piecewise
  []

  [./Periodic]
    # [./auto_boundary_x]
    # variable = disp_x
    #   auto_direction = 'x'
    # [../]

    # [./auto_boundary_y]
    # variable = disp_y
    #   auto_direction = 'y'
    # [../]

    [./auto_rho_edge_pos_boundary_x_1]
    variable = rho_edge_pos_1
    auto_direction = 'x y'
    [../]
    [./auto_rho_edge_neg_boundary_x_1]
     variable = rho_edge_neg_1
    auto_direction = 'x y'
    [../]

    [./auto_rho_edge_pos_boundary_x_2]
    variable = rho_edge_pos_2
    auto_direction = 'x y'
    [../]
    [./auto_rho_edge_neg_boundary_x_2]
     variable = rho_edge_neg_2
    auto_direction = 'x y'
    [../]
  [../]
[]

[AuxVariables]
  [./pk2]
    order = CONSTANT
    family = MONOMIAL
    block = '0'
  [../]
  [./fp_xy]
    order = CONSTANT
    family = MONOMIAL
    block = '0'
  [../]
  [./exy]
    order = CONSTANT
    family = MONOMIAL
    block = '0'
  [../]
  [./slip_increment]
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
  [./dislo_velocity]
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
  [./epeq]
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
[]


[Physics/SolidMechanics/QuasiStatic/all]
  strain = FINITE
  add_variables = true
  generate_output = 'strain_xx stress_xy'
  additional_generate_output = 'strain_xy'
[]

[Kernels]
  [Edeg_Pos_Time_Deri_1]
    type = MassLumpedTimeDerivative
    variable = rho_edge_pos_1
  []
  [Edge_Pos_Flux_1]
    type = ConservativeAdvectionSchmidNoSSD_patternKer
    variable = rho_edge_pos_1
    upwinding_type = full
      dislo_sign = positive
      slip_sys_index = 0
      dislo_character = edge
  []
  [Edeg_Neg_Time_Deri_1]
    type = MassLumpedTimeDerivative
    variable = rho_edge_neg_1
  []
  [Edge_Neg_Flux_1]
    type = ConservativeAdvectionSchmidNoSSD_patternKer
    variable = rho_edge_neg_1
    upwinding_type = full
      dislo_sign = negative
      slip_sys_index = 0
      dislo_character = edge
  []
  [Edeg_Pos_Time_Deri_2]
    type = MassLumpedTimeDerivative
    variable = rho_edge_pos_2
  []
  [Edge_Pos_Flux_2]
    type = ConservativeAdvectionSchmidNoSSD_patternKer
    variable = rho_edge_pos_2
    upwinding_type = full
      dislo_sign = positive
      slip_sys_index = 1
      dislo_character = edge
  []

  [Edeg_Neg_Time_Deri_2]
    type = MassLumpedTimeDerivative
    variable = rho_edge_neg_2
  []
  [Edge_Neg_Flux_2]
    type = ConservativeAdvectionSchmidNoSSD_patternKer
    variable = rho_edge_neg_2
    upwinding_type = full
      dislo_sign = negative
      slip_sys_index = 1
      dislo_character = edge
  []

[]

[AuxKernels]
  [./pk2]
   type = RankTwoAux
   variable = pk2
   rank_two_tensor = second_piola_kirchhoff_stress
   index_j = 0
   index_i = 0
   execute_on = timestep_end
   block = '0'
  [../]
  [./exy]
    type = RankTwoAux
    variable = exy
    rank_two_tensor = total_lagrangian_strain
    index_j = 0
    index_i = 1
    execute_on = timestep_end
    block = '0'
  [../]
  [./fp_xy]
    type = RankTwoAux
    variable = fp_xy
    rank_two_tensor = plastic_deformation_gradient
    index_j = 0
    index_i = 1
    execute_on = timestep_end
    block = '0'
  [../]
  [./slip_inc]
   type = MaterialStdVectorAux
   variable = slip_increment
   property = slip_increment
   index = 0
   execute_on = timestep_end
   block = '0'
  [../]
  [./dislo_vel]
   type = MaterialStdVectorAux
   variable = dislo_velocity
   property = dislo_velocity
   index = 0
   execute_on = timestep_end
   block = '0'
  [../]
  [./epeq]
   type = MaterialRealAux
   variable = epeq
   property = accumulated_equivalent_plastic_strain
   execute_on = timestep_end
   block = '0'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeElasticityTensorCP
    # C_ijkl = '168.5e3 121.5e3 121.5e3 168.5e3 121.5e3 168.5e3 75.6e3 75.6e3 75.6e3'
    C_ijkl = '151.5e3 113.5e3 113.5e3 151.5e3 113.5e3 151.5e3 63.6e3 63.6e3 63.6e3'
    fill_method = symmetric9
    euler_angle_1 = 0.0
    euler_angle_2 = 0.0 
    euler_angle_3 = 0.0 
    block = '0'
  [../]
  [./stress]
    type = ComputeCrystalPlasticityDislocationStress
    crystal_plasticity_models = 'trial_xtalpl'
    tan_mod_type = exact
    block = '0'
  [../]
  [./trial_xtalpl]
    type = CrystalPlasticityBussoUpdate_patternMat
    number_slip_systems = 2
    slip_sys_file_name  = input_2slip_sys_al_45.txt
      gdot0             = 1.0e6
      f0                = 2.77e-19
      boltzmann         = 1.38065e-23
      temperature       = 476.85                                         # 750 - 273.15 in degrees Celsius
      tau_0             = 20.0
      p                 = 0.2
      q                 = 1.2
      dlamb             = 0.3
      shear_modulus     = 45.0e3
      burgers           = 0.257e-6
        w1              = 0.0
        w2              = 0.0
      scaling_Cb        = 0.2
      scaling_Cd        = 0.2
      tauF_on           = 1.0                                             #ON-1.0, OFF-0.0 (tauF= St*(1 -+ tauF_on*rhoG/rhoT) on or off)

    edge_dislo_den_pos_1 = rho_edge_pos_1
    edge_dislo_den_neg_1 = rho_edge_neg_1
    edge_dislo_den_pos_2 = rho_edge_pos_2
    edge_dislo_den_neg_2 = rho_edge_neg_2
    block = '0'
  [../]

[]

[Preconditioning]
  active = 'smp'
  [./smp]
    type = SMP
    full = true
  [../]
[]

# Transient (time-dependent) details for simulations go here:
[Executioner]

  type = Transient
  solve_type = 'NEWTON'
  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'lu    boomeramg          31'
  line_search = 'none'
  l_max_its = 50
  nl_max_its = 50
  nl_rel_tol = 1e-3 #1e-5
  nl_abs_tol = 1e-3
  l_tol = 1e-3  #1e-5 

  start_time = 0.0
  end_time = 0.3 #0.01
  dt = 1.e-8
  dtmin = 1.e-12
[]

[Postprocessors]
  [./stress_xy]
    type = ElementAverageValue
    variable = stress_xy
    block = '0'
  [../]
  [./pk2]
   type = ElementAverageValue
   variable = pk2
   block = '0'
  [../]
  [./fp_xy]
    type = ElementAverageValue
    variable = fp_xy
    block = '0'
  [../]
  [./exy]
    type = ElementAverageValue
    variable = exy
    block = '0'
  [../]
  [./slip_increment]
   type = ElementAverageValue
   variable = slip_increment
   block = '0'
  [../]
  [./dislo_velocity]
   type = ElementAverageValue
   variable = dislo_velocity
   block = '0'
  [../]
  [./strain_xy]
    type = ElementAverageValue
    variable = strain_xy
    block = '0'
  [../]
  [./strain_xx]
    type = ElementAverageValue
    variable = strain_xx
    block = '0'
  [../]
  [./epeq]
    type = ElementAverageValue
    variable = epeq
    block = '0'
  [../]
  [./disp_x]
     type = NodalVariableValue
     variable = disp_x
     nodeid = 101
  [../]
[]

[VectorPostprocessors]
[]

[Outputs]
  exodus = true
  time_step_interval = 400
  [csv]
    type = CSV
    file_base = rhoe_x_out_h5
    execute_on = final
  []
[]
