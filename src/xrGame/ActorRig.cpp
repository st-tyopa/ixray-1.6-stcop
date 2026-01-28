#include "ActorRig.h"

CRig::CRig()
{
    m_flags.set(FUsingCondition, FALSE);
}

void CRig::Load(LPCSTR section)
{
    inherited::Load(section);

    m_additional_weight = READ_IF_EXISTS(pSettings, r_float, section, "additional_inventory_weight", 0.0f);
    m_rig_width = READ_IF_EXISTS(pSettings, r_u32, section, "rig_width", 7);
    m_rig_height = READ_IF_EXISTS(pSettings, r_u32, section, "rig_height", 2);

    m_flags.set(FUsingCondition, READ_IF_EXISTS(pSettings, r_bool, section, "use_condition", FALSE));
}