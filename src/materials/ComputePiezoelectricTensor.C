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
*/

#include "ComputePiezoelectricTensor.h"
#include "RotationTensor.h"
#include "PiezostrictiveTensorTools.h"

#include <stdexcept>

registerMooseObject("spfiApp", ComputePiezoelectricTensor);

InputParameters
ComputePiezoelectricTensor::validParams()
{
  InputParameters params = ComputeRotatedPiezoTensorBase::validParams();
  params.addClassDescription("Compute the converse piezoelectric tensor and optionally the piezostrictive tensor.");

  params.addParam<bool>("compute_piezostrictive_coeff", true,
      "Compute the piezostrictive coefficients C_ijmn * d_kij using the elasticity tensor from material properties");

  // Optional 27-component representation (3x3x3 flattened). If not provided, d_voigt may be used.
  params.addParam<std::vector<Real>>("d_ijk", std::vector<Real>(),
      "Optional: Piezoelectric tensor for material (3x3x3 = 27 entries). If omitted, supply d_voigt (18 entries).");

  // New optional Voigt input (3x6 flattened row-major). If provided (size 18) it takes precedence.
  params.addParam<std::vector<Real>>("d_voigt", std::vector<Real>(),
      "Optional: Piezoelectric tensor in 3x6 form flattened row-major: [pol=0,J=0..5, pol=1,J=0..5, pol=2,J=0..5]. "
      "Flattened index = pol*6 + J. If present (length 18), it will be used instead of d_ijk.");

  // Whether the user supplied engineering shear (gamma = 2*epsilon). If true, divide shear entries by 2 on expansion.
  params.addParam<bool>("voigt_engineering_shear", true,
      "If true, Voigt shear columns (J=3,4,5) are engineering shear (gamma = 2*epsilon) and will be divided by 2 when expanded.");

  params.addParam<MooseEnum>("fill_method", RankThreeTensor::fillMethodEnum() = "general",
      "The fill method for the piezoelectric tensor (used only if d_ijk vector is provided)");

  return params;
}

ComputePiezoelectricTensor::ComputePiezoelectricTensor(const InputParameters & parameters) :
    ComputeRotatedPiezoTensorBase(parameters),
    _compute_piezostrictive_coeff(getParam<bool>("compute_piezostrictive_coeff")),
    _dijk(), // will fill below
    _dijkT(),
    _Cijkl(getMaterialProperty<RankFourTensor>("elasticity_tensor"))
{
  // Voigt J -> symmetric index pair (a,b) (0-based).
  auto voigt_pair = [](unsigned J) -> std::pair<unsigned,unsigned>
  {
    switch(J)
    {
      case 0: return {0,0}; // 11
      case 1: return {1,1}; // 22
      case 2: return {2,2}; // 33
      case 3: return {1,2}; // 23
      case 4: return {0,2}; // 13
      case 5: return {0,1}; // 12
      default: return {0,0};
    }
  };

  std::vector<Real> dvoigt = getParam<std::vector<Real>>("d_voigt");
  std::vector<Real> dvec   = getParam<std::vector<Real>>("d_ijk");
  bool voigt_used = (dvoigt.size() == 18);
  bool full_used  = (dvec.size() == 27);

  bool voigt_engineering_shear = getParam<bool>("voigt_engineering_shear");
  RankThreeTensor::FillMethod fm = (RankThreeTensor::FillMethod)(int)getParam<MooseEnum>("fill_method");

  if (voigt_used)
  {
    // Zero the rank-3 tensor
    for (unsigned p=0; p<3; ++p)
      for (unsigned i=0; i<3; ++i)
        for (unsigned j=0; j<3; ++j)
          _dijk(p, i, j) = 0.0;

    // d_voigt is 3x6 row-major: for pol=0..2, for J=0..5 index = pol*6 + J
    for (unsigned pol = 0; pol < 3; ++pol)
    {
      for (unsigned J = 0; J < 6; ++J)
      {
        Real v = dvoigt[pol * 6 + J];
        if (voigt_engineering_shear && (J >= 3))
          v *= 0.5;
        auto pr = voigt_pair(J);
        unsigned a = pr.first;
        unsigned b = pr.second;
        // assign symmetric strain indices
        _dijk(pol, a, b) = v;
        _dijk(pol, b, a) = v;
      }
    }
  }
  else if (full_used)
  {
    // Use the original 27-component vector constructor (backwards compatible)
    _dijk = RankThreeTensor(dvec, fm);
  }
  else
  {
    throw std::runtime_error("ComputePiezoelectricTensor: neither d_voigt (length 18) nor d_ijk (length 27) provided. Provide one of them.");
  }

  /// Define a rotation according to Euler angle parameters
  RotationTensor R(_Euler_angles); // R type: RealTensorValue

  /// Rotate piezoelectric tensor -- note that it needs to be collinear with the elasticity tensor
  _dijk.rotate(R);

  if (_compute_piezostrictive_coeff)
  {
    /// Compute the major transpose once since _dijk is constant after rotation
    for (unsigned int i = 0; i < 3; ++i)
      for (unsigned int j = 0; j < 3; ++j)
        for (unsigned int k = 0; k < 3; ++k)
          _dijkT(i, j, k) = _dijk(k, j, i);
  }
}

void
ComputePiezoelectricTensor::computeQpPiezoTensor()
{
  /// Assign the piezoelectric tensor at a given quad point
  _piezo_tensor[_qp] = _dijk;

  if (_compute_piezostrictive_coeff)
  {
    /// Compute piezostrictive tensors using the elasticity tensor at the current quadrature point
    _Dkij = PiezostrictiveTensorTools::computeProduct(_Cijkl[_qp], _dijk);
    _DijkT = PiezostrictiveTensorTools::computePiezoTransposeProduct(_Cijkl[_qp], _dijkT);

    _piezostrictive_tensor[_qp]   = _Dkij;
    _piezostrictive_tensor_i[_qp] = _DijkT;
  }
}
