int a;
int b;
int program(int a, int b, int c){
	int i;
	int j;
	i = 0;
	if (a > (b + c)){
		j = a + (b * c + 1);
	}
	else{
		j = a;
	}
	a = a + 10;
	while (i <= 100){
		i = j * 2;
		j = i;
	}
	return i;
}

int demo(int a){
	a = a + 2;
	return a * 2;
}

void main(void){
	int a;
	int b;
	int c;
	a = 3;
	b = 4.23;
	c = 2+3e-2;
	c = b + a;
	a = program(a, b, demo(c));
	return;
}