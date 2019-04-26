# IPS_Header_Changer
A program that allows you to easily add/remove a header from an unheared/heared IPS patch.

IPS Header Changer allows you to easily add/remove a header from an unheared/heared IPS patch. The program is open source with a GPLv3 license and has highly portable source code.

The program receives as last two parameters the input file (the IPS patch to modify) and the output file (where to save the new IPS patch generated). To add the header to an unheared IPS file execute the program with the parameter "-m 0" (this is also the default behaviour if this parameter is not specified) and to remove the header of an heared IPS file run the program with the parameter "- m 1". You can configure the header size with the "-h" parameter (default is 512, which is the usual).

IPS Header Changer is specially prepared to detect certain special cases that can be dangerous (for example, when sliding an address of some IPS record the addresses can overflow the accepted values). The program will show Information, Warning or Error messages if something not expected happens. Information messages are displayed when the user needs to be informed of something important or when a special action has been taken that does not affect the integrity of the generated IPS patch. Warning messages show information about minor errors or some needed changes that do not stop the conversion and may affect the result of the final IPS patch. If you see any Warning message you should manually check the output file and compare it with the input file to verify that conversion was correct. Error messages are shown when a fatal error has occurred and the process could not be completed (the final file is not generated).

The program is written in C and uses the basic libraries that any OS has. An already compiled executable for Windows in included, but you must compile the code for the rest of OS.
