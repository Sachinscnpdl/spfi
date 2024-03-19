#include "spfiApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
spfiApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  return params;
}

spfiApp::spfiApp(InputParameters parameters) : MooseApp(parameters)
{
  spfiApp::registerAll(_factory, _action_factory, _syntax);
}

spfiApp::~spfiApp() {}

void 
spfiApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAllObjects<spfiApp>(f, af, s);
  Registry::registerObjectsTo(f, {"spfiApp"});
  Registry::registerActionsTo(af, {"spfiApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
spfiApp::registerApps()
{
  registerApp(spfiApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
spfiApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  spfiApp::registerAll(f, af, s);
}
extern "C" void
spfiApp__registerApps()
{
  spfiApp::registerApps();
}
