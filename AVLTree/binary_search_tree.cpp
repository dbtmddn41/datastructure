#include "AVL.hpp"

TreeNode	*binary_search_tree::insert(element data)
{
	TreeNode	*root_node;
	TreeNode	*parent_node;
	TreeNode	*new_node;
	
	root_node = binary_tree::get_root();
	if (!root_node)
	{
		new_node = create_node(data);
		return (binary_tree::set_root(new_node));
	}
	parent_node = search_recursive(root_node, data);
	while (parent_node->leftchild && parent_node->rightchild)	//중복될 경우
		parent_node = search_recursive(parent_node->rightchild, data);
	if (data >= parent_node->data)	//같은 경우 오른쪽
		new_node = binary_tree::insert_right_child(parent_node, data);
	else
		new_node = binary_tree::insert_left_child(parent_node, data);
	return (new_node);
}

TreeNode	*binary_search_tree::deletion(TreeNode *start, element data)
{
	TreeNode	*to_delete;
	TreeNode	*parent;
	bool		is_left;

	to_delete = binary_search_tree::search_recursive(start, data);
	if (to_delete->data != data)
		return (0);
	parent = to_delete->parent;
	if (data < parent->data)
		is_left = true;
	else
		is_left = false;
	if (binary_tree::is_leaf(to_delete))
	{
		if (!parent)
			binary_tree::set_root(NULL);
		else if (is_left)
			parent->leftchild = NULL;
		else
			parent->rightchild = NULL;
	}
	else if (to_delete->rightchild && to_delete->leftchild)
	{
		//TreeNode	*left_big = find_biggest(to_delete->leftchild);
		TreeNode	*right_small = find_least(to_delete->rightchild);

		//is_left = false;
		//to_delete->data = left_big->data;
		//to_delete = left_big;
		to_delete->data = right_small->data;
		return deletion(right_small, right_small->data);
	}
	else 
	{
		if (to_delete->leftchild)
			binary_tree::set_child(parent, to_delete->leftchild, is_left);
		else if (to_delete->rightchild)
			binary_tree::set_child(parent, to_delete->rightchild, is_left);
	}
	delete to_delete;
	return (parent);
}

TreeNode	*binary_search_tree::find_biggest(TreeNode *start)
{
	if (start->rightchild == NULL)
		return (start);
	return (find_biggest(start->rightchild));
}

TreeNode	*binary_search_tree::find_least(TreeNode *start)
{
	if (start->leftchild == NULL)
		return (start);
	return (find_least(start->leftchild));
}

TreeNode	*binary_search_tree::search_recursive(TreeNode *parent, int data)
{
	if (data > parent->data)
	{
		if (parent->rightchild == NULL)
			return (parent);
		else
			return search_recursive(parent->rightchild, data);
	}
	else if (data < parent->data)
	{
		if (parent->leftchild == NULL)
			return (parent);
		else
			return search_recursive(parent->leftchild, data);
	}
	else
		return (parent);	
}