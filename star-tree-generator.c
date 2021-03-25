//A star tree is a tree which consists of a single internal vertex (and n−1 leaves). In other words, a star tree of order n is a tree of order n with as many leaves as possible.
void starTree(int n, int tree[][n]){
    //generates path graph
    for(int i=1; i<n; i++){
        tree[0][i]=1;
        tree[i][0]=1;
    }
}
