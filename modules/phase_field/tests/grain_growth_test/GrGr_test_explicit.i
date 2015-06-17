[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  nz = 0
  xmin = 0
  xmax = 1000
  ymin = 0
  ymax = 1000
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 2
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  implicit = false
[]

[Variables]
  [./PolycrystalVariables]
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalCircleGrainIC]
      radius = 333.333
      x = 500
      y = 500
    [../]
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
  [./total_free_energy]
    order = CONSTANT
    family = MONOMIAL  #I'm not totally sure if this should be a constant monomial
  [../]
[]

[Kernels]
  [./PolycrystalKernel]
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]
  [./total_free_energy]
    type = TotalFreeEnergy
    variable = total_free_energy
    kappa_names = 'kappa_op kappa_op' #These are the default names for this material property
    interfacial_vars = 'gr0 gr1'
    execute_on = 'initial timestep_end'
  [../]
[]

[Materials]
  [./Copper]
    type = GBEvolution
    block = 0
    T = 500 # K
    wGB = 60 # nm
    GBmob0 = 2.5e-6 #m^4/(Js) from Schoenfelder 1997
    Q = 0.23 #Migration energy in eV
    GBenergy = 0.708 #GB energy in J/m^2
  [../]
  [./grain0]
    type = ParsedMaterial
    block = 0
    args = 'gr0 total_free_energy'
    function = 'total_free_energy*(gr0/gr0-gr0)'
    outputs = exodus
  [../]
  [./grain1]
    type = ParsedMaterial
    block = 0
    args = 'gr1 total_free_energy'                  #Am I creating or specifying a variable here?
    function = 'total_free_energy*(gr1/gr1-gr1)'
    outputs = exodus
  [../]
[]

[Functions]
  [./avg_GBenergy]
    type = ParsedFunction
    value = 2*t#'total_free_energy*grain1*grain0'
  [../]
[]

[Postprocessors]
  [./gr1area]
    type = ElementIntegralVariablePostprocessor
    variable = gr1
    execute_on = 'initial timestep_end'
  [../]
  [./average_GBenergy]
    type = PlotFunction
    function = 'avg_GBenergy'
    execute_on = 'initial timestep_end'
  [../]
  #[./grain1]                                             #This doesn't work
  #  type = ElementIntegralVariablePostprocessor
  #  variable = grain1
  #[../]
  #[./grain0]
  #  type = ElementIntegralVariablePostprocessor
  #  variable = grain0
  #[../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  #l_max_its = 3
  petsc_options_iname = -pc_type
  petsc_options_value = bjacobi
  type = Transient
  scheme = explicit-euler
  solve_type = NEWTON
  l_tol = 1.0e-6
  nl_rel_tol = 1.0e-6
  num_steps = 3
  dt = 1
[]

[Outputs]
  file_base = explicit
  output_initial = true
  output_final = true
  csv = true
  #interval = 20
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]

[Problem]
  use_legacy_uo_initialization = false
[]
