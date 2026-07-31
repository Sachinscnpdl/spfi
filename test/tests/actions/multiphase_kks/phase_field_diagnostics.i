[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 2
[]

[Variables]
  [eta1]
  []
  [eta2]
  []
[]

[Modules]
  [SPFI]
    [PhaseFieldDiagnostics]
      phase_names = 'alpha matrix'
      phase_labels = 'precipitate matrix'
      order_parameters = 'eta1 eta2'
      order_parameter_phases = 'alpha matrix'
      switching_functions = 'h1 h2'
    []
  []
[]

[Materials]
  [switching_functions]
    type = GenericConstantMaterial
    prop_names = 'h1 h2'
    prop_values = '0.25 0.75'
  []
[]

[Kernels]
  [diffusion]
    type = Diffusion
    variable = eta1
  []
  [diffusion_eta2]
    type = Diffusion
    variable = eta2
  []
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = false
[]
