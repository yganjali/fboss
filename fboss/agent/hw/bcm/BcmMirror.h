// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include "fboss/agent/FbossError.h"
#include "fboss/agent/hw/bcm/types.h"

namespace facebook {
namespace fboss {
class BcmSwitch;
class AclEntry;
class Mirror;
class BcmPort;
class MirrorTunnel;

enum class MirrorAction { START = 1, STOP = 2 };
enum class MirrorDirection { INGRESS = 1, EGRESS = 2 };

class BcmMirrorDestination {
 public:
  BcmMirrorDestination(int unit, BcmPort* egressPort);
  BcmMirrorDestination(
      int unit,
      BcmPort* egressPort,
      const MirrorTunnel& mirrorTunnel);
  ~BcmMirrorDestination();
  BcmMirrorHandle getHandle();

 private:
  int unit_;
  BcmMirrorHandle handle_;
};

class BcmMirror {
 public:
  BcmMirror(BcmSwitch* hw, const std::shared_ptr<Mirror>& mirror);
  ~BcmMirror();
  void applyPortMirrorAction(
      PortID port,
      MirrorAction action,
      MirrorDirection direction);
  void applyAclMirrorAction(
      BcmAclEntryHandle aclEntryHandle,
      MirrorAction action,
      MirrorDirection direction);
  bool isProgrammed() const {
    /* if this is programmed in hardware */
    return destination_ != nullptr;
  }


 private:
  BcmSwitch* hw_;
  std::shared_ptr<Mirror> mirror_;

  std::unique_ptr<BcmMirrorDestination> destination_;
  void program();
  void applyAclMirrorActions(MirrorAction action);
  void applyPortMirrorActions(MirrorAction action);
};

} // namespace fboss
} // namespace facebook
