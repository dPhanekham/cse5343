/*Derek Phanekham
	simple kernel
*/

void printString(char* string);
void readString(char* string);
void readSector(char * buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);
int mod(int a, int b);

char line[80];
char buf[512];
int sectorNum = 30;

int main() {

	printString("Hello World\r\n\0");

	printString("Enter a line: \r\n\0");
	readString(line);
	printString(line);

	readSector(buf, sectorNum);
	printString(buf);

	makeInterrupt21();
	interrupt(0x21, 0, 0, 0, 0);

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
	interrupt(0x10, 0xE * 256 + c, 0, 0, 0);

	while (c != 0xD) {
		if (c == 0x8 && index > 0) {
			index--;
			string[index] = ' ';
			interrupt(0x10, 0xE * 256 + '\b', 0, 0, 0);
			index++;
		} else {
			string[index] = c;
			c = interrupt(0x16, 0, 0, 0, 0);
			interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
			index++;
		}
	}
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

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	printString("calling interrupt 21.");
}

int mod(int a, int b) {
	while(a >=b) {
		a = a - b;
	}
	return a;
}
