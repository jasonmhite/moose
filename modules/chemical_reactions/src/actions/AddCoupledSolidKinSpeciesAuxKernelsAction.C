#include "AddCoupledSolidKinSpeciesAuxKernelsAction.h"
#include "MooseUtils.h"
#include "FEProblem.h"
#include "Factory.h"

#include <sstream>
#include <stdexcept>

// libMesh includes
#include "libmesh/libmesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/equation_systems.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"
#include "libmesh/fe.h"


template<>
InputParameters validParams<AddCoupledSolidKinSpeciesAuxKernelsAction>()
{
  InputParameters params = validParams<Action>();
  params.addRequiredParam<std::vector<std::string> >("kin_reactions", "The list of solid kinetic reactions");
  params.addRequiredParam<std::vector<Real> >("log10_keq", "The list of equilibrium constants for all reactions");
  params.addRequiredParam<std::vector<Real> >("specific_reactive_surface_area", "The list of specific reactive surface area for all minerals, (m^2/L)");
  params.addRequiredParam<std::vector<Real> >("kinetic_rate_constant", "The list of kinetic rate constant for all reactions, (mol/m^2/s)");
  params.addRequiredParam<std::vector<Real> >("activation_energy", "The list of activation energy values for all reactions, (J/mol)");
  params.addRequiredParam<Real>("gas_constant", "Gas constant, 8.314 (J/mol/K)");
  params.addRequiredParam<std::vector<Real> >("reference_temperature", "The list of reference temperatures for all reactions, (K)");
  params.addRequiredParam<std::vector<Real> >("system_temperature", "The list of system temperatures for all reactions, (K)");

  return params;
}


AddCoupledSolidKinSpeciesAuxKernelsAction::AddCoupledSolidKinSpeciesAuxKernelsAction(const std::string & name, InputParameters params) :
    Action(name, params)
{
}

void
AddCoupledSolidKinSpeciesAuxKernelsAction::act()
{
  std::vector<std::string> reactions = getParam<std::vector<std::string> >("kin_reactions");
  std::vector<Real> logk = getParam<std::vector<Real> >("log10_keq");
  std::vector<Real> r_area = getParam<std::vector<Real> >("specific_reactive_surface_area");
  std::vector<Real> ref_kconst = getParam<std::vector<Real> >("kinetic_rate_constant");
  std::vector<Real> e_act = getParam<std::vector<Real> >("activation_energy");
  Real gas_const = getParam<Real>("gas_constant");
  std::vector<Real> ref_temp = getParam<std::vector<Real> >("reference_temperature");
  std::vector<Real> sys_temp = getParam<std::vector<Real> >("system_temperature");

// NEED TO ADD AN ERROR MESSAGE IF THE SIZES OF ABOVE ARRAYS ARE NOT THE SAME //

  for (unsigned int j=0; j < reactions.size(); j++)
  {
    std::vector<std::string> tokens;
    std::vector<std::string> solid_kin_species(reactions.size());

    // Parsing each reaction
    MooseUtils::tokenize(reactions[j], tokens, 1, "+=");

    std::vector<Real> stos(tokens.size()-1);
    std::vector<VariableName> rxn_vars(tokens.size()-1);

    for (unsigned int k=0; k < tokens.size(); k++)
    {
      _console << tokens[k] << "\t";
      std::vector<std::string> stos_vars;
      MooseUtils::tokenize(tokens[k], stos_vars, 1, "()");
      if (stos_vars.size() == 2)
      {
        Real coef;
        std::istringstream iss(stos_vars[0]);
        iss >> coef;
        stos[k] = coef;
        rxn_vars[k] = stos_vars[1];
      }
      else
      {
        solid_kin_species[j] = stos_vars[0];
      }
    }

//    Moose::out << "the " << j+1 << "-th solid kinetic species: " << solid_kin_species[j] << "\n";

    InputParameters params_kin = _factory.getValidParams("KineticDisPreConcAux");
    params_kin.set<AuxVariableName>("variable") = solid_kin_species[j];
    params_kin.set<Real>("log_k") = logk[j];
    params_kin.set<Real>("r_area") = r_area[j];
    params_kin.set<Real>("ref_kconst") = ref_kconst[j];
    params_kin.set<Real>("e_act") = e_act[j];
    params_kin.set<Real>("gas_const") = gas_const;
    params_kin.set<Real>("ref_temp") = ref_temp[j];
    params_kin.set<Real>("sys_temp") = sys_temp[j];
    params_kin.set<std::vector<Real> >("sto_v") = stos;
    params_kin.set<std::vector<VariableName> >("v") = rxn_vars;
    _problem->addAuxKernel("KineticDisPreConcAux", "aux_"+solid_kin_species[j], params_kin);

    _console << "aux_"+solid_kin_species[j] << "\n";
    params_kin.print();
  }

}
