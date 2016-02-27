
#include "PicProcessor.h"
#include "PicProcessorBlackWhitePoint.h"
#include "PicProcPanel.h"
#include "FreeImage.h"
#include "FreeImage16.h"
#include "myTouchSlider.h"
#include "util.h"

class BlackWhitePointPanel: public PicProcPanel
{
	public:
		BlackWhitePointPanel(wxPanel *parent, PicProcessor *proc, wxString params): PicProcPanel(parent, proc, params)
		{
			wxArrayString p = split(params,",");

			double shd = atof(p[0]);
			double thr = atof(p[1]);

			SetSize(parent->GetSize());
			b->SetOrientation(wxHORIZONTAL);
			wxSizerFlags flags = wxSizerFlags().Center().Border(wxLEFT|wxRIGHT|wxTOP|wxBOTTOM);
			slide = new myTouchSlider((wxFrame *) this, wxID_ANY, "shadow", SLIDERWIDTH, shd, 1.0, -50.0, 50.0, "%2.0f");
			threshold = new myTouchSlider((wxFrame *) this, wxID_ANY, "threshold", SLIDERWIDTH, thr, 1.0, 128.0, 223.0, "%2.0f");
			b->Add(50,100,1);
			b->Add(slide, flags);
			b->Add(threshold, flags);
			b->Add(50,100,1);
			SetSizerAndFit(b);
			b->Layout();
			Refresh();
			Update();
			SetFocus();
			//slide->Bind(wxEVT_SCROLL_THUMBRELEASE,&ShadowPanel::paramChanged,this);
			Connect(wxID_ANY, wxEVT_SCROLL_THUMBRELEASE,wxCommandEventHandler(BlackWhitePointPanel::paramChanged));
		}

		~BlackWhitePointPanel()
		{
			slide->~myTouchSlider();
			threshold->~myTouchSlider();
		}

		void paramChanged(wxCommandEvent& event)
		{
			q->setParams(wxString::Format("%0.2f,%0.2f",slide->GetDoubleValue(),threshold->GetDoubleValue()));
			q->processPic();
			event.Skip();
		}


	private:
		myTouchSlider *slide, *threshold;

};


PicProcessorBlackWhitePoint::PicProcessorBlackWhitePoint(wxString name, wxString command, wxTreeCtrl *tree, PicPanel *display, wxPanel *parameters): PicProcessor(name, command,  tree, display, parameters) 
{
	showParams();
}

void PicProcessorBlackWhitePoint::showParams()
{
	if (!m_parameters) return;
	m_parameters->DestroyChildren();
	r = new BlackWhitePointPanel(m_parameters, this, c);
}

bool PicProcessorBlackWhitePoint::processPic() {
	((wxFrame*) m_parameters->GetParent())->SetStatusText("highlight...");
	m_tree->SetItemBold(GetId(), true);

	wxArrayString cp = split(getParams(),",");
	double shd = atof(cp[0]);
	double thr = atof(cp[1]);

	Curve ctrlpts;
	ctrlpts.insertpoint(0,0);
	ctrlpts.insertpoint(thr-20,thr-20);
	ctrlpts.insertpoint(thr,thr);
	ctrlpts.insertpoint((thr+thr/2)-shd,(thr+thr/2)+shd);
	ctrlpts.insertpoint(255,255);
	
	bool result = true;
	FIBITMAP *prev = dib;
	dib = FreeImage_Clone(getPreviousPicProcessor()->getProcessedPic());
	if (dib) {
		int bpp = FreeImage_GetBPP(dib);
		m_tree->SetItemBold(GetId(), true);
		if (!FreeImage_AdjustCurveControlPoints(dib, ctrlpts.getControlPoints(), FICC_RGB)) 
			result = false;
		else
			dirty = false;
		if (prev) FreeImage_Unload(prev);

		//put in every processPic()...
		if (m_tree->GetItemState(GetId()) == 1) m_display->SetPic(dib);
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


