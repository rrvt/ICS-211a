

ICS_211a.chm : *.htm ICS_211a.hhp ICS_211a.hhc App.css
  -1 "C:\Program Files (x86)\HTML Help Workshop\hhc" ICS_211a


Clean.cln    :
  @echo "Cleaning"
  cd $(MAKEDIR)
  if exist ICS_211a.chm del /F ICS_211a.chm


$(TgtFolder)/ICS_211a.chm : ICS_211a.chm
  copy $(MAKEDIR)\ICS_211a.chm  $(MAKEDIR)\..\$(TgtFolder)\ICS_211a.chm


$(TgtFolder)/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\$(TgtFolder)
  if exist ICS_211a.chm del /F ICS_211a.chm
  cd $(MAKEDIR)


$(TgtFolder)/Rebuild.rbd : $(TgtFolder)/Clean.cln $(TgtFolder)/ICS_211a.chm


