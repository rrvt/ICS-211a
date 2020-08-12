

ICS_211a.chm : *.htm ICS_211a.hhp ICS_211a.hhc App.css
  -1 "C:\Program Files (x86)\HTML Help Workshop\hhc" ICS_211a


Clean.cln    :
  @echo "Cleaning"
  cd $(MAKEDIR)
  del /F ICS_211a.chm


Release/ICS_211a.chm : ICS_211a.chm
  copy $(MAKEDIR)\ICS_211a.chm  $(MAKEDIR)\..\Release\ICS_211a.chm


Release/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Release
  del ICS_211a.chm
  cd $(MAKEDIR)


Debug/ICS_211a.chm : ICS_211a.chm
  copy $(MAKEDIR)\ICS_211a.chm  $(MAKEDIR)\..\Debug\ICS_211a.chm


Debug/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Debug
  del ICS_211a.chm
  cd $(MAKEDIR)


Release/Rebuild.rbd : Release/Clean.cln Release/ICS_211a.chm


Debug/Rebuild.rbd   : Debug/Clean.cln Debug/ICS_211a.chm


