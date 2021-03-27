//A starlike tree consists of a central vertex called root and several path graphs attached to it. More formally, a tree is starlike if it has exactly one vertex of degree greater than 2.
#include <math.h>
#include <time.h>



void starlikeTree(int n, int tree[][n]){
    //generates a starlike tree, vertex 0 is the middle node
    int last;
    int path;
    srand(time(0));
    int branches= 1+(rand() % (n - 2 + 1));
    int branchArr[branches]; //array of last node added to branch for each branch
    //printf("%d", branches); used to see how many branches were generated
    //printf("\n");
    for(int i= 1; i<=branches; i++){
        tree[0][i]=1; //creates first node in each branch
        tree[i][0]=1;
        branchArr[i-1]=i;
    }

    for(int i=branches+1; i<n; i++){ //starts at 1 more than the number of branches
        path= (rand() % (branches - 1 + 1)); //selects which branch to place the next node
        last=branchArr[path];//find last node added to this branch
        tree[last][i]=1;
        tree[i][last]=1;
        branchArr[path]=i; //sets this as new last node
    }
}
