#include "PicProcessorColorSpace.h"
#include "PicProcPanel.h"
#include "util.h"
//#include "gimage/curve.h"
#include <wx/fileconf.h>


class ColorspacePanel: public PicProcPanel
{

	public:
		ColorspacePanel(wxWindow *parent, PicProcessor *proc, wxString params): PicProcPanel(parent, proc, params)
		{
			wxSizerFlags flags = wxSizerFlags().Left().Border(wxLEFT|wxRIGHT).Expand();
			wxArrayString parms = split(params, ",");
			b->Add(new wxStaticText(this,-1, "colorspace", wxDefaultPosition, wxSize(100,20)), flags);
			edit = new wxTextCtrl(this, wxID_ANY, parms[0], wxDefaultPosition, wxSize(200,25),wxTE_PROCESS_ENTER);
			b->Add(edit, flags);

			wxArrayString opers;
			opers.Add("apply");
			opers.Add("assign");

			operselect = new wxRadioBox (this, wxID_ANY, "Operation", wxDefaultPosition, wxDefaultSize,  opers, 1, wxRA_SPECIFY_COLS);
			if (parms[1] != "-") operselect->SetSelection(operselect->FindString(parms[1]));
			b->Add(operselect,flags);

			SetSizerAndFit(b);
			b->Layout();
			Refresh();
			Update();
			SetFocus();
			Bind(wxEVT_TEXT_ENTER,&ColorspacePanel::paramChanged, this);
		}

		~ColorspacePanel()
		{
			
		}

		void paramChanged(wxCommandEvent& event)
		{
			q->setParams(wxString::Format("%s,%s",edit->GetLineText(0), operselect->GetString(operselect->GetSelection())));
			q->processPic();
			event.Skip();
		}


	private:
		wxTextCtrl *edit;
		wxRadioBox *operselect;

};

PicProcessorColorSpace::PicProcessorColorSpace(wxString name, wxString command, wxTreeCtrl *tree, PicPanel *display, wxPanel *parameters): PicProcessor(name, command, tree, display, parameters) 
{
	//showParams();
}

void PicProcessorColorSpace::createPanel(wxSimplebook* parent)
{
	toolpanel = new ColorspacePanel(parent, this, c);
	parent->ShowNewPage(toolpanel);
}

void PicProcessorColorSpace::showParams()
{
	if (!m_parameters) return;
	m_parameters->DestroyChildren();
	r = new ColorspacePanel(m_parameters, this, c);
	r->Refresh();
	r->Update();
}


bool PicProcessorColorSpace::processPic() 
{
	((wxFrame*) m_display->GetParent())->SetStatusText("colorspace...");
	bool result = true;
	
	wxArrayString cp = split(getParams(),",");

	wxFileName fname;
	fname.AssignDir(wxConfigBase::Get()->Read("cms.profilepath",""));
	if (cp.GetCount() > 0) fname.SetFullName(cp[0]);

	mark();
	if (dib) delete dib;
	dib = new gImage(getPreviousPicProcessor()->getProcessedPic());
	
	if (fname.IsOk() & fname.FileExists()) {

		if (cp[1] == "apply") {
			dib->ApplyColorspace(std::string(fname.GetFullPath().c_str()),INTENT_ABSOLUTE_COLORIMETRIC);
		}
		else if (cp[1] == "assign") {
			dib->AssignColorspace(std::string(fname.GetFullPath().c_str()));
		}
	
		wxString d = duration();

		if ((wxConfigBase::Get()->Read("tool.all.log","0") == "1") || (wxConfigBase::Get()->Read("tool.colorspace.log","0") == "1"))
			log(wxString::Format("tool=colorspace,imagesize=%dx%d,time=%s",dib->getWidth(), dib->getHeight(),d));

	}
	else wxMessageBox(wxString::Format("profile %s not found.",fname.GetFullName().c_str()));
	
	dirty = false;

	((wxFrame*) m_display->GetParent())->SetStatusText("");
	processNext();
	
	return result;
}





