# apply many large deformations, checking that the algorithm returns correctly to
# the yield surface each time


[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 1
  xmin = -0.5
  xmax = 0.5
  ymin = -0.5
  ymax = 0.5
  zmin = -0.5
  zmax = 0.5
[]


[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[Kernels]
  [./TensorMechanics]
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
[]


[BCs]
  [./bottomx]
    type = PresetBC
    variable = disp_x
    boundary = back
    value = 0.0
  [../]
  [./bottomy]
    type = PresetBC
    variable = disp_y
    boundary = back
    value = 0.0
  [../]
  [./bottomz]
    type = PresetBC
    variable = disp_z
    boundary = back
    value = 0.0
  [../]

  [./topx]
    type = FunctionPresetBC
    variable = disp_x
    boundary = front
    function = '(sin(0.05*t)+x)/1E0'
  [../]
  [./topy]
    type = FunctionPresetBC
    variable = disp_y
    boundary = front
    function = '(cos(0.04*t)+x*y)/1E0'
  [../]
  [./topz]
    type = FunctionPresetBC
    variable = disp_z
    boundary = front
    function = 't/1E2'
  [../]
[]

[AuxVariables]
  [./yield_fcn]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./yield_fcn_auxk]
    type = MaterialStdVectorAux
    index = 0
    property = plastic_yield_function
    variable = yield_fcn
  [../]
[]

[Postprocessors]
  [./yield_fcn_at_zero]
    type = PointValue
    point = '0 0 0'
    variable = yield_fcn
    outputs = 'console'
  [../]
  [./should_be_zero]
    type = PlotFunction
    function = should_be_zero_fcn
  [../]
[]

[Functions]
  [./should_be_zero_fcn]
    type = ParsedFunction
    value = 'if(a<1E-3,0,a)'
    vars = 'a'
    vals = 'yield_fcn_at_zero'
  [../]
[]

[UserObjects]
  [./mc]
    type = TensorMechanicsPlasticMohrCoulombExponential
    mc_cohesion = 1E3
    mc_friction_angle = 30
    mc_dilation_angle = 5
    tip_scheme = cap
    mc_tip_smoother = 0.0
    cap_start = 1000
    cap_rate = 1E-3
    mc_edge_smoother = 10
    yield_function_tolerance = 1E-3
    internal_constraint_tolerance = 1E-6
  [../]
[]

[Materials]
  [./mc]
    type = FiniteStrainMultiPlasticity
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    fill_method = symmetric_isotropic
    C_ijkl = '0 1E7'
    max_NR_iterations = 1000
    ep_plastic_tolerance = 1E-6
    plastic_models = mc
    debug_fspb = 1
    deactivation_scheme = safe
  [../]
[]


[Executioner]
  end_time = 1000
  dt = 1
  type = Transient
[]


[Outputs]
  file_base = many_deforms_cap
  output_initial = true
  exodus = false
  [./console]
    type = Console
    perf_log = true
    linear_residuals = false
  [../]
  [./csv]
    type = CSV
    interval = 1
  [../]
[]
