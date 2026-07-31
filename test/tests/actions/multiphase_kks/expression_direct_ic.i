[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 2
[]

[Modules]
  [SPFI]
    [MultiPhaseKKS]
      phase_names = 'alpha beta'
      phase_free_energies = 'F_alpha F_beta'
      phase_compositions = 'c_alpha; c_beta'
      phase_chemical_free_energies = 'F_alpha F_beta'
      phase_equilibrium_compositions = '0.2; 0.8'
      chemical_energy_factor = 2
      phase_chemical_free_energy_expressions = 'chemical_energy_factor*(c_alpha-c_alpha_eq)^4
                                                chemical_energy_factor*(c_beta-c_beta_eq)^2'

      global_compositions = c
      chemical_potentials = w
      order_parameters = 'eta_alpha eta_beta'
      order_parameter_phases = 'alpha beta'
      switching_functions = 'h_alpha h_beta'
      barrier_functions = 'g_alpha g_beta'

      # Direct mode accepts any number of variables and arbitrary expressions.
      initial_condition_variables = 'eta_alpha eta_beta c'
      initial_condition_expressions = 'if(x<0.5,1,0) if(x<0.5,0,1) if(x<0.5,0.2,0.8)'

      kks_reference_phase = alpha
      interface_mobility = L
      diffusion_mobility = M
      kappa = kappa
      gamma = gamma
    []
  []
[]

[Materials]
  [constants]
    type = GenericConstantMaterial
    prop_names = 'L M kappa gamma mu'
    prop_values = '1 1 1 1 1'
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
