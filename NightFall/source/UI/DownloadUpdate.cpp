/*Copyright (c) 2020 D3fau4

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "UI/DownloadUpdate.hpp"

#include <math.h>
#include <fstream>
#include <string>
#include <iostream>
#include "net/net.hpp"

namespace i18n = brls::i18n;	// for loadTranslations() and getStr()
using namespace i18n::literals; // for _i18n
json jso1n;
extern BackGround::BackgroundTasks Download;
bool nextframe = false;
bool prevframe = false;
bool showdialog = false;
DownloadUpdatePage::DownloadUpdatePage(brls::StagedAppletFrame *frame)
    : frame(frame)
{
	appletSetHandlingHomeButtonShortPressedEnabled(true);
    // Label
    i18n::loadTranslations();
    std::ifstream i("/switch/NightFall/temp.json");
    i >> jso1n;
    brls::Application::setGlobalQuit(false);
    this->progressDisp = new brls::ProgressDisplay();
    this->progressDisp->setProgress(Download.m_DownloadProgress, jso1n["fw_info"]["files"].get<int>());
    this->progressDisp->setParent(this);
    this->label = new brls::Label(brls::LabelStyle::DIALOG, "DownloadUpdate/downloading"_i18n.c_str(), true);
    this->label->setHorizontalAlign(NVG_ALIGN_CENTER);
    this->label->setParent(this);
    this->label1 = new brls::Label(brls::LabelStyle::DESCRIPTION, "Firmware: " + jso1n["fw_info"]["version"].get<std::string>());
    this->label1->setHorizontalAlign(NVG_ALIGN_CENTER);
    this->label1->setParent(this);
}

void DownloadUpdatePage::draw(NVGcontext *vg, int x, int y, unsigned width, unsigned height, brls::Style *style, brls::FrameContext *ctx)
{
    if (Download.m_DownloadProgress >= jso1n["fw_info"]["files"].get<int>())
    {
        Download.m_Download = false;
		this->frame->nextStage();
		/*
        brls::Dialog *dialog = new brls::Dialog("DownloadUpdate/start_update"_i18n.c_str());
        brls::GenericEvent::Callback ContinueCallback = [dialog](brls::View *view) {
            dialog->close();
            nextframe = true;
        };
        brls::GenericEvent::Callback cancelCallback = [dialog](brls::View *view) {
            dialog->close();
            prevframe = true;
        };
        if (nextframe == true)
        {
            this->frame->nextStage();
        }
        if (prevframe == true)
        {
            this->frame->previousStage();
        }
        dialog->addButton("DownloadUpdate/button/continue"_i18n.c_str(), ContinueCallback);
        dialog->addButton("DownloadUpdate/button/cancel"_i18n.c_str(), cancelCallback);
        dialog->setCancelable(false);
        if (showdialog != true)
        {
            showdialog = true;
            dialog->open();
        }
		
		
		*/
    }
    else if (Download.m_Download != true)
    {
        Download.m_Download = true;
    }
    this->progressDisp->setProgress(Download.m_DownloadProgress, jso1n["fw_info"]["files"].get<int>());
    this->progressDisp->frame(ctx);
    this->label->frame(ctx);
    this->label1->frame(ctx);
}

void DownloadUpdatePage::layout(NVGcontext *vg, brls::Style *style, brls::FontStash *stash)
{
    this->label->setWidth(roundf((float)this->width * style->CrashFrame.labelWidth));
    this->label->invalidate(true);

    this->label->setBoundaries(
        this->x + this->width / 2 - this->label->getWidth() / 2,
        this->y + (this->height - style->AppletFrame.footerHeight) / 2,
        this->label->getWidth(),
        this->label->getHeight());

    this->label1->setWidth(roundf((float)this->width * style->CrashFrame.labelWidth));
    this->label->invalidate(true);

    this->label1->setBoundaries(
        this->x + this->width / 2 - this->label->getWidth() / 9.2,
        this->y + (this->height - style->AppletFrame.footerHeight) / 1.7,
        this->label1->getWidth(),
        this->label1->getHeight());

    this->progressDisp->setBoundaries(
        this->x + this->width / 2 - style->CrashFrame.buttonWidth,
        this->y + this->height / 2,
        style->CrashFrame.buttonWidth * 2,
        style->CrashFrame.buttonHeight);
}

void DownloadUpdatePage::willAppear(bool resetState)
{
    this->progressDisp->willAppear(resetState);
}

void DownloadUpdatePage::willDisappear(bool resetState)
{
    this->progressDisp->willDisappear(resetState);
}

DownloadUpdatePage::~DownloadUpdatePage()
{
	Download.m_Download = false;
	appletSetHandlingHomeButtonShortPressedEnabled(false);
    delete this->progressDisp;
    delete this->label;
    delete this->label1;
}