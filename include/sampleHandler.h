// Author:  Ben Tannenwald
// Date:    Nov 16, 2018
// Purpose: Header for process cross-sections

#include <TString.h>
#include <iostream>


class sampleHandler{

   public: 
      // Constructor(s) and destructor
      sampleHandler();
      virtual ~sampleHandler();


      // === Functions === //
      void test();
      void SetSample(string sample);


      // === Variables === //
      double xSec;
      Color_t c_plotColor;
      string sampleName;
      double genEff;
      double nEvents;
      double sampleLumi;
      double lumiWeight;

   private:
      // === Functions === //
      calculateLumiScale();
      
      // === Variables === //

}; // End of class prototype


//#endif // _sampleHandler_h
