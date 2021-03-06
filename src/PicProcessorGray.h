#ifndef __PICPROCESSORGRAY_H__
#define __PICPROCESSORGRAY_H__

#include <vector>

#include "PicProcessor.h"

class PicProcessorGray: public PicProcessor
{
	public:
		PicProcessorGray(wxString name, wxString command, wxTreeCtrl *tree, PicPanel *display);
		void createPanel(wxSimplebook* parent);
		bool processPic(bool processnext=true);

	private:
		double rp, bp, gp;

};

#endif
