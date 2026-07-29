/*
   This file is part of FERRET, an add-on module for MOOSE

   FERRET is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   For help with FERRET please contact J. Mangeri <john.m.mangeri@gmail.com>
   and be sure to track new changes at github.com/mangerij/ferret

**/

#ifndef COMPUTEPIEZOELECTRICTENSOR_H
#define COMPUTEPIEZOELECTRICTENSOR_H

#include "ComputeRotatedPiezoTensorBase.h"
#include "RankThreeTensor.h"
#include "RankFourTensor.h"

/**
 * ComputePiezoelectricTensor defines a piezoelectric tensor material object with optional rotated property
 * and computes the piezostrictive tensor using the elasticity tensor from material properties.
 */
class ComputePiezoelectricTensor : public ComputeRotatedPiezoTensorBase
{
public:
  static InputParameters validParams();

  ComputePiezoelectricTensor(const InputParameters & parameters);

protected:
  virtual void computeQpPiezoTensor();

  const bool _compute_piezostrictive_coeff;

  RankThreeTensor _dijk;

  RankThreeTensor _dijkT;

  const MaterialProperty<RankFourTensor> & _Cijkl;

  RankThreeTensor _Dkij;

  RankThreeTensor _DijkT;
};

#endif // COMPUTEPIEZOELECTRICTENSOR_H
