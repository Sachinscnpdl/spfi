# SPFI examples

- `multiphase_kks_elasticity_action.i` is the compact, recommended input. SPFI
  Actions generate the repetitive KKS, elasticity, initialization, and
  diagnostic objects.
- `multiphase_kks_elasticity_explicit.i` is the fully expanded reference used
  to inspect or verify the objects produced by the Action formulation.

Run either input from this directory:

```bash
spfi multiphase_kks_elasticity_action.i
spfi multiphase_kks_elasticity_explicit.i
```

Exodus, CSV, and checkpoint files are written under `outputs/` with distinct
file bases for the Action and explicit formulations.

## Citation

If you use or adapt these input files, please cite both works:

```bibtex
@article{poudel2024unraveling,
  title={Unraveling elastochemical effects in microstructural evolution of Al--Cu--Ni system through DFT-informed multi-phase field simulations},
  author={Poudel, Sachin and Moelans, Nele and Thapa, Rubi and Timofiejczuk, Anna and Panthi, Dhruba and Kunwar, Anil},
  journal={International Journal of Solids and Structures},
  volume={300},
  pages={112894},
  year={2024},
  publisher={Elsevier}
}

@article{moelans2008quantitative,
  title={Quantitative analysis of grain boundary properties in a generalized phase field model for grain growth in anisotropic systems},
  author={Moelans, Nele and Blanpain, Bart and Wollants, Patrick},
  journal={Physical Review B---Condensed Matter and Materials Physics},
  volume={78},
  number={2},
  pages={024113},
  year={2008},
  publisher={APS}
}
```
