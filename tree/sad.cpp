#include <cstdlib>
#include <cstdio>

#define MAX_TREE_SIZE  100001
typedef int element;

class binary_tree
{
private:
	element	tree[MAX_TREE_SIZE];
public:
	element	*get_tree();
};

class	max_heap	:public	binary_tree
{
private:
	unsigned int	last;
public:
	max_heap()	:last(0)	{};
	bool	is_empty();
	bool	is_full();
	void	insert(element data);
	element	pop();
};

element	*binary_tree::get_tree()
{
	return (tree);
}

bool	max_heap::is_empty()
{
	if (last == 0)
		return true;
	else
		return false;
}

bool	max_heap::is_full()
{
	if (last == MAX_TREE_SIZE - 1)
		return true;
	else
		return false;
}

void	swap(element *a, element *b)
{
	element	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	max_heap::insert(element data)
{
	element			*tree;

	if (max_heap::is_full())
		return;
	tree = binary_tree::get_tree();
	last++;
	tree[last] = data;
	for (unsigned int i = last; i > 1; i /= 2)
	{
		if (abs(tree[i]) < abs(tree[i / 2]))
			swap(tree + i, tree + i / 2);
		else
			break ;
	}
}

element	max_heap::pop()
{
	element	*tree;
	element	res;
	unsigned int	larger;

	if (max_heap::is_empty())
		return (0);
	tree = binary_tree::get_tree();
	res = tree[1];
	tree[1] = tree[last];
	last--;
	for (unsigned int i = 1; i * 2 <= last; )
	{
		if (i * 2 == last)
			larger = i * 2;
		else
		{
			if (abs(tree[i * 2]) < abs(tree[i * 2 + 1]))
				larger = i * 2;
			else
				larger = i * 2 + 1;
		}
		if (abs(tree[larger]) < abs(tree[i]))
			swap(tree + larger, tree + i);
		else
			break ;
		i = larger;
	}
	return (res);
}

int	main(void)
{
	int			size;
	element		input;
	max_heap	maxheap;

	scanf("%d", &size);
	for (int i = 0; i < size; i++)
	{
		scanf("%d", &input);
		if (input == 0)
			printf("%d\n", maxheap.pop());
		else
			maxheap.insert(input);
	}
}