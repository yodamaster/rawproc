#Makefile using Codeblocks MinGW and wxWidgets 3.0.2

OBJDIR=build


OBJECTS := $(addprefix $(OBJDIR)/,util.o elapsedtime.o myConfig.o myEXIFDialog.o myDoubleSlider.o myPropertyDialog.o myHistogramPane.o myHistogramDialog.o CurvePane.o PicProcessor.o PicProcessorBlackWhitePoint.o PicProcessorHighlight.o PicProcessorShadow.o PicProcessorCurve.o PicProcessorGamma.o PicProcessorBright.o PicProcessorContrast.o PicProcessorSaturation.o PicProcessorExposure.o PicProcessorRedEye.o PicProcessorSharpen.o PicProcessorResize.o PicProcessorDenoise.o PicProcessorRotate.o PicProcessorGray.o PicProcessorCrop.o PicProcessorColorSpace.o rawprocFrm.o rawprocApp.o PicProcPanel.o PicPanel.o)

#localmake.txt: += any of these to add, = to replace:

#Change to the name of the cross compiler/tool, if applicable, e.g., x86_64-w64-mingw32-g++ to cross compile to 64-bit Windows on Linux
CXX=g++
WINDRES=windres

#Set to the libraries and flags needed for wxWidgets
WXLIBS=
WXFLAGS=
#if you have wxWidgets installed in a standard place, you can use wx-config as follows:
#WXLIBS=$(shell wx-config --libs std,aui,propgrid)
#WXFLAGS=$(shell wx-config --cxxflags)

LIBS=-lgimage -lraw -ltiff -ljpeg -llcms2 

LIBDIRS=	#-L to libraw, libjpeg, libtiff, liblcms2 if any is not installed in a gcc-searched lib path
INCLUDEDIRS=	#-I to libraw, libjpeg, libtiff, liblcms2 if any is not installed in a gcc-searched include path
CFLAGS=		#Use += to add any unique compiler flags
LFLAGS=		#Use += to add any unique linker flags
EXT=		#If you executable needs a file extension, specify it here (for cross-compiling, because I can't figure out how to make ld do it)

#This includes local unique configuration
-include $(OBJDIR)/localmake.txt

all: rawproc img makedoc

rawproc:  $(OBJECTS)
	$(CXX) $(LFLAGS) $(LIBDIRS) $(OBJECTS)  $(LIBS) $(WXLIBS) -o $(OBJDIR)/rawproc$(EXT)

img: $(OBJDIR)/img.o
	$(CXX) $(OBJDIR)/img.o $(OBJDIR)/elapsedtime.o $(LIBDIRS) $(LIBS) $(LFLAGS) -o $(OBJDIR)/img$(EXT)

$(OBJDIR)/img.o: img.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) -std=c++11 -c img.cpp -o$@

$(OBJDIR)/elapsedtime.o: elapsedtime.cpp
	$(CXX) -c elapsedtime.cpp  -o$@

$(OBJDIR)/util.o: util.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c util.cpp  -o$@

$(OBJDIR)/myPropertyDialog.o: myPropertyDialog.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c myPropertyDialog.cpp  -o$@

$(OBJDIR)/myEXIFDialog.o: myEXIFDialog.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c myEXIFDialog.cpp  -o$@

$(OBJDIR)/myDoubleSlider.o: myDoubleSlider.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c myDoubleSlider.cpp  -o$@

$(OBJDIR)/myHistogramPane.o: myHistogramPane.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c myHistogramPane.cpp  -o$@

$(OBJDIR)/myHistogramDialog.o: myHistogramDialog.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c myHistogramDialog.cpp  -o$@

$(OBJDIR)/myConfig.o: myConfig.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) -c myConfig.cpp  -o$@

$(OBJDIR)/CurvePane.o: CurvePane.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -w -c CurvePane.cpp  -o$@

$(OBJDIR)/PicProcPanel.o: PicProcPanel.cpp
	$(CXX)  $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcPanel.cpp  -o$@

$(OBJDIR)/PicPanel.o: PicPanel.cpp
	$(CXX)  $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicPanel.cpp  -o$@

$(OBJDIR)/PicProcessorBlackWhitePoint.o: PicProcessorBlackWhitePoint.cpp
	$(CXX)  $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorBlackWhitePoint.cpp    -o$@

$(OBJDIR)/PicProcessorHighlight.o: PicProcessorHighlight.cpp
	$(CXX)  $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorHighlight.cpp    -o$@

$(OBJDIR)/PicProcessorShadow.o: PicProcessorShadow.cpp
	$(CXX)  $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorShadow.cpp    -o$@

$(OBJDIR)/PicProcessorCurve.o: PicProcessorCurve.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS)  -c PicProcessorCurve.cpp    -o$@

$(OBJDIR)/PicProcessorGamma.o: PicProcessorGamma.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorGamma.cpp    -o$@

$(OBJDIR)/PicProcessorBright.o: PicProcessorBright.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorBright.cpp   -o$@

$(OBJDIR)/PicProcessorContrast.o: PicProcessorContrast.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorContrast.cpp   -o$@

$(OBJDIR)/PicProcessorSaturation.o: PicProcessorSaturation.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorSaturation.cpp   -o$@
	
$(OBJDIR)/PicProcessorExposure.o: PicProcessorExposure.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorExposure.cpp   -o$@
	
$(OBJDIR)/PicProcessorRedEye.o: PicProcessorRedEye.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorRedEye.cpp   -o$@
	
$(OBJDIR)/PicProcessorGray.o: PicProcessorGray.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorGray.cpp   -o$@

$(OBJDIR)/PicProcessorCrop.o: PicProcessorCrop.cpp 
	$(CXX)$(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorCrop.cpp   -o$@

$(OBJDIR)/PicProcessorResize.o: PicProcessorResize.cpp 
	$(CXX)  $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorResize.cpp   -o$@

$(OBJDIR)/PicProcessorSharpen.o: PicProcessorSharpen.cpp 
	$(CXX)  $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorSharpen.cpp   -o$@

$(OBJDIR)/PicProcessorRotate.o: PicProcessorRotate.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorRotate.cpp   -o$@

$(OBJDIR)/PicProcessorDenoise.o: PicProcessorDenoise.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorDenoise.cpp   -o$@

$(OBJDIR)/PicProcessorColorSpace.o: PicProcessorColorSpace.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessorColorSpace.cpp   -o$@

$(OBJDIR)/PicProcessor.o: PicProcessor.cpp
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c PicProcessor.cpp -o$@

$(OBJDIR)/rawprocFrm.o: rawprocFrm.cpp
	$(CXX) $(BUILDDATE) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c rawprocFrm.cpp   -o$@

$(OBJDIR)/rawprocApp.o: rawprocApp.cpp 
	$(CXX) $(CFLAGS) $(INCLUDEDIRS) $(WXFLAGS) -c rawprocApp.cpp   -o$@

$(OBJDIR)/resource.o: resource.rc
ifeq ($(OBJDIR),win)
	windres -c resource.rc -o$@
endif

makedoc:
	zip  -r -j $(OBJDIR)/rawprocdoc.zip doc/*

clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/rawproc$(EXT) $(OBJDIR)/img$(EXT) 

cleandoc:
	rm -f $(OBJDIR)/rawprocdoc.zip



