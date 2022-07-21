//test array define
const int N=12;
float d[N+1][2] = {1, 2, {3}, {5}, 7 , 8};
int e[4][2] = {{d[2][1], 1}, {3, 4}, {5, 6}, {7, 8}};
int main(){
    return e[3][1] + e[0][0] + e[0][1];
}