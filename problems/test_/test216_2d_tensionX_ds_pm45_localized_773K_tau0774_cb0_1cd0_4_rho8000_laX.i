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
    nx = 40
    ny = 40
    xmin = 0.0
    ymin = 0.0
    xmax = 1.011
    ymax = 1.011
    elem_type = 'QUAD9'   #QUAD4
  []
[]

[Variables]
  [disp_x]
    order = SECOND
      family = LAGRANGE
  []
  [disp_y]
    order = SECOND
      family = LAGRANGE
  []
  [rho_edge_pos_1]
    order = FIRST
    family = LAGRANGE
    block = '0'
    # initial_condition = 4000.0
  []
  [rho_edge_neg_1]
    order = FIRST
    family = LAGRANGE
    block = '0'
    # initial_condition = 4000.0
  []
    [rho_edge_pos_2]
    order = FIRST
    family = LAGRANGE
    # initial_condition = 4000.0
    block = '0'
  []
  [rho_edge_neg_2]
    order = FIRST
    family = LAGRANGE
    # initial_condition = 4000.0
    block = '0'
  []
[]

[ICs]
  [./rho_edge_pos_1]
    type = FunctionIC
    variable = 'rho_edge_pos_1'
    function = Gaussian_mxy
  []
  [./rho_edge_neg_1]
    type = FunctionIC
    variable = 'rho_edge_neg_1'
    function = Gaussian_mxy
  []
  [./rho_edge_pos_2]
    type = FunctionIC
    variable = 'rho_edge_pos_2'
    function = Gaussian_pxy
  []
  [./rho_edge_neg_2]
    type = FunctionIC
    variable = 'rho_edge_neg_2'
    function = Gaussian_pxy
  []  

  # [./rho_edge_pos_1]
  #   type = RandomIC
  #   variable = 'rho_edge_pos_1'
  #   min = ${fparse 4000.0 - 4000.0*1.732*0.01}
  #   max = ${fparse 4000.0 + 4000.0*1.732*0.01}
  #   seed = 128
  # []
  # [./rho_edge_neg_1]
  #   type = RandomIC
  #   variable = 'rho_edge_neg_1'
  #   min = ${fparse 4000.0 - 4000.0*1.732*0.01}
  #   max = ${fparse 4000.0 + 4000.0*1.732*0.01}
  #   seed = 128
  # []
  # [./rho_edge_pos_2]
  #   type = RandomIC
  #   variable = 'rho_edge_pos_2'
  #   min = ${fparse 4000.0 - 4000.0*1.732*0.01}
  #   max = ${fparse 4000.0 + 4000.0*1.732*0.01}
  #   seed = 128
  # []
  # [./rho_edge_neg_2]
  #   type = RandomIC
  #   variable = 'rho_edge_neg_2'
  #   min = ${fparse 4000.0 - 4000.0*1.732*0.01}
  #   max = ${fparse 4000.0 + 4000.0*1.732*0.01}
  #   seed = 128
  # []  

[]

[Functions]
  [./Gaussian_x]
    type = ParsedFunction
    expression = 'if(abs(x - 1.011) < 0.0079, 0.001*16000.0 * exp(-((x - 1.011)^2 / (2.0 * 0.0079^2))) + 4000.0, 4000.0)'
  [../]

  [./Gaussian_pxy]
    type = ParsedFunction
    #rho0=8000.0, l_md= 1.1um; rho0=16000.0, l_md=7.9um
    expression = '0.01*4000.0 * exp(-((x - y)^2 / (4*0.0011^2))) + 4000.0'
  [../]

  [./Gaussian_mxy]
    type = ParsedFunction
    #rho0=8000.0, l_md= 1.1um; rho0=16000.0, l_md=7.9um
    expression = '0.01*4000.0 * exp(-((x + y - 1.011)^2 / (4*0.0011^2))) + 4000.0'
  [../]

  [./disp_load]
    type = ParsedFunction
    expression = '0.1*1.011*t'
  [../]

  [./pressue_piecewise]
    type = PiecewiseLinear
    x = '0.0 1.0e-5  0.3'
    y = '0.0 1.0     1.0'
  [../]
[]

[BCs]
  [right_x]
    type = Pressure
    variable = disp_x
    boundary = 'right'
    factor = -0.744 #-0.683 -0.966
    function = pressue_piecewise
  []
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
  [./Periodic]
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

  [./slip_increment1]
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
  [./slip_increment2]
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]   

  [./rss_1]                 #Name shown in Paraview
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]

  [./rss_2]                 #Name shown in Paraview
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
  
  [./taub_1]                 #Name shown in Paraview
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
  [./taub_2]                 #Name shown in Paraview
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
  [./taud_1]                 #Name shown in Paraview
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
  [./taud_2]                 #Name shown in Paraview
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
  [./sT_1]                 #Name shown in Paraview
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]
    [./sT_2]                 #Name shown in Paraview
   order = CONSTANT
   family = MONOMIAL
   block = '0'
  [../]

  [./euler_angle_1]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./euler_angle_2]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./euler_angle_3]
    order = CONSTANT
    family = MONOMIAL
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
  [./slip_increment1]
   type = MaterialStdVectorAux
   variable = slip_increment1
   property = slip_increment
   index = 0
   execute_on = timestep_end
   block = '0'
  [../]

  [./slip_increment2]
   type = MaterialStdVectorAux
   variable = slip_increment2
   property = slip_increment
   index = 1
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

  [./rss_1]                     #Name arbiarbitrary
   type = MaterialStdVectorAux  #Use this for std::vector p
   variable = rss_1             #Consistant with AuxVaribles
   property = rss_qc         
   index = 0
   block = '0'
  [../]
  [./rss_2]                     #Name arbiarbitrary
   type = MaterialStdVectorAux  #Use this for std::vector p
   variable = rss_2             #Consistant with AuxVaribles
   property = rss_qc         
   index = 1
   block = '0'
  [../]

  [./taub_1]                     #Name arbiarbitrary
   type = MaterialStdVectorAux  #Use this for std::vector p
   variable = taub_1             #Consistant with AuxVaribles
   property = taub_qc         
   index = 0
   block = '0'
  [../] 
  [./taub_2]                     #Name arbiarbitrary
   type = MaterialStdVectorAux  #Use this for std::vector p
   variable = taub_2             #Consistant with AuxVariables
   property = taub_qc         
   index = 1
   block = '0'
  [../]
  [./taud_1]                     #Name arbiarbitrary
   type = MaterialStdVectorAux  #Use this for std::vector p
   variable = taud_1             #Consistant with AuxVariables
   property = taud_qc         
   index = 0
   block = '0'
  [../] 
  [./taud_2]                     #Name arbiarbitrary
   type = MaterialStdVectorAux  #Use this for std::vector p
   variable = taud_2             #Consistant with AuxVariables
   property = taud_qc         
   index = 1
   block = '0'
  [../]
  [./sT_1]                     #Name arbiarbitrary
   type = MaterialStdVectorAux  #Use this for std::vector p
   variable = sT_1             #Consistant with AuxVariables
   property = sT_qc         
   index = 0
   block = '0'
  [../] 
    [./sT_2]                     #Name arbiarbitrary
   type = MaterialStdVectorAux  #Use this for std::vector p
   variable = sT_2             #Consistant with AuxVariables
   property = sT_qc         
   index = 1
   block = '0'
  [../]

  [./euler_angle_1]
    type = MaterialRealVectorValueAux
    variable = euler_angle_1
    property = Euler_angles
    component = 0
    execute_on = timestep_end
  [../]
  [./euler_angle_2]
    type = MaterialRealVectorValueAux
    variable = euler_angle_2
    property = Euler_angles
    component = 1
    execute_on = timestep_end
  [../]
  [./euler_angle_3]
    type = MaterialRealVectorValueAux
    variable = euler_angle_3
    property = Euler_angles
    component = 2
    execute_on = timestep_end
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeElasticityTensorCP
    C_ijkl = '184.33e3 124.70e3 124.70e3 184.33e3 124.70e3 184.33e3 88.23e3 88.23e3 88.23e3'
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
    slip_sys_file_name  = input_dslip_sys_al_pm45.txt
      gdot0             = 1.0e6
      f0                = 2.77e-19
      boltzmann         = 1.38065e-23
      temperature       = 500.0                                         # 750 - 273.15 in degrees Celsius
      tau_0             = 20.0
      p                 = 0.2
      q                 = 1.2
      dlamb             = 0.3
      shear_modulus     = 45.0e3
      burgers           = 0.257e-6
        w1              = 0.0
        w2              = 0.0
      scaling_Cb        = 0.1
      scaling_Cd        = 0.4
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
  line_search = 'l2'  #'bt' 'l2' 'basic' 'nleqerr'
  l_max_its = 50
  nl_max_its = 50
  nl_rel_tol = 1e-3 #1e-5
  nl_abs_tol = 1e-3
  l_tol = 1e-3  #1e-5 

  start_time = 0.0
  end_time = 0.2 #0.01
  dt = 2.e-7
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
  [./slip_increment1]
   type = ElementAverageValue
   variable = slip_increment1
   block = '0'
  [../]
    [./slip_increment2]
   type = ElementAverageValue
   variable = slip_increment2
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
     nodeid = 1 #101
  [../]
  [./rss_1]
     type = ElementAverageValue
     variable = rss_1
     block = '0'
  [../]
  [./rss_2]
     type = ElementAverageValue
     variable = rss_2
     block = '0'
  [../]

  [./taub_1]
     type = ElementAverageValue
     variable = taub_1
     block = '0'
  [../]
  [./taub_2]
     type = ElementAverageValue
     variable = taub_2
     block = '0'
  [../]
  [./taud_1]
     type = ElementAverageValue
     variable = taud_1
     block = '0'
  [../]
  [./taud_2]
     type = ElementAverageValue
     variable = taud_2
     block = '0'
  [../]
  [./sT_1]
     type = ElementAverageValue
     variable = sT_1
     block = '0'
  [../]
    [./sT_2]
     type = ElementAverageValue
     variable = sT_2
      block = '0'
  [../]
[]

[VectorPostprocessors]
[]

[Outputs]
  exodus = true
  time_step_interval = 200
  [csv]
    type = CSV
    file_base = rhoe_x_out_h5
    execute_on = final
  []
[]
