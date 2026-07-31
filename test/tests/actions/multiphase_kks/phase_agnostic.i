[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 2
[]

[AuxVariables]
  [weighted_c_a]
    order = CONSTANT
    family = MONOMIAL
  []
  [weighted_c_b]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Modules]
  [SPFI]
    [MultiPhaseKKS]
      phase_names = 'alpha beta'
      phase_free_energies = 'F_alpha F_beta'
      phase_compositions = 'c_alpha_a c_alpha_b; c_beta_a c_beta_b'
      phase_chemical_free_energies = 'F_alpha F_beta'
      phase_equilibrium_compositions = '0.25 0.15; 0.75 0.65'
      phase_chemical_free_energy_expressions = 'energy_scale*((c_alpha_a-c_alpha_a_eq)^4+(c_alpha_b-c_alpha_b_eq)^2)
                                                energy_scale*((c_beta_a-c_beta_a_eq)^2+(c_beta_b-c_beta_b_eq)^6)'
      free_energy_material_properties = energy_scale

      global_compositions = 'c_a c_b'
      chemical_potentials = 'w_a w_b'

      order_parameters = 'eta1 eta2 eta3'
      order_parameter_phases = 'alpha beta beta'
      switching_functions = 'h1 h2 h3'
      barrier_functions = 'g1 g2 g3'

      initial_region_functions = 'if(x<0.5,1,0)'
      initial_region_order_parameters = eta1
      initial_background_order_parameter = eta2

      kks_reference_phase = alpha
      interface_mobility = L
      diffusion_mobility = M
      kappa = kappa
      gamma = gamma

      composition_aux_variables = 'weighted_c_a weighted_c_b'
    []
  []
[]

[Materials]
  [constants]
    type = GenericConstantMaterial
    prop_names = 'L M kappa gamma mu length_scale energy_scale molar_vol'
    prop_values = '1 1 1 1 1 1 1 1'
  []
[]

[Executioner]
  type = Transient
  num_steps = 1
  dt = 0.01
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Outputs]
  exodus = false
[]
