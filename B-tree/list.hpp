#ifndef LIST_HPP
# define LIST_HPP

#include <iostream>
#ifndef M
#define M 6
#endif

typedef int element;

class ArrayList
{
private:
	element list[M];
	int		length;
public:
	ArrayList();
	bool		is_empty();
	bool		is_full();
	int		len();
	void	add(int poswition, element item);
	element	del(int position);
	element	&operator[](int idx);
};

#endif