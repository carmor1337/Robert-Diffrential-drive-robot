#ifndef PATH_TRACKER
#define PATH_TRACKER

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <memory>

#include "Node.h"

using node_pointer = std::shared_ptr<Node>;
using coordinate   = std::pair<int,int> ;

class Path_tracker
{
private:
    std::vector<node_pointer> path {};
   
public:
    Path_tracker(/* args */);
    ~Path_tracker();
};

Path_tracker::Path_tracker(/* args */)
{
}

Path_tracker::~Path_tracker()
{
}

#endif