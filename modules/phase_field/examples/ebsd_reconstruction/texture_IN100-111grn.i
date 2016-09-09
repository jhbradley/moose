# This simulation demonstrates the ability of GrainTextureVectorPostprocessor to
# pull out information from the grain system that can then be used for texture
# analysis.

[Mesh]
  uniform_refine = 2
  type = EBSDMesh
  filename = IN100_128x128.txt
[]

[GlobalParams]
  op_num = 8
  var_name_base = gr
  length_scale = 1.0e-6
  time_scale = 1.0e-6
[]

[Variables]
  [./PolycrystalVariables]
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
  [./unique_grains]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./var_indices]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./ebsd_grains]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./phi1_aux_var]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./Phi_aux_var]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./phi2_aux_var]
    family = MONOMIAL
    order = CONSTANT
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./ReconVarIC]
      ebsd_reader = ebsd
      advanced_op_assignment = true
    [../]
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
    execute_on = 'initial timestep_end'
  [../]
  [./var_indices]
    type = FeatureFloodCountAux
    variable = var_indices
    execute_on = 'initial timestep_end'
    flood_counter = grain_tracker
    field_display = VARIABLE_COLORING
  [../]
  [./unique_grains]
    type = FeatureFloodCountAux
    variable = unique_grains
    execute_on = 'initial timestep_end'
    flood_counter = grain_tracker
    field_display = UNIQUE_REGION
  [../]
  [./grain_aux]
    type = EBSDReaderPointDataAux
    variable = ebsd_grains
    ebsd_reader = ebsd
    data_name = 'feature_id'
    execute_on = 'initial timestep_end'
  [../]
  [./phi1_aux]
    type = EBSDReaderPointDataAux
    variable = phi1_aux_var
    ebsd_reader = ebsd
    data_name = 'phi1'
    execute_on = 'initial timestep_end'
  [../]
  [./Phi_aux]
    type = EBSDReaderPointDataAux
    variable = Phi_aux_var
    ebsd_reader = ebsd
    data_name = 'phi'
    execute_on = 'initial timestep_end'
  [../]
  [./phi2_aux]
    type = EBSDReaderPointDataAux
    variable = phi2_aux_var
    ebsd_reader = ebsd
    data_name = phi2
    execute_on = 'initial timestep_end'
  [../]
[]

[Modules]
  [./PhaseField]
    [./EulerAngles2RGB]
      crystal_structure = cubic
      euler_angle_provider = ebsd
      grain_tracker = grain_tracker
    [../]
  [../]
[]

[Materials]
  [./Copper]
    type = GBEvolution
    T = 500                 # K
    wGB = 0.6               # um
    GBmob0 = 2.5e-6         # m^4/(Js) from Schoenfelder 1997
    Q = 0.23                # Migration energy in eV
    GBenergy = 0.708        # GB energy in J/m^2
    molar_volume = 7.11e-6  # Molar volume in m^3/mol
  [../]
[]

[UserObjects]
  [./ebsd]
    type = EBSDReader
  [../]
  [./grain_tracker]
    type = GrainTracker
    threshold = 0.2
    connecting_threshold = 0.2
    execute_on = 'initial timestep_begin'
    compute_op_maps = true
    ebsd_reader = ebsd
    flood_entity_type = ELEMENTAL
    outputs = none
  [../]
[]

[VectorPostprocessors]
  [./gbInfo]
    type = GrainTextureVectorPostprocessor
    max_refinement_level = 0
    index_by = x-index-fastest
    variable = 'unique_grains ebsd_grains phi1_aux_var Phi_aux_var phi2_aux_var'
    sort_by = id # This will sort by the global index, which is what we want
  [../]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = PJFNK

  petsc_options_iname = '-pc_type -pc_hypre_type -pc_hypre_boomeramg_strong_threshold'
  petsc_options_value = 'hypre    boomeramg      0.7'

  l_tol = 1.0e-4
  l_max_its = 20
  nl_max_its = 20
  nl_rel_tol = 1.0e-8

  start_time = 0.0
  num_steps = 2

  [./TimeStepper]
    type = IterationAdaptiveDT
    cutback_factor = 0.9
    dt = 10.0
    growth_factor = 1.1
    optimal_iterations = 7
  [../]

  [./Adaptivity]
    initial_adaptivity = 2
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
  [../]
[]

[Outputs]
  print_perf_log = true
  csv = true
[]
