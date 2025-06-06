#include "ICPlayer.h"
#include "GInterface.h"

GFX_IMPLEMENT_DYNAMIC_EXISTING(CICPlayer, 0x00ef1b44)


// CICPlayer::IsGameMasterMAYBE(void) .text 009D4C40 00000009   R . . . . T .
bool CICPlayer::IsGameMaster() {
    return N000094A7 & 1;
}

std::n_wstring CICPlayer::GetCharName() const {
    return m_wstrCharName;
}

unsigned char CICPlayer::GetCurrentLevel() const {
    return m_btLevel;
}

long long int CICPlayer::GetCurrentExp() const {
    return m_i64CurrentExp;
}

short CICPlayer::GetStatPointAvailable() const {
    return m_sStatPointAvailable;
}

short CICPlayer::GetStrength() const {
    return m_sStrStat;
}

short CICPlayer::GetIntelligence() const {
    return m_sIntStat;
}

const std::n_wstring &CICPlayer::GetJobAlias() const {
    return m_wstrJobAlias;
}

int CICPlayer::GetCurrentJobExperiencePoints() const {
    return reinterpret_cast<int (__thiscall *)(const CICPlayer *)>(0x009d4d20)(this);
}

void CICPlayer::Func_15(int param_1, float *param_2) {
    //printf("%s %d %p\n", __FUNCTION__, param_1, param_2);
    //reinterpret_cast<void (__thiscall *)(const CICharactor *, int param_1, float *param_2)>(0x009d49c0)(this, param_1, param_2);
    if ((param_1 == 3) && (g_pCGInterface != NULL)) {
        g_pCGInterface->FUN_00777a70(0, 1);
    }
    CICUser::Func_15(param_1, param_2);
}

void CICPlayer::Func_15_impl(int param_1, float *param_2) {
    CICPlayer::Func_15(param_1, param_2);
}

undefined4 CICPlayer::FUN_009d68f0() {
    return reinterpret_cast<undefined4 (__thiscall *)(const CICPlayer *)>(0x009d68f0)(this);
}
