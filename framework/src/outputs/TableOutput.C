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

// MOOSE includes
#include "TableOutput.h"
#include "FEProblem.h"
#include "Postprocessor.h"
#include "PetscSupport.h"
#include "Executioner.h"
#include "MooseApp.h"
#include "Conversion.h"

// libMesh includes
#include "libmesh/string_to_enum.h"

template<>
InputParameters validParams<TableOutput>()
{
  // Fit mode selection Enum
  MooseEnum pps_fit_mode(FormattedTable::getWidthModes());

  // Base class parameters
  InputParameters params = validParams<FileOutput>();
  params += Output::enableOutputTypes("postprocessor scalar");

  // Add option for appending file on restart
  params.addParam<bool>("append_restart", false, "Append existing file on restart");

  return params;
}

TableOutput::TableOutput(const std::string & name, InputParameters parameters) :
    FileOutput(name, parameters),
    _tables_restartable(getParam<bool>("append_restart")),
    _postprocessor_table(_tables_restartable ? declareRestartableData<FormattedTable>("postprocessor_table") : declareRecoverableData<FormattedTable>("postprocessor_table")),
    _scalar_table(_tables_restartable ? declareRestartableData<FormattedTable>("scalar_table") : declareRecoverableData<FormattedTable>("scalar_table")),
    _all_data_table(_tables_restartable ? declareRestartableData<FormattedTable>("all_data_table") : declareRecoverableData<FormattedTable>("all_data_table"))
{
}

TableOutput::~TableOutput()
{
}

void
TableOutput::outputNodalVariables()
{
  mooseError("Nodal nonlinear variable output not supported by TableOutput output class");
}

void
TableOutput::outputElementalVariables()
{
  mooseError("Elemental nonlinear variable output not supported by TableOutput output class");
}

void
TableOutput::outputPostprocessors()
{
  // List of names of the postprocessors to output
  const std::vector<std::string> & out = getPostprocessorOutput();

  // Loop through the postprocessor names and extract the values from the PostprocessorData storage
  for (std::vector<std::string>::const_iterator it = out.begin(); it != out.end(); ++it)
  {
    PostprocessorValue value = _problem_ptr->getPostprocessorValue(*it);
    _postprocessor_table.addData(*it, value, time());
    _all_data_table.addData(*it, value, time());
  }
}

void
TableOutput::outputVectorPostprocessors()
{
  // List of names of the postprocessors to output
  const std::vector<std::string> & out = getVectorPostprocessorOutput();

  // Loop through the postprocessor names and extract the values from the VectorPostprocessorData storage
  for (std::vector<std::string>::const_iterator it = out.begin(); it != out.end(); ++it)
  {
    std::string vpp_name = *it;

    const std::map<std::string, VectorPostprocessorValue*> & vectors = _problem_ptr->getVectorPostprocessorVectors(vpp_name);

    FormattedTable & table = _vector_postprocessor_tables[vpp_name];

    table.clear();
    table.outputTimeColumn(false);

    for (std::map<std::string, VectorPostprocessorValue*>::const_iterator vec_it = vectors.begin(); vec_it != vectors.end(); ++vec_it)
    {
      VectorPostprocessorValue vector = *(vec_it->second);

      for (unsigned int i=0; i<vector.size(); i++)
        table.addData(vec_it->first, vector[i], i);
    }
  }
}

void
TableOutput::outputScalarVariables()
{
  // List of scalar variables
  const std::vector<std::string> & out = getScalarOutput();

  // Loop through each variable
  for (std::vector<std::string>::const_iterator it = out.begin(); it != out.end(); ++it)
  {
    // Get reference to the variable and the no. of components
    VariableValue & variable = _problem_ptr->getScalarVariable(0, *it).sln();
    unsigned int n = variable.size();

    // If the variable has a single component, simply output the value with the name
    if (n == 1)
    {
      _scalar_table.addData(*it, variable[0], time());
      _all_data_table.addData(*it, variable[0], time());
    }

    // Multi-component variables are appended with the component index
    else
      for (unsigned int i = 0; i < n; ++i)
      {
        std::ostringstream os;
        os << *it << "_" << i;
        _scalar_table.addData(os.str(), variable[i], time());
        _all_data_table.addData(os.str(), variable[i], time());
      }
  }
}
