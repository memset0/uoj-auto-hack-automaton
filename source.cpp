#include <bits/stdc++.h>
#define M 500010
#define pb push_back
#define it set <int>::iterator

using namespace std;

void cmax(int& x, int y) {if(x < y) x = y;}
void cmin(int& x, int y) {if(x > y) x = y;}

int n, m;

struct opts
{
	int tp, x, y; // -1, 0, 1
	opts(){}
	opts(int a, int b, int c) {tp = a, x = b, y = c;}
	bool operator < (const opts& X) const {return x < X.x || x == X.x && min(tp, 0) < min(X.tp, 0) || x == X.x && min(tp, 0) == min(X.tp, 0) && y < X.y;}
};

vector <opts> op;

int C = 1, L[M], R[M], X[M], Y[M], Z[M], Ans[M], Sym[M];

set <int> St;

int hhd = 0;

struct Splay
{
	int L[M], R[M], F[M], V[M], Mn[M], Sz[M], Tg[M], Lis[M];
	
	void pd(int x)
	{
		if(!Tg[x]) return;
		int tmp = Tg[x], l = L[x], r = R[x];
		if(L[x]) V[l] += tmp, Mn[l] += tmp, Tg[l] += tmp;
		if(R[x]) V[r] += tmp, Mn[r] += tmp, Tg[r] += tmp;
		Tg[x] = 0;
	}
	
	void upd(int x)
	{
		Sz[x] = 1 + Sz[L[x]] + Sz[R[x]];
		Mn[x] = V[x];
		if(L[x]) cmin(Mn[x], Tg[x] + Mn[L[x]]);
		if(R[x]) cmin(Mn[x], Tg[x] + Mn[R[x]]);
	}
	
	void rot(int x, int c)
	{
		int f = F[x];
		pd(x), pd(c);
		if(L[f] == x) L[f] = c; else R[f] = c;
		if(c == L[x]) L[x] = R[c], R[c] = x, F[L[x]] = x;
		else R[x] = L[c], L[c] = x, F[R[x]] = x;
		F[c] = f, F[x] = c;
		Mn[c] = Mn[x], Sz[c] = Sz[x], upd(x);
	}
	
	void splay(int x, int tar = 0)
	{
		for(int f = F[x]; (f = F[x]) != tar; )
			if(F[f] == tar)	rot(f, x);
			else if(L[f] == x ^ L[F[f]] == f) rot(F[x], x), rot(F[x], x);
			else rot(F[f], f), rot(F[x], x);
	}
	
	int getrank(int s)
	{
		splay(s);
		return Sz[L[s]];
	}
	
	int getpre(int x)
	{
		if(!L[x])
		{
			for(; L[F[x]] == x; x = F[x]);
			return F[x];
		}
		else
		{
			for(x = L[x]; R[x]; x = R[x]);
			return x;
		}
	}
	
	int getnxt(int x)
	{
		if(!R[x])
		{
			for(; R[F[x]] == x; x = F[x]);
			return F[x];
		}
		else
		{
			for(x = R[x]; L[x]; x = L[x]);
			return x;
		}
	}
	
	int build(int l, int r)
	{
		if(l > r) return 0;
		int md = (l + r) / 2, Lm = Lis[md];
		L[Lm] = build(l, md - 1);
		R[Lm] = build(md + 1, r);
		F[L[Lm]] = F[R[Lm]] = Lm;
		upd(Lm);
		return Lm;
	}
	
	void init()
	{
		for(int i = 3; i <= C * 2; i++)
			V[i] = 1, Lis[i] = i;
		Lis[2] = 1, Lis[C * 2 + 1] = 2;
		Lis[1] = C * 2 + 1, Lis[C * 2 + 2] = C * 2 + 2;
		build(1, C * 2 + 2);
	}
	
	void mov(int T, int x, int y)
	{
		if(x > y) return;
		int tmp1, tmp2, tmp3, tmp4;
		splay(tmp1 = getnxt(y * 2));
		splay(getpre(x * 2 - 1), tmp1);
		F[tmp2 = R[L[tmp1]]] = 0;
		R[L[tmp1]] = 0;
		upd(L[tmp1]);
		splay(tmp3 = getnxt(T));
		splay(T, tmp3);
		int delta = V[T] + T % 2 - Mn[tmp2];
		Tg[tmp2] += delta, Mn[tmp2] += delta, V[tmp2] += delta;
		F[tmp2] = T;
		R[T] = tmp2;
		upd(T);
		upd(tmp3);
	}

	int ask(int x, int y)
	{
		if(x == y) return 0;
		int a1 = getrank(x * 2 - 1), a2 = getrank(y * 2 - 1), a3, tmp1, tmp2;
		if(a1 > a2) swap(x, y), swap(a1, a2);
		a3 = getrank(x * 2);
		splay(tmp1 = getpre(x * 2 - 1));
		splay(tmp2 = getnxt(y * 2 - 1), tmp1);
		if(a3 > a2) return V[x * 2 - 1] + V[y * 2 - 1] - 2 * Mn[L[tmp2]];
		return V[x * 2 - 1] + V[y * 2 - 1] - 2 * Mn[L[tmp2]] + 2;
	}
}calc;

void Read(int& x)
{
	x = 0;
	char ch = getchar();
	while(ch < '0') ch = getchar();
	while(ch >= '0') x = x * 10 + ch - '0', ch = getchar();
}

int main()
{
	Read(n), Read(m);
	L[1] = 1, R[1] = n, Sym[0] = 1, Z[0] = 1, St.insert(0), St.insert(m + 1);
	for(int T = 1; T <= m; T++)
	{
		int op, x, y, z;
		Read(op), Read(x), Read(y);
		::X[T] = x, ::Y[T] = y;
		if(!op) L[++C] = x, R[C] = y;
		else
		{
			Read(z),::Z[T] = z;
			if(op == 1)
			{
				cmax(x, L[z]),cmin(y, R[z]);
				if(x > y) continue;
				::op.pb(opts(1, x, T));
				if(y != n) ::op.pb(opts(-1, y + 1, T));
			}
			else ::op.pb(opts(0, x, T));
		}
		Sym[T] = C;
	}
	Sym[m + 1] = C;
	sort(op.begin(), op.end());
	calc.init();
	memset(Ans, -1, sizeof Ans);
	for(int i = 0; i < op.size(); i++)
	{
		opts opt = ::op[i];
		if(opt.tp == 1)
		{
			int tmp = *St.lower_bound(opt.y);
			calc.mov(Z[opt.y] * 2 - 1, Sym[opt.y] + 1, Sym[tmp]);
			St.insert(opt.y);
		}
		else if(opt.tp == 0) Ans[opt.y] = calc.ask(Y[opt.y], Z[opt.y]);
		else if(opt.tp == -1)
		{
			it it1 = St.lower_bound(opt.y), it2 = it1, it3 = it1;
			int tmp1 = *(--it2), tmp2 = *(++it3);
			if(Sym[tmp1] == Sym[opt.y]) calc.mov(Z[tmp1] * 2 - 1, Sym[opt.y] + 1, Sym[tmp2]);
			else calc.mov(Sym[opt.y] * 2, Sym[opt.y] + 1, Sym[tmp2]);
			St.erase(it1);
		}
	}
	for(int i = 1; i <= m; i++) if(~Ans[i]) printf("%d\n", Ans[i]);
	return 0;
}
