[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 256
    ny = 25
    xmin = 0.0
    ymin = 0.0
    xmax = 3.95
    ymax = 0.395
    elem_type = 'QUAD4'
  []
[]

[Variables]
  # [edge_dislo_den_pos_1]
  #   order = FIRST
  #   family = LAGRANGE
  # []
  # [edge_dislo_den_neg_1]
  #   order = FIRST
  #   family = LAGRANGE
  # []
  # [edge_dislo_den_pos_2]
  #   initial_condition = 1.0
  # []
  # [edge_dislo_den_neg_2]
  #   initial_condition = 1.0
  # []
[]

[ICs]
  [./edge_dislo_den_pos_1]
    type = FunctionIC
    variable = 'edge_dislo_den_pos_1'
    function = Gaussian
  []
  [./edge_dislo_den_neg_1]
    type = FunctionIC
    variable = 'edge_dislo_den_neg_1'
    function = Gaussian
  []
[]

[Functions]
  [./Gaussian]
    type = ParsedFunction
    # expression = '0.01 * (1 / sigma / sqrt(2.0 * pi) * exp(-((x-mu)^2/(2*sigma^2)))) + 0.5'
    # symbol_names = 'sigma mu'
    # symbol_values = '1.2 250.0'
    expression = '0.01*16000.0 *  exp(-((x-1.975)^2.0  / (2.0 * 0.0079^2.0))) + 8000.0'          # width = 1.0/sqrt(16000) um =  7.9 um
  [../]

  [./Fluctuation]
    type = ParsedFunction
    expression = 'if(x=250, 0.55, 0.5)'
  [../]
[]

[AuxVariables]
  # [rhot]
  #   order = CONSTANT
  #   family = MONOMIAL_VEC
  # []
  # [rhognd]
  #   order = CONSTANT
  #   family = MONOMIAL_VEC
  # []
[]

[Kernels]
  [Edeg_Pos_Time_Deri_1]
    type = TimeDerivative
    variable = edge_dislo_den_pos_1
  []
  [Edge_Pos_Flux_1]
    implicit = false
    type = ConservativeAdvectionSchmid_NoMech_DS_patternKer
    variable = edge_dislo_den_pos_1
    upwinding_type = full
      dislo_character = edge_00
      dislo_sign = positive
      slip_sys_index = 0
  []

  [Edeg_Neg_Time_Deri_1]
    type = TimeDerivative
    variable = edge_dislo_den_neg_1
  []
  [Edge_Neg_Flux_1]
    implicit = false
    type = ConservativeAdvectionSchmid_NoMech_DS_patternKer
    variable = edge_dislo_den_neg_1
    upwinding_type = full
      dislo_character = edge_00
      dislo_sign = negative
      slip_sys_index = 0
  []

  # [Edeg_Pos_Time_Deri_2]
  #   type = TimeDerivative
  #   variable = edge_dislo_den_pos_2
  # []
  # [Edge_Pos_Flux_2]
  #   implicit = false
  #   type = ConservativeAdvectionSchmid_NoMech_DS_patternKer
  #   variable = edge_dislo_den_pos_2
  #   upwinding_type = none
  #     dislo_character = edge_90
  #     dislo_sign = positive
  #     slip_sys_index = 1
  # []

  # [Edeg_Neg_Time_Deri_2]
  #   type = TimeDerivative
  #   variable = edge_dislo_den_neg_2
  # []
  # [Edge_Neg_Flux_2]
  #   implicit = false
  #   type = ConservativeAdvectionSchmid_NoMech_DS_patternKer
  #   variable = edge_dislo_den_neg_2
  #   upwinding_type = none
  #     dislo_character = edge_90
  #     dislo_sign = negative
  #     slip_sys_index = 1
  # []

[]

[DGKernels]

  [dg_edge_pos_1]
    implicit = false
    type = DGAdvectionCoupled_NoMech_DS
    variable = edge_dislo_den_pos_1
      dislo_character = edge_00
      dislo_sign = positive
      slip_sys_index = 0
  []

  [dg_edge_neg_1]
    implicit = false
    type = DGAdvectionCoupled_NoMech_DS
    variable = edge_dislo_den_neg_1
      dislo_character = edge_00
      dislo_sign = negative
      slip_sys_index = 0
  []

  # [dg_edge_pos_2]
  #   implicit = false
  #   type = DGAdvectionCoupled_NoMech_DS
  #   variable = edge_dislo_den_pos_2
  #     dislo_character = edge_90
  #     dislo_sign = positive
  #     slip_sys_index = 1
  # []

  # [dg_edge_neg_2]
  #   implicit = false
  #   type = DGAdvectionCoupled_NoMech_DS
  #   variable = edge_dislo_den_neg_2
  #     dislo_character = edge_90
  #     dislo_sign = negative
  #     slip_sys_index = 1
  # []

[]

[AuxKernels]
  # [rhot]
  #   type = TotalDislocationDensity
  #   variable = rhot
  #   execute_on = timestep_end
  #   rhoe1 = rhoep
  #   rhoe3 = rhoen
  #   initial_total_dislo_den = 16.e3
  # []
  # [rhognd]
  #   type = GNDDislocationDensity
  #   variable = rhognd
  #   execute_on = timestep_end
  #   rhoe1 = rhoep
  #   rhoe3 = rhoen
  #   initial_total_dislo_den = 16.e3
  # []
[]

[Materials]
  [vel]
    type = DisloVelocity_DbSlip_patternMat
    nss = 1
    taualpha = 0.483
    gdot0 = 1.0e6
    f0 = 2.77e-19
    abstemp = 750.0
    boltzmann = 1.38065e-23
    tau0hat = 20.0
    p = 0.2
    q = 1.2
    dlamb = 0.3
    shear_modulus = 45.0e3
    burgers = 0.257e-6
    w1 = 0.0
    w2 = 0.0
      c_backs = 0.4
      c_diffu = 0.3
    edge_dislo_den_pos_1 = edge_dislo_den_pos_1
    edge_dislo_den_neg_1 = edge_dislo_den_neg_1
    # edge_dislo_den_pos_2 = edge_dislo_den_pos_2
    # edge_dislo_den_neg_2 = edge_dislo_den_neg_2
  []
  #  [mat_bc]
  #    type = ParsedMaterial
  #    property_name = mat_bc
  #    coupled_variables = 'rhoep rhoen'
  #    expression = '(rhoep + rhoen) * 1e-5'
  #    outputs = exodus
  #  []
[]

[BCs]
  [./Periodic]
    [./auto_boundary_edge_pos_1_x]
      variable = edge_dislo_den_pos_1
      auto_direction = 'x'
    [../]
    [./auto_boundary_edge_neg_1_x]
      variable = edge_dislo_den_neg_1
      auto_direction = 'x'
    [../]
    [./auto_boundary_edge_pos_1_y]
      variable = edge_dislo_den_pos_1
      auto_direction = 'y'
    [../]
    [./auto_boundary_edge_neg_1_y]
      variable = edge_dislo_den_neg_1
      auto_direction = 'y'
    [../]
    # [./auto_boundary_edge_pos_2]
    #   variable = edge_dislo_den_pos_2
    #   auto_direction = 'y'
    # [../]
    # [./auto_boundary_edge_neg_2]
    #   variable = edge_dislo_den_neg_2
    #   auto_direction = 'y'
    # [../]
  [../]
[]

# Transient (time-dependent) details for simulations go here:
[Executioner]
  type = Transient   # Here we use the Transient Executioner (instead of steady)

  [./TimeIntegrator]
    # type = ImplicitEuler
    # type = BDF2
    # type = CrankNicolson
    # type = ImplicitMidpoint
    # type = LStableDirk2
    # type = LStableDirk3
    # type = LStableDirk4
    # type = AStableDirk4
    #
    # Explicit methods
    type = ExplicitEuler
    # type = ExplicitMidpoint
    # type = Heun
    # type = Ralston
    # type = ExplicitTVDRK2
  [../]

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'lu boomeramg          31'
  line_search = 'none'
  l_max_its = 50
  nl_max_its = 50
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-3
  l_tol = 1e-5

  start_time = 0.0
  end_time = 0.2
  dt = 5.e-7
  dtmin = 1.e-12
[]

[VectorPostprocessors]
  [edge_dislo_den_pos_1]
    type = LineValueSampler
    variable = edge_dislo_den_pos_1
    start_point = '0 0.005 0'
    end_point = '0.01 0.005 0'
    num_points = 41
    sort_by = x
  []
  [edge_dislo_den_neg_1]
    type = LineValueSampler
    variable = edge_dislo_den_neg_1
    start_point = '0 0.005 0'
    end_point = '0.01 0.005 0'
    num_points = 41
    sort_by = x
  []
[]

[Postprocessors]
  [slip_sys_1_pos]
    type = ElementIntegralVariablePostprocessor
    variable = edge_dislo_den_pos_1
  []
  [slip_sys_1_neg]
    type = ElementIntegralVariablePostprocessor
    variable = edge_dislo_den_neg_1
  []
  # [slip_sys_2_pos]
  #   type = ElementIntegralVariablePostprocessor
  #   variable = edge_dislo_den_pos_2
  # []
  # [slip_sys_2_neg]
  #   type = ElementIntegralVariablePostprocessor
  #   variable = edge_dislo_den_neg_2
  # []
[]

[Outputs]
  exodus = true
  time_step_interval = 30
  [csv]
    type = CSV
    file_base = patterning_test_x_out_l1e-1
    execute_on = final
  []
  [num_slip_sys_1_pos]
    type = CSV
    file_base = patterning_num_slip_sys_1_pos_wo_source_out
    show = 'slip_sys_1_pos'
  []
  [num_slip_sys_1_neg]
    type = CSV
    file_base = patterning_num_slip_sys_1_neg_wo_source_out
    show = 'slip_sys_1_neg'
  []
#   [num_slip_sys_2_pos]
#     type = CSV
#     file_base = patterning_num_slip_sys_2_pos_wo_source_out
#     show = 'slip_sys_2_pos'
#   []
#   [num_slip_sys_2_neg]
#     type = CSV
#     file_base = patterning_num_slip_sys_2_neg_wo_source_out
#     show = 'slip_sys_2_neg'
#   []
[]
