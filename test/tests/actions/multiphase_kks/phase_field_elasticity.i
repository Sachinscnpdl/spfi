[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 2
[]

[Variables]
  [disp_x]
  []
[]

[Modules]
  [SPFI]
    [PhaseFieldElasticity]
      phase_names = 'alpha beta'
      phase_base_names = 'C_alpha C_beta'
      phase_elastic_energies = 'fel_alpha fel_beta'
      phase_elasticity_tensors = '10 2 2 10 2 10 4 4 4;
                                  20 3 3 20 3 20 8 8 8'
      phase_eigenstrains = '0.01; 0'
      phase_eigenstrain_prefactors = '1 1'

      switching_functions = 'h_alpha h_beta'
      switching_function_phases = 'alpha beta'
      displacements = disp_x
      global_base_name = mixture
    []
  []
[]

[Materials]
  [switching_functions]
    type = GenericConstantMaterial
    prop_names = 'h_alpha h_beta'
    prop_values = '0.25 0.75'
  []
[]

[Kernels]
  [diffusion]
    type = Diffusion
    variable = disp_x
  []
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = false
[]
