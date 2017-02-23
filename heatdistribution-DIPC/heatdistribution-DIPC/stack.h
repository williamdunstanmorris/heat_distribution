//
//  stack.h
//  heatdistribution-DIPC
//
//  Created by William Morris on 22/02/2017.
//
//

#ifndef stack_h
#define stack_h

template <class T>

class Stack {
public:
    Stack(void);
    ~Stack(void); //Destructor
    int empty(void);
    int push(T &);
    T pop(void);
    T peek(void);
private:
    int top;
    T* nodes;
};

#endif /* stack_h */
