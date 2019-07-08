#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <utility>
#include <list>
#include <map>

#include "Config.hpp"
#include "Conversions.hpp"
#include "Exceptions.hpp"


class Node {
	friend class XmlParser;
public:
  Node(std::string name, Node * prevNode);
  ~Node();


  bool ifGood(){
  	bool statusTemp = status;
  	status = false;
  	return statusTemp;;
  }

  template <typename T>
	T getValue(){
  	if(value.size() != 0) {
  		status = true;
  		return convert<T>(value);
  	} else {
  		status = false;
  		return convert<T>("0");
  	}
  };


  template <typename T>
  T getAttr(std::string attrName){
  	auto it = params.find(attrName);
    if (it == params.end()){
    	status = false;
  		return convert<T>("0");
    }
    status = true;
  	return convert<T>(it->second);
  }



private:
  //Current Node Main Data
  Node * prevNode;
  std::string value;
  bool status;
  std::string name;

  //Node Parameters
  std::map<std::string, std::string> params;

  //Deeper Nodes
  std::list<Node*> nodes;
};


class XmlParser {

public:
  XmlParser();
  ~XmlParser();

  //Parsing XML
  bool load(std::string path, bool safeMode);
  void parse();

  //Creating XML
  void prepareRootNode();
  Node * addNode(std::string nodeName, std::initializer_list <std::string> path);
  Node * addNode(std::string nodeName, Node * parentNode);
  void setNodeValue(Node * node, std::string value);
  void setAttribute(Node * node, std::string key, std::string value);
  void save(std::string path);

  //Utils
  void printTree(Node * nd = nullptr);
  void printXml(Node * nd = nullptr);
  Node * getNode(std::initializer_list <std::string> nodesNames);
  std::list<Node*> getNodes(std::initializer_list <std::string> nodesNames,  Node * startNode = nullptr);
  bool checkResult(std::list<Node* > * nodesList);

private:

  void preDeleteComments();
  Node * createNode(Node * nd, std::string toProcess);

  void saveXmlHelper(std::string spaces, std::string & output, Node * nd);
  void printXmlHelper(std::string spaces, Node * nd);
  void printTreeHelper(std::string spaces, Node * nd);

  Node * getNodeHelper(std::string nodeName, Node * nd);
  void getNodeHelper(std::string nodeName, Node * nd, std::list<Node*> * output, std::list<Node*> * addNode, std::list<Node*> * removeNode);

  size_t findFirstCharPos(size_t start, const std::string & temp);
  size_t findFirstWhitePos(size_t start, const std::string & temp);
  size_t findFirstNoWhitePos(size_t start, const std::string & temp);
  std::string getElementName(std::string & temp);
  std::string getAttributeName(std::string & temp);
  std::string getAttributeValue(std::string & temp);


  std::string data;
  Node * rootNode;
};


#endif
