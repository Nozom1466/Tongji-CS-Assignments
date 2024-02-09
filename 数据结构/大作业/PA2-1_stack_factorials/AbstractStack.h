template<class dataType>


class MyStack
{
public:
	virtual bool isEmpty() = 0;
	virtual void push(const dataType& d) = 0;
	virtual dataType pop() = 0;
	virtual dataType top() = 0;

}; 