
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [generated]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 2
    ny = 20
    xmax = 1.0
    ymax = 1.0
  []
  [center_grain]
    type = ParsedSubdomainMeshGenerator
    input = generated
    combinatorial_geometry = 'x > 0.3 & x < 0.7 & y > 0.3 & y < 0.7'
    block_id = '1'
  []
  # Remaining area is block 0 (the 4 surrounding grains)
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]


[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
  []
[]

[AuxVariables]
  [euler_angle_1]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[BCs]
  [Periodic]
    [all]
      variable = 'disp_x disp_y'
      auto_direction = 'x y'
    []
  []
  [ext_force]
    type = ADDirichletBC
    variable = disp_y
    boundary = top
    value = 0.05 # 5% strain
  []
[]

[Materials]
[./center]
  [elasticity]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1e9
    poissons_ratio = 0.3
  []
  [stress]
    type = ComputeLinearElasticStress
  []
  block =  '0'
[../]
[./outers]
  [elasticity]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2e9
    poissons_ratio = 0.3
  []
  [stress]
    type = ComputeLinearElasticStress
  []
  block = '1'
[../]
[]

[Executioner]
  type = Transient
  end_time = 5
  dt = 1
[]

[Outputs]
  exodus = true
[]