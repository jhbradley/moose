[Mesh]
  # Mesh block.  Meshes can be read in or automatically generated
  type = GeneratedMesh
  dim = 2 # Problem dimension
  nx = 12 # Number of elements in the x-direction
  ny = 12 # Number of elements in the y-direction
  xmin = 0 # minimum x-coordinate of the mesh
  xmax = 1000 # maximum x-coordinate of the mesh
  ymin = 0 # minimum y-coordinate of the mesh
  ymax = 1000 # maximum y-coordinate of the mesh
  elem_type = QUAD4 # Type of elements used in the mesh
  uniform_refine = 3 # Initial uniform refinement of the mesh
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
[]

#No need to change anything here because the defaults are what we need
[Variables]
  [./PolycrystalVariables]
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalCircleGrainIC]
      radius = 250
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
[]

[BCs]
  [./Periodic]
    [./top_bottom]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./CuGrGr]
    type = GBEvolution
    block = 0
    T = 450 # K
    wGB = 14 # nm
    GBmob0 = 2.5e-6 #m^4/(Js) from Schoenfelder 1997
    Q = 0.23 #Migration energy in eV
    GBenergy = 0.708 #GB energy in J/m^2 (this energy is the default in Hanesn's file)
    molar_volume = 7.11e-6 #Need to override the default to match Hansen's file
  [../]
[]

#I don't think we have to change anything here
[Postprocessors]
  [./gr1area]
    type = ElementIntegralVariablePostprocessor
    variable = gr1
  [../]
[]

[Postprocessors]
  # Scalar postprocessors
# [./ngrains]
  #Counts the number of grains in the polycrystal
#type = FeatureFloodCount
#   variable = bnds
#   threshold = 0.7
# [../]
  [./grainArea]
    type = ElementIntegralVariablePostprocessor
    variable = gr0
  [../]
  [./dt]
    # Outputs the current time step
    type = TimestepSize
  [../]
[]

[Executioner]
  type = Transient # Type of executioner, here it is transient with an adaptive time step
  scheme = bdf2 # Type of time integration (2nd order backward euler), defaults to 1st order backward euler

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart -mat_mffd_type'
  petsc_options_value = 'hypre boomeramg 101 ds'
  l_max_its = 30 # Max number of linear iterations
  l_tol = 1e-4 # Relative tolerance for linear solves
  nl_max_its = 40 # Max number of nonlinear iterations
  nl_abs_tol = 1e-11 # Relative tolerance for nonlienar solves
  nl_rel_tol = 1e-8 # Absolute tolerance for nonlienar solves
  start_time = 0.0
  end_time = 8000
  dt = 25
  [./Adaptivity]
    # Block that turns on mesh adaptivity. Note that mesh will never coarsen beyond initial mesh (before uniform refinement)
    initial_adaptivity = 2 # Number of times mesh is adapted to initial condition
    refine_fraction = 0.7 # Fraction of high error that will be refined
    coarsen_fraction = 0.1 # Fraction of low error that will coarsened
    max_h_level = 4 # Max number of refinements used, starting from initial mesh (before uniform refinement)
  [../]
[]

[Outputs]
  file_base = circle_2D_isotropic2
  output_initial = true
  exodus = true
  csv = true
  print_perf_log = true
  [./console]
    type = Console
    max_rows = 20
  [../]
[]
