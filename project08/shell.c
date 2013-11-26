char line[80];
char buf[512];
int sectorNum = 30;
char badCommand[] = "Bad Command\r\n\0";
char typeString[] = "type\0";
char executeString[] = "execute\0";
char fileBuf[13312];


int foundType(char * string);
int foundExecute(char * string);

int main() {

	while(1){
		interrupt(0x21, 0, "$\0", 0, 0);
		interrupt(0x21, 1, line, 0, 0);
		if (foundType(line) == 1) {
			interrupt(0x21, 3, line + 5, fileBuf, 0);
			interrupt(0x21, 0, fileBuf, 0, 0);
		} else if (foundExecute(line) == 1) {
			interrupt(0x21, 4, line + 8, 0x2000, 0);
		} else {
			interrupt(0x21, 0, badCommand, 0, 0);
		}
	}

	return 0;
}

int foundType(char * string) {
	 
	 int found = 1;
	 int i = 0;
	 while (i < 4) {
	 	if (string[i] != typeString[i]) {
	 		found = 0;
	 		break;
	 	}
	 	i++;
	 }
	 return found;
}

int foundExecute(char * string) {
	 
	 int found = 1;
	 int i = 0;
	 while (i < 7) {
	 	if (string[i] != executeString[i]) {
	 		found = 0;
	 		break;
	 	}
	 	i++;
	 }
	 return found;
}