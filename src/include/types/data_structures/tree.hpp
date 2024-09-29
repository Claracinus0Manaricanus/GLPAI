#ifndef CM_TREE
#define CM_TREE

struct cm_TreeData {
  //
};

template <class T> struct TreeNode {
  TreeNode* parent;

  T* value;

  int childCount;
  TreeNode** childs;
};

template <class T> class cm_Tree {
protected:
  TreeNode<T> root;

public:
  // constructors
  cm_Tree();
  cm_Tree(cm_TreeData data);

  // destructors
  ~cm_Tree();

  // setters, for node variable if nullptr is given it means root node

  TreeNode<T>* addChildTo(
      TreeNode<T>* node,
      T* toAddAsChild); // returns a pointer to the newly created node

  // getters, for node variable if nullptr is given it means root node

  TreeNode<T>& getRootNode(); // returns root node

  TreeNode<T>* getChilds(TreeNode<T>* node,
                         int& childCount); // returns all childs of a node and
                                           // puts the count into childCount

  TreeNode<T>* getChild(TreeNode<T>* node,
                        int n); // returns the n'th child of a node
};

#endif
