#ifndef BICRYSTALCIRCLEGRAINICACTION_H
#define BICRYSTALCIRCLEGRAINICACTION_H

#include "InputParameters.h"
#include "Action.h"

/**
 * Automatically generates all variables to model a polycrystal with op_num orderparameters
 */
class BicrystalCircleGrainICAction: public Action
{
public:
  BicrystalCircleGrainICAction(const std::string & name, InputParameters params);

  virtual void act();

private:
  static const Real _abs_zero_tol;

  std::string _var_name_base;
  unsigned int _op_num;

  Real _radius;
  Real _x, _y, _z;
  Real _int_width;

  bool _3D_sphere;
};

template<>
InputParameters validParams<BicrystalCircleGrainICAction>();

#endif //BICRYSTALCIRCLEGRAINICACTION_H
