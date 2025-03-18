int main(void)
{
	int b;
	int a;
	a = 0;
	b = 2.0+4*7;
	while ( a > 3)
	{
		a = a + 1;
		b = b + a;
	}
	if(a <= b)
	{
	a = 1+2*3-6/3+2*(3+4);
	b = a + 2;
	}
	else
	{
	b = 9;
	a = a*b + b+(0-8*a);
	}
}