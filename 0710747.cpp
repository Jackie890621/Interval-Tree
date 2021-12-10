#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <time.h>
using namespace std;

vector<int> O;
vector<vector<int>> Overlap;
bool flag;
int min_area = INT_MAX; 
vector<int> area_list;
vector<int> search_list;

struct Interval
{
	int order;
	int x_low, y_low;
	int x_high, y_high;
};

struct Node
{
	Interval *n;
	Node *left;
	Node *right;
};

Node *newNode(Interval n)
{
	Node *temp = new Node;
	temp->n = new Interval(n);
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

Node *insert(Node *root, Interval n)
{
	if (root == NULL) {
		return newNode(n);
	}
	
	int root_low = root->n->x_low;
	int low = n.x_low;
	
	if (low < root_low) {
		root->left = insert(root->left, n);
	} else {
		root->right = insert(root->right, n);
	}

	return root; 
}	

bool overlap(Interval n1, Interval n2)
{
	if (n1.x_low <= n2.x_low && n1.x_high >= n2.x_high) {
		if (n1.y_low <= n2.y_low && n1.y_high >= n2.y_high) {
			if ((n2.x_low - n1.x_low) >= 20 && 
		    	    (n2.y_low - n1.y_low) >= 20 && 
		    	    (n1.x_high - n2.x_high) >= 20 && 
		      	    (n1.y_high - n2.y_high) >= 20) {
				flag = false;
			} else {
				flag = true;
			}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void overlapSearch(Node *root, Interval n)
{
	if (root == NULL) {
		return;
	}

	overlapSearch(root->left, n);
	
	if (overlap(*(root->n), n)) {
		O.push_back(root->n->order);
		if (flag) {
			O.push_back(1);
			flag = false;
		} else {
			O.push_back(0);
		}
		Overlap.push_back(O);
		O.clear();
	}

	overlapSearch(root->right, n);
}

void Search(Node *root, Interval n)
{
	if (root == NULL) {
		return;
	}
	if (root->n->x_low == n.x_low &&
	    root->n->y_low == n.y_low &&
	    root->n->x_high == n.x_high &&
	    root->n->y_high == n.y_high) {
		if (search_list.size() == 0) {
			search_list.push_back(root->n->order);
		} else if (search_list.back() != root->n->order) {
			search_list.push_back(root->n->order);
		}

		if (root->right != NULL) {
			Search(root->right, n);
		} else {
			return;
		}
		return;
	}

	if (root->n->x_low > n.x_low) {
		Search(root->left, n);
	} else {
		Search(root->right, n);
	}
}

Node *Delete(Node *root, Interval n) 
{
	if (root == NULL) {
		return root;
	}
	if (root->n->x_low > n.x_low) {
		root->left = Delete(root->left, n);
	} else if (root->n->x_low < n.x_low) {
		root->right = Delete(root->right, n);
	} else {
		if (root->n->x_high != n.x_high ||
		    root->n->y_low != n.y_low ||
		    root->n->y_high != n.y_high) {
			root->right = Delete(root->right, n);
		} else {
			if (root->left == NULL && root->right == NULL) {
                		return NULL;
			} else if (root->left == NULL){
				Node *temp = root->right;
				free(root);
				return temp;
			} else if (root->right == NULL){
                                Node *temp = root->left;
                                free(root);
                                return temp;
			}
			
			Node *temp = root->right;
			while (temp->left != NULL) {
				temp = temp->left;
				if (temp->n->x_low == n.x_low &&
				    temp->n->x_high == n.x_high &&
                    		    temp->n->y_low == n.y_low &&
                    		    temp->n->y_high == n.y_high) {
					break;
				}
			}
			root->n->x_low = temp->n->x_low;
			root->n->x_high = temp->n->x_high;
			root->n->y_low = temp->n->y_low;
			root->n->y_high = temp->n->y_high;
			root->n->order = temp->n->order;
			root->right = Delete(root->right, *temp->n);
		}
	}
	return root;
}

void Area(Node *root, int x_low, int y_low) 
{
	if (root == NULL) {
		return;
	}
	if (root->n->x_low == x_low && root->n->y_low == y_low) {
		int area = (root->n->x_high - x_low) * (root->n->y_high - y_low);
		if (area < min_area) {
			area_list.clear();	
			min_area = area;
			area_list.push_back(root->n->order);
		} else if (area == min_area && area_list.back() != root->n->order) {
			area_list.push_back(root->n->order);
		}
		if (root->right != NULL) {
			Area(root->right, x_low, y_low);
		} else {
			return;
		}
		return;
	}
	
	if (root->n->x_low > x_low) {
                Area(root->left, x_low, y_low);
        } else {
               	Area(root->right, x_low, y_low);
        }
}

int main(int argc, char *argv[])
{
	time_t t = clock();
	Node *root = NULL;
	vector<string> res;
	
	string result;
	string text;
	
	ifstream MyReadFile(argv[1]);
	ofstream MyFile(argv[2]);
	
	while (getline(MyReadFile, text)) {
		res.clear();
		stringstream input(text);
		while (input >> result) {
			res.push_back(result);
		}
		Interval n;
		switch (res[0][0]) {
		case 'I':
			n.order = stoi(res[1].substr(1));
			n.x_low = stoi(res[2]);
			n.y_low = stoi(res[3]);
			n.x_high = stoi(res[4]);
			n.y_high = stoi(res[5]);
			root = insert(root, n);
			break;
		case 'D':
			n.x_low = stoi(res[1]);
                        n.y_low = stoi(res[2]);
                        n.x_high = stoi(res[3]);
                        n.y_high = stoi(res[4]);
			root = Delete(root, n);
			break;
		case 'S':
			n.x_low = stoi(res[1]);
                        n.y_low = stoi(res[2]);
                        n.x_high = stoi(res[3]);
                        n.y_high = stoi(res[4]);
			Search(root, n);
			MyFile << 'S' << endl;
			for (int i = 0; i < search_list.size(); i++) {
				MyFile << 'r' << search_list[i] << endl;
			}
			search_list.clear();
			break;
		case 'O':
			n.x_low = stoi(res[1]);
                        n.y_low = stoi(res[2]);
                        n.x_high = stoi(res[3]);
                        n.y_high = stoi(res[4]);
			overlapSearch(root, n);
			MyFile << 'O' << endl;
			sort(Overlap.begin(), Overlap.end());
			for (int i = 0; i < Overlap.size(); i++) {
				if (Overlap[i][1] == 1) {
					MyFile << 'r' << Overlap[i][0] << " violate" << endl;
				} else {
					MyFile << 'r' << Overlap[i][0] << endl;
				}
			}
			Overlap.clear();
			break;
		case 'A':
                        Area(root, stoi(res[1]), stoi(res[2]));
			sort(area_list.begin(), area_list.end());
			MyFile << 'A' << endl;
			for (int i = 0; i < area_list.size(); i++) {
                        	MyFile << 'r' << area_list[i] << endl;
			}
			min_area = INT_MAX;
			area_list.clear();
			break;
		}

	}
	MyReadFile.close();
	t = clock() - t;
	cout << double(t) / CLOCKS_PER_SEC << endl;
	return 0;
}

