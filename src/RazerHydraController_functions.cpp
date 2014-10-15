/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#include <FabricEDK.h>
#include "RazerHydraController.h"

#include <sixense.h>
#include <sixense_math.hpp>
#ifdef _WIN32
#include <sixense_utils/mouse_pointer.hpp>
#endif

#include <sixense_utils/derivatives.hpp>
#include <sixense_utils/button_states.hpp>
#include <sixense_utils/event_triggers.hpp>
#include <sixense_utils/controller_manager/controller_manager.hpp>

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

using namespace Fabric::EDK;

typedef boost::shared_mutex Lock;
typedef boost::unique_lock< Lock >  WriteLock;
typedef boost::shared_lock< Lock >  ReadLock;
Lock gLock;

sixenseUtils::ControllerManager * gController = NULL;
unsigned int gNumInstances = 0;
bool gSetupDone = false;
std::string gLastStepString;

void gSetupCallback(sixenseUtils::ControllerManager::setup_step step)
{
  ReadLock w_lock(gLock);

  if(!gController)
    return;
  if(gController->isMenuVisible())
  {
    gSetupDone = false;

    std::string stepString = gController->getStepString();
    if(gLastStepString != stepString)
    {
      report("Setup step: %s.", stepString.c_str());
      gLastStepString = stepString;
      std::string destination = "[StatusBar]";
      std::string payload = stepString;
      queueStatusMessage(destination.c_str(), destination.length()+1, payload.c_str(), payload.length()+1);
    }

  } else {
    gSetupDone = true;

    if(gLastStepString.length() > 0)
    {
      gLastStepString = "";
      std::string destination = "[StatusBar]";
      std::string payload;
      queueStatusMessage(destination.c_str(), destination.length()+1, payload.c_str(), payload.length()+1);
    }
  }
}


// Defined at src\RazerHydraController.kl:38:1
FABRIC_EXT_EXPORT void Fabric_RazerHydraController_construct(
  KL::Traits< KL::RazerHydraController >::IOParam this_
) {
  WriteLock w_lock(gLock);

  if(gNumInstances == 0)
  {
    if(sixenseInit() == SIXENSE_SUCCESS)
    {
      gController = sixenseUtils::getTheControllerManager();
      gController->setGameType(sixenseUtils::ControllerManager::ONE_PLAYER_TWO_CONTROLLER);
      gController->registerSetupCallback(gSetupCallback);
    }
  }
  gNumInstances++;
  report("Controller initialized.");
}

// Defined at src\RazerHydraController.kl:39:1
FABRIC_EXT_EXPORT void Fabric_RazerHydraController_destruct(
  KL::Traits< KL::RazerHydraController >::IOParam this_
) {
  WriteLock w_lock(gLock);

  gNumInstances--;
  if(gNumInstances == 0)
  {
    gController = NULL;
    sixenseExit();
    report("Controller shutdown.");
  }
}

// Defined at src\RazerHydraController.kl:41:1
FABRIC_EXT_EXPORT KL::Boolean Fabric_RazerHydraController_valid(
  KL::Traits< KL::RazerHydraController >::INParam this_
) {
  ReadLock w_lock(gLock);
  return gController != NULL;
}

// Defined at src\RazerHydraController.kl:42:1
FABRIC_EXT_EXPORT KL::Boolean Fabric_RazerHydraController_readyForMotion(
  KL::Traits< KL::RazerHydraController >::INParam this_
) {
  return gSetupDone && Fabric_RazerHydraController_valid(this_);
}

// Defined at src\RazerHydraController.kl:44:1
FABRIC_EXT_EXPORT KL::SInt32 RazerHydraController_getMaxBases(
  KL::Traits< KL::RazerHydraController >::INParam this_
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return 0;
  return (KL::SInt32)sixenseGetMaxBases();
}



// Defined at src\RazerHydraController.kl:45:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_setActiveBase(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam i
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseSetActiveBase(i) == SIXENSE_SUCCESS;
}

// Defined at src\RazerHydraController.kl:46:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_isBaseConnected(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam i
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseIsBaseConnected(i) == SIXENSE_SUCCESS;
}

// Defined at src\RazerHydraController.kl:47:1
FABRIC_EXT_EXPORT KL::SInt32 RazerHydraController_getMaxControllers(
  KL::Traits< KL::RazerHydraController >::INParam this_
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return 0;
  return (KL::SInt32)sixenseGetMaxControllers();
}

// Defined at src\RazerHydraController.kl:48:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_isControllerEnabled(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam which
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseIsControllerEnabled(which) == SIXENSE_SUCCESS;
}

// Defined at src\RazerHydraController.kl:49:1
FABRIC_EXT_EXPORT KL::SInt32 RazerHydraController_getNumActiveControllers(
  KL::Traits< KL::RazerHydraController >::INParam this_
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return (KL::SInt32)sixenseGetNumActiveControllers();
}

// Defined at src\RazerHydraController.kl:50:1
FABRIC_EXT_EXPORT KL::SInt32 RazerHydraController_getHistorySize(
  KL::Traits< KL::RazerHydraController >::INParam this_
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return (KL::SInt32)sixenseGetHistorySize();
}

// Defined at src\RazerHydraController.kl:51:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_update(
  KL::Traits< KL::RazerHydraController >::INParam this_
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  if(sixenseGetAllNewestData((sixenseAllControllerData*)&this_->controls) != SIXENSE_SUCCESS)
    return false;
  sixenseUtils::getTheControllerManager()->update((sixenseAllControllerData*)&this_->controls[0]);
  return true;
}

// Defined at src\RazerHydraController.kl:52:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_setHemisphereTrackingMode(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam which_controller,
  KL::Traits< KL::SInt32 >::INParam state
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseSetHemisphereTrackingMode(which_controller, state);
}

// Defined at src\RazerHydraController.kl:53:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_getHemisphereTrackingMode(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam which_controller,
  KL::Traits< KL::SInt32 >::IOParam state
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  int state_ = 0;
  bool result = sixenseGetHemisphereTrackingMode(which_controller, &state_);
  state = state_;
  return result;
}

// Defined at src\RazerHydraController.kl:54:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_autoEnableHemisphereTracking(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam which_controller
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseAutoEnableHemisphereTracking(which_controller);
}

// Defined at src\RazerHydraController.kl:55:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_setHighPriorityBindingEnabled(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam on_or_off
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseSetHighPriorityBindingEnabled(on_or_off);
}

// Defined at src\RazerHydraController.kl:56:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_getHighPriorityBindingEnabled(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::IOParam on_or_off
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  int on_or_off_ = 0;
  bool result = sixenseGetHighPriorityBindingEnabled(&on_or_off);
  on_or_off = on_or_off_;
  return result;
}

// Defined at src\RazerHydraController.kl:57:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_triggerVibration(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam controller_id,
  KL::Traits< KL::SInt32 >::INParam duration_100ms,
  KL::Traits< KL::SInt32 >::INParam pattern_id
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseTriggerVibration(controller_id, duration_100ms, pattern_id);
}

// Defined at src\RazerHydraController.kl:58:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_setFilterEnabled(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam on_or_off
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseSetFilterEnabled(on_or_off);
}

// Defined at src\RazerHydraController.kl:59:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_getFilterEnabled(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::SInt32 >::IOParam on_or_off
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  int on_or_off_ = 0;
  bool result = sixenseGetFilterEnabled(&on_or_off);
  on_or_off = on_or_off_;
  return result;
}

// Defined at src\RazerHydraController.kl:60:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_setFilterParams(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::Float32 >::INParam near_range,
  KL::Traits< KL::Float32 >::INParam near_val,
  KL::Traits< KL::Float32 >::INParam far_range,
  KL::Traits< KL::Float32 >::INParam far_val
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseSetFilterParams(near_range, near_val, far_range, far_val);
}

// Defined at src\RazerHydraController.kl:61:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_getFilterParams(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::Float32 >::IOParam near_range,
  KL::Traits< KL::Float32 >::IOParam near_val,
  KL::Traits< KL::Float32 >::IOParam far_range,
  KL::Traits< KL::Float32 >::IOParam far_val
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseGetFilterParams(&near_range, &near_val, &far_range, &far_val);
}

// Defined at src\RazerHydraController.kl:62:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_setBaseColor(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::UInt8 >::INParam red,
  KL::Traits< KL::UInt8 >::INParam green,
  KL::Traits< KL::UInt8 >::INParam blue
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseSetBaseColor(red, green, blue);
}

// Defined at src\RazerHydraController.kl:63:1
FABRIC_EXT_EXPORT KL::Boolean RazerHydraController_getBaseColor(
  KL::Traits< KL::RazerHydraController >::INParam this_,
  KL::Traits< KL::UInt8 >::IOParam red,
  KL::Traits< KL::UInt8 >::IOParam green,
  KL::Traits< KL::UInt8 >::IOParam blue
) {
  ReadLock w_lock(gLock);
  if(!Fabric_RazerHydraController_valid(this_))
    return false;
  return sixenseGetBaseColor(&red, &green, &blue);
}
