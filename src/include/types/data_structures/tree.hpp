#ifndef CM_TREE
#define CM_TREE

#include <cstdlib>

template <class T> struct TreeNode {
  TreeNode<T>* parent;

  T* value;

  int childCount;
  TreeNode<T>** childs;
};

template <class T> class cm_Tree {
protected:
  TreeNode<T> root;
  /**
   * if freeEnabled is not 0 the tree also calls free on values
   */
  int freeEnabled = 1;

public:
  // constructors
  cm_Tree() {
    root.parent = nullptr;
    root.value = nullptr;
    root.childCount = 0;
    root.childs = nullptr;
  }

  // destructors
  void helperFreeFunc(TreeNode<T> node) {
    for (int i = 0; i < node.childCount; i++) {
      helperFreeFunc(*node.childs[i]);
      free(node.childs[i]);
    }

    if (freeEnabled)
      free(node.value);
    free(node.childs);
  }

  ~cm_Tree() { helperFreeFunc(root); }

  // setters, for node variable if nullptr is given it means root node

  void setFreeEnabled(int set) { freeEnabled = set; }

  TreeNode<T>* addChildTo(TreeNode<T>* node, T* toAddAsChild) {
    if (node == nullptr || node == NULL)
      node = &root;

    node->childCount++;
    node->childs =
        (TreeNode<T>**)realloc(node->childs, node->childCount * sizeof(T*));
    if (node->childs == NULL)
      return nullptr;

    TreeNode<T>* childNode = new TreeNode<T>();
    childNode->childCount = 0;
    childNode->value = toAddAsChild;
    childNode->childs = nullptr;
    childNode->parent = node;

    node->childs[node->childCount - 1] = childNode;

    return childNode;
  } // returns a pointer to the newly created node

  /**
   * deletes the node from its parent
   */
  void deleteChild(TreeNode<T>* node) {
    TreeNode<T>* parent = node->parent;
    if (parent == nullptr)
      return;

    int index = 0;
    for (int i = 0; i < parent->childCount; i++) {
      if (parent->childs[i] == node) {
        index = i;
        break;
      }
    }

    for (int i = index; i < parent->childCount - 1; i++) {
      parent->childs[i] = parent->childs[i + 1];
    }

    parent->childCount--;
    if (parent->childCount > 0) {
      parent->childs = (TreeNode<T>**)realloc(parent->childs,
                                              parent->childCount * sizeof(T*));
    } else {
      free(parent->childs);
      parent->childs = nullptr;
    }
  }

  /**
   * deletes a node with all its childs can't take a nullptr
   */
  void deleteNode(TreeNode<T>* node) {
    if (node != nullptr) {
      deleteChild(node);
      helperFreeFunc(*node);
    }

    free(node);
  }

  // getters, for node variable if nullptr is given it means root node

  TreeNode<T>& getRootNode() { return root; } // returns root node

  int getChildCount(TreeNode<T>* node) {
    if (node == nullptr || node == NULL)
      return root.childCount;

    return node->childCount;
  }

  TreeNode<T>** getChilds(TreeNode<T>* node, int* childCount) {
    if (node == nullptr || node == NULL)
      node = &root;

    (*childCount) = node->childCount;
    return node->childs;
  } // returns all childs of a node and
    // puts the count into childCount

  TreeNode<T>* getChild(TreeNode<T>* node, int n) {
    if (node == nullptr || node == NULL)
      node = &root;

    if (n >= node->childCount)
      return nullptr;
    return node->childs[n];
  } // returns the n'th child of a node
};

#endif
