char line[80];
char buf[512];
int sectorNum = 30;

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
	

	interrupt(0x21, 3, "messag\0", fileBuf, 0);
	interrupt(0x21, 0, fileBuf, 0, 0);

	interrupt(0x21, 0, "Executing program\r\n\0", 0, 0);
	interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);
	*/

	while(1){
		interrupt(0x21, 0, "$\0", 0, 0);
		interrupt(0x21, 1, line, 0, 0);
		interrupt(0x21, 0, line, 0, 0);
	}

	return 0;
}