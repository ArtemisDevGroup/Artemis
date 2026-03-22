#pragma once
#include "API/Memory.hxx"

#include <array>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <unordered_set>
#include <string>

using namespace Artemis::API;

int z() {
	address_t x;

	int a;
	x.read_one(a);

	int b = x.read_one<int>();

	int* c;
	x.read_many(c, 4);

	int d[4];
	x.read_many(d);

	std::array<int, 4> e;
	x.read_many(e);

	std::array<int, 4> f = x.read_many<int, 4>();

	std::list<int> g;
	x.read_many(g, 4);

	std::vector<int> h = x.read_many<int>(4);

	std::deque<int> i = x.read_many<std::deque<int>>(4);

	std::unordered_set<int> j;
	x.read_many(j, 4);

	std::set<int> k = x.read_many<std::set<int>>(4);

	std::string l;
	x.read_string(l);

	std::wstring m;
	x.read_string(m, 4);

	std::u16string o = x.read_string<char16_t>();

	std::u32string p = x.read_string<char32_t>(4);

	std::vector<int> q;
	x.walk_ptr(q);
	x.walk_ptr(std::move(q));

	std::list<long> r;
	x.walk_this_ptr(r);
	x.walk_this_ptr(std::move(r));

	int s;
	x.write_one(s);
	x.write_one(std::move(s));

	int* t;
	x.write_many(t, 4);

	int u[4];
	x.write_many(u);

	std::set<int> v;
	x.write_many(v);
	x.write_many(std::move(v));

	std::string w;
	x.write_string(w);
	x.write_string(std::move(w));
}