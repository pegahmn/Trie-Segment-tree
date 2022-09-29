
#include <iostream>
#include <string>
#include <stack>

#define M  1000000
#define M2  100000

using namespace std;

//First Phase

class TrieNode {
public:
	int bit;
	TrieNode* child[2];
	bool isleaf;

	TrieNode() {
		bit = -1;
		child[0] = NULL;
		child[1] = NULL;
		isleaf = false;
	}

	~TrieNode() {
	}
};

class TrieTree {
public:
	TrieNode* Root;
	TrieTree() {
		Root = NULL;
	}

	~TrieTree() {
		delete Root;
	}

	void insert(int v) {
		string r = decToBinary(v);
		TrieNode* temp = new TrieNode();

		if (Root == NULL) {
			temp->bit = -1;
			temp->isleaf = false;
			Root = temp;
		}
		temp = Root;

		for (int i = 0; i < 20; i++) {

			TrieNode* temp2 = new TrieNode();
			if (r[i] == '0') {

				if (temp->child[0] == NULL) {
					temp2->isleaf = false;
					temp2->bit = 0;
					temp->child[0] = temp2;
				}

				temp = temp->child[0];
			}

			else if (r[i] == '1') {

				if (temp->child[1] == NULL) {
					temp2->isleaf = false;
					temp2->bit = 1;
					temp->child[1] = temp2;
				}

				temp = temp->child[1];
			}

			if (i == 19) {
				temp2->isleaf = true;
			}

			///cout << temp->bit << " ";
		}
		//cout <<"  v:  " << v << endl;
	}

	int XOR(int x) {

		string r = decToBinary(x);
		string r2;
		TrieNode* temp = Root;

		for (int i = 0; i < 20; i++) {

			if (r[i] == '0') {

				if (temp->child[1] != NULL) {
					r2 += "1";
					temp = temp->child[1];
				}

				else {
					r2 += "0";
					temp = temp->child[0];
				}
			}

			if (r[i] == '1') {

				if (temp->child[0] != NULL) {
					r2 += "0";
					temp = temp->child[0];
				}

				else {
					r2 += "1";
					temp = temp->child[1];
				}
			}
		}
		return (x ^ (binaryToDec(r2)));
	}

	string decToBinary(int n) {

		string r;

		for (int i = 19; i >= 0; i--) {
			int k = n >> i;
			if (k & 1)
				r += "1";
			else
				r += "0";
		}
		return r;
	}

	int binaryToDec(string r) {
		string temp = r;
		int dec_value = 0;
		int base = 1;

		for (int i = 19; i >= 0; i--) {

			if (temp[i] == '1')
				dec_value += base;

			base = base * 2;
		}
		return dec_value;
	}

	void Remove(int x) {

		int a = 0;
		string r = decToBinary(x);
		stack <TrieNode*> s;
		TrieNode* temp = Root;
		TrieNode* temp2;
		cout << "r : " << r << "\n";

		for (int i = 0; i < 20; i++) {

			if (r[i] == '0')
				temp = temp->child[0];
			else if (r[i] == '1')
				temp = temp->child[1];

			s.push(temp);
			//cout << temp->bit << " ";
		}

		//cout << "\n";
		
		for (int i = 0; i < 20; i++)
		{
			temp = s.top();             //child
			s.pop();
			temp2 = s.top();          //parent node
			s.push(temp);

			bool isleft;
			isleft = (temp2->child[0] == temp);

			//cout << "pop: " << temp->bit << "\n ";
			//cout << "pop 2: " << temp2->bit << "\n ";

			if (temp->child[0] == NULL && temp->child[1] == NULL)
			{
				//cout << "pop: " << temp->bit << " ";
				delete temp;

				if (isleft)
					temp2->child[0] = NULL;
				else
					temp2->child[1] = NULL;

				s.pop();
			}

			else
				break;
		}
		
		
		//cout << "\n";
	}

};


//Second phase

class SegmentNode {
public:
	int low;
	int high;
	TrieTree* t;
	SegmentNode* left;
	SegmentNode* right;

	SegmentNode() {
		low = 0;
		high = 0;
		t = new TrieTree();
		left = NULL;
		right = NULL;
	}

	~SegmentNode() {
		t->~TrieTree();
	}
};

class SegmentTree {
public:
	SegmentNode* Root;

	SegmentTree() {
		Root = NULL;
	}

	~SegmentTree() {
		delete Root;
	}

	SegmentNode* creattree(SegmentNode* r, int l, int h, int a[]) {

		SegmentNode* temp = new SegmentNode();
		temp->high = h;
		temp->low = l;
		//cout << "  low:" << l << "   high: " << h << "--->" << endl;
		for (int i = l; i <= h; i++) {
			temp->t->insert(a[i]);
		}

		if (r == NULL) {

			if (Root == NULL) {
				Root = temp;
				//cout << " Root   --->  " << "  low:" << Root->low << "   high: " << Root->high <<  endl;
			}
			r = temp;
			//cout << " r   --->  " << "  low:" << r->low << "   high: " << r->high << endl;
		}

		if (l == h) {
			r = temp;
			return r;
		}

		else {
			r->low = l;
			r->high = h;

			r->left = creattree(r->left, l, (l + h) / 2, a);
			r->right = creattree(r->right, ((l + h) / 2) + 1, h, a);
			return r;
		}
	}

	void update(int indx, int x, int a) {

		SegmentNode* temp = Root;
		while (temp->left != NULL || temp->right != NULL) {
			//cout << "low : " << temp->low << "    high:  " << temp->high << "\n";
			temp->t->insert(x);
			temp->t->Remove(a);

			if (indx <= (temp->high + temp->low) / 2)
				temp = temp->left;

			else
				temp = temp->right;
		}

		temp->t->insert(x);
		temp->t->Remove(a);
	}

	int MaxXor(SegmentNode* root, int l, int r, int x) {

		int mid = (root->high + root->low) / 2;
		//int result = 0;
		//cout << "l : " << l << "    h :" << r << endl;
		//cout << " low :" << root->low << "     mid :" << mid << "     high:  " << root->high << endl;

		if (l == root->low && r == root->high) {
			return (root->t->XOR(x));
		}

		else {
			if (l <= mid && r <= mid)
				return (MaxXor(root->left, l, r, x));

			else if (l > mid && r > mid)
				return (MaxXor(root->right, l, r, x));

			else if (l <= mid && r > mid)
				return (MAX(MaxXor(root->left, l, mid, x), MaxXor(root->right, mid + 1, r, x)));

		}
	}

	int MAX(int x1, int x2) {
		if (x1 < x2)
			return x2;
		else
			return x1;
	}
};

//Third phase


int main() {

	SegmentTree t;

	int quest = 0, ind = 0, option = 0, result = 0, n = 0, l = 0, r = 0, x = 0;
	int* a = NULL;

	cin >> n;
	if (n > 0) {

		a = new int[n];

		cin >> quest;
		if (quest > 0 && quest <= M2) {
			for (int i = 0; i < n; i++) {
				cin >> a[i];
			}

			t.Root = t.creattree(t.Root, 0, n - 1, a);
			for (int i = 1; i <= quest; i++) {
				
				cin >> option;
				switch (option) {

				case 1:

					cin >> ind;
					if (ind > 0 && ind <= n) {

						cin >> x;
						if (x <= M) {
							t.update(ind - 1, x, a[ind - 1]);
							a[ind - 1] = x;
						}
					}
					break;

				case 2:

					cin >> l;
					cin >> r;
					if (1 <= l && l <= r && r <= n + 1) {
						cin >> x;
						if (x <= M) {
							result = t.MaxXor(t.Root, l - 1, r - 1, x);
							cout << result << " \n";
						}
					}

					break;
				}
			}
		}
	}

	//delete[]a;
	//t.~SegmentTree();

	return 0;
}
