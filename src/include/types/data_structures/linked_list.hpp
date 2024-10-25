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
  int freeEnabled = 1;

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
      if (freeEnabled)
        free(last_node->value);
      last_node = last_node->previous;
      free(last_node->next);
    }

    if (freeEnabled)
      free(last_node->value);
  }

  // setters
  void setFreeEnabled(int set) { freeEnabled = set; }

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
    if (length == 0)
      return;

    if (index == 0) {
      LinkedNode<T>* tmp = root.next;
      if (freeEnabled)
        free(root.value);

      if (tmp != nullptr) {
        root.value = tmp->value;
        if ((root.next = tmp->next) == nullptr)
          last_node = &root;
        else
          tmp->next->previous = &root;
        free(tmp);
      }
    } else {
      LinkedNode<T>* tmp = nth(index);
      tmp->previous->next = tmp->next;

      if (tmp->next != nullptr) {
        tmp->next->previous = tmp->previous;
      } else { // if tmp->next is nullptr then tmp->next is the last node so
               // last node needs to be updated
        last_node = tmp->previous;
      }

      if (freeEnabled)
        free(tmp->value);
      free(tmp);
    }

    length--;
  }

  /**
   * Remove the given node from the list.
   */
  void removeNode(LinkedNode<T>* toRemove) {
    if (toRemove == &root) {
      removeNode(0);
      return;
    }

    if (toRemove == nullptr)
      return;

    toRemove->previous->next = toRemove->next;

    if (toRemove->next != nullptr) {
      toRemove->next->previous = toRemove->previous;
    } else { // if toRemove->next is nullptr then toRemove->next is the last
             // node so last node needs to be updated
      last_node = toRemove->previous;
    }

    if (freeEnabled)
      free(toRemove->value);
    free(toRemove);

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

  LinkedNode<T>* current() { return current_pos; }

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

  LinkedNode<T>* last() { return last_node; }

  // reverts current_pos to root node
  LinkedNode<T>* rewind() {
    current_pos = &root;
    return &root;
  }

  int size() { return length; }
};

#endif
