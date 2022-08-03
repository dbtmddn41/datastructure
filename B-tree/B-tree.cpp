#include "B-tree.hpp"

BTree::BTree(element data)
{
	root = create_node(data);
}

BTree::~BTree()
{
	del_node(root);
}

BTreeNode	*BTree::get_root(void)
{
	return (root);
}

int	BTree::search(BTreeNode **node, BTreeNode *start, element data)	const
{
	int	i;
	BTreeNode *tmp = NULL;

	*node = start;
	while (1)
	{
		for (i = 0; i < (*node)->key.len(); i++)
		{
			if (data < (*node)->key[i])
			{
				tmp = (*node)->child[i];
				break ;
			}
			else if (data == (*node)->key[i])
				return (i);
			else if (i == (*node)->key.len() - 1)
			{
				tmp = (*node)->child[i + 1];
				break ;
			}
		}
		if (tmp == NULL)
			return (-1);
		*node = tmp;
	}
}

void	BTree::insert(element data)
{
	BTreeNode	*to_insert;
	int			i;

	if (!root)
	{
		root = create_node(data);
		return ;
	}
	to_insert = root;
	while (true)
	{
		search(&to_insert, to_insert, data);
		for (i = 0; i < to_insert->key.len(); i++)
		{
			if (data <= to_insert->key[i])
				break;
		}
		if (!to_insert->child[i])
			break ;
		else
			to_insert = to_insert->child[i];
	}
	to_insert->key.add(i, data);
	check_full(to_insert);
}

BTreeNode	*BTree::create_node(element data)
{
	BTreeNode	*new_node;
	
	new_node = new BTreeNode;
	new_node->key.add(0, data);
	for (int i = 0; i < M; i++)
		new_node->child[i] = NULL;
	new_node->parent = NULL;
	new_node->is_leaf = true;
	return new_node;
}

BTreeNode	*BTree::create_node(void)
{
	BTreeNode	*new_node;
	
	new_node = new BTreeNode;
	for (int i = 0; i < M; i++)
		new_node->child[i] = NULL;
	new_node->parent = NULL;
	new_node->is_leaf = true;
	return new_node;
}

void	BTree::check_full(BTreeNode *node)
{
	if (!(node->key.is_full()))
		return ;

	BTreeNode	*left = create_node();
	BTreeNode	*right = create_node();
	BTreeNode	*parent = node->parent;
	element		pivot;
	int			i;
	int			j;

	pivot = node->key[(M - 1) / 2];
	for (i = 0; i < (M - 1) / 2; i++)
	{
		left->key.add(i, node->key[i]);
		if (node->child[i])
		{
			left->child[i] = node->child[i]; 
			left->child[i]->parent = left;
			/*is_leaf에 대한 내용 추가*/
		}
	}
	if (node->child[i])
	{
		left->child[i] = node->child[i];
		left->child[i]->parent = left;	
	}
	for (i = (M - 1) / 2 + 1, j = 0; i < M; i++, j++)
	{
		right->key.add(j, node->key[i]);
		if (node->child[i])
		{
			right->child[j] = node->child[i];
			right->child[j]->parent = right;
		}
	}
	if (node->child[j])
	{
		right->child[j] = node->child[i];
		right->child[j]->parent = right;
	}
	if (parent == NULL)
	{
		root = create_node(pivot);
		root->child[0] = left;
		root->child[1] = right;
		left->parent = root;
		right->parent = root;
		root->is_leaf = false;
		return ;
	}
	i = search_index(parent->key, pivot);
	parent->key.add(i, pivot);
	add_child(parent, i, left);
	parent->child[i + 1] = right;
	right->parent = parent;
	delete node;
	return (check_full(parent));
}

void	BTree::add_child(BTreeNode *node, int idx, BTreeNode *child)
{
	int	i;

	for (i = node->key.len() - 1; i >= idx; i--)
		node->child[i + 1] = node->child[i];
	node->child[idx] = child;
	child->parent = node;
}

int	BTree::search_index(ArrayList lst, element data)
{
	int	i;

	for (i = 0; i < lst.len(); i++)
	{
		if (data <= lst[i])
			break ;
	}
	return (i);
}

void BTree::del(element data)
{
	int	idx;
	BTreeNode	*to_delete;

	idx = search(&to_delete, root, data);
	if (idx < 0)
	{
		perror("no data\n");
		return ;
	}
	if (!(to_delete->child[0]))
		delete_leaf(idx, to_delete, data);
	else
		delete_non_leaf(idx, to_delete, data);
}

void	BTree::delete_non_leaf(int idx, BTreeNode *to_del, element data)
{
	BTreeNode	*Lmax_node;

	Lmax_node = to_del->child[idx];
	while (Lmax_node->child[0])	//!is_leaf
		Lmax_node = Lmax_node->child[Lmax_node->key.len()];
	if (NMINLEN(Lmax_node->key.len()))
	{
		element	Lmax;
		
		Lmax = Lmax_node->key[Lmax_node->key.len() - 1];
		to_del->key.del(idx);
		Lmax_node->key.del(Lmax_node->key.len() - 1);
		to_del->key.add(idx, Lmax);
		Lmax_node->key.add(Lmax_node->key.len() - 1, data);
		delete_leaf(Lmax_node->key.len() - 1, Lmax_node, data);
		return ;
	}
	BTreeNode	*Rmin_node;

	Rmin_node = to_del->child[idx + 1];
	while (Rmin_node->child[0])	//!is_leaf
		Rmin_node = Rmin_node->child[Rmin_node->key.len()];
	element	Rmin;
	
	Rmin = Rmin_node->key[0];
	to_del->key.del(idx);
	Rmin_node->key.del(0);
	to_del->key.add(idx, Rmin);
	Rmin_node->key.add(0, data);
	delete_leaf(0, Rmin_node, data);
}

void	BTree::delete_leaf(int idx, BTreeNode *to_del, element data)
{
	BTreeNode	*parent = to_del->parent;

	if (NMINLEN(to_del->key.len()))	//해당 노드의 키 개수가 최소보다 큼
	{
		to_del->key.del(idx);
		return ;
	}
	else if (!parent)
	{
		delete to_del;
		root = NULL;
		return ;
	}

	int			my_idx;

	my_idx = search_index(parent->key, data);
	if (my_idx > 0 && NMINLEN(parent->child[my_idx - 1]->key.len()))	//왼쪽 노드에서 빌려올 수 있음
	{
		element Lmax;
		element	par;
		ArrayList	&left_key = parent->child[my_idx - 1]->key;

		Lmax = left_key[left_key.len() - 1];
		par = parent->key[my_idx - 1];
		left_key.del(left_key.len() - 1);
		parent->key.del(my_idx - 1);
		to_del->key.del(idx);
		parent->key.add(my_idx, Lmax);
		to_del->key.add(0, par);
	}
	else if (my_idx < parent->key.len() && NMINLEN(parent->child[my_idx + 1]->key.len()))	//오른쪽 노드에서 빌려올 수 있음
	{
		element Rmin;
		element	par;
		ArrayList	&right_key = parent->child[my_idx + 1]->key;

		Rmin = right_key[0];
		par = parent->key[my_idx];
		right_key.del(0);
		parent->key.del(my_idx);
		to_del->key.del(idx);
		parent->key.add(my_idx, Rmin);
		to_del->key.add(to_del->key.len(), par);
	}
	else
	{
		to_del->key.del(idx);
		do
		{
			if (root == to_del)
			{
				if (to_del->key.is_empty())
					root = to_del->child[0];
				break ;
			}
			if (my_idx < parent->key.len())	//오른쪽으로 넘김
			{
				merge(to_del, my_idx, false);
				check_full(parent->child[my_idx + 1]);
			}
			else	//왼쪽으로 넘김
			{
				merge(to_del, my_idx, true);
				check_full(parent->child[my_idx - 1]);
			}
			to_del = parent;
			parent = parent->parent;
			my_idx = search_index(parent->key, to_del->key[0]);
		} while (UNDERFLOW(to_del->key.len()));
	}
}

void	BTree::merge(BTreeNode *node, int my_idx, bool to_left)
{
	element		par;
	BTreeNode	*parent = node->parent;
	int			siblig_keylen;
	int			i;

	if (to_left)
	{
		ArrayList	&left_key = parent->child[my_idx - 1]->key;

		par = parent->key[my_idx - 1];
		parent->key.del(my_idx - 1);
		siblig_keylen = left_key.len();
		for (int i = node->key.len() - 1; i >= 0; i--)
			left_key.add(siblig_keylen, node->key[i]);
		left_key.add(siblig_keylen, par);
		if (node->child[0])
			for (int i = node->key.len(); i >= 0; i--)
				add_child(parent->child[my_idx - 1], siblig_keylen + 1, node->child[i]);
	}
	else
	{
		ArrayList	&right_key = parent->child[my_idx + 1]->key;

		par = parent->key[my_idx];
		parent->key.del(my_idx);
		right_key.add(0, par);
		for (int i = node->key.len() - 1; i >= 0; i--)
			right_key.add(0, node->key[i]);
		if (node->child[0])
			for (int i = node->key.len(); i >= 0; i--)
				add_child(parent->child[my_idx + 1], 0, node->child[i]);
	}
	del_child(parent, my_idx);
}

void	BTree::del_child(BTreeNode *parent, int idx)
{
	int	i;

	delete parent->child[idx];
	for (i = idx; i <= parent->key.len(); i++)
		parent->child[i] = parent->child[i + 1];
	while (i <= M)
	{
		parent->child[i] = NULL;
		i++;
	}
}

void	BTree::del_node(BTreeNode *node)
{
	if (!node->child[0])
	{
		delete node;
		return ;
	}

	for (int i = 0; i < node->key.len() + 1; i++)
		del_node(node->child[i]);
	delete node;
}