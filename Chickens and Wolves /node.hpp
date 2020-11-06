#ifndef node_hpp
#define node_hpp

#include <stdio.h>
struct Bank {
    int tot_chick, tot_wolf;
    bool boat;
};

struct State {
    Bank leftB, rightB;
};

struct Node {
    State currentState;
    Node* parent;
    int depth;
    int priority;
    bool operator < (const Node& x) const{
        return priority < x.priority;
    }
};
#endif /* node_hpp */
