#include <File.hpp>

#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace pandora;
using namespace H5;
using namespace std;

File::File(std::string name, std::string prefix, std::string mode) {
  if (mode == "r") {
    this->h5file = H5File(name.c_str(), H5F_ACC_RDONLY);
  } else {
    this->h5file = H5File(name.c_str(), H5F_ACC_TRUNC);
  }
  this->prefix = prefix;
  this->h5group = h5file.openGroup("/");
  vector<pair<string, DataType> > attribs;
  attribs.push_back(pair<string, DataType>("format", H5::StrType()));
  attribs.push_back(pair<string, DataType>("version", H5::StrType()));
  attribs.push_back(pair<string, DataType>("created_at", H5::StrType()));
  attribs.push_back(pair<string, DataType>("updated_at", H5::StrType()));

  checkAttributes(attribs);
}

File& File::operator=(const File &other) {
  this->h5file = other.h5file;
  this->h5group = other.h5group;
  this->prefix = other.prefix;

  return *this;
}

void File::checkAttributes(vector<pair<string, DataType> > attribs) {
  for (int i = 0; i < (int) attribs.size(); i++) {
    if (!this->attrExists(attribs[i].first)) {
      this->setAttr(attribs[i].first, "default");
    }
  }
}

Block File::createBlock(std::string name, std::string type) {
  H5::Group block_group = h5group.createGroup(name);

  Block block = Block(*this, block_group);
  block.type(type);
  return block;
}

string File::createId() const {
  static bool initialized = false;
  static const char* hex = "0123456789abcdef";

  if (!initialized) {
      srand(time(NULL));
  }

  string id = string(prefix);
  id += "_";
  for (int i = 0; i < 32; i++) {
    char c = hex[(size_t) (((double) (rand())) / RAND_MAX * 16)];
    id += c;
  }

  return id;
}

void File::close() {
  h5file.close();
}

File::~File() {
  close();
}

