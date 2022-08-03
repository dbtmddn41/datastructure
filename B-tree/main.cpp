#include "B-tree.hpp"
using std::cin;
using std::cout;

int main(void)
{
	BTree	tree;
	char	oper;
	int		input;
	BTreeNode *root;

	while (true)
	{
		cin >> oper;
		if (oper == 'I')
		{
			cin >> input;
			tree.insert(input);
		}
		else if (oper == 'S')
		{
			BTreeNode* output;
			root = tree.get_root();
			cin >> input;
			int	idx = tree.search(&output, root, input);
			cout << idx << std::endl;
			for (int i = 0; i < output->key.len(); i++)
				cout << output->key[i] << ' ';
		}
		else if (oper == 'D')
		{
			cin >> input;
			tree.del(input);
		}
	}
}