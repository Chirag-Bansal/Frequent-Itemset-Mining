#include <bits/stdc++.h>
using namespace std;

#ifdef NDEBUG
#define dlog(x)
#else
#define dlog(x) clog << x << endl
#endif

const int ITEMS_BUF = 20000;

FILE* infile;
FILE* outfile;
int n_items;
vector<int> item_at_rank;
vector<int> rank_of_item;

inline int read_int(int& x) {
	int c = getc_unlocked(infile);
	if (c == EOF) return -1;
	if (c == '\n') return -2;
	x = 0;
	while (c >= '0' && c <= '9') {
		x = x*10 + c-'0';
		c = getc_unlocked(infile);
	}
	if (c == '\n') return -3;
	return 0;
}

inline bool read_ranks(vector<int>& ranks) {
	int c = getc_unlocked(infile);
	if (c == EOF) return true;
	while (true) {
		int x = 0;
		while (c >= '0' && c <= '9') {
			x = x*10 + c-'0';
			c = getc_unlocked(infile);
		}
		int r = rank_of_item[x];
		if (r < n_items) {
			ranks.push_back(r);
		}
		if (c == '\n') return false;
		c = getc_unlocked(infile);
		if (c == '\n') return false;
	}
	return false;
}

inline void write_ranks(const vector<int>& ranks) {
	vector<string> items(ranks.size());
	for (int i = 0; i < (int)ranks.size(); ++i) {
		items[i] = to_string(item_at_rank[ranks[i]]);
	}
	sort(items.begin(), items.end());
	for (string& item: items) {
		for (char c: item) {
			putc_unlocked(c, outfile);
		}
		putc_unlocked(' ', outfile);
	}
	putc_unlocked('\n', outfile);
}

struct Node {
	int rank;
	int count;
	Node* parent;
	Node* copy;
	Node(int rank, Node* parent):
		rank(rank), count(0), parent(parent), copy(nullptr) {}
};

struct CNode {
	Node* corres;
	int rank;
	vector<CNode*> children;
	CNode(Node* corres, int rank):
		corres(corres), rank(rank), children(n_items-rank) {}
	inline CNode* get_child(int r) {
		return children[r-rank];
	}
	inline void set_child(int r, CNode* node) {
		children[r-rank] = node;
	}
};

double minsup;
vector<int> ranks;
CNode* croot;
Node* root;

void mine(vector<deque<Node>>& tree) {
	for (int rank = 0; rank < (int)tree.size(); ++rank) {
		int count = 0;
		for (Node& node: tree[rank]) count += node.count;

#ifndef NDEBUG
#if 0
		clog << "+ ";
		for (int rank: ranks) clog << rank << ' ';
		clog << rank << endl;
		clog << "count: " << count << endl;
#endif
#endif

		if (count < minsup) continue;
		ranks.push_back(rank);
		write_ranks(ranks);

		vector<deque<Node>> new_tree(rank);
		vector<Node*> copy_reg;
		for (Node& node: tree[rank]) {
			Node* prev = &node;
			int val = prev->count;
			Node* cur = prev->parent;
			while (cur != root) {
				if (!cur->copy) {
					copy_reg.push_back(cur);
					new_tree[cur->rank].emplace_back(cur->rank, root);
					cur->copy = &new_tree[cur->rank].back();
				}
				cur->copy->count += val;
				if (prev->copy) {
					prev->copy->parent = cur->copy;
				}
				prev = cur;
				cur = cur->parent;
			}
		}

		if (!new_tree.empty()) {
			mine(new_tree);
		}

		for (Node* cur: copy_reg) {
			cur->copy = nullptr;
		}
		ranks.pop_back();
	}
}

int main(int argc, char** argv) {
#ifndef NDEBUG
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL); clog.tie(NULL);
#endif
	dlog("db scan 1...");

	infile = fopen(argv[1], "r");
	dlog("reading file: " << argv[1]);

	vector<int> freq;
	freq.reserve(ITEMS_BUF);
	int n_trans = 0;
	//vector<int> items;
	while (true) {
		int item;
		int e = read_int(item);
		if (e == -1) break;
		if (e == -2 || e == -3) {
			++n_trans;
		}
		if (e == 0 || e == -3) {
			if ((int)freq.size() <= item) {
				freq.resize(item + 1);
			}
			++freq[item];
		}
	}
	dlog("");
	fclose(infile);

	dlog("#transactions: " << n_trans);
	n_items = freq.size();
	freq.push_back(-1); // for binary search later
	dlog("#items: " << n_items);
	minsup = stod(argv[2]) * n_trans / 100;

	item_at_rank.resize(n_items);
	for (int i = 0; i < n_items; ++i) {
		item_at_rank[i] = i;
	}
	sort(item_at_rank.begin(), item_at_rank.end(), [&](int l, int r) {
			return freq[l] > freq[r];
			});

	rank_of_item.resize(n_items);
	for (int i = 0; i < n_items; ++i) {
		rank_of_item[item_at_rank[i]] = i;
	}

	int lo = 0, hi = n_items;
	while (lo <= hi) {
		int mid = (lo+hi)/2;
		if (freq[item_at_rank[mid]] < minsup) {
			n_items = mid;
			hi = mid-1;
		} else {
			lo = mid+1;
		}
	}
	dlog("#frequent items: " << n_items);

	dlog("db scan 2...");
	infile = fopen(argv[1], "r");
	outfile = fopen(argv[3], "w");

	vector<deque<Node>> tree(n_items+1);
	vector<deque<CNode>> ctree(n_items+1);
	root = new Node(-1, nullptr);
	croot = new CNode(root, -1);
#ifndef NDEBUG
	int node_ctr = 0;
#endif

	vector<int> ranks;
	ranks.reserve(ITEMS_BUF);
	while (true) {
		CNode* ccur = croot;
		ranks.clear();
		bool eof = read_ranks(ranks);
		if (eof) break;
		sort(ranks.begin(), ranks.end());
		for (int rank: ranks) {
			if (ccur->get_child(rank) == nullptr) {
#ifndef NDEBUG
				++node_ctr;
#endif
				tree[rank].emplace_back(rank, ccur->corres);
				Node* node = &tree[rank].back();
				ctree[rank].emplace_back(node, rank);
				CNode* cnode = &ctree[rank].back();
				ccur->set_child(rank, cnode);
				ccur = cnode;
			} else {
				ccur = ccur->get_child(rank);
			}
			++ccur->corres->count;
		}
	}
	dlog("");
	dlog("node_ctr: " << node_ctr);

	ranks.reserve(ITEMS_BUF);
	if (!tree.empty()) {
		mine(tree);
	}
}
