#include "NavierStokesApp.h"
#include "Moose.h"
#include "AppFactory.h"

#include "NSMassInviscidFlux.h"
#include "NSMomentumInviscidFlux.h"
#include "NSEnergyInviscidFlux.h"
#include "NSGravityPower.h"
#include "NSGravityForce.h"
#include "NSThermalBC.h"
#include "NSVelocityAux.h"
#include "NSImposedVelocityBC.h"
#include "NSTemperatureAux.h"
#include "NSTemperatureL2.h"
#include "NSPressureAux.h"
#include "NSEnthalpyAux.h"
#include "NSEnergyThermalFlux.h"
#include "NSMomentumViscousFlux.h"
#include "NSEnergyViscousFlux.h"
#include "NSMomentumInviscidFluxWithGradP.h"
#include "NSSUPGMomentum.h"
#include "NSSUPGMass.h"
#include "NSSUPGEnergy.h"
#include "NSMassSpecifiedNormalFlowBC.h"
#include "NSMassUnspecifiedNormalFlowBC.h"
#include "NSInflowThermalBC.h"
#include "NSMomentumInviscidSpecifiedPressureBC.h"
#include "NSMomentumInviscidSpecifiedNormalFlowBC.h"
#include "NSMomentumViscousBC.h"
#include "NSEnergyInviscidSpecifiedPressureBC.h"
#include "NSEnergyInviscidSpecifiedNormalFlowBC.h"
#include "NSEnergyInviscidUnspecifiedBC.h"
#include "NSEnergyInviscidSpecifiedBC.h"
#include "NSEnergyInviscidSpecifiedDensityAndVelocityBC.h"
#include "NSEnergyViscousBC.h"
#include "NSStagnationPressureBC.h"
#include "NSStagnationTemperatureBC.h"
#include "NSImposedVelocityDirectionBC.h"
#include "NSMassWeakStagnationBC.h"
#include "NSMomentumConvectiveWeakStagnationBC.h"
#include "NSMomentumPressureWeakStagnationBC.h"
#include "NSEnergyWeakStagnationBC.h"
#include "NSPenalizedNormalFlowBC.h"
#include "NSMomentumInviscidNoPressureImplicitFlowBC.h"

//
// Incompressible
//

// Kernels
#include "INSMass.h"
#include "INSMomentumTimeDerivative.h"
#include "INSMomentum.h"
#include "INSTemperatureTimeDerivative.h"
#include "INSTemperature.h"
#include "INSSplitMomentum.h"
#include "INSProjection.h"
#include "INSPressurePoisson.h"
#include "INSChorinPredictor.h"
#include "INSChorinCorrector.h"
#include "INSChorinPressurePoisson.h"

// BCs
#include "INSMomentumNoBCBC.h"
#include "INSTemperatureNoBCBC.h"
#include "ImplicitNeumannBC.h"

// AuxKernels
#include "INSCourant.h"
#include "INSDivergenceAux.h"

// Postprocessors
#include "INSExplicitTimestepSelector.h"

template<>
InputParameters validParams<NavierStokesApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

NavierStokesApp::NavierStokesApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  NavierStokesApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  NavierStokesApp::associateSyntax(_syntax, _action_factory);
}

NavierStokesApp::~NavierStokesApp()
{
}

void
NavierStokesApp::registerApps()
{
  registerApp(NavierStokesApp);
}

void
NavierStokesApp::registerObjects(Factory & factory)
{
  registerKernel(NSMassInviscidFlux);
  registerKernel(NSMomentumInviscidFlux);
  registerKernel(NSEnergyInviscidFlux);
  registerKernel(NSGravityPower);
  registerKernel(NSGravityForce);
  registerKernel(NSTemperatureL2);
  registerBoundaryCondition(NSThermalBC);
  registerAux(NSVelocityAux);
  registerBoundaryCondition(NSImposedVelocityBC);
  registerAux(NSTemperatureAux);
  registerAux(NSPressureAux);
  registerAux(NSEnthalpyAux);
  registerKernel(NSEnergyThermalFlux);
  registerKernel(NSMomentumViscousFlux);
  registerKernel(NSEnergyViscousFlux);
  registerKernel(NSMomentumInviscidFluxWithGradP);
  registerKernel(NSSUPGMomentum);
  registerKernel(NSSUPGMass);
  registerKernel(NSSUPGEnergy);
  registerBoundaryCondition(NSMassSpecifiedNormalFlowBC);
  registerBoundaryCondition(NSMassUnspecifiedNormalFlowBC);
  registerBoundaryCondition(NSInflowThermalBC);
  registerBoundaryCondition(NSMomentumInviscidSpecifiedPressureBC);
  registerBoundaryCondition(NSMomentumInviscidSpecifiedNormalFlowBC);
  registerBoundaryCondition(NSMomentumViscousBC);
  registerBoundaryCondition(NSEnergyInviscidSpecifiedPressureBC);
  registerBoundaryCondition(NSEnergyInviscidSpecifiedNormalFlowBC);
  registerBoundaryCondition(NSEnergyInviscidUnspecifiedBC);
  registerBoundaryCondition(NSEnergyInviscidSpecifiedBC);
  registerBoundaryCondition(NSEnergyInviscidSpecifiedDensityAndVelocityBC);
  registerBoundaryCondition(NSEnergyViscousBC);
  registerBoundaryCondition(NSStagnationPressureBC);
  registerBoundaryCondition(NSStagnationTemperatureBC);
  registerBoundaryCondition(NSImposedVelocityDirectionBC);
  registerBoundaryCondition(NSMassWeakStagnationBC);
  registerBoundaryCondition(NSMomentumConvectiveWeakStagnationBC);
  registerBoundaryCondition(NSMomentumPressureWeakStagnationBC);
  registerBoundaryCondition(NSEnergyWeakStagnationBC);
  registerBoundaryCondition(NSPenalizedNormalFlowBC);
  registerBoundaryCondition(NSMomentumInviscidNoPressureImplicitFlowBC);

  //
  // Incompressible
  //

  // Kernels
  registerKernel(INSMass);
  registerKernel(INSMomentumTimeDerivative);
  registerKernel(INSMomentum);
  registerKernel(INSTemperatureTimeDerivative);
  registerKernel(INSTemperature);
  registerKernel(INSSplitMomentum);
  registerKernel(INSProjection);
  registerKernel(INSPressurePoisson);
  registerKernel(INSChorinPredictor);
  registerKernel(INSChorinCorrector);
  registerKernel(INSChorinPressurePoisson);

  // BCs
  registerBoundaryCondition(INSMomentumNoBCBC);
  registerBoundaryCondition(INSTemperatureNoBCBC);
  registerBoundaryCondition(ImplicitNeumannBC);

  // AuxKernels
  registerAux(INSCourant);
  registerAux(INSDivergenceAux);

  // Postprocessors
  registerPostprocessor(INSExplicitTimestepSelector);
}

void
NavierStokesApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}
