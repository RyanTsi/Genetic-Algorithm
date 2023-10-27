#include <vector>
#include <chrono>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <array>

const int INF = 2147483647;
const int N = 100;
unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count ();
inline int get_rand(int x = 2147483647) {
    seed = (int)(seed*48271ll%2147483647);
    return seed % x;
}
inline double get_real_rand() {
    return get_rand() * 1.0 / INF;
}
struct Population_individual {
    std::array<int, N> v;
    // 缓存
    double fit; bool cala_ok;
    Population_individual() {
        cala_ok = false;
        for(int &i : v) {
            i = get_rand(2);
        }
    }
    // 突变
    void mutate() {
        int idx = get_rand(N);
        v[idx] = 1 - v[idx];
    }
    // 杂交
    friend void hybrid(Population_individual &a, Population_individual &b) {
        int x = get_rand(N - 1);
        std::swap(a.v[x], b.v[x]), std::swap(a.v[x + 1], b.v[x + 1]); 
    }
    // 100 维求和
    int get_val() {
        int res = 0;
        for(const int &i : v) {
            res += i;
        }
        return res;
    }
    // 求适应度
    double get_fitness() {
        if(!cala_ok) {
            cala_ok = true;
            int x = get_val();
            fit = exp(100 - x) / (x + 0.01);
            return fit;
        }
        return fit;
    }
    bool operator<(Population_individual X) {
        return get_fitness() > X.get_fitness();
    }
};

struct xjq_GA {
    int limit; // 种群容量
    double p;
    std::vector<Population_individual> Populations;
    xjq_GA(int _limit, double _p) : limit(_limit), p(_p) {
        if(limit % 2) limit ++;
        while(Populations.size() < limit) {
            Populations.push_back(Population_individual());
        }
        sort(Populations.begin(), Populations.end());
    }
    // 种群适应度总和
    double get_all_fitness() {
        double res = 0;
        for(auto p : Populations) {
            res += p.get_fitness();
        }
        return res;
    }
    // 迭代
    void iterate() {
        std::random_shuffle(Populations.begin(), Populations.end());
        
        for(int i = 0; i < limit; i += 2) {
            Populations.push_back(Populations[i]);
            Populations.back().cala_ok = false;
            Populations.push_back(Populations[i + 1]);
            Populations.back().cala_ok = false;
            int m = Populations.size() - 1;
            hybrid(Populations[m], Populations[m - 1]);
            int p1 = get_real_rand();
            if(p1 < p) {
                Populations[m].mutate();
            }
            int p2 = get_real_rand();
            if(p2 < p) {
                Populations[m - 1].mutate();
            }
        }

        double all_fitness = get_all_fitness();
        std::vector<Population_individual> Populations_next;
        while(Populations_next.size () < limit) {
            for(auto Population : Populations) {
                double p = Population.get_fitness() / all_fitness;
                if(get_real_rand() < p) {
                    Populations_next.push_back(Population);
                }
                if(Populations_next.size() == limit) {
                    break;
                }
            }
        }
        std::swap(Populations_next, Populations);
        sort(Populations.begin(), Populations.end());
    }
    // 中位数
    int get_mid() {
        return Populations[limit / 2].get_val();
    }
    // 平均数
    double get_avg() {
        double all = 0;
        for(auto i : Populations) {
            all += i.get_val();
        }
        return all / limit;
    }
};

int main () {
    int limit = 50;
    double p = 0.001;
    std::cout << std::fixed << std::setprecision(6);
    xjq_GA ga(limit, p);
    for(int _ = 0; _ <= 300; _++) {
        std::cout << "第" << _ << "代:\n中位数: " << ga.get_mid() << "   平均数：" << ga.get_avg() << "\n种群中的个体值: "; 
        for(auto x : ga.Populations) {
            std::cout << x.get_val() << " ";
        }
        std::cout << "\n\n";
        ga.iterate();
    }
}