#include "list.hpp"

ArrayList::ArrayList()	:length(0){}

bool	ArrayList::is_empty()
{
	return (length == 0);
}

bool ArrayList::is_full()
{
	return (length == M);
}

int	ArrayList::len()
{
	return length;
}

void	ArrayList::add(int position, element item)
{
	if (is_full() || position < 0 || position > length)
		return ;
	for (int i = length - 1; i >= position; i--)
		list[i + 1] = list[i];
	list[position] = item;
	length++;
}

element	ArrayList::del(int position)
{
	int temp;

	if (is_empty() || position < 0 || position > length)
		return 0;
	temp = list[position];
	for (int i = position; i < length - 1; i++)
		list[i] = list[i + 1];
	length--;
	return (temp);
}

element	&ArrayList::operator[](int idx)
{
	if (idx < 0 || idx >= M)
	{
		std::cerr << "invalid index\n";
		exit(1);
	}
	return list[idx];
}