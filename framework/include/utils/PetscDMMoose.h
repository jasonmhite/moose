#ifndef _PETSCDMMOOSE
#define _PETSCDMMOOSE

#include "libmesh/petsc_macro.h"
// This only works with petsc-3.3 and above.

#if !PETSC_VERSION_LESS_THAN(3,3,0)
#include "NonlinearSystem.h"
#include <map>
#include <set>
#include <string>

#include <petscdm.h>
#define DMMOOSE "moose"

extern PetscErrorCode DMCreateMoose(MPI_Comm,NonlinearSystem&,DM*);
extern PetscErrorCode DMMooseSetNonlinearSystem(DM,NonlinearSystem&);
extern PetscErrorCode DMMooseGetNonlinearSystem(DM,NonlinearSystem*&);
extern PetscErrorCode DMMooseGetBlocks(DM,std::vector<std::string>&);
extern PetscErrorCode DMMooseGetVariables(DM,std::vector<std::string>&);
extern PetscErrorCode DMMooseSetBlocks(DM,const std::vector<std::string>&);
extern PetscErrorCode DMMooseSetVariables(DM,const std::vector<std::string>&);
extern PetscErrorCode DMMooseSetSplitNames(DM,const std::set<std::string>&);
extern PetscErrorCode DMMooseGetSplitNames(DM,const std::set<std::string>&);
extern PetscErrorCode DMMooseSetSplitVars(DM,const std::string&,const std::set<std::string>&);
extern PetscErrorCode DMMooseGetSplitVars(DM,const std::string&,std::set<std::string>&);
extern PetscErrorCode DMMooseSetSplitBlocks(DM,const std::string&,const std::set<std::string>&);
extern PetscErrorCode DMMooseGetSplitBlocks(DM,const std::string&,std::set<std::string>&);
extern PetscErrorCode DMMooseSetSplitSides(DM,const std::string&,const std::set<std::string>&);
extern PetscErrorCode DMMooseGetSplitSides(DM,const std::string&,std::set<std::string>&);
extern PetscErrorCode DMMooseSetDomainDecomposition(DM,const std::map<std::string,std::set<std::string> >&);
extern PetscErrorCode DMMooseGetDomainDecomposition(DM,std::map<std::string,std::set<std::string> >&);


#endif // #if !PETSC_VERSION_LESS_THAN(3,3,0)
#endif // #ifdef _petscdmmoose