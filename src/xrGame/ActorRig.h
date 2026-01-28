#pragma once

#include "inventory_item_object.h"

class CRig final : public CInventoryItemObject
{
private:
    using inherited = CInventoryItemObject;

public:
    CRig();
    ~CRig() = default;

    virtual void Load(LPCSTR section) override;
    virtual CRig* cast_rig() override { return this; }

    u32 m_rig_width = 7;
    u32 m_rig_height = 1;
    float m_additional_weight = 0.0f;
};
