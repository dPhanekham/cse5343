/*Derek Phanekham
	simple kernel with interrupts
*/

void printString(char* string);
void readString(char* string);
void readSector(char* buffer, int sector);
void readFile(char* fileName, char* fileBuffer);
void executeProgram(char* name, int segment);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void terminate();
int mod(int a, int b);

char line[80];
char buf[512];
int sectorNum = 30;
char fileBuf[13312];

int main() {

		/*
	//test printString
	printString("Hello World\r\n\0");
	//test readString
	printString("Enter a line: \r\n\0");
	readString(line);
	printString(line);
	//test readSector
	readSector(buf, sectorNum);
	printString(buf);
	//test interrupt21
	makeInterrupt21();
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);
	

	makeInterrupt21();
	interrupt(0x21, 3, "messag\0", fileBuf, 0);
	interrupt(0x21, 0, fileBuf, 0, 0);

	interrupt(0x21, 0, "Executing program\r\n\0", 0, 0);
	interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);
	*/
	makeInterrupt21();
	interrupt(0x21, 4, "shell\0", 0x2000, 0);

	while(1); //idle

	return 0;
}

void printString(char* string) {
	int index = 0;
	char c = string[index];
	while (c != 0) {
		interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
		c = string[++index];
	}
}

//TODO: get backspace working
void readString(char * string) {
	int index = 0;
	char c = 0;
	c = interrupt(0x16, 0, 0, 0, 0);

	while (c != 0xD) {
		if (c == 0x8 && index > 0) {
			index--;
			string[index] = ' ';
			interrupt(0x10, 0xE * 256 + '\b', 0, 0, 0);
			interrupt(0x10, 0xE * 256 + ' ', 0, 0, 0);
			interrupt(0x10, 0xE * 256 + '\b', 0, 0, 0);
			c = interrupt(0x16, 0, 0, 0, 0);
		} else {
			string[index] = c;
			interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
			c = interrupt(0x16, 0, 0, 0, 0);
			index++;
		}

	}
	interrupt(0x10, 0xE * 256 + '\r', 0, 0, 0);
	interrupt(0x10, 0xE * 256 + '\n', 0, 0, 0);
	string[index] = '\r';
	string[index + 1] = '\n';
	string[index + 2] = 0;
}

void readSector(char* buffer, int sector) {
	int AH = 2;
	int AL = 1;
	int DL = 0;
	int CL = mod(sector, 18) + 1;
	int DH = (sector / 18) % 2;
	int DX = DH * 256 + DL;
	int CH = sector / 36;
	int CX = CH * 256 + CL;
	int AX = AH * 256 + AL;

	interrupt(0x13, AX, buffer, CX, DX);

}

void readFile(char* fileName, char* fileBuffer) {
	char sectorChar;
	int sectorInt;
	int size = sizeof(fileName);
	int spaceInDirectory = 0;
	int numberOfSectors = 0;
	int i = 0;
	int j = 0;
	int equal = 1;
	char DirectoryBuffer[512];
	char directoryEntry[32];
	char message[6];

	readSector(DirectoryBuffer, 2);

	while (i < 512) {
		equal = 1;

		while (j < 6) {
			if (DirectoryBuffer[i+j] != fileName[j]) {
				equal = 0;
				break;
			}
			j++;
		}
		if(equal == 1) {
			spaceInDirectory = i;
			break;
		}
		j=0;
		i += 32;
	}

	if (i == 512 || equal == 0) {
		return;
	} 

	while(DirectoryBuffer[spaceInDirectory + 6 + numberOfSectors] != 0) {
		numberOfSectors += 1;
	}

	//i think you can convert any char to an int of its ascii value
	//with this statement int myNum = 'a';
	j=0;
	while( j < numberOfSectors) {
		sectorChar = DirectoryBuffer[i + 6 + j];
		sectorInt = (int)sectorChar;
		readSector(fileBuffer, sectorInt);
		j++;
		fileBuffer +=512;
	}

}

//void putInMemory (int segment, int address, char character)
void executeProgram(char* name, int segment) {
	char progBuffer[1000];
	int address = 0x0000;
	int i=0;
	char c;

	readFile(name, progBuffer);

	while (i < 1000) {
		c = progBuffer[i];
		putInMemory(segment, address, c);
		address += 0x0001;
		i++;
	}

	launchProgram(segment);

}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	if (ax == 0) printString(bx);
	else if (ax == 1) readString(bx);
	else if (ax == 2) readSector(bx, cx);
	else if (ax == 3) readFile(bx, cx);
	else if (ax == 4) executeProgram(bx, cx);
	else if (ax == 5) terminate();
	else if (ax >= 6) printString("Error. interrupt not recognized\r\n\0");
}

void terminate() {
	interrupt(0x21, 4, "shell\0", 0x2000, 0);
}

int mod(int a, int b) {
	while(a >=b) {
		a = a - b;
	}
	return a;
}
