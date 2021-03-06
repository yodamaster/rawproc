
#include "PicProcessor.h"
#include "PicProcessorHighlight.h"
#include "PicProcPanel.h"
#include "myConfig.h"
#include "undo.xpm"

#include "util.h"

class HighlightPanel: public PicProcPanel
{
	public:
		HighlightPanel(wxWindow *parent, PicProcessor *proc, wxString params): PicProcPanel(parent, proc, params)
		{
			wxArrayString p = split(params,",");

			int hlt = atoi(p[0]);
			int thr = atoi(p[1]);

			SetSize(parent->GetSize());
			g->Add(0,10, wxGBPosition(0,0));

			g->Add(new wxStaticText(this,wxID_ANY, "highlight: "), wxGBPosition(1,0), wxDefaultSpan, wxALIGN_LEFT | wxALL, 3);
			highlight = new wxSlider(this, wxID_ANY, hlt, -50, +50, wxPoint(10, 30), wxSize(140, -1));
			g->Add(highlight , wxGBPosition(1,1), wxDefaultSpan, wxALIGN_LEFT | wxALL, 3);
			val1 = new wxStaticText(this,wxID_ANY, wxString::Format("%4d",hlt), wxDefaultPosition, wxSize(30, -1));
			g->Add(val1 , wxGBPosition(1,2), wxDefaultSpan, wxALIGN_LEFT | wxALL, 3);
			btn1 = new wxBitmapButton(this, 1000, wxBitmap(undo_xpm), wxPoint(0,0), wxSize(-1,-1), wxBU_EXACTFIT);
			btn1->SetToolTip("Reset highlight to default");
			g->Add(btn1, wxGBPosition(1,3), wxDefaultSpan, wxALIGN_LEFT | wxALL, 3);


			g->Add(new wxStaticText(this,wxID_ANY, "threshold: "), wxGBPosition(2,0), wxDefaultSpan, wxALIGN_LEFT | wxALL, 3);
			threshold = new wxSlider(this, wxID_ANY, thr, 128, 255, wxPoint(10, 30), wxSize(140, -1));
			g->Add(threshold , wxGBPosition(2,1), wxDefaultSpan, wxALIGN_LEFT | wxALL, 3);
			val2 = new wxStaticText(this,wxID_ANY, wxString::Format("%4d",thr), wxDefaultPosition, wxSize(30, -1));
			g->Add(val2 , wxGBPosition(2,2), wxDefaultSpan, wxALIGN_LEFT | wxALL, 3);
			btn2 = new wxBitmapButton(this, 2000, wxBitmap(undo_xpm), wxPoint(0,0), wxSize(-1,-1), wxBU_EXACTFIT);
			btn2->SetToolTip("Reset threshold to default");
			g->Add(btn2, wxGBPosition(2,3), wxDefaultSpan, wxALIGN_LEFT | wxALL, 3);

			SetSizerAndFit(g);
			g->Layout();
			Refresh();
			Update();
			SetFocus();
			t = new wxTimer(this);
			Bind(wxEVT_BUTTON, &HighlightPanel::OnButton, this);
			Bind(wxEVT_SCROLL_CHANGED, &HighlightPanel::OnChanged, this);
			Bind(wxEVT_SCROLL_THUMBTRACK, &HighlightPanel::OnThumbTrack, this);
			Bind(wxEVT_TIMER, &HighlightPanel::OnTimer,  this);
		}

		~HighlightPanel()
		{
			t->~wxTimer();
		}

		void OnChanged(wxCommandEvent& event)
		{
			val1->SetLabel(wxString::Format("%4d", highlight->GetValue()));
			val2->SetLabel(wxString::Format("%4d", threshold->GetValue()));
			t->Start(500,wxTIMER_ONE_SHOT);
		}

		void OnThumbTrack(wxCommandEvent& event)
		{
			val1->SetLabel(wxString::Format("%4d", highlight->GetValue()));
			val2->SetLabel(wxString::Format("%4d", threshold->GetValue()));
		}

		void OnTimer(wxTimerEvent& event)
		{
			q->setParams(wxString::Format("%d,%d",highlight->GetValue(),threshold->GetValue()));
			q->processPic();
			event.Skip();
		}

		void OnButton(wxCommandEvent& event)
		{
			int resethighlightval, resetthresholdval;
			switch (event.GetId()) {
				case 1000:
					resethighlightval = atoi(myConfig::getConfig().getValueOrDefault("tool.highlight.level","0").c_str());
					highlight->SetValue(resethighlightval);
					val1->SetLabel(wxString::Format("%4d", resethighlightval));
					break;
				case 2000:
					resetthresholdval = atoi(myConfig::getConfig().getValueOrDefault("tool.highlight.threshold","192").c_str());
					threshold->SetValue(resetthresholdval);
					val2->SetLabel(wxString::Format("%4d", resetthresholdval));
					break;
			}
			q->setParams(wxString::Format("%d,%d", highlight->GetValue(),threshold->GetValue()));
			q->processPic();
			event.Skip();
		}


	private:
		wxSlider *highlight, *threshold;
		wxStaticText *val1, *val2;
		wxBitmapButton *btn1, * btn2;
		wxTimer *t;

};


PicProcessorHighlight::PicProcessorHighlight(wxString name, wxString command, wxTreeCtrl *tree, PicPanel *display): PicProcessor(name, command,  tree, display) 
{
	//showParams();
}

void PicProcessorHighlight::createPanel(wxSimplebook* parent)
{
	toolpanel = new HighlightPanel(parent, this, c);
	parent->ShowNewPage(toolpanel);
	toolpanel->Refresh();
	toolpanel->Update();
}

bool PicProcessorHighlight::processPic(bool processnext) {
	bool result = true;
	((wxFrame*) m_display->GetParent())->SetStatusText("highlight...");

	wxArrayString cp = split(getParams(),",");
	double shd = atof(cp[0]);
	double thr = atof(cp[1]);

	Curve ctrlpts;
	ctrlpts.insertpoint(0,0);
	ctrlpts.insertpoint(thr-20,thr-20);
	ctrlpts.insertpoint(thr,thr);
	ctrlpts.insertpoint((thr+thr/2)-shd,(thr+thr/2)+shd);
	ctrlpts.insertpoint(255,255);
	
	int threadcount =  atoi(myConfig::getConfig().getValueOrDefault("tool.highlight.cores","0").c_str());
	if (threadcount == 0) 
		threadcount = gImage::ThreadCount();
	else if (threadcount < 0) 
		threadcount = std::max(gImage::ThreadCount() + threadcount,0);

	mark();
	if (dib) delete dib;
	dib = new gImage(getPreviousPicProcessor()->getProcessedPic());
	dib->ApplyToneCurve(ctrlpts.getControlPoints(), threadcount);
	wxString d = duration();

	if ((myConfig::getConfig().getValueOrDefault("tool.all.log","0") == "1") || (myConfig::getConfig().getValueOrDefault("tool.highlight.log","0") == "1"))
		log(wxString::Format("tool=highlight,imagesize=%dx%d,threads=%d,time=%s",dib->getWidth(), dib->getHeight(),threadcount,d));

	dirty = false;
	
	((wxFrame*) m_display->GetParent())->SetStatusText("");
	if (processnext) processNext();

	return result;
}



