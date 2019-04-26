/*************************************************************
IPS Header Changer v0.1 - 2019

Author: millanzarreta    Date: 26/04/2019

License: GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LEN 1024

typedef struct {
	char filein[MAX_FILENAME_LEN];		// input filename
	char fileout[MAX_FILENAME_LEN];		// output filename
	int mode;							// mode: 0 = add header, 1 = remove header
	unsigned int hsize;					// header size in bytes
}param_options_type;

void showmenu() {
	printf("\nIPS Header Changer v0.1  - 2019 - millanzarreta\n");
	printf("\nUse: IPS_Header_Changer.exe [options] <file_in> <file_out>\n\n");
	printf("\nOptions:\n");
	printf("\n\t-m <0 or 1>,   --mode <0 or 1> : 0 = add header, 1 = remove header. Default: 0 (add)\n");
	printf("\n\t-h <size>,     --hsize <size>  : header size in bytes. Default: 512\n");
	printf("\n\t-?,            --help          : show this help message\n\n");
}

int main(int argc, char *argv[]) {
	FILE *idf;
	unsigned char *tmp_buff;
	unsigned char *mask_bdel_tmp_buff;
	unsigned int idf_size;
	param_options_type param_opt;
	param_opt.mode = 0;			//Default --mode parameter: 0
	param_opt.hsize = 512;		//Default --hsize parameter: 512
	
	// Read parameters
	if (argc < 3) {
		showmenu();
		return 1;
	}
	if ((strcmp(argv[1],"-?")==0) || (strcmp(argv[1],"--help")==0) || (strcmp(argv[2],"-?")==0) || (strcmp(argv[2],"--help")==0)) {
		showmenu();
		return 0;
	}
	if (strlen(argv[argc-2]) < MAX_FILENAME_LEN) {
		strcpy(param_opt.filein, argv[argc-2]);
	} else {
		printf("[!] Error: filename too long\n");
		return -3;
	}
	if (strlen(argv[argc-1]) < MAX_FILENAME_LEN) {
		strcpy(param_opt.fileout, argv[argc-1]);
	} else {
		printf("[!] Error: filename too long\n");
		return -3;
	}
	for(int i=1; i<(argc-2); i++) {
		if ((strcmp(argv[i],"-?")==0) || (strcmp(argv[i],"--help")==0)) {
			showmenu();
			return 0;
		} else if ((strcmp(argv[i],"-m")==0) || (strcmp(argv[i],"--mode")==0)) {
			if (++i < (argc-2)) {
				if ((strcmp(argv[i],"add")==0) || (strcmp(argv[i],"0")==0)) {
					param_opt.mode = 0;
				} else if ((strcmp(argv[i],"del")==0) || (strcmp(argv[i],"1")==0)) {
					param_opt.mode = 1;
				} else {
					printf("[!] Error: wrong -m or --mode parameter\n");
					return -2;
				}
			} else {
				printf("[!] Error: wrong number of parameters\n");
				return -2;
			}
		} else if ((strcmp(argv[i],"-h")==0) || (strcmp(argv[i],"--hsize")==0)) {
			if (++i < (argc-2)) {
				long int tmp = strtol(argv[i], NULL, 10);
				if (tmp > 0) {
					param_opt.hsize = (unsigned int) tmp;
				} else {
					printf("[!] Error: wrong -h or --hsize parameter\n");
					return -2;
				}
			} else {
				printf("[!] Error: wrong number of parameters\n");
				return -2;
			}
		} else {
			printf("[!] Error: Unrecognized parameter '%s'\n",argv[i]);
			return -2;
		}
	}
	
	printf("[+] Start conversion from %sheader IPS patch file to a %sheader IPS patch file\n", ((param_opt.mode == 1) ? "" : "un"), ((param_opt.mode == 1) ? "un" : ""));
	// Open input file
	if ((idf=fopen(param_opt.filein, "rb"))==NULL) {
		printf("[!] Error: Unable to open the input file \"%s\"\n",param_opt.filein);
		return -3;
	}
	fseek(idf,0,SEEK_END);
	idf_size = (unsigned int) ftell(idf);
	fseek(idf,0,SEEK_SET);
	if ((tmp_buff = (unsigned char *) malloc((idf_size+1)*sizeof(unsigned char)))==NULL) {
		printf("[!] Error: not enough memory\n");
		fclose(idf);
		return -4;
	}
	if ((mask_bdel_tmp_buff = (unsigned char *) calloc((idf_size+1),sizeof(unsigned char)))==NULL) {
		printf("[!] Error: not enough memory\n");
		fclose(idf);
		return -4;
	}
	fread(tmp_buff, idf_size, 1, idf);
	fclose(idf);
	
	// Comprobar que los 5 primeros bytes son PATCH
	if ((tmp_buff[0] != 'P') || (tmp_buff[1] != 'A') || (tmp_buff[2] != 'T') || (tmp_buff[3] != 'C') || (tmp_buff[4] != 'H')) {
		printf("[!] Error: The input file is not a valid IPS file\n");
		return -10;
	}
	
	// Empezamos el bucle con las transformaciones
	unsigned int i = 5;
	int eofFound = 0;
	while(i < idf_size) {
		if (i+3 < idf_size+1) {
			// Address offset: [3] bytes
			unsigned int offsetDir = tmp_buff[i+2] | (tmp_buff[i+1] << 8) | (tmp_buff[i] << 16);
			unsigned int originalOffsetDir = offsetDir;			
			if (offsetDir != 4542278U) {	// 4542278U = 'EOF' (0x454F46)
				if (param_opt.mode == 1) {
					offsetDir -= param_opt.hsize;
				} else {
					offsetDir += param_opt.hsize;
				}
				tmp_buff[i] = (offsetDir >> 16) & 0xFF;
				tmp_buff[i+1] = (offsetDir >> 8) & 0xFF;
				tmp_buff[i+2] = offsetDir & 0xFF;
				i+=3;
			} else {
				// EOF
				eofFound = 1;
				i+=3;
				if (i+3 < idf_size+1) {
					// CHS - Truncate (after EOF) ([3] bytes)
					unsigned int truncateOff = tmp_buff[i+2] | (tmp_buff[i+1] << 8) | (tmp_buff[i] << 16);
					if (param_opt.mode == 1) {
						if (truncateOff < param_opt.hsize) {
							printf("[?] Info: IPS CHS (Truncate) Record with address %#03x deleted because the offset goes out of bounds due to the displacement of the header\n",truncateOff);
							for(unsigned int j = 0; j < 3; j++) {
								mask_bdel_tmp_buff[i+j] = 1;
							}
						} else {
							truncateOff -= param_opt.hsize;
							tmp_buff[i] = (truncateOff >> 16) & 0xFF;
							tmp_buff[i+1] = (truncateOff >> 8) & 0xFF;
							tmp_buff[i+2] = truncateOff & 0xFF;
						}
					} else {
						if (truncateOff + param_opt.hsize > 16777215U) {
							printf("[!] Warning: IPS CHS (Truncate) Record with address %#03x deleted because the offset goes out of bounds due to the displacement of the header\n",truncateOff);
							for(unsigned int j = 0; j < 3; j++) {
								mask_bdel_tmp_buff[i+j] = 1;
							}
						} else {
							truncateOff += param_opt.hsize;
							tmp_buff[i] = (truncateOff >> 16) & 0xFF;
							tmp_buff[i+1] = (truncateOff >> 8) & 0xFF;
							tmp_buff[i+2] = truncateOff & 0xFF;
						}
					}
					i+=3;
					if (i < idf_size) {
						printf("[!] Warning: EOF+CHS has been found before the end of file. Maybe a rare \"0x454F46"
								" (EOF) offset bug\" has happened. Please, check manually the contents of the output file\n");
						for(; i < idf_size; i++) {
							mask_bdel_tmp_buff[i] = 1;
						}
					}
				} else {
					if (i < idf_size) {
						printf("[!] Warning: EOF has been found before the end of file. Maybe a rare \"0x454F46 (EOF) offset bug\""
								" has happened. Please, check manually the contents of the output file\n");
						for(; i < idf_size; i++) {
							mask_bdel_tmp_buff[i] = 1;
						}
					}
				}
				break;
			}
			if (i+2 < idf_size+1) {
				// Length: [2] bytes
				unsigned int lengthData = tmp_buff[i+1] | (tmp_buff[i] << 8);
				unsigned int originalLengthData = lengthData;
				i+=2;
				if (lengthData == 0) {
					//RLE -> [2] Length bytes + [1] Data byte
					unsigned int lengthRLEData = tmp_buff[i+1] | (tmp_buff[i] << 8);
					i+=2;
					if (param_opt.mode == 1) {
						if (originalOffsetDir + lengthRLEData <= param_opt.hsize) {
							printf("[?] Info: IPS RLE Record with address %#03x deleted because it pointers to a header address that is intended to be removed\n",originalOffsetDir);
							i-=7;
							for(unsigned int j = 0; j < 8; j++) {
								mask_bdel_tmp_buff[i+j] = 1;
							}
							i+=8;
						} else if (originalOffsetDir < param_opt.hsize) {
							printf("[?] Info: IPS RLE Record with address %#03x modified because it partially pointers to a header address that is intended to be removed."
									" A new IPS Record has been created with the data adjusted to the section that does not affect the header\n",originalOffsetDir);
							i-=2;
							lengthRLEData = originalOffsetDir + lengthRLEData - param_opt.hsize;
							tmp_buff[i] = (lengthRLEData >> 8) & 0xFF;
							tmp_buff[i+1] = lengthRLEData & 0xFF;
							i-=5;
							offsetDir = 0;
							tmp_buff[i] = (offsetDir >> 16) & 0xFF;
							tmp_buff[i+1] = (offsetDir >> 8) & 0xFF;
							tmp_buff[i+2] = offsetDir & 0xFF;
							i+=8;
						} else {
							i++;
						}
					} else {
						if (originalOffsetDir + param_opt.hsize > 16777215U) {	//16777215U = 0xFFFFFF
							printf("[!] Warning: IPS RLE Record with address %#03x deleted because it does not fit in the new file with the displacement caused by the header\n",originalOffsetDir);
							i-=7;
							for(unsigned int j = 0; j < 8; j++) {
								mask_bdel_tmp_buff[i+j] = 1;
							}
							i+=8;
						} else if (originalOffsetDir + lengthRLEData - 1 + param_opt.hsize > 16777215U) {	//16777215U = 0xFFFFFF
							printf("[!] Warning: IPS RLE Record with address %#03x modified because it does not fit partially in the new file with the displacement caused by the header."
									" A new IPS Record has been created with the data adjusted to the section that does not affect the header displacement\n",originalOffsetDir);
							i-=2;
							lengthRLEData = 16777216U - offsetDir;		//16777216U = 0xFFFFFF + 1
							tmp_buff[i] = (lengthRLEData >> 8) & 0xFF;
							tmp_buff[i+1] = lengthRLEData & 0xFF;
							i+=3;
						} else {
							i++;
						}
					}
				} else {
					//PAT -> [lengthData] Data bytes
					if (param_opt.mode == 1) {
						if (originalOffsetDir + lengthData <= param_opt.hsize) {
							printf("[?] Info: IPS PAT Record with address %#03x deleted because it pointers to a header address that is intended to be removed\n",originalOffsetDir);
							i-=5;
							for(unsigned int j = 0; j < 5+lengthData; j++) {
								mask_bdel_tmp_buff[i+j] = 1;
							}
							i+=(5+lengthData);
						} else if (originalOffsetDir < param_opt.hsize) {
							printf("[?] Info: IPS PAT Record with address %#03x modified because it partially pointers to a header address that is intended to be removed."
									" A new IPS Record has been created with the data adjusted to the section that does not affect the header\n",originalOffsetDir);
							i-=2;
							lengthData = originalOffsetDir + lengthData - param_opt.hsize;
							tmp_buff[i] = (lengthData >> 8) & 0xFF;
							tmp_buff[i+1] = lengthData & 0xFF;
							i-=3;
							offsetDir = 0;
							tmp_buff[i] = (offsetDir >> 16) & 0xFF;
							tmp_buff[i+1] = (offsetDir >> 8) & 0xFF;
							tmp_buff[i+2] = offsetDir & 0xFF;
							i+=5;
							for(unsigned int j = 0; j < (originalLengthData-lengthData); j++) {
								mask_bdel_tmp_buff[i+j] = 1;
							}
							i+=originalLengthData;
						} else {
							i+=lengthData;
						}
					} else {
						if (originalOffsetDir + param_opt.hsize > 16777215U) {	//16777215U = 0xFFFFFF
							printf("[!] Warning: IPS PAT Record with address %#03x deleted because it does not fit in the new file with the displacement caused by the header\n",originalOffsetDir);
							i-=5;
							for(unsigned int j = 0; j < 5+lengthData; j++) {
								mask_bdel_tmp_buff[i+j] = 1;
							}
							i+=(5+lengthData);
						} else if (originalOffsetDir + lengthData - 1 + param_opt.hsize > 16777215U) {	//16777215U = 0xFFFFFF
							printf("[!] Warning: IPS PAT Record with address %#03x modified because it does not fit partially in the new file with the displacement caused by the header."
									" A new IPS Record has been created with the data adjusted to the section that does not affect the header displacement\n",originalOffsetDir);
							i-=2;
							lengthData = 16777216U - offsetDir;		//16777216U = 0xFFFFFF + 1
							tmp_buff[i] = (lengthData >> 8) & 0xFF;
							tmp_buff[i+1] = lengthData & 0xFF;
							i+=2+lengthData;
							for(unsigned int j = 0; j < (originalLengthData-lengthData); j++) {
								mask_bdel_tmp_buff[i+j] = 1;
							}
							i+=(originalLengthData-lengthData);
						} else {
							i+=lengthData;
						}
					}
				}
			} else {
				break;
			}
		} else {
			break;
		}
	}
	if (!eofFound) {
		printf("[!] Warning: No EOF has been found. Please, check manually the contents of the input and output file\n");
	}
	
	// Save output file
	if ((idf=fopen(param_opt.fileout, "wb"))==NULL) {
		printf("[!] Error: Unable to open the output file \"%s\"\n",param_opt.fileout);
		return -3;
	}
	unsigned char *ptr_tmp_buff = tmp_buff;
	unsigned int tmpsize = 0;
	for (unsigned int j = 0; j < idf_size; j++) {
		if (!mask_bdel_tmp_buff[j]) {
			tmpsize++;
		} else {
			if (tmpsize > 0) {
				fwrite(ptr_tmp_buff, 1, tmpsize, idf);
			}
			ptr_tmp_buff+=tmpsize+1;
			tmpsize = 0;
		}
	}
	if (tmpsize > 0) {
		fwrite(ptr_tmp_buff, 1, tmpsize, idf);
	}
	fclose(idf);
	printf("[+] IPS Header conversion completed!!!\n");
	return 0;
}
