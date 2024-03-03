#include <iostream>

#include "joiner.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    Joiner joiner;
    // Read join relations
    std::string line;
    while (getline(std::cin, line)) {
        if (line == "Done") break;
        joiner.addRelation(line.c_str());
    }
    // 在relation中统计每个列的每个值有多少个(unordered map)，然后在建join tree的时候计算结果集大小，用一个优先队列来选择join的顺序，小的排前面，然后2个小的结合之后成为一个大表加入队列，直到只剩2个
    // 在建表之后这里15行进行排序，将表中数据排序后就可以用二分来找  b+树有用吗，如果需要的话要有重复键的b+树
    // Preparation phase (not timed)
    // Build histograms, indexes,...

    QueryInfo i;
    while (getline(std::cin, line)) {
        if (line == "F") continue; // End of a batch
        i.parseQuery(line);
        std::cout << joiner.join(i);
    }

    return 0;
}
