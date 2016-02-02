
#include "PicProcessor.h"
#include "PicProcessorHighlight.h"
#include "PicProcPanel.h"
#include "FreeImage.h"
#include "FreeImage16.h"
#include "wxTouchSlider.h"

class HighlightPanel: public PicProcPanel
{
	public:
		HighlightPanel(wxPanel *parent, PicProcessor *proc, wxString params): PicProcPanel(parent, proc, params)
		{
			wxSizerFlags flags = wxSizerFlags().Left().Border(wxLEFT|wxRIGHT).Expand();
			slide = new wxTouchSlider((wxFrame *) this, "", atoi(p.c_str()), -50, 50);
			b->Add(slide, flags);
			SetSizerAndFit(b);
			b->Layout();
			Refresh();
			Update();
			SetFocus();
			Connect(wxID_ANY, wxEVT_SCROLL_THUMBRELEASE,wxCommandEventHandler(HighlightPanel::paramChanged));
		}

		~HighlightPanel()
		{
			slide->~wxTouchSlider();
		}

		void paramChanged(wxCommandEvent& event)
		{
			q->setParams(wxString::Format("%d",event.GetInt()));
			event.Skip();
		}


	private:
		wxTouchSlider *slide;

};


PicProcessorHighlight::PicProcessorHighlight(wxString name, wxString command, wxTreeCtrl *tree, PicPanel *display, wxPanel *parameters): PicProcessor(name, command,  tree, display, parameters) 
{
	showParams();
}

void PicProcessorHighlight::showParams()
{
	if (!m_parameters) return;
	m_parameters->DestroyChildren();
	r = new HighlightPanel(m_parameters, this, c);
}

bool PicProcessorHighlight::processPic() {
	((wxFrame*) m_parameters->GetParent())->SetStatusText("highlight...");
	m_tree->SetItemBold(GetId(), true);

	Curve ctrlpts;
	ctrlpts.insertpoint(0,0);
	ctrlpts.insertpoint(100,100);
	ctrlpts.insertpoint(128,128);
	ctrlpts.insertpoint(192-atoi(getParams()),192+atoi(getParams()));
	ctrlpts.insertpoint(255,255);
	
	bool result = true;
	FIBITMAP *prev = dib;
	dib = FreeImage_Clone(getPreviousPicProcessor()->getProcessedPic());
	if (dib) {
		int bpp = FreeImage_GetBPP(dib);
		m_tree->SetItemBold(GetId(), true);
		if (!FreeImage_AdjustCurveControlPoints(dib, ctrlpts.getControlPoints(), FICC_RGB)) result = false;
		if (prev) FreeImage_Unload(prev);

		//put in every processPic()...
		if (m_tree->GetItemState(GetId()) == 1) m_display->SetPic(dib);
		m_tree->SetItemBold(GetId(), false);
		wxTreeItemId next = m_tree->GetNextSibling(GetId());
		if (next.IsOk()) {
			PicProcessor * nextitem = (PicProcessor *) m_tree->GetItemData(next);
			nextitem->processPic();
		}
	}
	else result = false;
	m_tree->SetItemBold(GetId(), false);
	((wxFrame*) m_parameters->GetParent())->SetStatusText("");
	return result;
}



