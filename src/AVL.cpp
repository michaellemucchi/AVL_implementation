#include "AVL.h"
#include <cassert>
#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include "json.hpp"

AVLNode::AVLNode(int key) :
  key_(key),
  parent_(std::weak_ptr<AVLNode>()),
  left_(nullptr),
  right_(nullptr) {}

AVLNode::AVLNode(int key, std::weak_ptr<AVLNode> parent) :
  key_(key),
  parent_(parent),
  left_(nullptr),
  right_(nullptr) {}

AVLTree::AVLTree() : root_(nullptr), size_(0) {}

int AVLTree::GetHeight(std::shared_ptr<AVLNode> node) {
  if (node == nullptr) {
    return -1;
  } else {
    return std::max(GetHeight(node->left_), GetHeight(node->right_)) + 1;
  }
}

int AVLTree::GetBalance(std::shared_ptr<AVLNode> node) {
  if (node == nullptr) {
    return 0;
  }
  return GetHeight(node->left_) - GetHeight(node->right_);
}

void AVLTree::LL(std::shared_ptr<AVLNode> originalRoot) {
  std::shared_ptr<AVLNode> newRoot = originalRoot->left_;
  originalRoot->left_ = newRoot->right_;
  newRoot->right_ = originalRoot;
  if (originalRoot->left_ != nullptr) {
    originalRoot->left_->parent_ = originalRoot;
  }
  newRoot->parent_ = originalRoot->parent_;
  if (originalRoot->parent_.lock() == nullptr) {
    return;
  }
  else if (originalRoot == originalRoot->parent_.lock()->left_) {
    originalRoot->parent_.lock()->left_ = newRoot;
  }
  else if (originalRoot == originalRoot->parent_.lock()->right_) {
    originalRoot->parent_.lock()->right_ = newRoot;
  }
  originalRoot->parent_ = newRoot;
}

void AVLTree::RR(std::shared_ptr<AVLNode> originalRoot) {
  std::shared_ptr<AVLNode> newRoot = originalRoot->right_;
  originalRoot->right_ = newRoot->left_;
  newRoot->left_ = originalRoot;
  if (originalRoot->right_ != nullptr) {
    originalRoot->right_->parent_ = originalRoot;
  }
  newRoot->parent_ = originalRoot->parent_;
  if (originalRoot->parent_.lock() == nullptr) {
    return;
  }
  else if (originalRoot == originalRoot->parent_.lock()->left_) {
    originalRoot->parent_.lock()->left_ = newRoot;
  }
  else if (originalRoot == originalRoot->parent_.lock()->right_) {
    originalRoot->parent_.lock()->right_ = newRoot;
  }
  originalRoot->parent_ = newRoot;
}

void AVLTree::Balancing(int key, std::shared_ptr<AVLNode> root_) {
  std::shared_ptr<AVLNode> x = root_, lastNode = nullptr;
  while (x != nullptr) {
    if(GetBalance(x) > 1) {
      if(GetHeight(x->left_->left_) >= GetHeight(x->left_->right_)) {
        LL(x);
      }
      else {
        RR(x->left_);
        LL(x);
      }
    }
    if(GetBalance(x) < -1) {
      if(GetHeight(x->right_->right_) >= GetHeight(x->right_->left_)) {
        RR(x);
      } else {
        LL(x->right_);
        RR(x);
      }
    }
    x = (key < x->key_) ?
      x->left_ : x->right_;
  }
}

void AVLTree::Insert(int key) {
  if (root_ == nullptr) {
    root_ = std::make_shared<AVLNode>(key);
    size_++;
    return;
  }
  std::shared_ptr<AVLNode> currentNode = root_, lastNode = nullptr;
  while (currentNode != nullptr) {
    lastNode = currentNode;
    currentNode = (key < currentNode->key_) ?
      currentNode->left_ : currentNode->right_;
  }
  if (key < lastNode->key_) {
    lastNode->left_ = std::make_shared<AVLNode>(key, lastNode);
    Balancing(key, root_);
  } else {
    lastNode->right_ = std::make_shared<AVLNode>(key, lastNode);
    Balancing(key, root_);
  }

  size_++;
}

std::string AVLTree::JSON() {
	nlohmann::json result;
	std::queue< std::shared_ptr<AVLNode> > nodes;
	if (root_ != nullptr) {
		result["root"] = root_->key_;
		nodes.push(root_);
    result["height"] = GetHeight(root_);
		while (!nodes.empty()) {
			auto v = nodes.front();
			nodes.pop();
			std::string key = std::to_string(v->key_);
      result[key]["height"] = GetHeight(v);
      result[key]["balance factor"] = -1 * GetBalance(v);
			if (v->left_ != nullptr) {
				result[key]["left"] = v->left_->key_;
				nodes.push(v->left_);
			}
			if (v->right_ != nullptr) {
				result[key]["right"] = v->right_->key_;
				nodes.push(v->right_);
			}
			if (v->parent_.lock() != nullptr) {
				result[key]["parent"] = v->parent_.lock()->key_;
			} else {
				result[key]["root"] = true;
			}
		}
	}
	result["size"] = size_;
	return result.dump(2) + "\n";
}
