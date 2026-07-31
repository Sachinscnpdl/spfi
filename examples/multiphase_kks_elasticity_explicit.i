# Fully explicit reference for multiphase_kks_elasticity_action.i.
# It lists every generated KKS and elasticity object for regression checks.
# Phases: eta1-2 = Al2Cu; eta3-5 = Al3Ni; eta6 = Al-rich matrix.
#
# Moelans et al., PRB 78, 024113 (2008), doi:10.1103/PhysRevB.78.024113:
# interface calibration. Poudel et al., IJSS 300, 112894 (2024),
# doi:10.1016/j.ijsolstr.2024.112894: Al-Cu-Ni elastochemical model.

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
[Variables]
  [wa]
    order = FIRST
    family = LAGRANGE
    scaling = 1.0E6
  []

  [wb]
    order = FIRST
    family = LAGRANGE
    scaling = 1.0E6
  []

  [ca]
    order = FIRST
    family = LAGRANGE
  []

  [cb]
    order = FIRST
    family = LAGRANGE
  []

  [c1a]
    order = FIRST
    family = LAGRANGE
  []
  [c2a]
    order = FIRST
    family = LAGRANGE
  []
  [c3a]
    order = FIRST
    family = LAGRANGE
  []
  [c1b]
    order = FIRST
    family = LAGRANGE
  []
  [c2b]
    order = FIRST
    family = LAGRANGE
  []
  [c3b]
    order = FIRST
    family = LAGRANGE
  []
  [eta1]
    order = FIRST
    family = LAGRANGE
  []
  [eta2]
    order = FIRST
    family = LAGRANGE
  []
  [eta3]
    order = FIRST
    family = LAGRANGE
  []
  [eta4]
    order = FIRST
    family = LAGRANGE
  []
  [eta5]
    order = FIRST
    family = LAGRANGE
  []
  [eta6]
    order = FIRST
    family = LAGRANGE
  []
  [disp_x]
    scaling = 1.0E-05
  []
  [disp_y]
    scaling = 1.0E-05
  []
[]

[ICs]

  [eta1]
    variable = eta1
    type = FunctionIC
    function = 'r:=sqrt(0.6*(x-200)^2+(y-150)^2);if(r<=80,1,0)'
  []
  [eta2]
    variable = eta2
    type = FunctionIC
    function = 'r:=sqrt((x-280)^2+(y-380)^2);if(r<=90,1,0)'
  []
  [eta3]
    variable = eta3
    type = FunctionIC
    function = 'r:=sqrt((x-210)^2+(y-490)^2);if(r<=40,1,0)'
  []
  [eta4]
    variable = eta4
    type = FunctionIC
    function = 'r:=sqrt((x-420)^2+(y-400)^2);if(r<=50,1,0)'
  []
  [eta5]
    variable = eta5
    type = FunctionIC
    function = 'r:=sqrt(0.6*(x-400)^2+(y-200)^2);if(r<=60,1,0)'
  []

  [eta6]
    variable = eta6
    type = FunctionIC
    function = 'r1:=sqrt(0.6*(x-200)^2+(y-150)^2); r2:=sqrt((x-280)^2+(y-380)^2); r3:=sqrt((x-210)^2+(y-490)^2);r4:=sqrt((x-420)^2+(y-400)^2);r5:= sqrt(0.6*(x-400)^2+(y-200)^2);  if(r1<=80,0,if(r2<=90,0,if(r3<=40,0,if(r4<=50,0,if(r5<=60,0,1)))))'
  []
  [ca]
    variable = ca
    type = FunctionIC
    function = 'r1:=sqrt(0.6*(x-200)^2+(y-150)^2); r2:=sqrt((x-280)^2+(y-380)^2); r3:=sqrt((x-210)^2+(y-490)^2);r4:=sqrt((x-420)^2+(y-400)^2);r5:= sqrt(0.6*(x-400)^2+(y-200)^2);  if(r1<=80,0.66,if(r2<=90,0.66,if(r3<=40,0.74,if(r4<=50,0.74,if(r5<=60,0.74,0.9)))))'
  []

  [cb]
    variable = cb
    type = FunctionIC
    function = 'r1:=sqrt(0.6*(x-200)^2+(y-150)^2); r2:=sqrt((x-280)^2+(y-380)^2); r3:=sqrt((x-210)^2+(y-490)^2);r4:=sqrt((x-420)^2+(y-400)^2);r5:= sqrt(0.6*(x-400)^2+(y-200)^2);  if(r1<=80,0.01,if(r2<=90,0.01,if(r3<=40,0.25,if(r4<=50,0.25,if(r5<=60,0.25,0.05)))))'
  []
[]

[Materials]
  # Chemical free energies
  [fch3]
    type = DerivativeParsedMaterial
    property_name = Fch3
    constant_names = 'factor_f3'
    constant_expressions = '1.0E+03'
    material_property_names = 'length_scale energy_scale molar_vol'
    coupled_variables = 'c3a c3b'
    expression = '(energy_scale/(length_scale)^3) *(-48.64 + 30*(c3a-0.9)^2+ 36*(c3b-0.05)^2 )*factor_f3/molar_vol'

    outputs = exodus
  []
  [fch1]
    type = DerivativeParsedMaterial
    property_name = Fch1
    constant_names = 'factor_f1'
    constant_expressions = '1.0E+03'
    material_property_names = 'length_scale energy_scale molar_vol'
    coupled_variables = 'c1a c1b'
    expression = '(energy_scale/(length_scale)^3) *(-28.95 + 48*(c1a-0.66)^2+ 45*(c1b-0.01)^2)*factor_f1/molar_vol'

    outputs = exodus
  []
  [fch2]
    type = DerivativeParsedMaterial
    property_name = Fch2
    constant_names = 'factor_f2'
    constant_expressions = '1.0E+03'
    material_property_names = 'length_scale energy_scale molar_vol'
    coupled_variables = 'c2a c2b'
    expression = '(energy_scale/(length_scale)^3) *(-66.05 + 49*(c2a-0.74)^2+ 50*(c2b-0.25)^2)*factor_f2/molar_vol'
    outputs = exodus
  []
  # Switching and barrier functions
  [h1]
    type = SwitchingFunctionMultiPhaseMaterial
    h_name = h1
    all_etas = 'eta1 eta2 eta3 eta4 eta5 eta6'
    phase_etas = eta1
  []
  [h2]
    type = SwitchingFunctionMultiPhaseMaterial
    h_name = h2
    all_etas = 'eta1 eta2 eta3 eta4 eta5 eta6'
    phase_etas = eta2
  []
  [h3]
    type = SwitchingFunctionMultiPhaseMaterial
    h_name = h3
    all_etas = 'eta1 eta2 eta3 eta4 eta5 eta6'
    phase_etas = eta3
  []
  [h4]
    type = SwitchingFunctionMultiPhaseMaterial
    h_name = h4
    all_etas = 'eta1 eta2 eta3 eta4 eta5 eta6'
    phase_etas = eta4
  []
  [h5]
    type = SwitchingFunctionMultiPhaseMaterial
    h_name = h5
    all_etas = 'eta1 eta2 eta3 eta4 eta5 eta6'
    phase_etas = eta5
  []
  [h6]
    type = SwitchingFunctionMultiPhaseMaterial
    h_name = h6
    all_etas = 'eta1 eta2 eta3 eta4 eta5 eta6'
    phase_etas = eta6
  []
  [g1]
    type = BarrierFunctionMaterial
    g_order = SIMPLE
    eta = eta1
    function_name = g1
  []
  [g2]
    type = BarrierFunctionMaterial
    g_order = SIMPLE
    eta = eta2
    function_name = g2
  []
  [g3]
    type = BarrierFunctionMaterial
    g_order = SIMPLE
    eta = eta3
    function_name = g3
  []
  [g4]
    type = BarrierFunctionMaterial
    g_order = SIMPLE
    eta = eta4
    function_name = g4
  []
  [g5]
    type = BarrierFunctionMaterial
    g_order = SIMPLE
    eta = eta5
    function_name = g5
  []
  [g6]
    type = BarrierFunctionMaterial
    g_order = SIMPLE
    eta = eta6
    function_name = g6
  []

  # Unit scaling and kinetic constants
  [scale]
    type = GenericConstantMaterial
    prop_names = 'length_scale energy_scale time_scale'
    # SI conversion factors: m -> nm, J -> eV, and s -> ns
    prop_values = '1e9 6.24150943e18 1.0e9'
  []

  [constants]
    type = GenericConstantMaterial
    prop_names = 'pseudo_L_si   pseudo_kappa  D  M_si molar_vol'
    prop_values = '12  0.5    1  2.7851e-25 1.0e-5'
  []

  [model_constants]
    # Moelans (2008), gamma=1.5: kappa=3*sigma*delta/4; m=6*sigma/delta.
    type = GenericConstantMaterial
    prop_names = 'sigma   delta gamma'
    prop_values = '0.50 35.0e-09 1.5'
  []

  [kappa_isotropy]
    type = ParsedMaterial
    property_name = kappa
    material_property_names = 'length_scale energy_scale sigma delta'
    expression = '(energy_scale/length_scale)*(0.75*sigma*delta)'
  []

  [mu]
    # ACGrGrMulti calls Moelans' barrier m "mu"; it is not GB mobility.
    type = ParsedMaterial
    property_name = mu
    material_property_names = 'length_scale energy_scale sigma delta'
    expression = '(energy_scale/(length_scale)^3)*6*(sigma/delta)'
  []

  [interface_mobility]
    # Poudel (2024): L=(16/3)*m*M/kappa; factor_L is the legacy multiplier.
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

  # Phase elasticity from Poudel et al. (2024).
  # symmetric9 ordering: C11 C12 C13 C22 C23 C33 C44 C55 C66
  # Elastic constants are expressed in eV/nm^3:
  #   1 GPa = 6.241509 eV/nm^3
  #   1 eV/angstrom^3 = 1000 eV/nm^3 = 160.218 GPa

  # Al2Cu phase (eta1, eta2)
  [elasticity_tensor_1]
    type = ComputeElasticityTensor
    base_name = C_eta1
    fill_method = symmetric9
    C_ijkl = '832.44 314.00 275.87 832.44 275.87 811.40 170.62 170.62 181.69'
  []
  [strain_1]
    type = ComputeSmallStrain
    base_name = C_eta1
    eigenstrain_names = eigenstrain
    displacements = 'disp_x disp_y'
  []
  [stress_1]
    type = ComputeLinearElasticStress
    base_name = C_eta1
  []
  [eigenstrain_1]
    type = ComputeEigenstrain
    base_name = C_eta1
    eigen_base = '4.29e-2'
    prefactor = -2
    eigenstrain_name = eigenstrain
  []
  [pre]
    type = GenericConstantMaterial
    prop_names = pre
    prop_values = 0.002
  []

  [fel_eta1]
    type = ElasticEnergyMaterial
    base_name = C_eta1
    property_name = fel1
    output_properties = fel1
    outputs = exodus
  []

  # Al3Ni phase (eta3, eta4, eta5)
  [elasticity_tensor_345]
    type = ComputeElasticityTensor
    base_name = C_eta345
    fill_method = symmetric9
    C_ijkl = '1409.18 565.49 430.73 1507.68 479.39 1394.29 595.87 541.93 437.02'
  []
  [strain_345]
    type = ComputeSmallStrain
    base_name = C_eta345
    eigenstrain_names = 'C_eigenstrain'
    displacements = 'disp_x disp_y'
  []
  [stress_345]
    type = ComputeLinearElasticStress
    base_name = C_eta345
  []
  [eigenstrain_345]
    type = ComputeEigenstrain
    base_name = C_eta345
    eigen_base = '4.29e-2'
    prefactor = -2
    eigenstrain_name = 'C_eigenstrain'
  []

  [fel_eta345]
    type = ElasticEnergyMaterial
    base_name = C_eta345
    property_name = fel2
    outputs = exodus
    output_properties = fel2
  []

  # Al-rich matrix (eta6)
  [elasticity_tensor_6]
    type = ComputeElasticityTensor
    base_name = C_eta6
    fill_method = symmetric9
    C_ijkl = '483.45 247.42 247.42 483.45 247.42 483.45 120.54 120.54 120.54'
  []
  [strain_6]
    type = ComputeSmallStrain
    base_name = C_eta6
    eigenstrain_names = 'C_eigenstrain'
    displacements = 'disp_x disp_y'
  []
  [stress_6]
    type = ComputeLinearElasticStress
    base_name = C_eta6
  []
  [eigenstrain_6]
    type = ComputeEigenstrain
    base_name = C_eta6
    eigen_base = '4.29e-2'
    prefactor = -0.01
    eigenstrain_name = 'C_eigenstrain'
  []

  [fel_eta6]
    type = ElasticEnergyMaterial
    base_name = C_eta6
    property_name = fel3
    outputs = exodus
    output_properties = fel3
  []
  [global_stress]
    type = MultiPhaseStressMaterial
    phase_base = 'C_eta1  C_eta1  C_eta345 C_eta345 C_eta345 C_eta6'
    h = 'h1 h2 h3 h4 h5 h6'
    base_name = global
  []
  [global_strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y'
  []

  # Combined chemical and elastic free energies
  [F_1]
    type = DerivativeSumMaterial
    property_name = F1
    coupled_variables = 'c1a c1b'
    sum_materials = 'Fch1 fel1'
    outputs = exodus
  []
  [F_2]
    type = DerivativeSumMaterial
    property_name = F2
    coupled_variables = 'c2a c2b'
    sum_materials = 'Fch2 fel2'
    outputs = exodus
  []
  [F_3]
    type = DerivativeSumMaterial
    property_name = F3
    coupled_variables = 'c3a c3b'
    sum_materials = 'Fch3 fel3'
    outputs = exodus
  []
[]

[Kernels]
  # KKS phase-composition constraints
  [chempot12a]
    type = KKSPhaseChemicalPotential
    variable = c1a
    cb = c2a
    fa_name = F1
    fb_name = F2
    args_a = c1b
    args_b = c2b
  []
  [chempot23a]
    type = KKSPhaseChemicalPotential
    variable = c2a
    cb = c3a
    fa_name = F2
    fb_name = F3
    args_a = c2b
    args_b = c3b
  []
  [chempot31a]
    type = KKSPhaseChemicalPotential
    variable = c3a
    cb = c1a
    fa_name = F3
    fb_name = F1
    args_a = c3b
    args_b = c1b
  []
  [phaseconcentration_a]
    type = KKSMultiPhaseConcentration
    variable = c3a
    cj = 'c1a c1a c2a c2a c2a c3a'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    etas = 'eta1 eta2 eta3 eta4 eta5 eta6'
    c = ca
  []
  [chempot12b]
    type = KKSPhaseChemicalPotential
    variable = c1b
    cb = c2b
    fa_name = F1
    fb_name = F2
    args_a = c1a
    args_b = c2a
  []
  [chempot23b]
    type = KKSPhaseChemicalPotential
    variable = c2b
    cb = c3b
    fa_name = F2
    fb_name = F3
    args_a = c2a
    args_b = c3a
  []
  [chempot31b]
    type = KKSPhaseChemicalPotential
    variable = c3b
    cb = c1b
    fa_name = F3
    fb_name = F1
    args_a = c3a
    args_b = c1a
  []
  [phaseconcentration_b]
    type = KKSMultiPhaseConcentration
    variable = c3b
    cj = 'c1b c1b c2b c2b c2b c3b'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    etas = 'eta1 eta2 eta3 eta4 eta5 eta6'
    c = cb
  []

  # Split Cahn-Hilliard diffusion
  [CHBulka]
    type = KKSSplitCHCRes
    variable = ca
    ca = c2a
    fa_name = F2
    w = wa
    args_a = c2b
  []
  [dcdta]
    type = CoupledTimeDerivative
    variable = wa
    v = ca
  []
  [ckernela]
    type = SplitCHWRes
    mob_name = M
    variable = wa
    coupled_variables = 'eta1 eta2 eta3 eta4 eta5 eta6'
  []

  [CHBulkb]
    type = KKSSplitCHCRes
    variable = cb
    ca = c2b
    fa_name = F2
    w = wb
    args_a = c2a
  []
  [dcdtb]
    type = CoupledTimeDerivative
    variable = wb
    v = cb
  []
  [ckernelb]
    type = SplitCHWRes
    mob_name = M
    variable = wb
    coupled_variables = 'eta1 eta2 eta3 eta4 eta5 eta6'
  []

  # Allen-Cahn evolution: wi=10 is the KKS barrier; ACGrGrMulti adds mu*f0.
  [deta1dt]
    type = TimeDerivative
    variable = eta1
  []
  [ACBulkF1]
    type = KKSMultiACBulkF
    variable = eta1
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    gi_name = g1
    eta_i = eta1
    wi = 10.0
    coupled_variables = 'c1a c2a c1b c2b c3a c3b eta2 eta3 eta4 eta5 eta6'
    mob_name = L
  []
  [ACBulkC1a]
    type = KKSMultiACBulkC
    variable = eta1
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1a c1a c2a c2a c2a c3a'
    eta_i = eta1
    coupled_variables = 'eta2 eta3 eta4 eta5 eta6 c1b c2b c3b'
    mob_name = L
  []
  [ACBulkC1b]
    type = KKSMultiACBulkC
    variable = eta1
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1b c1b c2b c2b c2b c3b'
    eta_i = eta1
    coupled_variables = 'eta2 eta3 eta4 eta5 eta6 c1a c2a c3a'
    mob_name = L
  []
  [ACInterface1]
    type = ACInterface
    variable = eta1
    kappa_name = kappa
    mob_name = L
  []

  [ACdfintdeta1]
    type = ACGrGrMulti
    variable = eta1
    v = 'eta2 eta3 eta4 eta5 eta6'
    gamma_names = 'gamma gamma gamma gamma gamma'
    mob_name = L
    coupled_variables = 'eta2 eta3 eta4 eta5 eta6'
  []

  [deta2dt]
    type = TimeDerivative
    variable = eta2
  []
  [ACBulkF2]
    type = KKSMultiACBulkF
    variable = eta2
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    gi_name = g2
    eta_i = eta2
    wi = 10.0
    coupled_variables = 'c1a c2a c1b c2b c3a c3b eta1 eta3 eta4 eta5 eta6'
    mob_name = L
  []

  [ACBulkC2a]
    type = KKSMultiACBulkC
    variable = eta2
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1a c1a c2a c2a c2a c3a'
    eta_i = eta2
    coupled_variables = 'eta1 eta3 eta4 eta5 eta6 c1b c2b c3b'
    mob_name = L
  []
  [ACBulkC2b]
    type = KKSMultiACBulkC
    variable = eta2
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1b c1b c2b c2b c2b c3b'
    eta_i = eta2
    coupled_variables = 'eta1 eta3 eta4 eta5 eta6 c1a c2a c3a'
    mob_name = L
  []

  [ACInterface2]
    type = ACInterface
    variable = eta2
    kappa_name = kappa
    mob_name = L
  []

  [ACdfintdeta2]
    type = ACGrGrMulti
    variable = eta2
    v = 'eta1 eta3 eta4 eta5 eta6'
    gamma_names = 'gamma gamma gamma gamma gamma'
    mob_name = L
    coupled_variables = 'eta1 eta3 eta4 eta5 eta6'
  []

  [deta3dt]
    type = TimeDerivative
    variable = eta3
  []
  [ACBulkF3]
    type = KKSMultiACBulkF
    variable = eta3
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    gi_name = g3
    eta_i = eta3
    wi = 10.0
    coupled_variables = 'c1a c2a c1b c2b c3a c3b eta1 eta2 eta4 eta5 eta6'
    mob_name = L
  []
  [ACBulkC3a]
    type = KKSMultiACBulkC
    variable = eta3
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1a c1a c2a c2a c2a c3a'
    eta_i = eta3
    coupled_variables = 'eta1 eta2 eta4 eta5 eta6 c1b c2b c3b'
    mob_name = L
  []
  [ACBulkC3b]
    type = KKSMultiACBulkC
    variable = eta3
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1b c1b c2b c2b c2b c3b'
    eta_i = eta3
    coupled_variables = 'eta1 eta2 eta4 eta5 eta6 c1a c2a c3a'
    mob_name = L
  []
  [ACInterface3]
    type = ACInterface
    variable = eta3
    kappa_name = kappa
    mob_name = L
  []

  [ACdfintdeta3]
    type = ACGrGrMulti
    variable = eta3
    v = 'eta1 eta2 eta4 eta5 eta6'
    gamma_names = 'gamma gamma gamma gamma gamma'
    mob_name = L
    coupled_variables = 'eta1 eta2 eta4 eta5 eta6'
  []

  [deta4dt]
    type = TimeDerivative
    variable = eta4
  []
  [ACBulkF4]
    type = KKSMultiACBulkF
    variable = eta4
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    gi_name = g4
    eta_i = eta4
    wi = 10.0
    coupled_variables = 'c1a c2a c1b c2b c3a c3b eta1 eta2 eta3 eta5 eta6'
    mob_name = L
  []
  [ACBulkC4a]
    type = KKSMultiACBulkC
    variable = eta4
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1a c1a c2a c2a c2a c3a'
    eta_i = eta4
    coupled_variables = 'eta1 eta2 eta3 eta5 eta6 c1b c2b c3b'
    mob_name = L
  []
  [ACBulkC4b]
    type = KKSMultiACBulkC
    variable = eta4
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1b c1b c2b c2b c2b c3b'
    eta_i = eta4
    coupled_variables = 'eta1 eta2 eta3 eta5 eta6 c1a c2a c3a'
    mob_name = L
  []
  [ACInterface4]
    type = ACInterface
    variable = eta4
    kappa_name = kappa
    mob_name = L
  []

  [ACdfintdeta4]
    type = ACGrGrMulti
    variable = eta4
    v = 'eta1 eta2 eta3 eta5 eta6'
    gamma_names = 'gamma gamma gamma gamma gamma'
    mob_name = L
    coupled_variables = 'eta1 eta2 eta3 eta5 eta6'
  []

  [deta5dt]
    type = TimeDerivative
    variable = eta5
  []
  [ACBulkF5]
    type = KKSMultiACBulkF
    variable = eta5
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    gi_name = g5
    eta_i = eta5
    wi = 10.0
    coupled_variables = 'c1a c2a c1b c2b c3a c3b eta1 eta2 eta3 eta4 eta6'
    mob_name = L
  []
  [ACBulkC5a]
    type = KKSMultiACBulkC
    variable = eta5
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1a c1a c2a c2a c2a c3a'
    eta_i = eta5
    coupled_variables = 'eta1 eta2 eta3 eta4 eta6 c1b c2b c3b'
    mob_name = L
  []
  [ACBulkC5b]
    type = KKSMultiACBulkC
    variable = eta5
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1b c1b c2b c2b c2b c3b'
    eta_i = eta5
    coupled_variables = 'eta1 eta2 eta3 eta4 eta6 c1a c2a c3a'
    mob_name = L
  []
  [ACInterface5]
    type = ACInterface
    variable = eta5
    kappa_name = kappa
    mob_name = L
  []

  [ACdfintdeta5]
    type = ACGrGrMulti
    variable = eta5
    v = 'eta1 eta2 eta3 eta4 eta6'
    gamma_names = 'gamma gamma gamma gamma gamma'
    mob_name = L
    coupled_variables = 'eta1 eta2 eta3 eta4 eta6'
  []

  [deta6dt]
    type = TimeDerivative
    variable = eta6
  []
  [ACBulkF6]
    type = KKSMultiACBulkF
    variable = eta6
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    gi_name = g6
    eta_i = eta6
    wi = 10.0
    coupled_variables = 'c1a c2a c1b c2b c3a c3b eta1 eta2 eta3 eta4 eta5'
    mob_name = L
  []
  [ACBulkC6a]
    type = KKSMultiACBulkC
    variable = eta6
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1a c1a c2a c2a c2a c3a'
    eta_i = eta6
    coupled_variables = 'eta1 eta2 eta3 eta4 eta5 c1b c2b c3b'
    mob_name = L
  []
  [ACBulkC6b]
    type = KKSMultiACBulkC
    variable = eta6
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    cj_names = 'c1b c1b c2b c2b c2b c3b'
    eta_i = eta6
    coupled_variables = 'eta1 eta2 eta3 eta4 eta5 c1a c2a c3a'
    mob_name = L
  []
  [ACInterface6]
    type = ACInterface
    variable = eta6
    kappa_name = kappa
    mob_name = L
  []

  [ACdfintdeta6]
    type = ACGrGrMulti
    variable = eta6
    v = 'eta1 eta2 eta3 eta4 eta5'
    gamma_names = 'gamma gamma gamma gamma gamma'
    mob_name = L
    coupled_variables = 'eta1 eta2 eta3 eta4 eta5'
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

[AuxKernels]
  [bnds]
    type = BndsCalcAux
    variable = bnds
    var_name_base = eta
    op_num = 6
    v = 'eta1 eta2 eta3 eta4 eta5 eta6'
  []
  [Energy_total]
    # Legacy diagnostic only: w=1 does not affect the solved equations.
    type = KKSMultiFreeEnergy
    Fj_names = 'F1 F1 F2 F2 F2 F3'
    hj_names = 'h1 h2 h3 h4 h5 h6'
    gj_names = 'g1 g2 g3 g4 g5 g6'
    variable = Energy
    w = 1
    interfacial_vars = 'eta1 eta2 eta3 eta4 eta5 eta6'
    kappa_names = 'kappa kappa kappa kappa kappa kappa'
  []

  [ca_hsquarec]
    type = SixPhasesSumCdothsquare
    variable = gr_ca
    var1 = ca
    h1_name = h1
    h2_name = h2
    h3_name = h3
    h4_name = h4
    h5_name = h5
    h6_name = h6
  []

  [cb_hsquarec]
    type = SixPhasesSumCdothsquare
    variable = gr_cb
    var1 = cb
    h1_name = h1
    h2_name = h2
    h3_name = h3
    h4_name = h4
    h5_name = h5
    h6_name = h6
  []

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

[Postprocessors]
  [area_al_eta6]
    type = ElementIntegralMaterialProperty
    mat_prop = h6
    execute_on = 'Initial TIMESTEP_END'
  []
  [area_al2cu_eta1]
    type = ElementIntegralMaterialProperty
    mat_prop = h1
    execute_on = 'Initial TIMESTEP_END'
  []
  [area_al2cu_eta2]
    type = ElementIntegralMaterialProperty
    mat_prop = h2
    execute_on = 'Initial TIMESTEP_END'
  []
  [area_al3ni_eta3]
    type = ElementIntegralMaterialProperty
    mat_prop = h3
    execute_on = 'Initial TIMESTEP_END'
  []
  [area_al3ni_eta4]
    type = ElementIntegralMaterialProperty
    mat_prop = h4
    execute_on = 'Initial TIMESTEP_END'
  []
  [area_al3ni_eta5]
    type = ElementIntegralMaterialProperty
    mat_prop = h5
    execute_on = 'Initial TIMESTEP_END'
  []
  # Elapsed wall time since SPFI started and time spent on each completed step.
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

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  # A larger GMRES space and overlapping ASM subdomains improve the strongly
  # coupled solve without changing the phase-field model.
  petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_shift_type -ksp_gmres_restart -pc_asm_overlap'
  petsc_options_value = 'asm       ilu          nonzero                    100                2'
  l_max_its = 100
  nl_max_its = 20
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-10
  nl_abs_tol = 1.0e-11

  end_time = 6.00E+20
  dtmax = 1.00E5
  [TimeStepper]
    type = IterationAdaptiveDT
    dt = 1.00E5
    cutback_factor = 0.8
    growth_factor = 1.1
    # Modern MOOSE also counts accumulated Krylov iterations. Raise that
    # threshold so accepted solves do not reduce dt geometrically.
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

[Outputs]
  # Use SPFI's colored solver console instead of MOOSE's default console.
  console = false
  exodus = true
  csv = true
  file_base = outputs/multiphase_kks_elasticity_explicit
  time_step_interval = 1
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
[]
