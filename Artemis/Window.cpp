#include "Window.h"

#include "Hashes.h"
#include "ClassNames.h"

using namespace Artemis;

MainWindow::MainWindow() : IWindow("Main Window", 1) {}

void MainWindow::Window() {
    ImGui::Text("Welcome to Artemis!");
}

HashWindow::HashWindow() : IWindow("Hash Window", 2) {}

void HashWindow::Window() {
    static CHAR szBuffer[64];
    static CHAR szOutBuffer[64] = { '\0' };
    static Midnight* pInst = Midnight::GetInst();
    static Memory* pm = &pInst->Mem;
    static Logger* pLog = &pInst->Log;

    ImGui::InputText("Vtable*", szBuffer, sizeof(szBuffer));

    if (ImGui::Button("Compute")) {
        ADDRESS uVtblAddress;
        sscanf_s(szBuffer, "%llX", &uVtblAddress);
        pLog->LogInfo(__FUNCTION__, "Vtable address received: 0x%llX", uVtblAddress);
        try {
            pLog->LogInfo(__FUNCTION__, "Reading address of function.");
            ADDRESS uFn = pm->Read<ADDRESS>(uVtblAddress + 7 * sizeof(ADDRESS));
            pLog->LogSuccess(__FUNCTION__, "Address of function: 0x%llX", uFn);

            pLog->LogInfo(__FUNCTION__, "Checking if class has a name function.");
            BYTE szBytes[3];
            static const BYTE szRealBytes[3] = { 0x48, 0x8B, 0x05 };
            pm->ReadArray(uFn, szBytes);

            BOOL bFailed = FALSE;
            for (INT i = 0; i < 3; i++)
                if (szBytes[i] != szRealBytes[i]) {
                    pLog->LogError(__FUNCTION__, "Function is not a name function.");
                    strcpy_s(szOutBuffer, "No name function found.");
                    bFailed = TRUE;
                }

            if (!bFailed) {
                pLog->LogInfo(__FUNCTION__, "Reading RIP offset.");
                DWORD dwRel = pm->Read<DWORD>(uFn + 3);
                pLog->LogSuccess(__FUNCTION__, "RIP offset: 0x%lX", dwRel);

                pLog->LogInfo(__FUNCTION__, "Double pointer to encrypted hash: 0x%llX", uFn + 7 + dwRel);

                pLog->LogInfo(__FUNCTION__, "Reading pointer to hash region.");
                ADDRESS uHashPtr = pm->Read<ADDRESS>(uFn + 7 + dwRel);
                pLog->LogSuccess(__FUNCTION__, "Pointer to hash region: 0x%llX", uHashPtr);

                pLog->LogInfo(__FUNCTION__, "Reading encrypted hash.");
                UINT64 uEncryptedHash = pm->Read<UINT64>(uHashPtr + 4 * sizeof(ADDRESS));
                pLog->LogSuccess(__FUNCTION__, "Encrypted hash: 0x%llX", uEncryptedHash);

                UINT64 uDecryptedHash = uEncryptedHash ^ 0x62B7A045;
                pLog->LogInfo(__FUNCTION__, "Decrypted hash: 0x%llX", uDecryptedHash);

                INT nIndex = -1;
                for (INT i = 0; i < 6981; i++) {
                    if (uDecryptedHash == szHashes[i]) {
                        nIndex = i;
                        break;
                    }
                }

                if (nIndex == -1) {
                    pLog->LogError(__FUNCTION__, "No matching hash found.");
                    strcpy_s(szOutBuffer, "No matching hash found.");
                }
                else {
                    pLog->LogSuccess(__FUNCTION__, "Matching hash found. Class name: %s", lpszClassNames[nIndex]);
                    strcpy_s(szOutBuffer, lpszClassNames[nIndex]);
                }
            }
        }
        catch (MemoryAccessViolationException& e) {
            pLog->LogError(__FUNCTION__, "Failed to read address 0x%llX.", e.GetAddress());
            strcpy_s(szOutBuffer, "Failed to read memory.");
        }
    }

    ImGui::InputText("Name", szOutBuffer, sizeof(szOutBuffer));
}

#include "Draw.h"

ESPWindow::ESPWindow() : IWindow("ESP Window", 3) {}

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

    ImGui::SameLine();

    ImGui::ColorPicker4("Bone Color", BoneAndESPDraw::Color().szVector, ImGuiColorEditFlags_RGB);
}