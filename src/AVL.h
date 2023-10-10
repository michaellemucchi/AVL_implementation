#include <memory>
#include <string>

class AVLTree;

class AVLNode {
  public:
    AVLNode(int key);
    AVLNode(int key, std::weak_ptr<AVLNode> parent);
    int key_;
  private:
    std::weak_ptr<AVLNode> parent_;
    std::shared_ptr<AVLNode> left_;
    std::shared_ptr<AVLNode> right_;
    friend AVLTree;
};

class AVLTree {
  public:
    AVLTree();
    void Balancing(int key, std::shared_ptr<AVLNode> root_);
    void Insert(int key);
    int GetHeight(std::shared_ptr<AVLNode> node);
    int GetBalance(std::shared_ptr<AVLNode> node);
    void LL(std::shared_ptr<AVLNode> x);
    void RR(std::shared_ptr<AVLNode> x);
    std::string JSON();
    std::shared_ptr<AVLNode> root_;
    size_t size_;
};
