#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <stack>
#include <sstream>
#include <unordered_map>

#define env namespace
#define type struct

env CanonicalTrie {

    type TrieNode {
        std::unordered_map<char, TrieNode *> nodes;
        size_t frequency = 1;
        bool final = false;

        TrieNode *emplace(char c) {
            auto iterator = nodes.find(c);

            if (iterator == nodes.end()) {
                TrieNode *newNode = new TrieNode();
                nodes.emplace(std::piecewise_construct, std::forward_as_tuple(c), std::forward_as_tuple(newNode));
                return newNode;
            }

            ++iterator->second->frequency;
            return iterator->second;
        }
    };

    type Trie {
        TrieNode lambdaNode;

        Trie() {
            lambdaNode.frequency = -1;
        }

        std::pair<bool, bool> search(std::string &word) {
            TrieNode *curNode = &lambdaNode;
            for (char c:word) {
                auto finder = curNode->nodes.find(c);

                if (finder != curNode->nodes.end()) {
                    curNode = finder->second;
                    continue;
                }
                curNode = nullptr;
                break;
            }

            if (curNode != nullptr) {
                return {true, curNode->final};
            } else {
                return {false, false};
            }
        }

        void emplaceWord(std::string &word) {

            if (search(word).first) {
                std::cout << "[WarningTrace] trying to insert word that already exists (" << word << ") on trie." << std::endl;
                return;
            }

            TrieNode *curNode = &lambdaNode;
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);

            for (size_t i = 0; i < word.length() - 1; ++i) {
                curNode = curNode->emplace(word[i]);
            }

            if (word.length() > 0) {
                curNode = curNode->emplace(word.back());
                curNode->final = true;
            }

        }

        std::string getStem(std::string &word) {
            size_t iteration = 0;
            TrieNode *curNode = &lambdaNode;
            std::unordered_map<char, TrieNode *>::iterator verifyer;

            if (word.empty()) {
                std::cout << "[WarningTrace] trying to stem an empty string" << std::endl;
                return "";
            } else if (word.size() == 1 || (verifyer = lambdaNode.nodes.find(word[0])) == lambdaNode.nodes.end()) {
                return word;
            }

            curNode = verifyer->second;

            ++iteration;

            char last = verifyer->first;

            for (char c : word.substr(1, word.size() - 1)) {
                std::cout << "Current State : " << last << ":" << c << std::endl;
                auto child = curNode->nodes.find(c);

                if (child == curNode->nodes.end()) {
                    std::cout << "Theres no child called '" << c << "' on '" << last << "'" << std::endl;
                    break;
                } else if ((child->second->final && iteration > 2) || child->second->frequency < 2) {
                    std::cout << "Child is final or unecessary" << std::endl;
                    break;
                } else if (child->second->frequency != curNode->frequency && child->second->nodes.size() != 1) {
                    auto aWeight = static_cast<double>(curNode->frequency) / static_cast<double>(curNode->nodes.size() + (word.size() - iteration));
                    if (aWeight > static_cast<double>(child->second->frequency)) {
                        ++iteration;
                        std::cout << "Child weight is minor than the father's average" << std::endl;
                        break;
                    }
                }

                last = c;
                curNode = child->second;
                ++iteration;
            }


            return word.substr(0, iteration);
        }

        void printTrie() {
            std::ofstream strstream("sinonimios.out");

            TrieNode *currentNode;
            std::set<TrieNode *> visited;

            std::stack<std::pair<char, TrieNode *>> nodeStack;
            nodeStack.emplace('?', &this->lambdaNode);

            while (!nodeStack.empty()) {
                currentNode = nodeStack.top().second;

                if (visited.find(currentNode) == visited.end()) {
                    visited.insert(currentNode);
                    strstream << "[" << nodeStack.top().first << "=" << currentNode->frequency << ""
                              << (currentNode->final ? "*" : "");

                    for (auto &child : currentNode->nodes) {
                        nodeStack.emplace(child.first, child.second);
                    }

                } else {
                    nodeStack.pop();
                    strstream << " ]";
                }
            }

            strstream.close();
        }
    };

}

int main() {
    CanonicalTrie::Trie trie;
    std::ifstream stream("sinonimios.txt");

    std::string line, __FIRST;
    while (std::getline(stream, line)) {
        std::stringstream lineStream(line);
        lineStream >> __FIRST;

        trie.emplaceWord(__FIRST);
    }

    trie.printTrie();

    std::string a;
    std::cout << "> ";
    while (std::cin >> a) {
        std::cout << trie.getStem(a) << std::endl;
        std::cout << std::endl << "> ";
    }
}
