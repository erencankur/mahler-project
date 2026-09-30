#include "mahler/ulam.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>

int main() {
    try {
        int x=0,y=0,direction=0,run=1,used=0,legs=0;
        constexpr int dx[]{1,0,-1,0},dy[]{0,1,0,-1};
        std::set<std::pair<int,int>> occupied;
        for (std::uint64_t n=1;n<=1000000;++n) {
            const auto point=mahler::ulam_point(n);
            if (point.x!=x || point.y!=y) throw std::runtime_error("coordinate differs from independent walk");
            if (n<=10000 && !occupied.emplace(x,y).second) throw std::runtime_error("duplicate cell");
            const auto r=static_cast<int>(mahler::ulam_side(n)/2);
            if (std::max(std::abs(x),std::abs(y))>r) throw std::runtime_error("point outside grid");
            x+=dx[direction]; y+=dy[direction];
            if (++used==run) { used=0; direction=(direction+1)%4; if (++legs%2==0) ++run; }
        }
        const auto primes=mahler::prime_flags(100);
        unsigned count=0;
        for (unsigned n=1;n<=100;++n) {
            bool reference=n>=2;
            for (unsigned d=2;d<n;++d) if (n%d==0) reference=false;
            if (reference!=primes[n]) throw std::runtime_error("sieve mismatch");
            count+=primes[n];
        }
        if (count!=25 || mahler::ulam_side(1000000)!=1001) throw std::runtime_error("incorrect count or side");
        std::cout << "one million coordinates and prime sieve verified\n";
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
