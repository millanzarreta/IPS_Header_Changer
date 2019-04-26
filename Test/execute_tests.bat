REM Test the MD5 Checksum of ***_chk2.ips files, must be the same MD5 Checksum as ***_chk.ips files.

..\IPS_Header_Changer.exe -m 1 TestPATDelDel.ips TestPATDelDel_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestPATDelMod.ips TestPATDelMod_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestPATDelFitD.ips TestPATDelFitD_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestPATDelFitA.ips TestPATDelFitA_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestPELDelDel.ips TestPELDelDel_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestPELDelMod.ips TestPELDelMod_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestPELDelFitD.ips TestPELDelFitD_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestPELDelFitA.ips TestPELDelFitA_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestPATAddDel.ips TestPATAddDel_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestPATAddMod.ips TestPATAddMod_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestPATAddFitD.ips TestPATAddFitD_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestPATAddFitA.ips TestPATAddFitA_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestPELAddDel.ips TestPELAddDel_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestPELAddMod.ips TestPELAddMod_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestPELAddFitD.ips TestPELAddFitD_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestPELAddFitA.ips TestPELAddFitA_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestCHSDelDel.ips TestCHSDelDel_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestCHSDelFitA.ips TestCHSDelFitA_chk2.ips
..\IPS_Header_Changer.exe -m 1 TestCHSDelFitD.ips TestCHSDelFitD_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestCHSAddDel.ips TestCHSAddDel_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestCHSAddFitA.ips TestCHSAddFitA_chk2.ips
..\IPS_Header_Changer.exe -m 0 TestCHSAddFitD.ips TestCHSAddFitD_chk2.ips
