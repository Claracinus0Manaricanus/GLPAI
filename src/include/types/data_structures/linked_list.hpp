#ifndef CM_LINKED_LIST
#define CM_LINKED_LIST

template <class T> struct LinkedNode {
  T* value;
  LinkedNode* previous;
  LinkedNode* next;
};

template <class T> class cm_LinkedList {
private:
  LinkedNode<T> root;
  LinkedNode<T>* current_pos = &root;
  LinkedNode<T>* last_node = &root;
  int length = 0;

public:
  // constructors
  cm_LinkedList() {
    root.previous = nullptr;
    root.next = nullptr;
    root.value = nullptr;
  }

  // destructors
  ~cm_LinkedList() {
    while (last_node->previous != nullptr) {
      free(last_node->value);
      last_node = last_node->previous;
      free(last_node->next);
    }

    free(last_node->value);
  }

  // setters
  LinkedNode<T>* addNode(T* toAdd) {
    if (root.value == nullptr) {
      root.value = toAdd;
      length++;
      return &root;
    }

    LinkedNode<T>* tmp = new LinkedNode<T>;

    tmp->previous = last_node;
    tmp->next = nullptr;
    tmp->value = toAdd;

    last_node->next = tmp;
    last_node = tmp;
    length++;

    return tmp;
  }

  // delete index'th node from root, if 0 is given root node takes the values of
  // the next node and deletes it, if a value greater than length is given last
  // node is deleted
  void removeNode(int index) {
    if (index == 0) {
      LinkedNode<T>* tmp = root.next;
      free(root.value);
      root.value = tmp->value;
      root.next = tmp->next;
      tmp->next->previous = &root;
      free(tmp);
    } else {
      LinkedNode<T>* tmp = nth(index);
      tmp->previous->next = tmp->next;
      tmp->next->previous = tmp->previous;
      free(tmp->value);
      free(tmp);
    }

    length--;
  }

  // getters

  // returns current_pos'th element and increments it by
  // one, loops back if end is reached
  LinkedNode<T>* next() {
    LinkedNode<T>* tmp = current_pos;

    if (current_pos->next == nullptr) {
      current_pos = &root;
    } else {
      current_pos = current_pos->next;
    }

    return tmp;
  }

  // returns current_pos'th element and decrements it
  // by one, loops back if start is reached
  LinkedNode<T>* previous() {
    LinkedNode<T>* tmp = current_pos;

    if (current_pos->previous == nullptr) {
      current_pos = last_node;
    } else {
      current_pos = current_pos->previous;
    }

    return tmp;
  }

  LinkedNode<T>* nth(int index) {
    LinkedNode<T>* tmp = &root;

    for (int i = 0; i < index; i++) {
      if (tmp->next != nullptr)
        tmp = tmp->next;
      else
        break;
    }

    return tmp;
  }

  // reverts current_pos to root node
  LinkedNode<T>* rewind() {
    current_pos = &root;
    return &root;
  }

  int size() { return length; }
};

#endif
