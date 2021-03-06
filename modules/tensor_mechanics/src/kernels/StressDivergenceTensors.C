#include "StressDivergenceTensors.h"

#include "Material.h"

template<>
InputParameters validParams<StressDivergenceTensors>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  params.addCoupledVar("disp_x", "The x displacement");
  params.addCoupledVar("disp_y", "The y displacement");
  params.addCoupledVar("disp_z", "The z displacement");
  params.addCoupledVar("temp", "The temperature");
  params.addParam<std::string>("appended_property_name", "", "Name appended to material properties to make them unique");

  return params;
}


StressDivergenceTensors::StressDivergenceTensors(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _stress(getMaterialProperty<RankTwoTensor>("stress" + getParam<std::string>("appended_property_name"))),
    _Jacobian_mult(getMaterialProperty<ElasticityTensorR4>("Jacobian_mult" + getParam<std::string>("appended_property_name"))),
    // _d_stress_dT(getMaterialProperty<RankTwoTensor>("d_stress_dT"+ getParam<std::string>("appended_property_name"))),
    _component(getParam<unsigned int>("component")),
    _xdisp_coupled(isCoupled("disp_x")),
    _ydisp_coupled(isCoupled("disp_y")),
    _zdisp_coupled(isCoupled("disp_z")),
    _temp_coupled(isCoupled("temp")),
    _xdisp_var(_xdisp_coupled ? coupled("disp_x") : 0),
    _ydisp_var(_ydisp_coupled ? coupled("disp_y") : 0),
    _zdisp_var(_zdisp_coupled ? coupled("disp_z") : 0),
    _temp_var(_temp_coupled ? coupled("temp") : 0)
{
}

Real
StressDivergenceTensors::computeQpResidual()
{
  return _stress[_qp].row(_component) * _grad_test[_i][_qp];
}

Real
StressDivergenceTensors::computeQpJacobian()
{
  return _Jacobian_mult[_qp].elasticJacobian(_component, _component, _grad_test[_i][_qp], _grad_phi[_j][_qp]);
}

Real
StressDivergenceTensors::computeQpOffDiagJacobian(unsigned int jvar)
{
  unsigned int coupled_component = 0;
  bool active(false);

  if (_xdisp_coupled && jvar == _xdisp_var)
  {
    coupled_component = 0;
    active = true;
  }
  else if (_ydisp_coupled && jvar == _ydisp_var)
  {
    coupled_component = 1;
    active = true;
  }
  else if (_zdisp_coupled && jvar == _zdisp_var)
  {
    coupled_component = 2;
    active = true;
  }

  if ( active )
    return _Jacobian_mult[_qp].elasticJacobian(_component, coupled_component,
                                          _grad_test[_i][_qp], _grad_phi[_j][_qp]);

  if (_temp_coupled && jvar == _temp_var)
  {
    //return _d_stress_dT[_qp].rowDot(_component, _grad_test[_i][_qp]) * _phi[_j][_qp];
    return 0.0;
  }

  return 0;
}
