#include "Window.h"

#include "Draw.h"

ESPWindow::ESPWindow() : IWindow("ESP Window", WND_ESPWINDOW) {}

bool bBoneEsp = false;

void ESPWindow::Window() {
    static DrawManager* pDraw = &Midnight::GetInst()->ESPDrawManager;
    static Memory* pm = &Midnight::GetInst()->Mem;
    static Logger* pLog = &Midnight::GetInst()->Log;
    const ADDRESS c_GameManager = *(ADDRESS*)(pm->GetModuleBase() + 0x6E3FF30);

    if (bBoneEsp) {
        ADDRESS uPtr = *(ADDRESS*)(c_GameManager + 0xB0);
        uPtr = ((uPtr - 0x36) >> 0x11 | (uPtr - 0x36) << 0x2F) - 0x58;

        ADDRESS uCount = *(ADDRESS*)(c_GameManager + 0xB8);
        INT nCount = (((uCount - 0x36) >> 0x11 | (uCount - 0x36) << 0x2F) - 0x58) ^ 0x18C0000000;

        try {
            for (INT i = 0; i < nCount; i++) {
                ADDRESS uEntity = *(ADDRESS*)(uPtr + (i * sizeof(ADDRESS)));
                try {
                    pDraw->GetDrawById(*(DWORD*)(&uEntity));
                }
                catch (ObjectNotFoundException&) {
                    pDraw->RegisterDraw(new BoneAndESPDraw(uEntity));
                }
            }
        }
        catch (AttributeException&) {}
    }

    if (ImGui::Checkbox("Bone ESP", &bBoneEsp)) {
        if (!bBoneEsp) pDraw->Release();
    }

    ImGui::ColorPicker4("Bone Color", BoneAndESPDraw::Color().szVector, ImGuiColorEditFlags_NoInputs);
}