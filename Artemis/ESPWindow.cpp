#include "Window.h"
#include "Draw.h"

#include <ArtemisSpecific/Const.h>

ESPWindow::ESPWindow() : IWindow("ESP Window", WND_ESPWINDOW), IOnFrame(ONFRAME_ESP), bBoneEsp(false) {}

void ESPWindow::Window() {
    static DrawManager* pDraw = &Midnight::GetInst()->ESPDrawManager;

    if (ImGui::Checkbox("Bone ESP", &bBoneEsp)) {
        if (!bBoneEsp) pDraw->Release();
    }

    ImGui::ColorPicker4(
        "Bone Color",
        BoneAndESPDraw::Color(),
        ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8
    );
}

void ESPWindow::OnFrame() {
    static Memory* pm = &Midnight::GetInst()->Mem;
    static DrawManager* pDraw = &Midnight::GetInst()->ESPDrawManager;
    static const ADDRESS c_uGameManager = *(ADDRESS*)(pm->GetModuleBase() + Artemis::Constants::c_GameManager);

    if (bBoneEsp) {
        ADDRESS uPtr = *(ADDRESS*)(c_uGameManager + 0xB0);
        uPtr = ((uPtr - 0x36) >> 0x11 | (uPtr - 0x36) << 0x2F) - 0x58;

        ADDRESS uCount = *(ADDRESS*)(c_uGameManager + 0xB8);
        INT nCount = (((uCount - 0x36) >> 0x11 | (uCount - 0x36) << 0x2F) - 0x58) ^ 0x18C0000000;

        try {
            for (INT i = 0; i < nCount; i++) {
                ADDRESS uEntity = *(ADDRESS*)(uPtr + (i * sizeof(ADDRESS)));
                if (!pDraw->IsIdPresent(*(DWORD*)(&uEntity))) pDraw->RegisterDraw(new BoneAndESPDraw(uEntity));
            }
        }
        catch (AttributeException&) {}
    }
}