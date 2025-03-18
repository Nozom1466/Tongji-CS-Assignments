int a;
int b;
int main(void){
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
	return;
}



