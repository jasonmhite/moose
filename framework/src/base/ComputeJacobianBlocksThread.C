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

#include "ComputeJacobianBlocksThread.h"
#include "NonlinearSystem.h"
#include "FEProblem.h"
#include "KernelBase.h"
#include "IntegratedBC.h"
#include "DGKernel.h"
// libmesh includes
#include "libmesh/threads.h"

ComputeJacobianBlocksThread::ComputeJacobianBlocksThread(FEProblem & fe_problem, std::vector<JacobianBlock*> & blocks) :
    ComputeFullJacobianThread(fe_problem, fe_problem.getNonlinearSystem(), blocks[0]->_jacobian /* have to pass something */),
    _blocks(blocks)
{
}

// Splitting Constructor
ComputeJacobianBlocksThread::ComputeJacobianBlocksThread(ComputeJacobianBlocksThread & x, Threads::split split) :
    ComputeFullJacobianThread(x, split),
    _blocks(x._blocks)
{
}

ComputeJacobianBlocksThread::~ComputeJacobianBlocksThread()
{
}


void
ComputeJacobianBlocksThread::postElement(const Elem * elem)
{
  std::vector<unsigned int> dof_indices; // Do this out here to avoid creating and destroying it thousands of times

  Threads::spin_mutex::scoped_lock lock(Threads::spin_mtx);

  std::vector<JacobianBlock*>::iterator it = _blocks.begin();
  std::vector<JacobianBlock*>::iterator end = _blocks.end();

  for (; it != end; ++it)
  {
    JacobianBlock & block = *(*it);

    const DofMap & dof_map = block._precond_system.get_dof_map();
    dof_map.dof_indices(elem, dof_indices);

    _fe_problem.addJacobianBlock(block._jacobian, block._ivar, block._jvar, dof_map, dof_indices, _tid);
  }
}
