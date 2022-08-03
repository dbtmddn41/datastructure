#ifndef B_TREE_HPP
# define B_TREE_HPP

#include "list.hpp"
#ifndef M
# define M 6
#endif

#include <iostream>

#if M == 3
# define UNDERFLOW(x) (x) < 1
# define NMINLEN(x) (x) > 1
#else
# define UNDERFLOW(x) (x) < M / 2 - 1
# define NMINLEN(x) (x) > M / 2 - 1
#endif

typedef int element;

typedef struct BTreeNode{
	ArrayList			key;
	struct BTreeNode	*child[M + 1];
	struct BTreeNode	*parent;
	bool				is_leaf;
}BTreeNode;

class	BTree
{
private:
	BTreeNode	*root;
public:
	BTree()	:root(NULL)	{};
	BTree(element data);
	~BTree(void);
	BTreeNode	*get_root();
	int			search(BTreeNode **node, BTreeNode *start, element data)	const;
	void		insert(element data);
	void		del(element data);
private:
	BTreeNode	*create_node(element data);
	void		check_full(BTreeNode *node);
	BTreeNode	*create_node(void);
	int			search_index(ArrayList lst, element data);
	void		add_child(BTreeNode *node, int idx, BTreeNode *child);
	void		delete_leaf(int idx, BTreeNode *to_delete, element data);
	void		delete_non_leaf(int idx, BTreeNode *to_delete, element data);
	void		del_node(BTreeNode *node);
	void		del_child(BTreeNode *parent, int idx);
	void		merge(BTreeNode *node, int my_idx, bool to_left);
};

# endif