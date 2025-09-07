#include <bits/stdc++.h>

using namespace std;

int main(){
    ios_base::sync_with_stdio(0); cin.tie(NULL);
    
    // ler o arquivo 
    // IR são todas as intruções
    // PC é a linha do IR

    // F0 - F1 - ENA - ENB - INVA - INC - A - B

    // Cada execução devemos anotar: IR - PC - A - B - S - Vai-um

    int PC = 0;
    vector<array<int, 8>> IR;

    int n; cin >> n;
    IR.resize(n);

    for (int i = 0; i < n; i++){
        string s; cin >> s;
        for (int j = 0; j < 8; j++){
            IR[i][j] = (s[j] - '0');
        }
    }

    while (PC < IR.size()){

        int F0 = IR[PC][0];
        int F1 = IR[PC][1];

        int ENA = IR[PC][2];
        int ENB = IR[PC][3];
        
        int INVA = IR[PC][4];

        int INC = IR[PC][5]; // Vem-um

        int A = IR[PC][6]; // tem?
        int B = IR[PC][7]; // tem?

        int V1 = 0, V2 = 0, V3 = 0, V4 = 0, VAI = 0;
        
        auto calc1 = [&](int inva, int a, int ena) -> int{
            return (inva ^ (a & ena));
        };

        auto calc2 = [&](int b, int enb) -> int{
            return (b & enb);
        };

        if (!F0 && !F1){
            // A & B
            V1 = ((calc1(INVA, A, ENA)) & (calc2(B, ENB)));
        }
        else if (!F0 && F1){
            // A + B
            V2 = ((calc1(INVA, A, ENA)) | (calc2(B, ENB)));
        }
        else if (F0 && !F1){
            // ~B
            // if (calc2(B, ENB)){
            //     V3 = (~B);
            // }
            V3 = (!B);
        }
        else if (F0 && F1){
            // Soma
            V4 = (((calc1(INVA, A, ENA) ^ INC) ^ (calc2(B, ENB))) & (F0 & F1));
            VAI = ((calc1(INVA, A, ENA) & (calc2(B, ENB))) | ((calc1(INVA, A, ENA) ^ (calc2(B, ENB))) & INC));
        }

        int S = ((V1 | V2) | (V3 | V4));

        for (auto i : IR[PC]) cout << i;
        cout << " PC:" << PC+1 << " A:" << A << " B:" << B << " S:" << S << " VAI:" << VAI << "\n";

        PC = PC + 1;
    }
}