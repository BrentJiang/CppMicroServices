/*=============================================================================
 
 Library: CppMicroServices
 
 Copyright (c) The CppMicroServices developers. See the COPYRIGHT
 file at the top-level directory of this distribution and at
 https://github.com/CppMicroServices/CppMicroServices/COPYRIGHT .
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 =============================================================================*/

//#include "cppmicroservices/Any.h"
#include "cppmicroservices/Bundle.h"
#include "cppmicroservices/BundleContext.h"
#include "cppmicroservices/Constants.h"
#include "cppmicroservices/Framework.h"
#include "cppmicroservices/FrameworkEvent.h"
#include "cppmicroservices/FrameworkFactory.h"

#include "TestingConfig.h"
#include "TestingMacros.h"
#include "TestUtils.h"

using namespace cppmicroservices;

// bundle.activator property not specified in the manifest and bundle has no activator class
void TestNoPropertyNoClass(Framework& framework)
{
  auto bundle = testing::InstallLib(framework.GetBundleContext(), "TestBundleR");
  US_TEST_CONDITION_REQUIRED(bundle, "Test for existing bundle TestBundleR");
  US_TEST_NO_EXCEPTION(bundle.Start());
}

// bundle.activator property not specified in the manifest and bundle has an activator class
void TestNoPropertyWithClass(Framework& framework)
{
  auto bundle = testing::InstallLib(framework.GetBundleContext(), "TestBundleBA_X1");
  US_TEST_CONDITION_REQUIRED(bundle, "Test for existing bundle TestBundleBA_X1");
  US_TEST_NO_EXCEPTION(bundle.Start());
  // verify bundle activator was not called => service not registered
  ServiceReferenceU ref = bundle.GetBundleContext().GetServiceReference("cppmicroservices::TestBundleBA_X1Service");
  US_TEST_CONDITION(!ref, "Invalid reference");
}

// bundle.activator property set to false in the manifest and bundle has no activator class
void TestPropertyFalseWithoutClass(Framework& framework)
{
  auto bundle = testing::InstallLib(framework.GetBundleContext(), "TestBundleBA_00");
  US_TEST_CONDITION_REQUIRED(bundle, "Test for existing bundle TestBundleBA_00");
  US_TEST_NO_EXCEPTION(bundle.Start());
}

// bundle.activator property set to false in the manifest and bundle has an activator class
void TestPropertyFalseWithClass(Framework& framework)
{
  auto bundle = testing::InstallLib(framework.GetBundleContext(), "TestBundleBA_01");
  US_TEST_CONDITION_REQUIRED(bundle, "Test for existing bundle TestBundleBA_01");
  US_TEST_NO_EXCEPTION(bundle.Start());
  // verify bundle activator was not called => service not registered
  ServiceReferenceU ref = bundle.GetBundleContext().GetServiceReference("cppmicroservices::TestBundleBA_01Service");
  US_TEST_CONDITION(!ref, "Invalid reference");
}

// bundle.activator property set to true in the manifest and bundle has no activator class
void TestPropertyTrueWithoutClass(Framework& framework)
{
  auto bundle = testing::InstallLib(framework.GetBundleContext(), "TestBundleBA_10");
  US_TEST_CONDITION_REQUIRED(bundle, "Test for existing bundle TestBundleBA_10");
  US_TEST_FOR_EXCEPTION(std::runtime_error,bundle.Start());
}

// bundle.activator property set to true in the manifest and bundle has an activator class
void TestPropertyTrueWithClass(Framework& framework)
{
  auto bundle = testing::InstallLib(framework.GetBundleContext(), "TestBundleA");
  US_TEST_CONDITION_REQUIRED(bundle, "Test for existing bundle TestBundleA");
  US_TEST_NO_EXCEPTION(bundle.Start());
  // verify bundle activator was called => service is registered
  ServiceReferenceU ref = bundle.GetBundleContext().GetServiceReference("cppmicroservices::TestBundleAService");
  US_TEST_CONDITION(ref, "Valid reference");
}

int BundleActivatorTest(int /*argc*/, char* /*argv*/[])
{
  US_TEST_BEGIN("BundleActivatorTest");
  
  FrameworkFactory factory;
  auto framework = factory.NewFramework();
  framework.Start();
  
  // Test points to validate Bundle behavior based on bundle.activator property
  TestNoPropertyNoClass(framework);
  TestNoPropertyWithClass(framework);
  TestPropertyFalseWithoutClass(framework);
  TestPropertyFalseWithClass(framework);
  TestPropertyTrueWithoutClass(framework);
  TestPropertyTrueWithClass(framework);
  
  framework.Stop();
  framework.WaitForStop(std::chrono::milliseconds(0));
  
  US_TEST_END()
}
