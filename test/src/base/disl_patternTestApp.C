//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "disl_patternTestApp.h"
#include "disl_patternApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
disl_patternTestApp::validParams()
{
  InputParameters params = disl_patternApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

disl_patternTestApp::disl_patternTestApp(const InputParameters & parameters) : MooseApp(parameters)
{
  disl_patternTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

disl_patternTestApp::~disl_patternTestApp() {}

void
disl_patternTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  disl_patternApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"disl_patternTestApp"});
    Registry::registerActionsTo(af, {"disl_patternTestApp"});
  }
}

void
disl_patternTestApp::registerApps()
{
  registerApp(disl_patternApp);
  registerApp(disl_patternTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
disl_patternTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  disl_patternTestApp::registerAll(f, af, s);
}
extern "C" void
disl_patternTestApp__registerApps()
{
  disl_patternTestApp::registerApps();
}
