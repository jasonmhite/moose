#ifndef DOMAININTEGRALACTION_H
#define DOMAININTEGRALACTION_H

#include "Action.h"
#include "MooseTypes.h"
#include "libmesh/vector_value.h"

class DomainIntegralAction;

template<>
InputParameters validParams<DomainIntegralAction>();

class DomainIntegralAction : public Action
{
public:
  DomainIntegralAction(const std::string & name, InputParameters params);
  ~DomainIntegralAction();

  virtual void act();
protected:
  enum INTEGRAL
  {
    J_INTEGRAL,
    INTERACTION_INTEGRAL_KI,
    INTERACTION_INTEGRAL_KII,
    INTERACTION_INTEGRAL_KIII
  };

  unsigned int calcNumCrackFrontNodes();

  std::set<INTEGRAL> _integrals;
  const std::vector<BoundaryName> & _boundary_names;
  const std::string _order;
  const std::string _family;
  MooseEnum _direction_method_moose_enum;
  MooseEnum _end_direction_method_moose_enum;
  bool _have_crack_direction_vector;
  RealVectorValue _crack_direction_vector;
  bool _have_crack_direction_vector_end_1;
  RealVectorValue _crack_direction_vector_end_1;
  bool _have_crack_direction_vector_end_2;
  RealVectorValue _crack_direction_vector_end_2;
  std::vector<BoundaryName> _crack_mouth_boundary_names;
  bool _treat_as_2d;
  unsigned int _axis_2d;
  std::vector<Real> _radius_inner;
  std::vector<Real> _radius_outer;
  std::vector<VariableName> _output_variables;
  bool _use_displaced_mesh;
};

#endif //DOMAININTEGRALACTION_H