#ifndef __PICPROCESSORDENOISE_H__
#define __PICPROCESSORDENOISE_H__

#include "PicProcessor.h"


class PicProcessorDenoise: public PicProcessor
{
	public:
		PicProcessorDenoise(wxString name, wxString command, wxTreeCtrl *tree, PicPanel *display);
		void createPanel(wxSimplebook* parent);
		bool processPic(bool processnext=true);
};

#endif
