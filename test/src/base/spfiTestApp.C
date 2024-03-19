//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "spfiTestApp.h"
#include "spfiApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
spfiTestApp::validParams()
{
  InputParameters params = spfiApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  return params;
}

spfiTestApp::spfiTestApp(InputParameters parameters) : MooseApp(parameters)
{
  spfiTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

spfiTestApp::~spfiTestApp() {}

void
spfiTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  spfiApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"spfiTestApp"});
    Registry::registerActionsTo(af, {"spfiTestApp"});
  }
}

void
spfiTestApp::registerApps()
{
  registerApp(spfiApp);
  registerApp(spfiTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
spfiTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  spfiTestApp::registerAll(f, af, s);
}
extern "C" void
spfiTestApp__registerApps()
{
  spfiTestApp::registerApps();
}
