/*Derek Phanekham
	simple kernel
*/

int main(){
	putInMemory(0xB000, 0x8000, 'H');
	putInMemory(0xB000, 0x8002, 'e');
	putInMemory(0xB000, 0x8004, 'l');
	putInMemory(0xB000, 0x8006, 'l');
	putInMemory(0xB000, 0x8008, 'o');
	
	putInMemory(0xB000, 0x800C, 'W');
	putInMemory(0xB000, 0x800E, 'o');
	putInMemory(0xB000, 0x8010, 'r');
	putInMemory(0xB000, 0x8012, 'l');
	putInMemory(0xB000, 0x8014, 'd');


	while(1){

	}
	return 0;
}