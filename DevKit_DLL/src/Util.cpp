#include "Util.h"

#include <sys/stat.h>

#include "support/hook.h"

#include "hooks/CGame_Hook.h"
#include "hooks/GFXVideo3d_Hook.h"
#include "hooks/Hooks.h"

#include "hooks/WndProc_Hook.h"

#include "ICMonster.h"
#include "ICPlayer.h"
#include "QuickStart.h"
#include <AutoPotion.h>
#include <BSLib/Debug.h>
#include <GFX3DFunction/RStateMgr.h>
#include <GInterface.h>
#include <ICUser.h>
#include <IFChatViewer.h>
#include <NetProcessIn.h>
#include <NetProcessSecond.h>
#include <NetProcessThird.h>
#include <PSCharacterSelect.h>

std::vector<const CGfxRuntimeClass *> register_objects;
std::vector<overrideFnPtr> override_objects;

QuickStart quickstart;

void Setup() {

#ifdef CONFIG_DEBUG_CONSOLE
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
#endif

    vftableHook(0x00E0963C, 17, addr_from_this(&CGFXVideo3D_Hook::CreateThingsHook));
    vftableHook(0x00E0963C, 26, addr_from_this(&CGFXVideo3D_Hook::EndSceneHook));
    vftableHook(0x00E0963C, 20, addr_from_this(&CGFXVideo3D_Hook::SetSizeHook));

    vftableHook(0x00db95a4, 10, addr_from_this(&CGInterface::OnCreateIMPL));
    vftableHook(0x00db95a4, 5, addr_from_this(&CGInterface::OnTimerIMPL));

    replaceAddr(0x00d71100 + 1, addr_from_this(&CGInterface::OnCharIMPL));

    vftableHook(0x00dd811c, 10, addr_from_this(&CPSCharacterSelect::OnCreateIMPL));

    vftableHook(0x00de2e7c, 15, addr_from_this(&CICUser::Func_15_impl));
    vftableHook(0x00de256c, 15, addr_from_this(&CICharactor::Func_15_impl));
    vftableHook(0x00de2c24, 15, addr_from_this(&CICPlayer::Func_15_impl));
    vftableHook(0x00de26c4, 15, addr_from_this(&CICMonster::Func_15_impl));

    replaceAddr(0x00831337 + 4, (int) &WndProcHook);

    placeHook(0x0065c6f0, addr_from_this(&CAlramGuideMgrWnd::CreateGuideIcon));

    replaceOffset(0x008491d1, addr_from_this(&CGame_Hook::LoadGameOption));

    replaceOffset(0x00832a11, addr_from_this(&CGame_Hook::InitGameAssets_Impl));

    replaceOffset(0x0084c9bf, addr_from_this(&CNetProcessIn::RegisterPacketHandlers));
    replaceOffset(0x00898656, addr_from_this(&CNetProcessSecond::RegisterPacketHandlers));
    replaceOffset(0x008a4876, addr_from_this(&CNetProcessThird::RegisterPacketHandlers));

    replaceOffset(0x009ded0d, addr_from_this(&CRStateMgr::FUN_00470060));

    replaceOffset(0x0060bbbf, addr_from_this(&CNIFUnderMenuBar::UseSlot));

    placeHook(0x0049d620, Put);

#ifdef CONFIG_DEBUG_REDIRECT_PUTDUMP
    replaceAddr(0x00832927 + 1, (int) &DebugPrintCallback);
#endif // CONFIG_DEBUG_REDIRECT_PUTDUMP

#ifdef CONFIG_TRANSLATIONS_DEBUG
    placeHook(0x008C9C30, addr_from_this(&CTextStringManager::GetString));
#endif // CONFIG_TRANSLATIONS_DEBUG

#ifdef CONFIG_CHATVIEWER
    replaceOffset(0x008774f4, (int)&WriteToChatWindow);
    replaceOffset(0x00877b5c, (int)&WriteToChatWindow);

    placeHook(0x007a9bd0, addr_from_this(&CIFChatViewer::ShowHideControls));
#endif // CONFIG_CHATVIEWER

    replaceOffset(0x00558618, addr_from_this(&CIFConsole::SetVisibleMode));
    replaceOffset(0x0055a027, addr_from_this(&CIFConsole::InitializeCommandsList));

    quickstart.Setup();
}

bool DoesFileExists(const std::string &name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

void RegisterObject(const CGfxRuntimeClass *obj) {
    register_objects.push_back(obj);
}

void OverrideObject(overrideFnPtr fn) {
    override_objects.push_back(fn);
}

void InstallRuntimeClasses(CGame *) {
    // Replace Create & Delete for existing classes
    // Note: We can't just inject existing objects like we would do with new objects.
    //       Joymax uses == on GFX_RUNTIME_CLASS(), so we would end up breaking this comparison

    for (std::vector<const CGfxRuntimeClass *>::const_iterator it = register_objects.begin();
         it != register_objects.end(); ++it) {
        reinterpret_cast<void (__thiscall *)(const CGfxRuntimeClass *, const char *, void *, void *,const CGfxRuntimeClass *, size_t, int)>(0x00B9C9C0)(*it,(*it)->m_lpszClassName, (*it)->m_pfnCreateObject, (*it)->m_pfnDeleteObject, (*it)->m_pBaseClass, (*it)->m_nObjectSize, 0);
    }

    for (std::vector<overrideFnPtr>::const_iterator it = override_objects.begin(); it != override_objects.end(); ++it) {
        (*it)();
    }
}
