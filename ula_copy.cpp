#include <bits/stdc++.h>

using namespace std;

void decodificadorSinaisULA(int F0, int F1, int INVA, int ENA, int ENB, int INC, int A, int B, int& V1, int& V2, int& V3, int& V4, int& VAI) {

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
}

int32_t decodificadorBarramentoB(uint8_t codigoB, int32_t MDR, int32_t PC, uint8_t MBR, int32_t SP, int32_t LV, int32_t CPP, int32_t TOS, int32_t OPC) {
        switch (codigoB) {
        case 0:
                return MDR;

        case 1:
                return PC;

        case 2:
                return static_cast<int32_t>(static_cast<int8_t>(MBR));

        case 3:
                return static_cast<int32_t>(static_cast<uint8_t>(MBR));

        case 4:
                return SP;

        case 5:
                return LV;

        case 6:
                return CPP;

        case 7:
                return TOS;

        default:
                return OPC;
        }
}

void seletorBarramentoC(std::array<int, 21> instrucao, int32_t Sd,
                        int32_t& H, int32_t& OPC, int32_t& TOS, int32_t& CPP,
                        int32_t& LV, int32_t& SP, int32_t& PC, int32_t& MDR, int32_t& MAR) {

        if (instrucao[8])
                H = Sd; // bit 8 - H
        if (instrucao[9])
                OPC = Sd; // bit 7 - OPC
        if (instrucao[10])
                TOS = Sd; // bit 6 - TOS
        if (instrucao[11])
                CPP = Sd; // bit 5 - CPP
        if (instrucao[12])
                LV = Sd; // bit 4 - LV
        if (instrucao[13])
                SP = Sd; // bit 3 - SP
        if (instrucao[14])
                PC = Sd; // bit 2 - PC
        if (instrucao[15])
                MDR = Sd; // bit 1 - MDR
        if (instrucao[16])
                MAR = Sd; // bit 0 - MAR
}

int main() {
        // ler o arquivo
        // IR são todas as intruções
        // PC é a linha do IR

        // F0 - F1 - ENA - ENB - INVA - INC - A - B

        // Cada execução devemos anotar: IR - PC - A - B - S - Vai-um

        int32_t PC = 0;
        vector<array<int, 21>> IR;

        // As instruções virão de um arquivo
        // fstream Entrada("programa_etapa2_tarefa1.txt");
        // O resultado sairá em um arquivo
        // ofstream Saida("saida_etapa2_tarefa1.txt");

        // Ler automaticamente todas as instruções (linhas) do arquivo de entrada
        vector<string> linhas;
        // string s;
        // while (Entrada >> s) {
        //         // ignora linhas vazias ou curtas
        //         if (s.size() != 21)
        //                 continue;
        //         linhas.push_back(s);
        // }

        int n; cin >> n;
        IR.resize(n);
        for (int i = 0; i < n; ++i) {
                string s; cin >> s;
                // string linha = linhas[i];
                for (int j = 0; j < s.size(); j++) {
                        IR[i][j] = (s[j] - '0');
                }
        }

        // Entrada.close();
        
        int32_t H = 0, OPC = 0, TOS = 0, CPP = 0, LV = 0, SP = 0, MDR = 0, MAR = 0;
        uint8_t MBR = 0;

        

        while (PC < IR.size()) {

                int SLL8 = IR[PC][0]; // Deslocamento lógico para esquerda de 8 bits
                int SRA1 = IR[PC][1]; // Deslocamento aritmético para a direita de 1 bit

                int F0 = IR[PC][2];
                int F1 = IR[PC][3];

                int ENA = IR[PC][4];
                int ENB = IR[PC][5];

                int INVA = IR[PC][6];

                int INC = IR[PC][7]; // Vem-um

                int8_t codigoB = (IR[PC][17] << 3) | (IR[PC][18] << 2) | (IR[PC][19] << 1) | IR[PC][20];

                // Na Mic-1, o A é sempre H
                int32_t A = H;

                int32_t B = decodificadorBarramentoB(codigoB, MDR, PC, MBR, SP, LV, CPP, TOS, OPC);

                string temp;

                int V1 = 0, V2 = 0, V3 = 0, V4 = 0, VAI = 0;

                decodificadorSinaisULA(F0, F1, INVA, ENA, ENB, INC, A, B, V1, V2, V3, V4, VAI);

                auto S = ((V1 | V2) | (V3 | V4));

                int32_t Sd = S;

                if (SLL8) {
                        // Deslocamento lógico para a esquerda (acaba sendo igual ao deslocamento aritmético)
                        Sd = (int32_t)((uint32_t)S << 8);
                }

                if (SRA1) {
                        // Deslocamento aritmético para a direita
                        Sd = S >> 1;
                }

                int N = (Sd < 0) ? 1 : 0;

                int Z = (Sd == 0) ? 1 : 0;

                seletorBarramentoC(IR[PC], Sd, H, OPC, TOS, CPP, LV, SP, PC, MDR, MAR);

                for (auto i : IR[PC]) cout << i;
                cout << " PC:" << PC + 1 << " A:" << A << " B:" << B << " S:" << S << " VAI:" << VAI << " Sd: " << Sd << "\n";
                // for (auto i : IR[PC]) Saida << i;
                // Saida << " PC:" << PC + 1 << " A:" << A << " B:" << B << " S:" << S << " VAI:" << VAI << " Sd: " << Sd << "\n";

                PC = PC + 1;
        }
}