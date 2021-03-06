/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "JouleHeating.h"

// MOOSE
#include "Function.h"

template<>
InputParameters validParams<JouleHeating>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("potential", "Gradient of the electrical potential");
  return params;
}

JouleHeating::JouleHeating(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _grad_potential(coupledGradient("potential")),
    _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))
{
}

Real
JouleHeating::computeQpResidual()
{
  return - _thermal_conductivity[_qp] * _grad_potential[_qp]*_grad_potential[_qp]*_test[_i][_qp];
}

