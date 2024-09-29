#include "tree.hpp"
#include <cstdlib>

// constructors
template <class T> cm_Tree<T>::cm_Tree() {
  root.parent = nullptr;
  root.value = nullptr;
  root.childCount = 0;
  root.childs = nullptr;
}
template <class T> cm_Tree<T>::cm_Tree(cm_TreeData data) {
  root.parent = nullptr;
  root.value = nullptr;
  root.childCount = 0;
  root.childs = nullptr;
}

// destructors

template <class T> void helperFreeFunc(TreeNode<T> node) {
  for (int i = 0; i < node.childCount; i++) {
    helperFreeFunc<T>(node.childs[i]);
    free(node.childs[i]);
  }

  free(node.childs);
}

template <class T> cm_Tree<T>::~cm_Tree() {
  //
  helperFreeFunc<T>(root);
}

// setters

// getters
