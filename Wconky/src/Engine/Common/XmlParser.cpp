#include "XmlParser.hpp"

//------------------------------------------------------------------------------
//Class: Node
//------------------------------------------------------------------------------

Node::Node(std::string name, Node * prevNode){
  this->name = name;
  this->prevNode = prevNode;
};

Node::~Node(){
  for(auto n : nodes){
    delete n;
  }
}

//------------------------------------------------------------------------------
//Class: XmlParser
//------------------------------------------------------------------------------

XmlParser::XmlParser(){
  rootNode = nullptr;
};

XmlParser::~XmlParser(){
  if(rootNode != nullptr) delete rootNode;
}

void XmlParser::printTree(Node * nd){
  if(nd == nullptr){
    printTreeHelper("  ", rootNode);
  } else {
    printTreeHelper("  ", nd);
  }
}

void XmlParser::printTreeHelper(std::string spaces, Node * nd){
  std::cout<<spaces<<nd->name;
  if(nd->params.size() != 0){
    std::cout<<"      Params: ";
    for(auto it = nd->params.begin(); it != nd->params.end(); it++){
      std::cout<<(*it).first<<"["<<(*it).second<<"],";
    }
  }

  if(nd->value.size() != 0) std::cout<<"      Value: "<<nd->value<<std::endl;
  std::cout<<std::endl;
  spaces += "    ";
  for( auto it = nd->nodes.begin(); it != nd->nodes.end(); it++){
    printTreeHelper(spaces, (*it));
  }
}

bool XmlParser::load(std::string path, bool safeMode){

  //Clear data (for using one occurence of class for multiply files)
  data.clear();
  if(rootNode != nullptr){
    delete rootNode;
  }
  rootNode = new Node("root", nullptr);

  //Load file
  std::fstream file;
  file.open(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
  if( file.good() == true ){

    int fileSize = file.tellg();
    file.seekg(0, std::ios_base::beg);

    char buf[fileSize];
    file.read(buf, fileSize);

    if(file.gcount() != fileSize){
      if(DEBUG) std::cerr<<"[XmlParser] -> load: Cannot load enough data!"<<std::endl;
      file.close();
      return false;
    }

    //File is loaded here to "data" string
    data = buf;
    if(safeMode) preDeleteComments();


    file.close();
    return true;
  }
  if(DEBUG) std::cerr<<"[XmlParser] -> load: Cannot open file!"<<std::endl;
  return false;
}

void XmlParser::preDeleteComments(){
	size_t startPos = data.find("<!--", 0);
	size_t endPos = data.find("-->", 0);

	while(startPos != std::string::npos or endPos != std::string::npos){

		if(startPos != std::string::npos and endPos != std::string::npos){
			if(startPos < endPos){
				data.erase(startPos, (endPos+3)-startPos);
			} else {
				throw Exception("XmlParser", "preDeleteComments()", "Inverted comment tag order.");
			}
		} else if(startPos != std::string::npos) {
			throw Exception("XmlParser", "preDeleteComments()", "Cannot find end tag to pair with \"<!--\"");
		} else if(endPos != std::string::npos){
			throw Exception("XmlParser", "preDeleteComments()", "Cannot find start tag to pair with \"-->\"");
		}

		startPos = data.find("<!--", 0);
		endPos = data.find("-->", 0);

	}
}


Node * XmlParser::createNode(Node * curNode, std::string toProcess){

	//Find tag name & create node
	std::string tagName = getElementName(toProcess);

	curNode->nodes.push_back(new Node(tagName, curNode));
	curNode = curNode->nodes.back();

	//Find all attributes and add them
	while(toProcess.size()){
		//Find attrib name
		std::string attrName = getAttributeName(toProcess);
		if(attrName.size() == 0) break;
		//Find attrib value
		std::string attribValue = getAttributeValue(toProcess);
		//Add
	  curNode->params.insert(std::pair<std::string, std::string>(attrName, attribValue));
	}

	return curNode;

}

void XmlParser::parse(){

  Node * curNode = rootNode;

  //std::regex word_regex("(<\\s*\\w[^<>&]*)>|<(\\s*\\/\\w[^<>&]*)>|(\\s*\\w[^<>/]*)|(<\\/{0,}>)");       //Explode to fragments
  std::regex word_regex("(<\\s*\\w[^<>&]*)>|<(\\s*\\/\\w[^<>&]*)>|(<\\/{0,}>)|(\\S[^<>&]*)");       //Explode to fragments
  std::regex self_regex_new("(<\\s*\\w[^<>&]*)>|(<>)");                                  //Check if new tag

  std::regex self_regex_end("(<\\s*\\/.[^<>&]*>)");                                 //Check if end tag
  std::regex word_regex_end("\\s*\\w[^<>& ]*");                                      //    -> Security check if ending tag does not contain more (-> corrupted) data then is should

  std::regex self_regex_ne("(<\\s*\\w[^<>&]*.*\\/[ ]*>)|(<\\/>)");													//New tag with rapid end (example: <node/>)


  auto words_begin = std::sregex_iterator(data.begin(), data.end(), word_regex);
  auto words_end = std::sregex_iterator();

  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
      std::smatch match = *i;
      std::string match_str = match.str();


     //std::cout<<match_str<<std::endl;

      //Check if new TAG without next nodes
      if(std::regex_match(match_str, self_regex_ne)){
    	//curNode->value = "4";
        std::size_t last = match_str.find_last_of("/");
        match_str = match_str.substr(1, match_str.size()-1-(match_str.size()-last));
      	createNode(curNode, match_str);
        continue;
      }


      //Check if new TAG
      if (std::regex_match(match_str, self_regex_new)) {\
      	match_str = match_str.substr(1, match_str.size()-2);
      	curNode = createNode(curNode, match_str);
        continue;
      }

      //Check if END TAG
      if (std::regex_match(match_str, self_regex_end)) {

          auto words_params_begin = std::sregex_iterator(match_str.begin(), match_str.end(), word_regex_end);
          auto words_params_end = std::sregex_iterator();

          for(std::sregex_iterator p = words_params_begin; p != words_params_end; ++p){

            std::smatch match_p = *p;
            std::string match_p_str = match_p.str();

            if(p == words_params_begin){
              curNode = curNode->prevNode;
            } else {
        			throw Exception("XmlParser", "parser()", "Malformated element closing tag: "+match_str);
            }

          }
          continue;
      }

      //That is just value
      curNode->value = match_str;
      if(DEBUG)std::cout<<"Just Value: "<< match_str<<std::endl;

  }

}


Node * XmlParser::getNode(std::initializer_list <std::string> nodesNames){
  Node * curNode = rootNode;
  std::string nTemp;

  for (auto n : nodesNames){
    nTemp = n;
    Node * temp = getNodeHelper(n, curNode);
    if(temp != nullptr){
      curNode = temp;
    } else {
      //curNode = curNode->prevNode;
      return nullptr;
    }
  }
  if(curNode->name != nTemp) return nullptr;
  return curNode;
}

Node * XmlParser::getNodeHelper(std::string nodeName, Node * nd){

  if(nd->name == nodeName) return nd;

  for(auto it = nd->nodes.begin(); it != nd->nodes.end(); it++){
      if((*it)->name == nodeName){
        return (*it);
      }
  }
  return nullptr;
}

std::list<Node*> XmlParser::getNodes(std::initializer_list <std::string> nodesNames, Node * startNode){

  std::list<Node*> output;
  std::list<Node*> addNode;
  std::list<Node*> removeNode;


  if(startNode == nullptr) startNode = rootNode;
  output.push_back(startNode);

  for (auto n : nodesNames){
   // std::cout<<"dupa"<<std::endl;

    for(auto it = output.begin(); it != output.end(); it++){
       getNodeHelper(n, (*it), &output, &addNode, &removeNode);
    }
    for(auto it = removeNode.begin(); it != removeNode.end(); it++){
      output.remove((*it));
    }
    removeNode.clear();

    for(auto it = addNode.begin(); it != addNode.end(); it++){
      output.push_back((*it));
    }
    addNode.clear();

  }

  if(output.size() == 0){
  	output.push_back(nullptr);
  }

  return output;
}

bool XmlParser::checkResult(std::list<Node* > * nodesList){
	if(nodesList->front() == nullptr) return false;
	return true;
}


void XmlParser::getNodeHelper(std::string nodeName, Node * nd, std::list<Node*> * output, std::list<Node*> * addNode, std::list<Node*> * removeNode){
  if(nd != nullptr){
  	for(auto it = nd->nodes.begin(); it != nd->nodes.end(); it++){
      if((*it)->name == nodeName){
        addNode->push_back((*it));
      }
    }
    removeNode->push_back(nd);
  }
}


size_t XmlParser::findFirstCharPos(size_t start, const std::string & temp){
	for(unsigned int i = start; i<temp.size(); i++){
		bool result = std::isalpha(temp[i]);
		if(result == 1) return i;
	}
	return std::string::npos;
}

size_t XmlParser::findFirstWhitePos(size_t start, const std::string & temp){
	for(unsigned int i = start; i<temp.size(); i++){
		bool result = std::isspace(temp[i]);
		if(result == 1) return i;
	}
	return std::string::npos;
}

size_t XmlParser::findFirstNoWhitePos(size_t start, const std::string & temp){
	for(unsigned int i = start; i<temp.size(); i++){
		bool result = std::isgraph(temp[i]);
		if(result == 1) return i;
	}
	return std::string::npos;
}

std::string XmlParser::getElementName(std::string & temp){
	//std::cout<<temp<<"  Size: "<<temp.size()<<std::endl;
	if(temp.size() == 0){
		throw Exception("XmlParser", "getElementName()", "Some element name is empty?");
	} else if(temp.size() == 1 and std::isalpha(temp[0])){
		std::string elementName; elementName.append(&temp[0]);
		//std::cout<<elementName<<std::endl;
		temp.clear();
		return elementName;
	}
	size_t startPos = findFirstCharPos(0, temp);
	if(startPos != std::string::npos){
		size_t endPos = findFirstWhitePos(startPos, temp);
		if(endPos == std::string::npos){
			endPos = temp.size();
		}

		if(findFirstNoWhitePos(endPos, temp) != std::string::npos and temp.find('=', endPos) == std::string::npos){
			throw Exception("XmlParser", "getElementName()", "Malformated element name: "+temp);
		}

		std::string elementName = temp.substr(startPos, (endPos-startPos));
		temp = temp.substr(endPos, temp.size()-endPos);
		return elementName;
	}
	throw Exception("", "getElementName()", "Cannot find valid element name!");
	return std::string("");
}

std::string XmlParser::getAttributeName(std::string & temp){
	size_t startPos = findFirstCharPos(0, temp);
	if(startPos != std::string::npos){
		size_t endPos = temp.find('=', startPos);
		if(endPos != std::string::npos){
			std::string attrName = temp.substr(startPos, (endPos-startPos));

			attrName.erase(0, attrName.find_first_not_of(" \t\n\r\f\v"));
			attrName.erase(attrName.find_last_not_of(" \t\n\r\f\v") + 1);
			if(findFirstWhitePos(0, attrName) != std::string::npos){
				throw Exception("", "getAttributeName()", "Attribute name malformated: "+attrName);
			}

			temp = temp.substr(endPos+1, temp.size()-endPos-1);
			return attrName;
		}
	}
	//throw Exception("", "getAttributeName", "Cannot find attribute name!");
	return std::string("");
}

std::string XmlParser::getAttributeValue(std::string & temp){
	size_t startPos_1 = temp.find('"');
	size_t startPos_2 = temp.find('\'');

	char mainChar;
	size_t startPos;

	if(startPos_1 != std::string::npos and startPos_2 != std::string::npos){
		if(startPos_1 < startPos_2){
			mainChar = '"';
			startPos = startPos_1;
		} else {
			mainChar = '\'';
			startPos = startPos_2;
		}
	} else if(startPos_1 != std::string::npos){
		mainChar = '"';
		startPos = startPos_1;
	} else if(startPos_2 != std::string::npos){
		mainChar = '\'';
		startPos = startPos_2;
	} else {
		throw Exception("", "getAttributeValue()", "Cannot find value - start position!");
	}

	size_t endPos = temp.find(mainChar, startPos+1);

	//Hurray
	if(endPos != std::string::npos){
		std::string attribValue = temp.substr(startPos+1, (endPos-startPos)-1);
		temp = temp.substr(endPos+1, temp.size()-endPos-1);
		return attribValue;
	}
	throw Exception("", "getAttributeValue()", "Cannot find value - end position!");
}


