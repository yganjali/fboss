/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include "SaiApi.h"
#include "SaiAttribute.h"

#include <folly/logging/xlog.h>

#include <boost/variant.hpp>

extern "C" {
  #include <sai.h>
}

namespace facebook {
namespace fboss {

struct VlanTypes {
  struct Attributes {
    using EnumType = sai_vlan_attr_t;
    using VlanId = SaiAttribute<EnumType, SAI_VLAN_ATTR_VLAN_ID, sai_uint16_t>;
    using MemberList = SaiAttribute<
        EnumType,
        SAI_VLAN_ATTR_MEMBER_LIST,
        sai_object_list_t,
        std::vector<sai_object_id_t>>;
  };
  using AttributeType =
      boost::variant<Attributes::VlanId, Attributes::MemberList>;
  struct MemberAttributes {
    using EnumType = sai_vlan_member_attr_t;
    using BridgePortId = SaiAttribute<
        EnumType,
        SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID,
        sai_object_id_t,
        SaiObjectIdT>;
    using VlanId = SaiAttribute<
        EnumType,
        SAI_VLAN_MEMBER_ATTR_VLAN_ID,
        sai_object_id_t,
        SaiObjectIdT>;
  };
  using MemberAttributeType =
      boost::variant<MemberAttributes::BridgePortId, MemberAttributes::VlanId>;
  struct EntryType {};
};

class VlanApi : public SaiApi<VlanApi, VlanTypes> {
 public:
  VlanApi() {
    sai_status_t res = sai_api_query(SAI_API_VLAN, (void**)&api_);
    if (res != SAI_STATUS_SUCCESS) {
      throw SaiApiError(res);
    }
  }
  VlanApi(const VlanApi& other) = delete;
 private:
  sai_status_t _create(
      sai_object_id_t* vlan_id,
      sai_attribute_t* attr_list,
      size_t count,
      sai_object_id_t switch_id) {
    return api_->create_vlan(vlan_id, switch_id, count, attr_list);
  }
  sai_status_t _remove(sai_object_id_t vlan_id) {
    return api_->remove_vlan(vlan_id);
  }
  sai_status_t _getAttr(sai_attribute_t* attr, sai_object_id_t handle) const {
    return api_->get_vlan_attribute(handle, 1, attr);
  }
  sai_status_t _setAttr(const sai_attribute_t* attr, sai_object_id_t handle) {
    return api_->set_vlan_attribute(handle, attr);
  }
  sai_status_t _createMember(
      sai_object_id_t* vlan_member_id,
      sai_attribute_t* attr_list,
      size_t count,
      sai_object_id_t switch_id) {
    return api_->create_vlan_member(
        vlan_member_id, switch_id, count, attr_list);
  }
  sai_status_t _removeMember(sai_object_id_t vlan_member_id) {
    return api_->remove_vlan_member(vlan_member_id);
  }
  sai_status_t _getMemberAttr(sai_attribute_t* attr, sai_object_id_t handle)
      const {
    return api_->get_vlan_member_attribute(handle, 1, attr);
  }
  sai_status_t _setMemberAttr(
      const sai_attribute_t* attr,
      sai_object_id_t handle) {
    return api_->set_vlan_member_attribute(handle, attr);
  }
  sai_vlan_api_t* api_;
  friend class SaiApi<VlanApi, VlanTypes>;
};

} // namespace fboss
} // namespace facebook
