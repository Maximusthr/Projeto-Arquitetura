#include <bits/stdc++.h>

using namespace std;

void carregarRegistradores(fstream& Registradores, int32_t& MAR, int32_t& MDR, int32_t& PC, uint8_t& MBR, int32_t& SP, int32_t& LV, int32_t& CPP, int32_t& TOS, int32_t& OPC, int32_t& H) {
        string linha;
        map<string, int32_t> valores;

        while (getline(Registradores, linha)) {
                int pos = linha.find('=');
                if (pos == string::npos)
                        continue;
                string nome = linha.substr(0, pos);
                string valorBinario = linha.substr(pos + 1);

                // Remover espaços
                nome.erase(remove_if(nome.begin(), nome.end(), ::isspace), nome.end());
                valorBinario.erase(remove_if(valorBinario.begin(), valorBinario.end(), ::isspace), valorBinario.end());

                // Transforma p/ aceitar o binario
                valores[nome] = stoi(valorBinario, nullptr, 2);
        }

        // Atribuindo os valores aos registradores
        MAR = valores["mar"];
        MDR = valores["mdr"];
        PC = valores["pc"];
        MBR = static_cast<uint8_t>(valores["mbr"]);
        SP = valores["sp"];
        LV = valores["lv"];
        CPP = valores["cpp"];
        TOS = valores["tos"];
        OPC = valores["opc"];
        H = valores["h"];

        Registradores.close();
}

void decodificadorSinaisULA(int F0, int F1, int INVA, int ENA, int ENB, int INC, int32_t A, int32_t B, int32_t& S, int& VAI) {
        int32_t entrada_A = 0;
        int32_t entrada_B = 0;

        if (ENA) {
                entrada_A = INVA ? ~A : A;
        }

        if (ENB) {
                entrada_B = B;
        }

        S = 0;
        VAI = 0;

        if (!F0 && !F1) { // 00
                // A & B (AND)
                S = (entrada_A & entrada_B);
        } else if (!F0 && F1) { // 01
                // A + B (OR)
                S = (entrada_A | entrada_B);
        } else if (F0 && !F1) { // 10
                // ~B (NOT B)
                S = ~entrada_B;
        } else if (F0 && F1) { // 11
                // Soma com carry-in
                uint64_t soma = (uint64_t)(uint32_t)entrada_A + (uint64_t)(uint32_t)entrada_B + (uint64_t)INC;
                S = (int32_t)soma;
                VAI = (soma > 0xFFFFFFFF); // Define o Vai-um (carry out)
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

string nomeDoRegistradorNoBarramentoB(uint8_t codigoB) {
        switch (codigoB) {
        case 0:
                return "MDR";

        case 1:
                return "PC";

        case 2:
                return "MBR (signed)";

        case 3:
                return "MBR (unsigned)";

        case 4:
                return "SP";

        case 5:
                return "LV";

        case 6:
                return "CPP";

        case 7:
                return "TOS";

        default:
                return "OPC";
        }
}

string nomesDosRegistradoresC(array<int, 23>& instrucao) {
        vector<string> nomes = {"H", "OPC", "TOS", "CPP", "LV", "SP", "PC", "MDR", "MAR"};
        string resultado;
        bool primeiro = true;

        for (size_t i = 0; i < nomes.size(); ++i) {
                // Verifica o bit correspondente (índices 8 a 16)
                if (instrucao[8 + i]) {
                        if (!primeiro) {
                                resultado += ", ";
                        }
                        resultado += nomes[i];
                        primeiro = false;
                }
        }

        if (resultado.empty()) {
                return "Nenhum";
        }

        return resultado;
}

void seletorBarramentoC(std::array<int, 23> instrucao, int32_t Sd,
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

string conversor_binario(int valor) {
        string s = "";
        for (int i = 0; i < 32; i++) {
                if ((valor & (1 << i))) {
                        s.push_back('1');
                } else
                        s.push_back('0');
        }

        reverse(s.begin(), s.end());

        return s;
}

string conversor_MBR(int valor) {
        string s = "";
        for (int i = 0; i < 8; i++) {
                if ((valor & (1 << i))) {
                        s.push_back('1');
                } else
                        s.push_back('0');
        }

        reverse(s.begin(), s.end());

        return s;
}

// Abre o arquivo, lê os tokens de 32 bits, converte cada string de binário pra int32 e coloca no vetor MEM
bool lerMemoria32Bin(const string& path, vector<int32_t>& MEM) {
        ifstream fin(path);
        if (!fin) {
                return false;
        }

        MEM.clear();
        string line;

        while (fin >> line) {
                if (line.size() != 32) { // ignora linhas que estejam invalidas
                        continue;
                }

                int32_t v = 0;
                for (char c : line) {
                        v = (v << 1) | (c == '1' ? 1 : 0);
                }

                MEM.push_back(v);
        }
        return true;
}

int main() {
        // ler o arquivo
        // IR são todas as intruções
        // PC é a linha do IR

        // F0 - F1 - ENA - ENB - INVA - INC - A - B

        // Cada execução devemos anotar: IR - PC - A - B - S - Vai-um

        int32_t PC = 0;
        vector<array<int, 23>> IR;

        vector<int32_t> MEM;
        // As instruções virão de um arquivo
        fstream Entrada("projeto_arquitetura_testes/microinstrucoes_etapa3_tarefa1.txt");
        // Os dados virão de um arquivo
        string caminho_dados = "dados_parece_que_certos.txt";
        // O estado inicial dos registradores virá de um arquivo
        fstream Registradores("projeto_arquitetura_testes/registradores_etapa3_tarefa1.txt");
        // O resultado sairá em um arquivo
        ofstream Saida("./saida_etapa3_tarefa1.txt");

        if (!lerMemoria32Bin(caminho_dados, MEM)) {
                // Adiciona um tratamento de erro, caso o arquivo não seja encontrado
                cerr << "ERRO: Não foi possível ler o arquivo de dados da memória: " << caminho_dados << endl;
                return 1; // Encerra o programa se a memória não puder ser carregada
        }

        // Ler automaticamente todas as instruções (linhas) do arquivo de entrada
        vector<string> linhasEntrada;
        string s;
        while (Entrada >> s) {
                // ignora linhas vazias ou curtas
                if (s.size() != 23)
                        continue;
                linhasEntrada.push_back(s);
        }

        IR.resize(linhasEntrada.size());
        for (int i = 0; i < linhasEntrada.size(); ++i) {
                string linha = linhasEntrada[i];
                for (int j = 0; j < 23; j++) {
                        IR[i][j] = (linha[j] - '0');
                }
        }

        Entrada.close();

        int32_t H = 0, OPC = 0, TOS = 0, CPP = 0, LV = 0, SP = 0, MDR = 0, MAR = 0;
        uint8_t MBR = 0;
        int32_t PC_registrador = 0;

        carregarRegistradores(Registradores, MAR, MDR, PC_registrador, MBR, SP, LV, CPP, TOS, OPC, H);

        while (PC < IR.size()) {

                int SLL8 = IR[PC][0]; // Deslocamento logico para esquerda de 8 bits
                int SRA1 = IR[PC][1]; // Deslocamento aritmetico para a direita de 1 bit

                int F0 = IR[PC][2];
                int F1 = IR[PC][3];

                int ENA = IR[PC][4];
                int ENB = IR[PC][5];

                int INVA = IR[PC][6];

                int INC = IR[PC][7]; // Vem-um

                int WRITE = IR[PC][17];

                int READ = IR[PC][18];

                Saida << "--- Instrução " << PC + 1 << " ---" << endl;
                Saida << "Instrução (IR): " << linhasEntrada[PC] << endl;

                Saida << "Registradores no Início:" << endl;
                Saida << "H =" << H << " OPC =" << OPC << " TOS =" << TOS << " CPP =" << CPP << " LV =" << LV << " SP =" << SP << " PC =" << PC_registrador << " MDR =" << MDR << " MAR =" << MAR << " MBR =" << (int)MBR << endl;

                int8_t codigoB = (IR[PC][19] << 3) | (IR[PC][20] << 2) | (IR[PC][21] << 1) | (IR[PC][22]);
                Saida << "Barramento B comandado por: " << nomeDoRegistradorNoBarramentoB(codigoB) << endl;
                Saida << "Barramento C habilitado para: " << nomesDosRegistradoresC(IR[PC]) << " | READ =" << READ << " WRITE" << WRITE << endl;

                // Na Mic-1, o A é sempre H
                int32_t A = H;

                int32_t B = decodificadorBarramentoB(codigoB, MDR, PC_registrador, MBR, SP, LV, CPP, TOS, OPC);

                int32_t S = 0;
                // string temp;

                int VAI = 0;

                decodificadorSinaisULA(F0, F1, INVA, ENA, ENB, INC, A, B, S, VAI);

                int32_t Sd = S;

                if (SLL8) {
                        // Deslocamento lógico para a esquerda (acaba sendo igual ao deslocamento aritmético)
                        Sd <<= 8;
                }

                if (SRA1) {
                        // Deslocamento aritmético para a direita
                        Sd >>= 1;
                }

                int N = (Sd < 0) ? 1 : 0;

                int Z = (Sd == 0) ? 1 : 0;
                // DEPOIS execute o seletor barramento C
                seletorBarramentoC(IR[PC], Sd, H, OPC, TOS, CPP, LV, SP, PC_registrador, MDR, MAR);

                if (READ && !WRITE) {
                        Saida << "[READ] MDR <- MEM[" << MAR << "]";
                        if (MAR >= 0 && MAR < MEM.size()) {
                                Saida << "(" << MEM[MAR] << ")" << endl;
                                MDR = MEM[MAR];
                        } else {
                                Saida << endl
                                      << "[READ][ERRO] Endereço MAR inválido: " << MAR << endl;
                        }
                }

                if (WRITE && !READ) {
                        Saida << "[WRITE] MEM[" << MAR << "] <- MDR(" << MDR << ")" << endl;
                        if (MAR >= 0 && MAR < MEM.size()) {
                                MEM[MAR] = MDR;
                        } else {
                                Saida << "[WRITE][ERRO] Endereço MAR inválido: " << MAR << endl;
                        }
                }

                Saida << "Registradores no Fim:" << endl;
                Saida << "H=" << H << " OPC=" << OPC << " TOS=" << TOS << " CPP=" << CPP << " LV=" << LV << " SP=" << SP << " PC=" << PC_registrador << " MDR=" << MDR << " MAR=" << MAR << " MBR=" << (int)MBR << endl
                      << endl;

                Saida << "Memória de Dados:" << endl;
                for (int i = 0; i < MEM.size(); ++i) {
                        Saida << "MEM[" << i << "] = " << conversor_binario(MEM[i]) << endl;
                }
                Saida << endl;

                Saida << "Convertendo para binario:" << endl;
                Saida << "H=" << conversor_binario(H) << "\nOPC=" << conversor_binario(OPC) << "\nTOS=" << conversor_binario(TOS)
                      << "\nCPP=" << conversor_binario(CPP) << "\nLV=" << conversor_binario(LV) << "\nSP=" << conversor_binario(SP)
                      << "\nPC=" << conversor_binario(PC_registrador) << "\nMDR=" << conversor_binario(MDR) << "\nMAR=" << conversor_binario(MAR)
                      << "\nMBR=" << conversor_MBR((int)MBR) << endl
                      << endl;

                PC = PC + 1;
        }
}