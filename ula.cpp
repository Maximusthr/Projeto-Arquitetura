#include <bits/stdc++.h>

using namespace std;

int main() {
        // ios_base::sync_with_stdio(0);
        // cin.tie(NULL);

        // ler o arquivo
        // IR são todas as intruções
        // PC é a linha do IR

        // F0 - F1 - ENA - ENB - INVA - INC - A - B

        // Cada execução devemos anotar: IR - PC - A - B - S - Vai-um

        int PC = 0;
        vector<array<int, 8>> IR;

        // As instruções virão de um arquivo
        fstream Entrada("programa_etapa1.txt");
        // O resultado sairá em um arquivo
        ofstream Saida("saida_etapa1.txt");

        // Ler automaticamente todas as instruções (linhas) do arquivo de entrada
        vector<string> linhas;
        string s;
        while (Entrada >> s) {
                // ignora linhas vazias ou curtas
                if (s.size() != 8)
                        continue;
                linhas.push_back(s);
        }

        IR.resize(linhas.size());
        for (int i = 0; i < linhas.size(); ++i) {
                string linha = linhas[i];
                for (int j = 0; j < 8; j++) {
                        IR[i][j] = (linha[j] - '0');
                }
        }

        Entrada.close();

        while (PC < IR.size()) {

                int SLL8 = IR[PC][0]; // Deslocamento lógico para esquerda de 8 bits
                int SRA1 = IR[PC][1]; // Deslocamento aritmético para a direita de 1 bit

                int F0 = IR[PC][2];
                int F1 = IR[PC][3];

                int ENA = IR[PC][4];
                int ENB = IR[PC][5];

                int INVA = IR[PC][6];

                int INC = IR[PC][7]; // Vem-um

                
                cout << "Digite A:" << "\n";
                int A;
                cin >> A;

                cout << "Digite B:" << "\n";
                
                int B;
                cin >> B;

                int V1 = 0, V2 = 0, V3 = 0, V4 = 0, VAI = 0;

                auto calc1 = [&](int inva, int a, int ena) -> int {
                        return (inva ^ (a & ena));
                };

                auto calc2 = [&](int b, int enb) -> int {
                        return (b & enb);
                };

                if (!F0 && !F1) {
                        // A & B
                        V1 = ((calc1(INVA, A, ENA)) & (calc2(B, ENB)));
                } else if (!F0 && F1) {
                        // A + B
                        V2 = ((calc1(INVA, A, ENA)) | (calc2(B, ENB)));
                } else if (F0 && !F1) {
                        // ~B
                        // if (calc2(B, ENB)){
                        //     V3 = (~B);
                        // }
                        V3 = (!B);
                } else if (F0 && F1) {
                        // Soma
                        V4 = (((calc1(INVA, A, ENA) ^ INC) ^ (calc2(B, ENB))) & (F0 & F1));
                        VAI = ((calc1(INVA, A, ENA) & (calc2(B, ENB))) | ((calc1(INVA, A, ENA) ^ (calc2(B, ENB))) & INC));
                }

                auto S = ((V1 | V2) | (V3 | V4));

                int32_t Sd = S;

                if (SLL8) {
                        // Deslocamento lógico para a esquerda (acaba sendo igual ao desclocamento aritmético)
                        Sd = S << 8;
                }

                if (SRA1) {
                        // Deslocamento aritmético para a direita
                        Sd = S >> 1;
                }

                int N = (S < 0) ? 1 : 0;

                int Z = (S == 0) ? 1 : 0;

                for (auto i : IR[PC]) cout << i;
                cout << " PC:" << PC + 1 << " A:" << A << " B:" << B << " S:" << S << " VAI:" << VAI << " Sd: " << Sd << "\n";
                for (auto i : IR[PC]) Saida << i;
                Saida << " PC:" << PC + 1 << " A:" << A << " B:" << B << " S:" << S << " VAI:" << VAI << " Sd: " << Sd << "\n";

                PC = PC + 1;
        }
}