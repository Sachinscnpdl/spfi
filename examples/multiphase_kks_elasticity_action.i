# Action-based six-order-parameter Al-Cu-Ni KKS model with diffusion and elasticity.
# Variables: global (ca, cb), phase compositions (c1*, c2*, c3*),
# order parameters (eta1...eta6), and displacements (disp_x, disp_y).
# Phases: eta1-2 = Al2Cu; eta3-5 = Al3Ni; eta6 = Al-rich matrix.
#
# Moelans et al., PRB 78, 024113 (2008), doi:10.1103/PhysRevB.78.024113:
# interface calibration of kappa, m, L, and gamma.
# Poudel et al., IJSS 300, 112894 (2024), doi:10.1016/j.ijsolstr.2024.112894:
# coupled Al-Cu-Ni KKS diffusion, elasticity, and eigenstrain model.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 34
  ny = 34
  nz = 0
  xmin = 0
  xmax = 600
  ymin = 0
  ymax = 600
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]
[BCs]
  [Periodic]
    [all]
      auto_direction = 'x y'
    []
  []

  [right_x]
    type = DirichletBC
    variable = disp_x
    boundary = right
    value = 0
  []

  [left_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []

  [top_x]
    type = DirichletBC
    variable = disp_x
    boundary = top
    value = 0
  []

  [bottom_x]
    type = DirichletBC
    variable = disp_x
    boundary = bottom
    value = 0
  []
  [right_y]
    type = DirichletBC
    variable = disp_y
    boundary = right
    value = 0
  []

  [left_y]
    type = DirichletBC
    variable = disp_y
    boundary = left
    value = 0
  []

  [top_y]
    type = DirichletBC
    variable = disp_y
    boundary = top
    value = 0
  []

  [bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  []
[]
[AuxVariables]
  [bnds]
  []
  [Energy]
    order = CONSTANT
    family = MONOMIAL
  []

  [gr_ca]
    order = CONSTANT
    family = MONOMIAL
  []

  [gr_cb]
    order = CONSTANT
    family = MONOMIAL
  []

  [von_mises]
    order = CONSTANT
    family = MONOMIAL
  []
  [sigma11]
    order = CONSTANT
    family = MONOMIAL
  []
  [sigma22]
    order = CONSTANT
    family = MONOMIAL
  []
  [sigma12]
    order = CONSTANT
    family = MONOMIAL
  []
  [e11]
    order = CONSTANT
    family = MONOMIAL
  []
  [e12]
    order = CONSTANT
    family = MONOMIAL
  []
  [e22]
    order = CONSTANT
    family = MONOMIAL
  []
  [e33]
    order = CONSTANT
    family = MONOMIAL
  []
[]
[Modules]
  [SPFI]
    [MultiPhaseKKS]
      # Phase table: rows follow phase_names; columns follow global_compositions.
      phase_names = 'al2cu al3ni matrix'
      global_compositions = 'ca cb'
      chemical_potentials = 'wa wb'
      phase_compositions = 'c1a c1b; c2a c2b; c3a c3b'
      # These values define c1a_eq...c3b_eq and initialize the compositions.
      phase_equilibrium_compositions = '0.66 0.01; 0.74 0.25; 0.90 0.05'

      kks_reference_phase = al3ni
      concentration_constraint_phase = matrix

      # Free-energy properties use the same phase order as the table above.
      phase_free_energies = 'F1 F2 F3'
      phase_chemical_free_energies = 'Fch1 Fch2 Fch3'
      phase_additional_free_energies = 'fel1 fel2 fel3'
      # Expressions are in kJ/mol: 1e3 converts kJ to J, then the material
      # property converts J/mol to eV/nm^3.
      chemical_energy_factor = 1.0e3
      free_energy_material_properties = chemical_free_energy_conversion
      phase_chemical_free_energy_expressions = 'chemical_free_energy_conversion*chemical_energy_factor*(-28.95+48*(c1a-c1a_eq)^2+45*(c1b-c1b_eq)^2)
                                                chemical_free_energy_conversion*chemical_energy_factor*(-66.05+49*(c2a-c2a_eq)^2+50*(c2b-c2b_eq)^2)
                                                chemical_free_energy_conversion*chemical_energy_factor*(-48.64+30*(c3a-c3a_eq)^2+36*(c3b-c3b_eq)^2)'
      output_generated_free_energies = true

      # Order-parameter table: every entry maps to the phase at the same position.
      order_parameters = 'eta1 eta2 eta3 eta4 eta5 eta6'
      order_parameter_phases = 'al2cu al2cu al3ni al3ni al3ni matrix'
      switching_functions = 'h1 h2 h3 h4 h5 h6'
      barrier_functions = 'g1 g2 g3 g4 g5 g6'

      interface_mobility = L
      diffusion_mobility = M
      kappa = kappa
      gamma = gamma

      # Region functions map positionally to the listed order parameters.
      initial_region_order_parameters = 'eta1 eta2 eta3 eta4 eta5'
      initial_region_functions = 'r:=sqrt(0.6*(x-200)^2+(y-150)^2);if(r<=80,1,0)
                                  r:=sqrt((x-280)^2+(y-380)^2);if(r<=90,1,0)
                                  r:=sqrt((x-210)^2+(y-490)^2);if(r<=40,1,0)
                                  r:=sqrt((x-420)^2+(y-400)^2);if(r<=50,1,0)
                                  r:=sqrt(0.6*(x-400)^2+(y-200)^2);if(r<=60,1,0)'
      # The Action derives eta6, ca, and cb outside those regions.
      initial_background_order_parameter = eta6

      # KKS barrier in the solved Allen-Cahn equation, additional to mu*f0.
      # Keep at 10 to preserve the original simulation behavior.
      allen_cahn_barrier_height = 10

      # Used only for the Energy output; it does not affect the solution.
      # Energy is a legacy diagnostic because this value differs from 10.
      energy_barrier_height = 1

      boundary_aux_variable = bnds
      energy_aux_variable = Energy
      composition_aux_variables = 'gr_ca gr_cb'
    []

    [PhaseFieldElasticity]
      # Independent elasticity block; tensors follow Poudel et al. (2024),
      # doi:10.1016/j.ijsolstr.2024.112894.
      phase_names = 'al2cu al3ni matrix'
      phase_base_names = 'C_eta1 C_eta345 C_eta6'
      phase_elastic_energies = 'fel1 fel2 fel3'

      # symmetric9 ordering: C11 C12 C13 C22 C23 C33 C44 C55 C66
      # Values are in eV/nm^3:
      #   1 GPa = 6.241509 eV/nm^3
      #   1 eV/angstrom^3 = 1000 eV/nm^3 = 160.218 GPa
      elasticity_fill_method = symmetric9
      phase_elasticity_tensors = '832.44 314.00 275.87 832.44 275.87 811.40 170.62 170.62 181.69;
                                  1409.18 565.49 430.73 1507.68 479.39 1394.29 595.87 541.93 437.02;
                                  483.45 247.42 247.42 483.45 247.42 483.45 120.54 120.54 120.54'

      phase_eigenstrains = '4.29e-2; 4.29e-2; 4.29e-2'
      phase_eigenstrain_prefactors = '-2 -2 -0.01'

      # Switching functions may come from any phase-field formulation.
      switching_functions = 'h1 h2 h3 h4 h5 h6'
      switching_function_phases = 'al2cu al2cu al3ni al3ni al3ni matrix'
      displacements = 'disp_x disp_y'
      global_base_name = global
      output_elastic_energies = true
    []

    [PhaseFieldDiagnostics]
      # Generate one area integral for every order parameter.
      phase_names = 'al2cu al3ni matrix'
      phase_labels = 'al2cu al3ni al'
      order_parameters = 'eta1 eta2 eta3 eta4 eta5 eta6'
      order_parameter_phases = 'al2cu al2cu al3ni al3ni al3ni matrix'
      switching_functions = 'h1 h2 h3 h4 h5 h6'
    []
  []
[]

[Variables]
  # KKS variables are generated by MultiPhaseKKSAction.
  [disp_x]
    scaling = 1.0E-05
  []
  [disp_y]
    scaling = 1.0E-05
  []
[]

[Materials]
  # Unit scaling and kinetic constants
  [scale]
    type = GenericConstantMaterial
    prop_names = 'length_scale energy_scale time_scale'
    # SI conversion factors: m -> nm, J -> eV, and s -> ns
    prop_values = '1e9 6.24150943e18 1.0e9'
  []

  [constants]
    type = GenericConstantMaterial
    prop_names = 'pseudo_L_si pseudo_kappa D M_si      molar_vol'
    prop_values = '12          0.5          1 2.7851e-25 1.0e-5'
  []

  # Moelans (2008), gamma=1.5: kappa=3*sigma*delta/4; m=6*sigma/delta.
  [model_constants]
    type = GenericConstantMaterial
    prop_names = 'sigma   delta gamma'
    prop_values = '0.50 35.0e-09 1.5'
  []

  # Shared J/mol -> eV/nm^3 conversion; current value: 0.000624150943.
  [chemical_energy_density_scale]
    type = ParsedMaterial
    property_name = chemical_free_energy_conversion
    material_property_names = 'length_scale energy_scale molar_vol'
    expression = '(energy_scale/(length_scale)^3)/molar_vol'
  []

  [kappa_isotropy]
    type = ParsedMaterial
    property_name = kappa
    material_property_names = 'length_scale energy_scale sigma delta'
    expression = '(energy_scale/length_scale)*(0.75*sigma*delta)'
  []

  # ACGrGrMulti calls Moelans' barrier m "mu"; it is not GB mobility.
  # Current m: 8.5714286e7 J/m^3 = 0.534986523 eV/nm^3.
  [moelans_barrier_energy]
    type = ParsedMaterial
    property_name = mu
    material_property_names = 'chemical_free_energy_conversion molar_vol sigma delta'
    expression = 'chemical_free_energy_conversion*molar_vol*(6*sigma/delta)'
  []

  [interface_mobility]
    # Poudel (2024): L=(16/3)*m*M/kappa; M_si is atomic mobility.
    # Keep factor_L unchanged to preserve the original simulations.
    type = ParsedMaterial
    property_name = L
    constant_names = 'factor_L'
    constant_expressions = '1.6e19'
    material_property_names = 'length_scale energy_scale time_scale M_si mu kappa'
    expression = '((length_scale)^3/(energy_scale*time_scale))*(16/3)*(mu*M_si/kappa)*factor_L'
  []

  [ch_mobility]
    type = ParsedMaterial
    property_name = M
    material_property_names = 'length_scale energy_scale time_scale M_si M_gb h1 h2 h3 h4 h5 h6'
    expression = '((length_scale)^5/(energy_scale*time_scale))*((h1+h2+h3+h4+h5+h6)*M_si+(h1+h2+h3+h4+h5)*(h1+h2+h3+h4+h5)*M_gb)'
  []

  [M_gb]
    type = ParsedMaterial
    material_property_names = 'M_si'
    property_name = M_gb
    expression = '1000*M_si'
  []
[]

[Physics]
  [SolidMechanics]
    [QuasiStatic]
      displacements = 'disp_x disp_y'
      [all]
        displacements = 'disp_x disp_y'
        base_name = global
        strain = SMALL
        planar_formulation = PLANE_STRAIN
      []
    []
  []
[]

[Kernels]
  # KKS evolution kernels are generated by MultiPhaseKKSAction.
[]

[AuxKernels]
  # KKS diagnostics are generated by MultiPhaseKKSAction.
  [von_mises_kernel]
    type = RankTwoScalarAux
    variable = von_mises
    rank_two_tensor = global_stress
    execute_on = timestep_end
    scalar_type = VonMisesStress
  []
  [matl_sigma11]
    type = RankTwoAux
    rank_two_tensor = global_stress
    index_i = 0
    index_j = 0
    variable = sigma11
  []
  [matl_sigma22]
    type = RankTwoAux
    rank_two_tensor = global_stress
    index_i = 1
    index_j = 1
    variable = sigma22
  []
  [matl_sigma12]
    type = RankTwoAux
    rank_two_tensor = global_stress
    index_i = 0
    index_j = 1
    variable = sigma12
  []
  [matl_e11]
    type = RankTwoAux
    rank_two_tensor = total_strain
    index_i = 0
    index_j = 0
    variable = e11
  []
  [matl_e12]
    type = RankTwoAux
    rank_two_tensor = total_strain
    index_i = 0
    index_j = 1
    variable = e12
  []
  [matl_e22]
    type = RankTwoAux
    rank_two_tensor = total_strain
    index_i = 1
    index_j = 1
    variable = e22
  []
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  # The late-time coupled system needs a larger Krylov space and stronger
  # communication between neighboring ASM subdomains. These values were
  # verified by recovering checkpoint 0050 and converging timestep 51.
  petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_shift_type -ksp_gmres_restart -pc_asm_overlap'
  petsc_options_value = 'asm       ilu          nonzero                    100                2'
  l_max_its = 100
  nl_max_its = 20
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-10
  nl_abs_tol = 1.0e-11
  num_steps = 1000
  # Do not exceed the largest timestep already used by the successful old run.
  dtmax = 1.00E5
  # end_time = 6.00E+20
  [TimeStepper]
    type = IterationAdaptiveDT
    dt = 1.00E5
    cutback_factor = 0.8
    growth_factor = 1.1
    # Current MOOSE counts both nonlinear and accumulated Krylov iterations.
    # These limits retain difficult-but-converged steps after mesh adaptation;
    # failed solves still trigger the independent failure cutback.
    optimal_iterations = 15
    iteration_window = 5
    linear_iteration_ratio = 100
  []
  [Adaptivity]
    interval = 5
    initial_adaptivity = 4
    refine_fraction = 0.9
    coarsen_fraction = 0.1
    max_h_level = 2
    weight_names = 'eta1 eta2 eta3 eta4 eta5 eta6'
    weight_values = '1 1 1 1 1 1'
  []
[]

[Preconditioning]
  active = 'full'
  [full]
    type = SMP
    full = true
  []
  [mydebug]
    type = FDP
    full = true
  []
[]

[Postprocessors]
  # Cumulative elapsed wall time and the increment spent on each completed timestep.
  [total_wall_time]
    type = PerfGraphData
    section_name = Root
    data_type = TOTAL
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [step_wall_time]
    type = ChangeOverTimePostprocessor
    postprocessor = total_wall_time
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[Outputs]
  # Use SPFI's colored solver console instead of MOOSE's default console.
  console = false
  file_base = outputs/multiphase_kks_elasticity_action
  checkpoint = true
  [spfi_console]
    type = SPFIConsole
    time_step_interval = 20
    execute_on = 'INITIAL TIMESTEP_BEGIN FAILED TIMESTEP_END'
    execute_postprocessors_on = 'INITIAL TIMESTEP_END'
    execute_scalars_on = 'INITIAL TIMESTEP_END'
  []
  [spfi_residuals]
    type = SPFIResidualOutput
    time_step_interval = 20
  []
  [variable_residuals]
    type = VariableResidualNormsDebugOutput
    time_step_interval = 20
  []
  [exodus]
    type = Exodus
    time_step_interval = 1
  []
  [csv]
    type = CSV
    time_step_interval = 1
  []
[]
