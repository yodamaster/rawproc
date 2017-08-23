#ifndef __PICPROCESSORBLACKWHITEPOINT_H__
#define __PICPROCESSORBLACKWHITEPOINT_H__

#include <vector>

#include "PicProcessor.h"

class PicProcessorBlackWhitePoint: public PicProcessor
{
	public:
		PicProcessorBlackWhitePoint(wxString name, wxString command, wxTreeCtrl *tree, PicPanel *display, wxPanel *parameters);
		void createPanel(wxSimplebook* parent);
		void showParams();
		bool processPic();

};

#endif
