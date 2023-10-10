#include "AVL.h"
#include <cassert>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include "json.hpp"

using namespace std;

int main(int argc, char** argv) {
  nlohmann::json jsonObject1;
  int currentKey;
  ifstream file;
  file.open(argv[1]);
  if (file.is_open()) {
    file >> jsonObject1;
  }
  file.close();
  AVLTree tree;
  for (auto itr = jsonObject1.begin(); itr.key() != "metadata"; ++itr) {
    currentKey = jsonObject1[itr.key()]["key"];
    tree.Insert(currentKey);
  }
  cout << tree.JSON() << endl;
}
