#include "Window.h"

CreditsWindow::CreditsWindow() : IWindow("Credits Window", WND_CREDITSWINDOW) {}

void CreditsWindow::Window() {
	ImGui::Text("Reserved Artemis_Credits");
	ImGui::NewLine();
	ImGui::Text("ImGui license:");
	ImGui::Text("The MIT License(MIT)");
	ImGui::NewLine();
	ImGui::Text("Copyright(c) 2014 - 2022 Omar Cornut");
	ImGui::NewLine();
	ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
	ImGui::Text("of this softwareand associated documentation files(the \"Software\"), to deal");
	ImGui::Text("in the Software without restriction, including without limitation the rights");
	ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and /or sell");
	ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
	ImGui::Text("furnished to do so, subject to the following conditions :");
	ImGui::NewLine();
	ImGui::Text("The above copyright notice and this permission notice shall be included in all");
	ImGui::Text("copies or substantial portions of the Software.");
	ImGui::NewLine();
	ImGui::Text("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
	ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
	ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
	ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
	ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
	ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
	ImGui::Text("SOFTWARE.");
	ImGui::NewLine();

	ImGui::Text("MinHook license:");
	ImGui::Text("MinHook - The Minimalistic API Hooking Library for x64/x86");
	ImGui::Text("Copyright (C) 2009-2017 Tsuda Kageyu.");
	ImGui::Text("All rights reserved.");
	ImGui::NewLine();
	ImGui::Text("Redistribution and use in source and binary forms, with or without");
	ImGui::Text("modification, are permitted provided that the following conditions");
	ImGui::Text("are met:");
	ImGui::NewLine();
	ImGui::Text("1. Redistributions of source code must retain the above copyright");
	ImGui::Text("notice, this list of conditions and the following disclaimer.");
	ImGui::Text("2. Redistributions in binary form must reproduce the above copyright");
	ImGui::Text("notice, this list of conditions and the following disclaimer in the");
	ImGui::Text("documentation and/or other materials provided with the distribution.");
	ImGui::NewLine();
	ImGui::Text("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS");
	ImGui::Text("\"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED");
	ImGui::Text("TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A");
	ImGui::Text("PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER");
	ImGui::Text("OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,");
	ImGui::Text("EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,");
	ImGui::Text("PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR");
	ImGui::Text("PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF");
	ImGui::Text("LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING");
	ImGui::Text("NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS");
	ImGui::Text("SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.");
}