#include "disl_patternApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
disl_patternApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

disl_patternApp::disl_patternApp(const InputParameters & parameters) : MooseApp(parameters)
{
  disl_patternApp::registerAll(_factory, _action_factory, _syntax);
}

disl_patternApp::~disl_patternApp() {}

void
disl_patternApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAllObjects<disl_patternApp>(f, af, syntax);
  Registry::registerObjectsTo(f, {"disl_patternApp"});
  Registry::registerActionsTo(af, {"disl_patternApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
disl_patternApp::registerApps()
{
  registerApp(disl_patternApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
disl_patternApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  disl_patternApp::registerAll(f, af, s);
}
extern "C" void
disl_patternApp__registerApps()
{
  disl_patternApp::registerApps();
}
